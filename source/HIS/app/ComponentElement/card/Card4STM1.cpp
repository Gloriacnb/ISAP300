/*
 * Card4STM1.cpp
 *
 *  Created on: 2014-5-15
 *      Author: Administrator
 */

#include "Card4STM1.h"
#include "CardTypeID_define.h"
#include "ChipRC7883.h"
#include <iostream>
#include "RegisterAccess.h"
#include "uid.h"
#include "SysError.h"
#include "Slot155Channel.h"
#include "CrossConnectionManager.h"
#include "PairRscSDHChannel.h"
//#include "DCCTribRcv.h"
//#include "DCCTribSnd.h"
#include "os.h"
//#include "GeneralLogic.h"
#include "Trib4STM1AlarmModule.h"


//TASK void Do_Trib_LED(void* pxc);

Card4STM1::Card4STM1(std::string& name, CBaseSlot* slot) :
        CBaseCard(name, slot), cpld(slot), fpga1(slot), fpga2(slot), tmchip1(
                slot, 0x8000, mode_PHY), tmchip2(slot, 0xa000, mode_PHY), driver(
                tmchip1, tmchip2, fpga1, fpga2) {
//	if(GeneralLogic::instance().ifColdStart()){

		std::cout << "init chips" << std::endl;
		/* ����оƬ��ʼ�� */
		Chip_INIT_RESULT rst = cpld.ChipInit();
		if ( rst == failed ) {
			throw SysError("!!!Card 4STM1 FPGA update failed!!!");
		}
		else if( rst == succeed ) {
            tmchip1.ChipInit();
            tmchip2.ChipInit();

            fpga1.ChipInit();
            fpga2.ChipInit();
		}
        else {
            std::cout << "FPGAs have been running!" << std::endl;
        }
//	}

    if (!fetchConfig()) {
        throw SysError("!!!Card 4STM1 config data error!!!");
    }
    /*
     *
     */
    ST_SDH_STM info;
    info.slot = slot->GetSn();
    info.xcsn = 0;
    for (int i = 0; i < 4; i++) {
        info.stm = i;
        tribstm_obj[i] = new PortSTM4(UID::makeUID(&info), this, driver,
                &ConfigData.port[i]);
    }

    ST_Sltch_channel info2;
    info2.slot = slot->GetSn();

    for (int i = 0; i < 126; i++) {
        info2.group = i / 63;
        info2.channel = i % 63;
        uint32 uid = UID::makeUID(&info2);
        sltch_obj[i] = new Slot155Channel(uid, getConnectedSDHChannel(uid),
                this, driver);
    }
    /* �������ӹ�����  */
    ccm_obj = new CrossConnectionManager(slot->GetSn());

    cardversionInfo = cpld.GetVerInfo();

    Am = new Trib4STM1AlarmModule(this);
    Am->initModule();
    cpld.enableDCC();

}

Card4STM1::~Card4STM1() {

    delete Am;
	delete ccm_obj;
	for (int i = 0; i < 126; i++) {
		delete sltch_obj[i];
	}
	for (int i = 0; i < 4; i++) {
		delete tribstm_obj[i];
	}
}

int Card4STM1::GetCartTypeID() {
    return STM_4_CARD_TYPEID;
}

void Card4STM1::loadDefaultData(void) {
    CBaseCard::loadDefaultData();
    for (int i = 0; i < 4; ++i) {
        ConfigData.port[i].DCCSel = 0xfff;
        ConfigData.port[i].TXEN = 1;
        for (int j = 0; j < 6; ++j) {
            ConfigData.AlarmData.STMAlmData[i].Property[j].level = 1;
        }
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 7; ++k) {
                ConfigData.AlarmData.STMAlmData[i].VC4AlmData[j].Property[k].level = 2;
            }
        }
    }
}

PairRscSDHChannel* Card4STM1::getConnectedSDHChannel(uint32 puid) {
    UN_Info sltchinfo = UID::breakUID(puid);
    ST_Sdh_channel chinfo;
    chinfo.slot = sltchinfo.sltch.slot;
    chinfo.bus = sltchinfo.sltch.group;
    chinfo.channel = sltchinfo.sltch.channel;
    uint32 chUID = UID::makeUID(&chinfo);
    return PairRscSDHChannel::getInstance(chUID);
}

void Card4STM1::closeInterrupt(void) {
    cpld.disableDCC();
}


