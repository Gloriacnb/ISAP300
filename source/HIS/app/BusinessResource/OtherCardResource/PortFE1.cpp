/*
 * PortFE1.cpp
 *
 *  Created on: 2015年6月22日
 *      Author: Administrator
 */

#include "PortFE1.h"
#include "SysError.h"
#include "UID.h"
#include "DriverPCM.h"
#include <string.h>

std::map<uint32, PortFE1*> PortFE1::group;

PortFE1::PortFE1(uint32 uid, ConfigCell* card, FE1_Config_Data_T* config,
        DriverPCM& dri, bool paired) :
        RealPortBase(uid, card, paired), driver(dri) {
    if( !paired ) { //只有扩展卡才加入map
        group.insert(std::pair<uint32, PortFE1*>(uid, this));
    }
    ConfigData = config;
    setPortEnable(ConfigData->TXEN, false);
}

PortFE1::~PortFE1() {
    group.erase(getUID());
}


int PortFE1::setLoop(const int type){
    UN_Info e1info = UID::breakUID(getUID());
    uint8 ch = e1info.E1.E1;
    switch( type ) {
    case dev_and_line_loop:
        driver.setPortLoopOut(ch, 1);
        driver.setPortLoopIn(ch, 1);
        break;
    case line_loop:
        driver.setPortLoopOut(ch, 1);
        driver.setPortLoopIn(ch, 0);
        break;
    case dev_loop:
        driver.setPortLoopOut(ch, 0);
        driver.setPortLoopIn(ch, 1);
        break;
    case no_loop:
        driver.setPortLoopOut(ch, 0);
        driver.setPortLoopIn(ch, 0);
        break;
    default:
        return -1;
    }
    return 0x5A;
}

uint8 PortFE1::getPortEnable(void) {
    return ConfigData->TXEN;
}
bool PortFE1::setPortEnable(uint8 en, bool save) {
    UN_Info e1info = UID::breakUID(getUID());
    uint8 ch = e1info.E1.E1;
    driver.setPortEnable(ch, en);
    if( save ) {
        ConfigData->TXEN = en;
        return saveConfig();
    }
    return true;
}

char* PortFE1::getDescription(uint32* len) {
    if( len ) {
        *len = ConfigData->DescLen;
    }
    return ConfigData->Desc;
}
bool PortFE1::setDescription(char* s, uint32 len, bool save) {
    memcpy( ConfigData->Desc, s, len );
    ConfigData->DescLen = len;
    if( save ) {
        return saveConfig();
    }
    return true;
}

