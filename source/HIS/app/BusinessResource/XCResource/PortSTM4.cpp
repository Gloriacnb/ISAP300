/*
 * PortSTM4.cpp
 *
 *  Created on: 2013-11-29
 *      Author: Administrator
 */

#include "PortSTM4.h"
#include "UID.h"
#include "CPPTools.h"
#include "DriverSDH.h"
#include "CRC7.h"
#include <string.h>
#include "DriverPCM.h"
#include "RscVC4.h"

std::map<uint32, PortSTM4*> PortSTM4::group;


PortSTM4::PortSTM4(uint32 uid, ConfigCell* card, DriverSDH& driversdh, STM_Config_Data_T* cfg, bool paired )
        : RealPortBase(uid, card, paired) , driver(driversdh) {

    ConfigData = cfg;
    group.insert( std::pair<uint32, PortSTM4*>(uid, this));



    UN_Info stminfo = UID::breakUID(uid);
    hid = stminfo.stm.stm;
    if( UID::getPortTypeByUID(uid) == UID::type_SDH_155 ) {
        vc4Number = 1;
    }
    else {
        vc4Number = 4;
    }
    ST_SDH_VC4 info;
    info.slot = stminfo.stm.slot;
    info.xcsn = stminfo.stm.xcsn;
    info.stm = hid;
    for( int i = 0; i < vc4Number; i++ ) {
        info.hp = i;
        uint32 vc4UID = UID::makeUID(&info);
        vc4_obj[i] = new RscVC4(vc4UID, card, driver, &ConfigData->RscVC4[i], paired);
    }

    setJ0RLen(ConfigData->J0ModeReceive, false);
    setJ0TLen(ConfigData->J0ModeTransmit, false);
    setJ0Transmit(ConfigData->J0Transmit, 15, false);
    setJ0Expected(ConfigData->J0Expect, 15, false);
    setDCCSel(ConfigData->DCCSel, false);
    setPortEnable(ConfigData->TXEN, false);

}

PortSTM4::~PortSTM4() {
    for( int i = 0; i < vc4Number; i++ ) {
        delete vc4_obj[i];
    }
    group.erase(getUID());

}

int PortSTM4::setLoop(const int type) {
    switch( type ) {
    case dev_and_line_loop:
        driver.setLoopInAndOut(hid);
        break;
    case dev_loop:
        driver.setLoopInOnly(hid);
        break;
    case line_loop:
        driver.setLoopOutOnly(hid);
        break;
    case no_loop:
        driver.setLoopClear(hid);
        break;
    }
    return 0x5A;
}

uint32 PortSTM4::getB1Counter(void) {
    return driver.getB1(hid);
}
uint32 PortSTM4::getB2Counter(void) {
    return driver.getB2(hid);

}
bool PortSTM4::getLos(bool direct) {
    if( !direct ) {
        if( getPortEnable() == 0 ) {
            return false;
        }
    }
    return driver.getLOS(hid);
}

bool PortSTM4::getLof(void) {
    if( getPortEnable() == 0 ) {
        return false;
    }
    return driver.getLOF(hid);
}
bool PortSTM4::getOof(void) {
    if( getPortEnable() == 0 ) {
        return false;
    }
    return driver.getOOF(hid);
}
bool PortSTM4::getMsAis(void) {
    if( getPortEnable() == 0 ) {
        return false;
    }
    return driver.getMSAIS(hid);
}
bool PortSTM4::getMsRdi(void) {
    if( getPortEnable() == 0 ) {
        return false;
    }
    return driver.getMSRDI(hid);
}
bool PortSTM4::getMsRei(void) {
    if( getPortEnable() == 0 ) {
        return false;
    }
    return driver.getMSREI(hid);
}


uint8* PortSTM4::getJ0Transmit(uint32* len) {  //read config
    if( getJ0TLen() == 0 ) {
        *len = 1;
    }
    else {
        *len = 15;
    }
    return ConfigData->J0Transmit;
}
uint8* PortSTM4::getJ0Transmit(void) {         //read register
    static uint8 j0[16] = {0};
    driver.getJ0T(hid, j0);
    return &j0[1];
}
uint8* PortSTM4::getJ0Expected(uint32* len) {  //read config
    if( getJ0RLen() == 0 ) {
        *len = 1;
    }
    else {
        *len = 15;
    }
    return ConfigData->J0Expect;
}
uint8* PortSTM4::getJ0Expected(void) {         //read register
    static uint8 j0[16] = {0};
    driver.getJ0E(hid, j0);
    return &j0[1];
}
uint8* PortSTM4::getJ0Received(uint32* len) {
    static uint8 j0[16] = {0};
    driver.getJ0R(hid, j0);
    if( getJ0RLen() == 0 ) {
        *len = 1;
        return &j0[0];
    }
    else {
        *len = 15;
        return &j0[1];
    }
}

