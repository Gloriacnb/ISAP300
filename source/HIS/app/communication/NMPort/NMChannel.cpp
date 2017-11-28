/*
 * NMChannel.cpp
 *
 *  Created on: 2015年10月12日
 *      Author: Administrator
 */

#include "NMChannel.h"
#include <string.h>
#include "PriPacket.h"
#include <stdio.h>
#include "NMPort.h"
#include "SwitchPortInner.h"
#include "SysError.h"

NMChannel::NMChannel(uint32 uid) : Resource(uid) {
    // TODO Auto-generated constructor stub
    belongPort = 0;
    WorkMode = dcc;
}

NMChannel::~NMChannel() {
    // TODO Auto-generated destructor stub
}

bool NMChannel::receivData(void) {
    bool flag = false;
    while( havePacet() ) {
        int pkgLen = getPacketLen();
        if( (pkgLen < 20)  || (pkgLen > getMTU()) ) {
#ifdef EZ_DEBUG
            printf("\nNMChannel %8x len %d error\n", getUID(), pkgLen);
#endif
            discardPacket();
            continue;
        }
        LAYER2FRAME *frame = alloc_Eth_mem (pkgLen-4);
        if (frame != NULL ) {
            uint8* dp = new uint8[pkgLen];
            getPacketData(dp,pkgLen); //一次性全部读出，私有头+frame
            memcpy(frame->frame, dp+4, pkgLen-4);
            Layer2Tag tag;
            tag.sn = (dp[0] << 24) | (dp[1] << 16) | (dp[2] << 8 ) | dp[3];
            memcpy( tag.decMAC, (dp+4), 6);
            memcpy( tag.srcMAC, (dp+10), 6);
            delete []dp;
            PriPacket* p = new PriPacket(tag, belongPort->getInnerPort()->getPortSn(),frame);
            if( p ) {
            	p->recordProcessInfo(21);
#ifdef SW_DEBUG
                printf("NMChannel::receivData at %4d, len %d\n", p->getPrivateTag().sn, frame->length);
#endif
                p->setRcvNMPort(belongPort); //for topo
                if( belongPort->getInnerPort()->inputPacket(p) >= 0 ) {
                	flag = true;
                }
            }
            else {
                printf("\nPriPacket alloc error\n");
//                discardPacket();
//                throw ErrorNullPointer();
            }
        }
        else {
            printf("\nLAYER2FRAME alloc error\n");
//            discardPacket();
//            throw ErrorEthMemFull();
        }
        discardPacket();
    }
    return flag;

}
