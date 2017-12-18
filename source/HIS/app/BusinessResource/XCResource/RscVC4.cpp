/*
 * RscVC4.cpp
 *
 *  Created on: 2013-11-30
 *      Author: Administrator
 */

#include "RscVC4.h"
#include "RscVC12.h"
#include "CPPTools.h"
#include "UID.h"
#include "DriverSDH.h"
#include "CRC7.h"
#include <string.h>

std::map<uint32, RscVC4*> RscVC4::group;

RscVC4::RscVC4(uint32 uid, ConfigCell* card, DriverSDH& chipDriver, VC4_Config_Data_T* cfg, bool paired)
        : ConnectableResource(uid, card, paired), driver(chipDriver) {

    group.insert( std::pair<uint32, RscVC4*>(uid, this));

    ConfigData = cfg;
    UN_Info info = UID::breakUID(uid);
    HID = (info.vc4.stm << 2) + info.vc4.hp;
    ST_SDH_VC12 chinfo;
    chinfo.slot = info.vc4.slot;
    chinfo.xcsn = info.vc4.xcsn;
    chinfo.stm = info.vc4.stm;
    chinfo.hp = info.vc4.hp;
    for( int i = 0; i < 63; i++ ) {
        chinfo.lp = i;
        uint32 chuid = UID::makeUID(&chinfo);
        vc12_obj[i] = new RscVC12(chuid, card, driver, paired);
    }

    setJ1TLen(ConfigData->J1ModeTransmit, false);
    setJ1RLen(ConfigData->J1ModeReceive, false);
    setJ1Transmit(ConfigData->J1Transmit, 62, false);
    setJ1Expected(ConfigData->J1Expect, 62, false);
}

RscVC4::~RscVC4() {
    for( int i = 0; i < 63; i++ ) {
        delete vc12_obj[i];
    }
    group.erase(getUID());
}

int RscVC4::getSpeedLevel(void) {
    return 155;
}
bool RscVC4::setFrom(int fromHID) {
    int rtn = driver.connectVC4(fromHID, getHID());
    if( rtn < 0 ) {
        return false;
    }
    return true;
}
uint32 RscVC4::getHID() {
    return itsHardwareID(getUID());
}

/*
 * Ӳ��ID��ɲ��ԣ�XC���� ��STM4���룬˫���VC4�ֱ�Ϊ0,1,2,3��4,5,6,7
 * ����֧·���ϣ���SDHBUS�Խӵ� SLOT155ͨ��ռ��0,1�������������е�VC4��Դ���Ϊ 2,3,4,5,6,7,8,9
 */
uint8 RscVC4::itsHardwareID(uint32 uid) {
    UN_Info info = UID::breakUID(uid);
    if( info.vc4.slot == 0 || info.vc4.slot == 1 ) {
        return info.vc4.hp + info.vc4.stm * 4;
    }
    else {
        return info.vc4.stm + 2;
    }

}

uint32 RscVC4::getB3Counter(void) {
    return driver.getB3(HID);
}
uint8* RscVC4::getJ1Transmit(uint32* len) {  //read config
    if( getJ1TLen() == 0 ) {
        *len = 16;
    }
    else {
        *len = 64;
    }
    return ConfigData->J1Transmit;
}
uint8* RscVC4::getJ1Transmit(void) {         //read register
    return 0;
}
uint8* RscVC4::getJ1Expected(uint32* len) {  //read config
    if( getJ1TLen() == 0 ) {
        *len = 15;
    }
    else {
        *len = 63;
    }
    return ConfigData->J1Expect;
}
uint8* RscVC4::getJ1Expected(void) {         //read register
    return 0;
}
uint8* RscVC4::getJ1Received(uint32* len) {
    static uint8 j1[64] = {0};
    driver.getJ1R(HID, j1);
    if( getJ1RLen() == 0 ) {
        *len = 15;
    }
    else {
        *len = 63;
    }
    return &j1[1];
}

bool RscVC4::setJ1Transmit(uint8* d, uint32 len, bool save ) {
    uint8 j1[64];
    memset(j1, ' ', sizeof(j1));
    if(len > 63) {
        len = 63;
    }
    memcpy(&j1[1], d, len);
    j1[0] = CRC7::getCRC7(&j1[1], 15) | 0x80;
    if( driver.setJ1T(HID, j1) ) {
        if( save ) {
            memset( ConfigData->J1Transmit, ' ', sizeof(ConfigData->J1Transmit) );
            memcpy( ConfigData->J1Transmit, d, len );
            return saveConfig();
        }
        return true;
    }
    return false;

}
bool RscVC4::setJ1Expected(uint8* d, uint32 len, bool save ) {
    uint8 j1[64];
    memset(j1, ' ', sizeof(j1));
    if(len > 63) {
        len = 63;
    }
    memcpy(&j1[1], d, len);
    j1[0] = CRC7::getCRC7(&j1[1], 15) | 0x80;
    if( driver.setJ1E(HID, j1) ) {
        if( save ) {
            memset( ConfigData->J1Expect, ' ', sizeof(ConfigData->J1Expect) );
            memcpy( ConfigData->J1Expect, d, len );
            return saveConfig();
        }
        return true;
    }
    return false;
}

uint8 RscVC4::getJ1TLen(void) {
    return ConfigData->J1ModeTransmit;

}
bool RscVC4::setJ1TLen(uint8 len, bool save ) {
    if (driver.setJ1LenTransmit(HID, len) ) {
        if( save ) {
            ConfigData->J1ModeTransmit = len;
            return saveConfig();
        }
        return true;
    }
    return false;
}
uint8 RscVC4::getJ1RLen(void) {
    return ConfigData->J1ModeReceive;

}
bool RscVC4::setJ1RLen(uint8 len, bool save ) {
    if (driver.setJ1LenExpected(HID, len) ) {
        if( save ) {
            ConfigData->J1ModeReceive = len;
            return saveConfig();
        }
        return true;
    }
    return false;

}

bool RscVC4::getAUAIS(void) {
    return driver.getAUAIS(HID);
}
bool RscVC4::getAULOP(void) {
    return driver.getAULOP(HID);
}
bool RscVC4::getHPRDI(void) {
    return driver.getHPRDI(HID);
}
bool RscVC4::getHPREI(void) {
    return driver.getHPREI(HID);
}
bool RscVC4::getHPTIM(void) {
    return driver.getHPTIM(HID);
}
bool RscVC4::getHPSLM(void) {
    return driver.getHPSLM(HID);
}
bool RscVC4::getHPUNEQ(void) {
    return driver.getHPUNEQ(HID);
}