bool PortSTM4::setJ0Transmit(uint8* d, uint32 len, bool save ) {
    uint8 j0[16];
    memset(j0, ' ', sizeof(j0));
    if(len > 15) {
        len = 15;
    }
    if( getJ0TLen() == 0 ) {
        if( driver.setSingleByteJ0T(hid, d[0]) ) {
            if( save ) {
                memset( ConfigData->J0Transmit, 0, sizeof(ConfigData->J0Expect) );
                ConfigData->J0Transmit[0] = d[0];
                return saveConfig();
            }
            return true;
        }
        else {
            return false;
        }
    }
    memcpy(&j0[1], d, len);
    j0[0] = CRC7::getCRC7(&j0[1], 15) | 0x80;
    if( driver.setJ0T(hid, j0) ) {
        if( save ) {
            memset( ConfigData->J0Transmit, ' ', sizeof(ConfigData->J0Transmit) );
            memcpy( ConfigData->J0Transmit, d, len );
            return saveConfig();
        }
        return true;
    }
    return false;
}

bool PortSTM4::setJ0Expected(uint8* d, uint32 len, bool save ) {
    uint8 j0[16];
    memset(j0, ' ', sizeof(j0));
    if(len > 15) {
        len = 15;
    }
    memcpy(&j0[1], d, len);
    j0[0] = CRC7::getCRC7(&j0[1], 15) | 0x80;
    if( driver.setJ0E(hid, j0) ) {
        if( save ) {
            memset( ConfigData->J0Expect, ' ', sizeof(ConfigData->J0Expect) );
            memcpy( ConfigData->J0Expect, d, len );
            return saveConfig();
        }
        return true;
    }
    return false;
}

uint8 PortSTM4::getJ0TLen(void) {
    return ConfigData->J0ModeTransmit;
}


bool PortSTM4::setJ0TLen(uint8 len, bool save) {
    if (driver.setJ0LenTransmit(hid, len) ) {
        if( save ) {
            ConfigData->J0ModeTransmit = len;
            return saveConfig();
        }
        return true;
    }
    return false;
}

uint8 PortSTM4::getJ0RLen(void) {
    return ConfigData->J0ModeReceive;
}

bool PortSTM4::setJ0RLen(uint8 len, bool save ) {
    if (driver.setJ0LenExpected(hid, len) ) {
        if( save ) {
            ConfigData->J0ModeReceive = len;
            return saveConfig();
        }
        return true;
    }
    return false;
}

uint16 PortSTM4::getDCCSel(void) {
    return ConfigData->DCCSel;
}
bool PortSTM4::setDCCSel(uint16 v, bool save) {
    if( driver.setDCCOH(hid, v) ) {
        if( save ) {
            ConfigData->DCCSel = v;
            return saveConfig();
        }
        return true;
    }
    return false;

}

uint8 PortSTM4::getPortEnable(void) {
    return ConfigData->TXEN;
}
bool PortSTM4::setPortEnable(uint8 en, bool save) {
    if( driver.setSTMPortTxEn(hid, en) ) {
        if( save ) {
            ConfigData->TXEN = en;
            return saveConfig();
        }
        return true;
    }
    return false;
}

uint8 PortSTM4::getAlsEnable(void) {
    return ConfigData->ALSEnable;
}
bool PortSTM4::setAlsEnable(uint8 als, bool save) {
    if( driver.setSTMAlsEn(hid, als) ) {
        if( save ) {
            ConfigData->ALSEnable = als;
            return saveConfig();
        }
        return true;
    }
    return false;
}

std::string PortSTM4::getDescription(void) {
    std::string desc(ConfigData->Desc, ConfigData->DescLen);
    return desc;
}
bool PortSTM4::setDescription(char* buf, uint8 len, bool save) {
    if( save ) {
        memcpy(ConfigData->Desc, buf, len);
        ConfigData->DescLen = len;
        return saveConfig();
    }
    return true;
}
