/*
 * CardVF.cpp
 *
 *  Created on:
 *      Author: Administrator
 */

#include "CardVF.h"
#include "PortVFA.h"
#include "PortVFG.h"
#include "UID.h"
#include "PairTSChannel.h"
#include "CardTypeID_define.h"
#include "CBaseSlot.h"
#include "VFCommand.h"
#include <string.h>
#include "SysError.h"
#include <stdio.h>
#include "VFCmdCardInit.h"
#include "VFCmdMcuVer.h"
#include <assert.h>


const uint8 CardVF::port2ts[30] = { 1, 3, 5, 7, 2, 4, 6, 8, 9, 11, 13, 15, 10,
        12, 14, 18, 17, 19, 21, 23, 20, 22, 24, 26, 25, 27, 29, 31, 28, 30 };


CardVF::CardVF(std::string& name, CBaseSlot* slot, uint8 pn) :
        CBaseCard(name, slot), cpld(slot), chip(slot) {//

    PortNumber = pn;
    os_mut_init(mut_vf_cmd);

    VFCmdCardInit cmd;
    if( processVFCommand(cmd) < 0 ) {
        throw SysError("!!!VF Card init error!!!");
    }
	if (!fetchConfig()) {
		throw SysError("!!!Card " + name + " config data error!!!");//
	}
    bool chip1054 = false;
    VFCmdMcuVer vercmd;
    if( processVFCommand(vercmd) < 0 ) {
        throw SysError("!!!VF Card init error!!!");
    }

    if(*vercmd.getResult() > 1) {
    	chip1054 = true;
    }
	ST_VF info;
	info.slot = slot->GetSn();
	for (int i = 0; i < PortNumber; i++) {
		info.port = i;
		uint32 portUID = UID::makeUID(&info);
		if(chip1054) {
			printf("new vf 1054\r\n");
			port_obj[i] = new PortVFG(portUID, getConnectedTS(portUID),
					&ConfigData.port[i], &ConfigData.group[i/4], this);
		}
		else {
			printf("old vf 1054A\r\n");
			port_obj[i] = new PortVFA(portUID, getConnectedTS(portUID),
					&ConfigData.port[i], &ConfigData.group[i/4], this);
		}
	}


    cardversionInfo = cpld.GetVerInfo() + "," + getMcuVersion();

}

CardVF::~CardVF() {
    for (int i = 0; i < PortNumber; i++) {
        delete port_obj[i];
    }
}



PairTSChannel* CardVF::getConnectedTS(uint32 vfuid) {
    UN_Info vfinfo = UID::breakUID(vfuid);
    ST_Time_Slot tsinfo;
    tsinfo.slot = vfinfo.vf.slot;
    tsinfo.xcsn = 0;
    tsinfo.E1 = 0;
    tsinfo.TS = port2ts[vfinfo.vf.port];
    uint32 tsUID = UID::makeUID(&tsinfo);
    return PairTSChannel::getInstance(tsUID);
}

int CardVF::processVFCommand(VFCommand& cmd) {
    os_mut_wait(mut_vf_cmd, 0xffff);

    if (chip.ifSendBusy()) {
        os_dly_wait(1);
        if (chip.ifSendBusy()) {
            os_dly_wait(1);
            if (chip.ifSendBusy()) {

#ifdef EZ_DEBUG
            printf("\n!!!VFComand send busy!!!\n");
#endif
                os_mut_release(mut_vf_cmd);
                return -1;
            }
        }
    }

    chip.sendPacket(cmd.toByteStream(), cmd.bytes());

    os_dly_wait(1);
    if (!chip.ifHavePacket()) {
        os_dly_wait(10);
        if (!chip.ifHavePacket()) {
            os_dly_wait(20);
            if (!chip.ifHavePacket()) {
#ifdef EZ_DEBUG
            printf("\n!!!VFComand No packet received!!!\n");
#endif
                os_mut_release(mut_vf_cmd);
                return -1;
            }
        }
    }
    chip.readPacket(cmd.getResultBuff());

    os_mut_release(mut_vf_cmd);
    return 1;
}

void CardVF::loadDefaultData(void) {
    VFPort_Config_T temp;
    memset(&temp, 0, sizeof(temp));
    temp.enable = 1;
    temp.offHookSig = 0x02;
    temp.onHookSig = 0x0a;
    for (int i = 0; i < 30; ++i) {
        memcpy(&ConfigData.port[i], &temp, sizeof(VFPort_Config_T));
    }
    for (int i = 0; i < 8; ++i) {
        ConfigData.group[i].rcvgain = 0;
        ConfigData.group[i].sndGain = 0;
    }
}

std::string CardVF::getMcuVersion(void) {
    VFCmdMcuVer cmd;
    if( processVFCommand(cmd) < 0 ) {
        return "MCU:V1.0";
    }
    uint8* pval = cmd.getResultBuff();
    assert(pval != 0);
    if( pval[2] != 0xee ) {
        return "MCU:V" + CPPTools::number2string(pval[2]) + "." + CPPTools::number2string(pval[3]);
    }
    else {
        return "MCU:V1.0";
    }
}
