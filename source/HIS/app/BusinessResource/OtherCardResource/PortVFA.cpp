/*
 * PortVFAA.cpp
 *
 *  Created on: 2018年1月19日
 *      Author: Administrator
 */

#include <PortVFA.h>
#include "VFCmdTSRcvGain.h"
#include "VFCmdTSSndGain.h"
#include "PairTSChannel.h"
#include "CardVF.h"

PortVFA::PortVFA(uint32 uid, PairTSChannel* t, VFPort_Config_T* config, VFGroup_Config_T* gconfig, CardVF* card)
				: PortVF(uid, t, config, gconfig, card) {
	// TODO Auto-generated constructor stub
	ts = t;
//    PortType = readPortType();
//    if( PortType == DEF_VFType_4W || PortType == DEF_VFType_2W ) {
        setRcvGain(ConfigDataG->rcvgain, false);
        setSndGain(ConfigDataG->sndGain, false);
//    }

}

PortVFA::~PortVFA() {
	// TODO Auto-generated destructor stub
}

uint8 PortVFA::getRcvGain() {
    return ConfigDataG->rcvgain;
}
uint8 PortVFA::getSndGain() {
    return ConfigDataG->sndGain;
}
bool PortVFA::setRcvGain(uint8 g, bool save) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdTSRcvGain* cmd = new VFCmdTSRcvGain(tsn, g);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    if( !save ) {
        return true;
    }
    if( rtn == 0x5a ) {
        ConfigDataG->rcvgain = g;
        return BelongCard->saveConfig();
    }
    return false;
}
bool PortVFA::setSndGain(uint8 g, bool save) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdTSSndGain* cmd = new VFCmdTSSndGain(tsn, g);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    if( !save ) {
        return true;
    }
    if( rtn == 0x5a ) {
        ConfigDataG->sndGain = g;
        return BelongCard->saveConfig();
    }
    return false;
}
