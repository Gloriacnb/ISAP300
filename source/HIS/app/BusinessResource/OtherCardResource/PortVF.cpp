/*
 * PortVF.cpp
 *
 *  Created on: 2014-2-21
 *      Author: Administrator
 */

#include "PortVF.h"
#include "PairTSChannel.h"
#include "UID.h"
#include "CardVF.h"
#include "VFCmdTSLoop.h"
#include "VFCmdTsType.h"
#include "VFCmdTSEnable.h"
#include "VFCmdTSMode.h"
#include "VFcmdTSPolarTurn.h"
//#include "VFCmdTSRcvGain.h"
//#include "VFCmdTSSndGain.h"
#include "VFCmdSignalConfig.h"
#include "VFCmdSignalStatus.h"
#include "CPPTools.h"
#include <string.h>

std::map<uint32, PortVF*> PortVF::group;

PortVF::PortVF(uint32 uid, PairTSChannel* t, VFPort_Config_T* config, VFGroup_Config_T* gconfig, CardVF* card) : RealPortBase(uid, card){


    group.insert( std::pair<uint32, PortVF*>(uid, this));

    ts = t;
    if( ts ) {
        ts->setDownStream(uid);
    }
    ConfigData = config;
    ConfigDataG = gconfig;
    BelongCard = card;


    setEnable(ConfigData->enable, false);
    setWorkMode(ConfigData->mode, false);
    setPolarTurn(ConfigData->Polarturn, false);
    setSignalConfig((ConfigData->offHookSig<<4) | ConfigData->onHookSig, false);
//    PortType = readPortType();
//    if( PortType == DEF_VFType_4W || PortType == DEF_VFType_2W ) {
//        setRcvGain(ConfigDataG->rcvgain, false);
//        setSndGain(ConfigDataG->sndGain, false);
//    }
}

PortVF::~PortVF() {
    if( ts ) {
        ts->setDownStream(0);
    }
    group.erase(getUID());
}

int PortVF::setLoop(const int type) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdTSLoop* cmd = new VFCmdTSLoop(tsn, type);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    return rtn;
}

char* PortVF::getDescription(uint32* len) {
    if( len ) {
        *len = ConfigData->dlen;
    }
    return ConfigData->desc;
}
bool PortVF::setDescription(char* s, uint32 len, bool save) {
    memcpy( ConfigData->desc, s, len );
    ConfigData->dlen = len;
    if( save ) {
        return saveConfig();
    }
    return true;
}

int PortVF::getPortType(void) {
    return PortType;
}

int PortVF::readPortType(void) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdTsType* cmd = new VFCmdTsType(tsn);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    return rtn;

}

int PortVF::getEnable(void) {
    return ConfigData->enable;
}
bool PortVF::setEnable(int en, bool save) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdTSEnable* cmd = new VFCmdTSEnable(tsn, en);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    if( rtn == 0x5a && save == true ) {
        ConfigData->enable = en;
        return BelongCard->saveConfig();
    }
    return false;
}
int PortVF::getWorkMode(void) {
    return ConfigData->mode;
}
bool PortVF::setWorkMode(int md, bool save) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdTSMode* cmd = new VFCmdTSMode(tsn, md);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    if( rtn == 0x5a && save == true ) {
        ConfigData->mode = md;
        return BelongCard->saveConfig();
    }
    return false;
}
int PortVF::getPolarTurn(void) {
    return ConfigData->Polarturn;
}
bool PortVF::setPolarTurn(int polar, bool save) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdTSPolarTurn* cmd = new VFCmdTSPolarTurn(tsn, polar);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    if( rtn == 0x5a && save == true ) {
        ConfigData->Polarturn = polar;
        return BelongCard->saveConfig();
    }
    return false;
}

//uint8 PortVF::getRcvGain() {
//    return ConfigDataG->rcvgain;
//}
//uint8 PortVF::getSndGain() {
//    return ConfigDataG->sndGain;
//}
//bool PortVF::setRcvGain(uint8 g, bool save) {
//    uint8 tsn = (uint8)ts->getHID();
//    VFCmdTSRcvGain* cmd = new VFCmdTSRcvGain(tsn, g);
//    BelongCard->processVFCommand(*cmd);
//    int rtn = *(cmd->getResult());
//    delete cmd;
//    if( !save ) {
//        return true;
//    }
//    if( rtn == 0x5a ) {
//        ConfigDataG->rcvgain = g;
//        return BelongCard->saveConfig();
//    }
//    return false;
//}
//bool PortVF::setSndGain(uint8 g, bool save) {
//    uint8 tsn = (uint8)ts->getHID();
//    VFCmdTSSndGain* cmd = new VFCmdTSSndGain(tsn, g);
//    BelongCard->processVFCommand(*cmd);
//    int rtn = *(cmd->getResult());
//    delete cmd;
//    if( !save ) {
//        return true;
//    }
//    if( rtn == 0x5a ) {
//        ConfigDataG->sndGain = g;
//        return BelongCard->saveConfig();
//    }
//    return false;
//}

uint8 PortVF::getSignalConfig(void) {
    return (ConfigData->offHookSig << 4) | ConfigData->onHookSig;
}
bool PortVF::setSignalConfig(uint8 sig, bool save) {
    uint8 offhook = sig >> 4;
    uint8 onhook = sig & 0xf;
    uint8 tsn = (uint8)ts->getHID();
    VFCmdSignalConfig* cmd = new VFCmdSignalConfig(tsn, offhook, onhook);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    if( !save ) {
        return true;
    }
    if( rtn == 0x5a ) {
        ConfigData->offHookSig = offhook;
        ConfigData->onHookSig = onhook;
        return BelongCard->saveConfig();
    }
    return false;

}

uint8 PortVF::getSignalRcved(void) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdSignalStatus* cmd = new VFCmdSignalStatus(tsn);
    BelongCard->processVFCommand(*cmd);
    uint8 rtn = *(cmd->getResult());
    delete cmd;
    return rtn;
}
