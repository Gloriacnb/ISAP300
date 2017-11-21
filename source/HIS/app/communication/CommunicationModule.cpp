/*
 * CommunicationModule.cpp
 *
 *  Created on: 2013-11-27
 *      Author: Administrator
 */

#include "CommunicationModule.h"
#include "os.h"
#include "SwitchCore.h"
#include "SNMP_Process.h"
#include "EthMemory.h"
#include "switchPortSNMP.h"
#include "SwitchPortCPU.h"
#include "sysapi.h"
#include <Net_Config.h>
#include "DeviceComponent.h"
#include "DeviceAttribute.h"
#include <string.h>
#include "net_api.h"
#include "InnerDCCManager.h"
#include "LPC32_EMAC.h"
#include <LPC325x.H>                    /* LPC325x definitions               */
#include "PriPacket.h"
#include <stdio.h>
#include "TrapList.h"
#include "TopoManager.h"
#include "SoftWDT.h"


static uint8 ip[] = {192, 168, 100, 254};
static uint8 mask[] = {255, 255, 255, 0};
static uint8 gw[] = {192, 168, 100, 1};

static TASK void tcp_tick(void);
static TASK void tcp_main(void);
static TASK void snmp_init(void);
static TASK void T_Eth_Rcv(void);
static TASK void T_Aging(void );
switchPortSNMP* port1 = 0;
SwitchPortCPU* port2 = 0;

OS_TID t_tcpTask;
OS_TID t_eth_rcv;
OS_TID t_aging;
OS_TID t_tcptick;
OS_TID t_snmp;

os_mbx_declare(mbx_EMAC_input, 128);

extern "C" INTR_HANDLER void interrupt_ethernet(void); //use by IRQ.c
extern "C" void SendFrameBySwitch(LAYER2FRAME* frame); //use by LPC3250_MAC.c

extern U16 Process_SNMP(U8 socket, U8 *remip, U16 remport, U8 *buf, U16 len);

uint8 CommunicationModule::soc_snmp;

CommunicationModule::CommunicationModule() {
    soc_snmp = 0;
}

CommunicationModule::~CommunicationModule() {
    // TODO Auto-generated destructor stub
}

void CommunicationModule::initCommon(void) {
#ifdef EXT_DBG
    printf("\ninitCommon() start.\n");
#endif
    os_mbx_init(mbx_EMAC_input, sizeof(mbx_EMAC_input));
    SwitchCore::instance().initSwitchCore();
    port1 = new switchPortSNMP();   //EMAC����
    port2 = new SwitchPortCPU();    //Э��ջ����
    InitEthMemory();
    initIpPotocal();
    t_tcptick = os_tsk_create (tcp_tick, P_TCP_Time);
    t_tcpTask = os_tsk_create (tcp_main, P_TCP_Main);


    t_eth_rcv = os_tsk_create(T_Eth_Rcv, P_ETH_RCV);
    t_aging = os_tsk_create(T_Aging, P_Agint_MAC);
#ifdef EXT_DBG
    printf("\ninitCommon() end.\n");
#endif
}

void CommunicationModule::clearCommon(void) {
#ifdef EXT_DBG
    printf("\nclearCommon() start.\n");
#endif
    int_disable_eth_real();
    os_tsk_delete(t_aging);
    os_tsk_delete(t_eth_rcv);
    os_tsk_delete(t_tcpTask);
    os_tsk_delete(t_tcptick);
    delete port2;
    delete port1;
#ifdef EXT_DBG
    printf("\nclearCommon() end.\n");
#endif
}

