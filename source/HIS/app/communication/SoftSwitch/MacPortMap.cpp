/*
 * MacPortMap.cpp
 *
 *  Created on: 2013-6-25
 *      Author: Administrator
 */

#include "MacPortMap.h"
#include "PriPacket.h"

#include <rtl.h>
#include <stdio.h>

OS_MUT mut_portmac;

MacPortMap::MacPortMap() {
	// TODO Auto-generated constructor stub
	os_mut_init(mut_portmac);
}

MacPortMap::~MacPortMap() {
	// TODO Auto-generated destructor stub
}

void MacPortMap::clear(void) {
    os_mut_wait(mut_portmac, 0xffff);
    macPortTable.clear();
    os_mut_release(mut_portmac);

}

void MacPortMap::learnAddress(PriPacket& pkt) {
	MACAddr m(pkt.getPrivateTag().srcMAC);
	PortRecord r(pkt.getSourcePort());
	os_mut_wait(mut_portmac, 0xffff);
	std::map<MACAddr, PortRecord>::iterator it = macPortTable.find(m);
	if( it != macPortTable.end() ) {
	    //已有记录，则判断变化
	    if( it->second.portSn != r.portSn ) {
	        //有变化
	        if( it->second.reduce() ) { //降低原记录权重
	            //权重降低到阈值，则替换原记录
#ifdef SW_DEBUG
		FILE* f = fopen("macport.txt", "a");
                fprintf(f, "%8dMACAddress %02x-%02x-%02x-%02x-%02x-%02x changed from %d to %d\n",
                        os_time_get(), m.mac[0],m.mac[1],m.mac[2],m.mac[3],m.mac[4],m.mac[5],
                        it->second.portSn, r.portSn);
        fclose(f);
#endif
                it->second = r;
	        }
	    }
	    else {
	        //无变化
	        it->second.advance(); //增加原记录权重
	        it->second.AgingCounter = MacPortMap::LiveSecs; //更新老化计数

	    }
	}
	else {
	    //无记录，则新增记录
		macPortTable.insert( std::pair<MACAddr, PortRecord>(m, r));
//#ifdef EZ_DEBUG
//        printf("MACAddress %02x-%02x-%02x-%02x-%02x-%02x detected at port %d\n",
//                m.mac[0],m.mac[1],m.mac[2],m.mac[3],m.mac[4],m.mac[5],
//                r.portSn);
//#endif
	}
	os_mut_release(mut_portmac);
}

int MacPortMap::findOutputPort(PriPacket& pkt) {
	MACAddr m(pkt.getPrivateTag().decMAC);
	os_mut_wait(mut_portmac, 0xffff);
	std::map<MACAddr, PortRecord>::iterator it = macPortTable.find(m);
	if( it != macPortTable.end() ) {
	    os_mut_release(mut_portmac);
		return it->second.portSn;
	}
	os_mut_release(mut_portmac);
	return -1;
}


void MacPortMap::aging(void) {
	os_mut_wait(mut_portmac, 0xffff);
	std::map<MACAddr, PortRecord>::iterator it = macPortTable.begin();
	while( it != macPortTable.end() ) {
		if( it->second.AgingCounter == 0 ) {
			macPortTable.erase(it++);
		}
		else{
			--(it->second.AgingCounter);
            ++it;
		}
	}
	os_mut_release(mut_portmac);
}

void MacPortMap::Print(void) {
    os_mut_wait(mut_portmac, 0xffff);
    std::map<MACAddr, PortRecord>::iterator it = macPortTable.begin();
    while( it != macPortTable.end() ) {
        printf("MACAddress %02x-%02x-%02x-%02x-%02x-%02x at port %d\n",
                it->first.mac[0],it->first.mac[1],it->first.mac[2],it->first.mac[3],it->first.mac[4],it->first.mac[5],
                it->second.portSn);
        ++it;
    }
    os_mut_release(mut_portmac);
}
