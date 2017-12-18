/*
 * PairPortSTM.cpp
 *
 *  Created on: 2014��11��24��
 *      Author: Administrator
 */

#include "PairPortSTM.h"
#include "PortSTM4.h"
#include "PairRscVC4.h"
#include "UID.h"

std::map<uint32, PairPortSTM*> PairPortSTM::group;

PairPortSTM::PairPortSTM(uint32 uid) : PairResource(uid) {

    group.insert( std::pair<uint32, PairPortSTM*>(uid, this));

    UN_Info stminfo = UID::breakUID(uid);
    vc4Number = 1;
    if( UID::getPortTypeByUID(uid) == UID::type_SDH_622 ) {
        vc4Number = 4;
    }
    ST_SDH_VC4 info;
    info.slot = stminfo.stm.slot;
    info.xcsn = stminfo.stm.xcsn;
    info.stm = stminfo.stm.stm;
    for( int i = 0; i < vc4Number; i++ ) {
        info.hp = i;
        uint32 vc4UID = UID::makeUID(&info);
        vc4_obj[i] = new PairRscVC4(vc4UID);
    }

}

PairPortSTM::~PairPortSTM() {
    for( int i = 0; i < vc4Number; i++ ) {
        delete vc4_obj[i];
    }
    group.erase(getUID());
}


uint8* PairPortSTM::getJ0Transmit(uint32* len) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first ) {
        return p.first->getJ0Transmit(len);
    }
    if( p.second ) {
        return p.second->getJ0Transmit(len);
    }
    return 0;
}
uint8* PairPortSTM::getJ0Expected(uint32* len) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first ) {
        return p.first->getJ0Expected(len);
    }
    if( p.second ) {
        return p.second->getJ0Expected(len);
    }
    return 0;
}

bool PairPortSTM::setJ0Transmit(uint8* d, uint32 len) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first && p.second ) {
        return p.first->setJ0Transmit(d, len) &&
                p.second->setJ0Transmit(d,len);
    }
    else if( p.second ) {
        return p.second->setJ0Transmit(d, len);
    }
    else if( p.first ) {
        return p.first->setJ0Transmit(d,len);
    }
    return false;
}
bool PairPortSTM::setJ0Expected(uint8* d, uint32 len) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first && p.second ) {
        return p.first->setJ0Expected(d, len) &&
                p.second->setJ0Expected(d,len);
    }
    else if( p.second ) {
        return p.second->setJ0Expected(d, len);
    }
    else if( p.first ) {
        return p.first->setJ0Expected(d,len);
    }
    return false;
}

uint8 PairPortSTM::getJ0TLen(void) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first ) {
        return p.first->getJ0TLen();
    }
    if( p.second ) {
        return p.second->getJ0TLen();
    }
    return 0;
}
bool PairPortSTM::setJ0TLen(uint8 len) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first && p.second ) {
        return p.first->setJ0TLen(len) &&
                p.second->setJ0TLen(len);
    }
    else if( p.second ) {
        return p.second->setJ0TLen(len);
    }
    else if( p.first ) {
        return p.first->setJ0TLen(len);
    }
    return false;
}
uint8 PairPortSTM::getJ0RLen(void) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first ) {
        return p.first->getJ0RLen();
    }
    if( p.second ) {
        return p.second->getJ0RLen();
    }
    return 0;

}
bool PairPortSTM::setJ0RLen(uint8 len) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first && p.second ) {
        return p.first->setJ0RLen(len) &&
                p.second->setJ0RLen(len);
    }
    else if( p.second ) {
        return p.second->setJ0RLen(len);
    }
    else if( p.first ) {
        return p.first->setJ0RLen(len);
    }
    return false;
}

//D1~D12����ѡ��
uint16 PairPortSTM::getDCCSel(void) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first ) {
        return p.first->getDCCSel();
    }
    if( p.second ) {
        return p.second->getDCCSel();
    }
    return 0;

}
bool PairPortSTM::setDCCSel(uint16 v) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first && p.second ) {
        return p.first->setDCCSel(v) &&
                p.second->setDCCSel(v);
    }
    else if( p.second ) {
        return p.second->setDCCSel(v);
    }
    else if( p.first ) {
        return p.first->setDCCSel(v);
    }
    return false;
}

//normal configuration
uint8 PairPortSTM::getPortEnable(void) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first ) {
        return p.first->getPortEnable();
    }
    if( p.second ) {
        return p.second->getPortEnable();
    }
    return 0;
}
bool PairPortSTM::setPortEnable(uint8 en) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first && p.second ) {
        return p.first->setPortEnable(en) &&
                p.second->setPortEnable(en);
    }
    else if( p.second ) {
        return p.second->setPortEnable(en);
    }
    else if( p.first ) {
        return p.first->setPortEnable(en);
    }
    return false;

}

uint8 PairPortSTM::getAlsEnable(void) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first ) {
        return p.first->getAlsEnable();
    }
    if( p.second ) {
        return p.second->getAlsEnable();
    }
    return 0;
}
bool PairPortSTM::setAlsEnable(uint8 als) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first && p.second ) {
        return p.first->setAlsEnable(als) &&
                p.second->setAlsEnable(als);
    }
    else if( p.second ) {
        return p.second->setAlsEnable(als);
    }
    else if( p.first ) {
        return p.first->setAlsEnable(als);
    }
    return false;
}

std::string PairPortSTM::getDescription(void) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first ) {
        return p.first->getDescription();
    }
    if( p.second ) {
        return p.second->getDescription();
    }
    return 0;
}
bool PairPortSTM::setDescription(char* buf, uint8 len) {
    std::pair<PortSTM4*, PortSTM4*> p = getParts<PortSTM4>();
    if( p.first && p.second ) {
        return p.first->setDescription(buf, len) &&
                p.second->setDescription(buf, len);
    }
    else if( p.second ) {
        return p.second->setDescription(buf, len);
    }
    else if( p.first ) {
        return p.first->setDescription(buf, len);
    }
    return false;
}
