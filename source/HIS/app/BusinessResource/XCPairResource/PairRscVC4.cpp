/*
 * PairRscVC4.cpp
 *
 *  Created on: 2014��11��25��
 *      Author: Administrator
 */

#include "PairRscVC4.h"
#include "UID.h"
#include "RscVC4.h"
#include "PairRscVC12.h"

std::map<uint32, PairRscVC4*> PairRscVC4::group;

PairRscVC4::PairRscVC4(uint32 uid) : PairRscConnectable(uid) {

    group.insert( std::pair<uint32, PairRscVC4*>(uid, this));

    UN_Info info = UID::breakUID(uid);
    ST_SDH_VC12 chinfo;
    chinfo.slot = info.vc4.slot;
    chinfo.xcsn = info.vc4.xcsn;
    chinfo.stm = info.vc4.stm;
    chinfo.hp = info.vc4.hp;
    for (int i = 0; i < 63; ++i) {
        chinfo.lp = i;
        uint32 chuid = UID::makeUID(&chinfo);
        vc12_obj[i] = new PairRscVC12(chuid);
    }

}

PairRscVC4::~PairRscVC4() {
    for (int i = 0; i < 63; ++i) {
        delete vc12_obj[i];
    }
    group.erase(getUID());
}

uint8* PairRscVC4::getJ1Transmit(uint32* len) {
    std::pair<RscVC4*, RscVC4*> p = getPairTool().getParts<RscVC4>();
    if( p.first ) {
        return p.first->getJ1Transmit(len);
    }
    if( p.second ) {
        return p.second->getJ1Transmit(len);
    }
    return 0;
}
uint8* PairRscVC4::getJ1Expected(uint32* len) {
    std::pair<RscVC4*, RscVC4*> p = getPairTool().getParts<RscVC4>();
    if( p.first ) {
        return p.first->getJ1Expected(len);
    }
    if( p.second ) {
        return p.second->getJ1Expected(len);
    }
    return 0;
}

bool PairRscVC4::setJ1Transmit(uint8* d, uint32 len) {
    std::pair<RscVC4*, RscVC4*> p = getPairTool().getParts<RscVC4>();
    if( p.first && p.second ) {
        return p.first->setJ1Transmit(d, len) &&
                p.second->setJ1Transmit(d,len);
    }
    else if( p.second ) {
        return p.second->setJ1Transmit(d, len);
    }
    else if( p.first ) {
        return p.first->setJ1Transmit(d,len);
    }
    return false;

}
bool PairRscVC4::setJ1Expected(uint8* d, uint32 len) {
    std::pair<RscVC4*, RscVC4*> p = getPairTool().getParts<RscVC4>();
    if( p.first && p.second ) {
        return p.first->setJ1Expected(d, len) &&
                p.second->setJ1Expected(d,len);
    }
    else if( p.second ) {
        return p.second->setJ1Expected(d, len);
    }
    else if( p.first ) {
        return p.first->setJ1Expected(d,len);
    }
    return false;

}

uint8 PairRscVC4::getJ1TLen(void) {
    std::pair<RscVC4*, RscVC4*> p = getPairTool().getParts<RscVC4>();
    if( p.first ) {
        return p.first->getJ1TLen();
    }
    if( p.second ) {
        return p.second->getJ1TLen();
    }
    return 0;

}
bool PairRscVC4::setJ1TLen(uint8 len) {
    std::pair<RscVC4*, RscVC4*> p = getPairTool().getParts<RscVC4>();
    if( p.first && p.second ) {
        return p.first->setJ1TLen(len) &&
                p.second->setJ1TLen(len);
    }
    else if( p.second ) {
        return p.second->setJ1TLen(len);
    }
    else if( p.first ) {
        return p.first->setJ1TLen(len);
    }
    return false;
}
uint8 PairRscVC4::getJ1RLen(void) {
    std::pair<RscVC4*, RscVC4*> p = getPairTool().getParts<RscVC4>();
    if( p.first ) {
        return p.first->getJ1RLen();
    }
    if( p.second ) {
        return p.second->getJ1RLen();
    }
    return 0;
}
bool PairRscVC4::setJ1RLen(uint8 len) {
    std::pair<RscVC4*, RscVC4*> p = getPairTool().getParts<RscVC4>();
    if( p.first && p.second ) {
        return p.first->setJ1RLen(len) &&
                p.second->setJ1RLen(len);
    }
    else if( p.second ) {
        return p.second->setJ1RLen(len);
    }
    else if( p.first ) {
        return p.first->setJ1RLen(len);
    }
    return false;

}
