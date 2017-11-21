/*
 * InnerDCCManager.cpp
 *
 *  Created on: 2013-7-1
 *      Author: Administrator
 */

#include "InnerDCCManager.h"
#include "UID.h"
#include "SwitchPortInner.h"
#include <stdio.h>
#include "SoftSwitch_define.h"
#include "os.h"
#include "GeneralLogic.h"
#include "bsp_interrupt.h"
#include <iostream>
#include "NMPort.h"
#include "SoftWDT.h"

extern "C" INTR_HANDLER void IntHandler_DCC(void);

//static const char* fname = "InnerDccInfo2_7_20.cfg";
TASK void rcvInnerNmPaket(void);
InnerDCCManager InnerDCCManager::dccmng;

//std::vector<Inner_Port_Config> InnerDCCManager::configData;
//std::list<DCCChannelRcv*> ch_have_packet;

OS_TID t_dcc_rcv = 0;
//OS_TID t_dcc_process;
//OS_SEM sem_dcc_nm;

InnerDCCManager::InnerDCCManager() {

}

InnerDCCManager::~InnerDCCManager() {
	// TODO Auto-generated destructor stub
}


void InnerDCCManager::start(bool first) {
    if( first ) {
        SIC_2_Enable();
//        os_sem_init(sem_dcc_nm, 0);
        t_dcc_rcv = os_tsk_create(rcvInnerNmPaket,P_DCC_RCV);
    }
    Dcc_interrupt_enable();
}

void InnerDCCManager::stop(bool end) {
    Dcc_interrupt_disable();
    if( end ) {
        os_tsk_delete(t_dcc_rcv);
    }
}


bool InnerDCCManager::join(NMPort* port, uint8 swportid) {
    if( port == 0 ) {
        return false;
    }
    tsk_lock();
    SwitchPortInner* p = (SwitchPortInner*)SwitchPort::getSwitchPort(swportid);
    if( p == 0 ) {
        if( swportid == 0 ) {
            //自动创建
            p = new SwitchPortInner();
        }
        else {
            //指定创建
            p = new SwitchPortInner(swportid);
        }

    }
    if( p == 0 ) {
        tsk_unlock();
        return false; //获取交换端口失败
    }
    //绑定软交换端口和管理接口
    p->addNmPort(port);

    //将该管理接口加入工作接口组
    workingNMPort.insert(std::pair<uint32, NMPort*>(port->getUID(), port));

    tsk_unlock();
    return true;
}
void InnerDCCManager::leave(NMPort* port) {
    if( port == 0 ) {
        return;
    }
    tsk_lock(); //关任务切换，防止资源冲突

    SwitchPortInner* swp = port->getInnerPort();
    if( swp != 0 ) {
        //解绑定软交换口和管理接口
        swp->removePort(port);
        //如果该交换端口无NMPort，则删除该接口
        if( swp->getNMPortNumber() == 0 ) {
            delete swp;
        }
    }
    else {
        //该NMPort未绑定任何交换端口
    }

    //在工作接口组中删除该管理接口
    workingNMPort.erase(port->getUID());
    tsk_unlock();

}

TASK void rcvInnerNmPaket(void) {
    WDT_Para_T sw;
    SoftWDT::instance().add(os_tsk_self(), &sw);
    while(1) {
        OS_RESULT rst = os_evt_wait_or(0x0001, 100);
        if( rst != OS_R_TMO ) {
            /*��ѯ����ͨ���հ�*/
            bool result = false;

            NMPort* nmport = InnerDCCManager::instance().getfirstNMPort();
            while( nmport != 0 ) {
                result |= nmport->receivData();
                nmport = InnerDCCManager::instance().getNextNMPort(nmport->getUID());
            }

            /*��ѯ��ɺ���ж�*/
            if( !result ) {
                printf("\n!!!No DCCChannelRcv, please reset device!!!\n");
            }
            InnerDCCManager::instance().start();
        }
        SoftWDT::instance().feed(os_tsk_self());
    }
}


INTR_HANDLER void IntHandler_DCC(void)  {
    /* �������жϴ�������е����κ��漰EZ BUS�ĺ��� */
    InnerDCCManager::instance().stop();
    isr_evt_set(0x0001, t_dcc_rcv);
}
