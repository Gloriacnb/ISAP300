/*
 * PortVFG.cpp
 *
 *  Created on: 2018年1月19日
 *      Author: Administrator
 */

#include <PortVFG.h>
#include "VFCmdTSRcvGain.h"
#include "VFCmdTSSndGain.h"
#include "PairTSChannel.h"
#include "CardVF.h"

PortVFG::PortVFG(uint32 uid, PairTSChannel* t, VFPort_Config_T* config, VFGroup_Config_T* gconfig, CardVF* card)
				: PortVF(uid, t, config, gconfig, card) {
	// TODO Auto-generated constructor stub
	setRcvGain(ConfigData->rcvgain, false);
	setSndGain(ConfigData->sndGain, false);

}

PortVFG::~PortVFG() {
	// TODO Auto-generated destructor stub
}

uint8 PortVFG::getRcvGain() {
    return ConfigData->rcvgain;
}
uint8 PortVFG::getSndGain() {
    return ConfigData->sndGain;
}
bool PortVFG::setRcvGain(uint8 g, bool save) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdTSRcvGain* cmd = new VFCmdTSRcvGain(tsn, g);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    if( !save ) {
        return true;
    }
    if( rtn == 0x5a ) {
        ConfigData->rcvgain = g;
        return BelongCard->saveConfig();
    }
    return false;
}
bool PortVFG::setSndGain(uint8 g, bool save) {
    uint8 tsn = (uint8)ts->getHID();
    VFCmdTSSndGain* cmd = new VFCmdTSSndGain(tsn, g);
    BelongCard->processVFCommand(*cmd);
    int rtn = *(cmd->getResult());
    delete cmd;
    if( !save ) {
        return true;
    }
    if( rtn == 0x5a ) {
        ConfigData->sndGain = g;
        return BelongCard->saveConfig();
    }
    return false;
}
