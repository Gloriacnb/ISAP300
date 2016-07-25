/*
 * TopoManager.cpp
 *
 *  Created on: 2013-7-12
 *      Author: Administrator
 */

#include "TopoManager.h"
#include "PriPacket.h"
#include "DeviceComponent.h"
#include "DeviceAttribute.h"
#include "NMPort.h"
#include <string.h>
#include "SwitchPort.h"
#include "SwitchPortInner.h"
#include <stdio.h>
#include "os.h"
#include "InnerDCCManager.h"

TASK void taskTopoAging(void);
OS_TID task_topo;

TopoPkt::TopoPkt(uint32 head, uint32 belongPort) {
	memcpy( &strPkt[4], PriPacket::multicastAddress, 6);
	DeviceComponent::getDeviceAttribute().getDeviceMac(&strPkt[10]);
	DeviceComponent::getDeviceAttribute().getDeviceIP(&strPkt[20]);
	DeviceComponent::getDeviceAttribute().getDeviceIPMask(&strPkt[24]);
	tag.sn = head;
	memcpy(tag.decMAC, PriPacket::multicastAddress, 6);
	memcpy(tag.srcMAC, &strPkt[10], 6);
	setSndPort(belongPort);
}
TopoPkt::~TopoPkt() {

}

PriPacket* TopoPkt::getPriPacket(void) {
	LAYER2FRAME* f = alloc_Eth_mem(pktSize-4);
	memcpy(f->frame, &strPkt[4], pktSize-4);
	return new PriPacket(tag, 0, f);

}

//uint8 TopoManager::Topoing = 0;
enum {TopoTimeOut = 10};

TopoManager::TopoManager() {
}

TopoManager::~TopoManager() {
}

void TopoManager::sendTopoPkt() {
    uint32 priHead = 0x80000000;
	NMPort* p = NMPort::getFirstInstance();
	while( p ) {
        TopoPkt* pkt = new TopoPkt(priHead++, p->getUID());

		SwitchPortInner* port = p->getInnerPort();
		if( port ) {
		    PriPacket* pk = pkt->getPriPacket();
		    if( pk ) {
		        port->outputAPacket(*pk);
		    }
		}
        delete pkt;
		p = NMPort::getNextInstance(p->getUID());
	}
}

/*
 * 初始化拓扑发现功能，
 * 1.重置拓扑结果
 * 2.建立拓扑心跳和老化任务
 */
void TopoManager::startTopoSearch() {
    NMPort* rch = NMPort::getFirstInstance();
    uint8 data[4] = {0};
    while( rch ) {
        rch->setUpStreamIP(data);
        rch->setUpStreamIpMask(data);
        rch->setUpStreamPortUid(0);
        rch = NMPort::getNextInstance(rch->getUID());
    }
    task_topo = os_tsk_create(taskTopoAging, P_Agint_MAC);
}


void TopoManager::stopTopoSearch() {
    os_tsk_delete(task_topo);
}

/*
 * 接收到拓扑包后的处理
 * 1.排除环回的包
 * 2.存储拓扑结果
 */
void TopoManager::proccessTopoSearch(PriPacket& p) {
    if( ifMyPacket(p) ) {
        p.deletePacket();
        return;
    }
	uint16 len = 0;
	uint8* data = p.getStdStream(&len);
	if( data == 0 || len != 24 )  {
	    p.deletePacket();
		return;
	}
	/* ����������Ϣ */
	SwitchPortInner* port = (SwitchPortInner*)SwitchPort::getSwitchPort(p.getSourcePort());
	if( port ) {
		NMPort* ch = p.getRcvNMPort();
		if( ch ) {
			uint32 uid = (data[12] << 24) | (data[13] << 16) | (data[14] << 8) | (data[15] << 0);
			ch->setUpStreamPortUid(uid);
			ch->setUpStreamIP(&data[16]);
			ch->setUpStreamIpMask(&data[20]);
			ch->resetAgingCounter(); //接收到拓扑包后，重置老化计数
		}
	}
	p.deletePacket();
}

/*
 * 根据收到的拓扑包的源MAC地址，判断是否为环回来的拓扑包
 */
bool TopoManager::ifMyPacket(PriPacket& pkt) {
    uint8 myMAC[6] =  {0};
    DeviceComponent::getDeviceAttribute().getDeviceMac(myMAC);
    uint8* mac = pkt.getSrcMAC();
    for (int i = 0; i < 6; ++i) {
        if( myMAC[i] != mac[i] ) {
            return false;
        }
    }
    return true;
}

/*
 * 维护拓扑的心跳发送，并处理结果的老化
 * 以10s为周期定时发送拓扑包。
 * 以1s为周期维护老化计数，当计数为0时，重置该拓扑结果
 */
TASK void taskTopoAging() {
    uint8 counter = 0;
    uint8 data[4] = {0};
    while( 1 ) {
        os_dly_wait(100);
        counter++;
        if( (counter % 10) == 0 ) {
            //发送拓扑包/10s
            TopoManager::sendTopoPkt();
        }

        //老化拓扑计数
        NMPort* nm = NMPort::getFirstInstance();
        while( nm ) {
            if( nm->AgingTopo() ) {
                nm->setUpStreamIP(data);
                nm->setUpStreamIpMask(data);
                nm->setUpStreamPortUid(0);
            }
            nm = NMPort::getNextInstance(nm->getUID());
       }
     }
}

