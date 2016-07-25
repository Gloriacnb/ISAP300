/*
 * SwitchPort.cpp
 *
 *  Created on: 2013-6-24
 *      Author: Administrator
 */

#include "SwitchPort.h"
#include "SwitchCore.h"
#include "SoftSwitch_define.h"
#include "PriPacket.h"
#include <iostream>
#include "SwitchCore.h"
#include "SysError.h"
#include <stdio.h>
#include "SoftWDT.h"

std::map<uint32, SwitchPort*> SwitchPort::portGroup;

TASK void taskPortSend(void*);

SwitchPort::SwitchPort(uint32 portSn) {
    os_mbx_init(&mbx_send, sizeof(mbx_send));
	sn = portSn;
	portGroup.insert( std::pair<uint32, SwitchPort*>(sn, this) );

	uint8 pri = (portSn < 3) ? (P_PORT_Send) : (P_PORT_Send-1); //两个标准口的优先级高
	task_send_packet = os_tsk_create_ex(taskPortSend, pri, this);
}

SwitchPort::~SwitchPort() {
	portGroup.erase(sn);
	os_tsk_delete(task_send_packet);
}


int SwitchPort::inputPacket(PriPacket* f) {
    if( f ) {
#ifdef SW_DEBUG
    printf("port inputPacket %d at %d\n", f->getPrivateTag().sn, getPortSn());
#endif
        return SwitchCore::instance().inputAPacket(f);
    }
    return -1;
}

int SwitchPort::broadcastThePacket( PriPacket& pkt ) {
    if( portGroup.size() == 0 ) {
        return -1;
    }
    pkt.setRefenceCounter(portGroup.size());
	std::map<uint32, SwitchPort*>::iterator it = portGroup.begin();
	while( it != portGroup.end() ) {
        it->second->outputAPacket(pkt);
		it++;
	}
	return 1;
}
/*
 * ���һ����ݰ����뷢�Ͷ���,�����ں˵��ô˺���
 */
int SwitchPort::outputAPacket(PriPacket& pkg) {
#ifdef SW_DEBUG
    printf("port outputAPacket %d at %d\n", pkg.getPrivateTag().sn, getPortSn());
#endif
    if( os_mbx_check(&mbx_send) != 0 ) {
        os_mbx_send(&mbx_send, &pkg, 0xffff);
    }
    else {
#ifdef SW_DEBUG
        printf("port %d output buf is full\n", getPortSn());
#endif
        pkg.deletePacket();
    }
    return 1;
}


bool SwitchPort::sendAPacket(PriPacket* pkg) {
    if( (pkg->packetType() == broadcast) && ifDropThePacket(*pkg) ) {
#ifdef SW_DEBUG
        printf("pkt %d droped at swport %d\n", pkg->getPrivateTag().sn, getPortSn());
#endif
        return false;
    }
    int rtn = outputPacket(*pkg);
#ifdef SW_DEBUG
    printf("pkt %d send over at %d", pkg->getPrivateTag().sn, getPortSn());
    if( rtn < 0 ) {
        printf(" failed!\n");
    }
    else {
        printf(" succeed!\n");
    }
#endif
    if( rtn < 0 ) {
        return false;
    }
    return true;
}

TASK void taskPortSend(void* p) {
    SwitchPort* port = reinterpret_cast<SwitchPort*>(p);
    void* msg = 0;
    WDT_Para_T sw;
    SoftWDT::instance().add(os_tsk_self(), &sw);
    while( 1 ) {
        OS_RESULT rst = os_mbx_wait(port->getMyMbx(), &msg, 100);
        if( rst != OS_R_TMO ) {
            PriPacket* p = reinterpret_cast<PriPacket*>(msg);
            if( p ) {
                port->sendAPacket( p );
                p->deletePacket();
            }
        }
        SoftWDT::instance().feed(os_tsk_self());
    }
}