void CommunicationModule::initWorking(void) {
#ifdef EXT_DBG
    printf("\n initWorking() start.\n");
#endif
    DeviceComponent::getDeviceAttribute().getDeviceIP(ip);
    DeviceComponent::getDeviceAttribute().getDeviceIPMask(mask);
    setNetIP(ip, mask);
    t_snmp = os_tsk_create(snmp_init, 2);
    soc_snmp = udp_get_socket (0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, Process_SNMP);
    if (soc_snmp != 0) {
    /* Open UDP port 161 for communication */
        udp_open (soc_snmp, 161);
    }
    else {
        printf("\n!!!socket get error!!!\n");
    }
    int_enable_eth_real();
    TrapList::instance().Initial();
    TopoManager::startTopoSearch();
#ifdef EXT_DBG
    printf("\n initWorking() end.\n");
#endif
}



void CommunicationModule::clearWorking(void) {
#ifdef EXT_DBG
    printf("\n clearWorking() start.\n");
#endif
    int_disable_eth_real();
    InnerDCCManager::instance().stop(true);
    TopoManager::stopTopoSearch();
    TrapList::instance().deInitial();
    if( soc_snmp != 0 ) {
        udp_close(soc_snmp);
        if(!udp_release_socket(soc_snmp)) {
            printf("\n!!!udp_release_socket %d failed!!\n", soc_snmp);
        }
        soc_snmp = 0;
    }
    os_tsk_delete(t_snmp);

    SwitchCore::instance().resetSwitchCore();
#ifdef EXT_DBG
    printf("\n clearWorking() end.\n");
#endif
}

void CommunicationModule::initStandby(void) {
#ifdef EXT_DBG
    printf("\n initStandby() start.\n");
#endif
    DeviceComponent::getDeviceAttribute().getProtectMCUIP(ip);
    DeviceComponent::getDeviceAttribute().getDeviceIPMask(mask);
    setNetIP(ip, mask);
    int_enable_eth_real();
#ifdef EXT_DBG
    printf("\n initStandby() end.\n");
#endif
}

void CommunicationModule::clearStandby(void) {
#ifdef EXT_DBG
    printf("\n clearStandby() start.\n");
#endif
    int_disable_eth_real();
    SwitchCore::instance().resetSwitchCore();
#ifdef EXT_DBG
    printf("\n clearStandby() end.\n");
#endif
}


void CommunicationModule::initIpPotocal(void) {
    init_TcpNet ();
    initNetApi();
    uint8 mac[6] = {0};
    DeviceComponent::getDeviceAttribute().getDeviceMac(mac);
    setNetMAC(mac);
    DeviceComponent::getDeviceAttribute().getDeviceIPGateway(gw);
    setNetGW(gw);
}

TASK void tcp_tick (void) {
  os_itv_set (1);
  while (1) {
    os_itv_wait ();
    /* Timer tick every 100 ms */
    timer_tick ();
    os_evt_set(0x0001, t_tcpTask);

  }
}

TASK void tcp_main (void) {
    WDT_Para_T sw;
    SoftWDT::instance().add(os_tsk_self(), &sw);
    while (1) {
        OS_RESULT rst = os_evt_wait_and(0x0001, 100);
        if( rst == OS_R_EVT ) {
            while(main_TcpNet () == __TRUE);
        }
        SoftWDT::instance().feed(os_tsk_self());
    }
}


TASK void snmp_init(void) {
    Init_SNMP();
    os_tsk_delete_self ();
}



TASK void T_Aging(void ) {
    while( 1 ) {
        os_dly_wait(100);
        SwitchCore::instance().aging();
    }

}

/*
 * LPC3250��̫���հ�����
 */
TASK void T_Eth_Rcv(void) {
    void* msg = 0;
    WDT_Para_T sw;
    SoftWDT::instance().add(os_tsk_self(), &sw);
    while( 1 ) {
        OS_RESULT rst = os_mbx_wait(mbx_EMAC_input, &msg, 100);
        if( rst != OS_R_TMO ) {


    #ifdef EZ_DEBUG
            SwitchPortStd* p = dynamic_cast<SwitchPortStd*>(SwitchPort::getSwitchPort(1));
            if( p ) {
                p->trace->receiveOnePkg();
            }
    #endif

            LAYER2FRAME* f = reinterpret_cast<LAYER2FRAME*>(msg);
            if( f ) {
                PriPacket* p = SwitchPortStd::makePacket(f, 1);
                p->recordProcessInfo(11);
    #ifdef SW_DEBUG
                printf("SNMP input: %4d\n", p->getPrivateTag().sn);
    #endif

                SwitchPort::getSwitchPort(1)->inputPacket( p );
            }
        }
        SoftWDT::instance().feed(os_tsk_self());
    }

}

void SendFrameBySwitch(LAYER2FRAME* frame) {
    if( frame ) {
        PriPacket* p = SwitchPortStd::makePacket(frame, 2);
#ifdef SW_DEBUG
        printf("IPStack input: %4d\n", p->getPrivateTag().sn);
#endif
#ifdef EZ_DEBUG
        SwitchPortStd* port = dynamic_cast<SwitchPortStd*>(SwitchPort::getSwitchPort(2));
        if( port ) {
            port->trace->receiveOnePkg();
        }
#endif
        SwitchPort::getSwitchPort(2)->inputPacket(p);
    }
}

/*--------------------------- interrupt_ethernet ----------------------------*/

INTR_HANDLER void interrupt_ethernet (void) {
  /* EMAC Ethernet Controller Interrupt function. */
    LAYER2FRAME *frame;
    U32 idx, int_stat, RxLen, info;
    U32 *sp, *dp;
    U8* p;
    U16 frameType = 0;

    while ((int_stat = (MAC_INTSTATUS & MAC_INTENABLE)) != 0) {
        MAC_INTCLEAR = int_stat;
        if (int_stat & INT_RX_DONE) {
            /* Packet received, check if packet is valid. */
            idx = MAC_RXCONSUMEINDEX;
            while (idx != MAC_RXPRODUCEINDEX) {
                info = Rx_Stat[idx].Info;
                if (!(info & RINFO_LAST_FLAG)) {
                    goto rel;
                }

                RxLen = (info & RINFO_SIZE) - 3;
                if ((RxLen > DEF_MTU) || (info & RINFO_ERR_MASK)) {
                    /* Invalid frame, ignore it and free buffer. */
#ifdef SW_DEBUG
                    printf("\n!!!interrupt_ethernet() Invalid frame, length:%d, err_mask:0x%x!!!\n", RxLen, RINFO_ERR_MASK);
#endif
                    goto rel;
                }

                p = (U8 *) Rx_Desc[idx].Packet;
                frameType = ((p[12] << 8) | p[13]);
                if (frameType != 0x0800 && frameType != 0x0806) {
#ifdef SW_DEBUG
                    printf("\n!!!interrupt_ethernet() Invalid frame type: %4x!!!\n", frameType);
#endif
                    goto rel;
                }
                /* Flag 0x80000000 to skip sys_error() call when out of memory. */
                if( isr_mbx_check(mbx_EMAC_input) != 0 ) {
                    frame = alloc_Eth_mem_interrupt(RxLen);
                    if (frame != NULL) {
                        dp = (U32 *) frame->frame;
                        sp = (U32 *) Rx_Desc[idx].Packet;
                        for (RxLen = (RxLen + 3) >> 2; RxLen; RxLen--) {
                            *dp++ = *sp++;
                        }
                        isr_mbx_send(mbx_EMAC_input, frame); //send msg to T_Eth_Rcv
                    }
                    else {
                        printf("\n !!!interrupt_ethernet() alloc_Eth_mem(%d) failed!!!\n", RxLen);
                    }
                }
                else {
#ifdef EZ_DEBUG
        printf("!!!interrupt_ethernet buff full!!!\n");
#endif
                }
                rel: if (++idx == NUM_RX_FRAG)
                    idx = 0;
                /* Release frame from EMAC buffer. */
                MAC_RXCONSUMEINDEX = idx;
            }
        }
        if (int_stat & INT_TX_DONE) {
            /* Frame transmit completed. */
        }
    }
}
