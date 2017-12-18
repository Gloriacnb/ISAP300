/*
 * Card8STM1.cpp
 *
 *  Created on: 2014��8��12��
 *      Author: Administrator
 */

#include "Card8STM1.h"
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
#include "GeneralLogic.h"
#include "Trib8STM1AlarmModule.h"

//TASK void Do_Trib8_LED(void* pxc);

Card8STM1::Card8STM1(std::string& name, CBaseSlot* slot) :
        CBaseCard(name, slot), cpld(slot), fpga1(slot), fpga2(slot), tmchip1(
                slot, 0x8000, mode_PHY), tmchip2(slot, 0xa000, mode_PHY), tmchip3(
                slot, 0xc000, mode_PHY), tmchip4(slot, 0xe000, mode_PHY), driver(
                tmchip1, tmchip2, tmchip3, tmchip4, fpga1, fpga2) {
    // TODO Auto-generated constructor stub
//    if (GeneralLogic::instance().ifColdStart()) {

        std::cout << "init chips" << std::endl;
        /* ����оƬ��ʼ�� */
        Chip_INIT_RESULT result = cpld.ChipInit();
        if ( result == failed ) {
            throw SysError("!!!Card 8STM1 FPGA update failed!!!");
        }
        else if( result != donothing ) {
            tmchip1.ChipInit();
            tmchip2.ChipInit();
            tmchip3.ChipInit();
            tmchip4.ChipInit();

            fpga1.ChipInit();
            fpga2.ChipInit();
        }
        else {
            std::cout << "FPGAs have been running!" << std::endl;
        }
//    }

    if (!fetchConfig()) {
        throw SysError("!!!Card 8STM1 config data error!!!");
    }
    /*
     *
     */
    ST_SDH_STM info;
    info.slot = slot->GetSn();
    info.xcsn = 0;
    for (int i = 0; i < 8; i++) {
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

    /* ����˿� */
//    for (int i = 0; i < 8; ++i) {
//        uint32 index = (slot->GetSn() << 8) | i | (type_dcc << 16);
//        dccsdhrcv_obj[i] = new DCCTribRcv(index, tribstm_obj[i]->getUID(),
//                fpga2);
//        dccsdhsnd_obj[i] = new DCCTribSnd(index, fpga2);
//    }
//    ST_NM_Channel nminfo;
//    nminfo.slot = slot->GetSn();
//    for (int i = 0; i < 8; ++i) {
//        nminfo.subtype = subtype_dcc;
//        nminfo.sn = i;
//        uint32 index = UID::makeUID(&nminfo);
//        dccsdhrcv_obj[i] = new DCCTribRcv(index, tribstm_obj[i]->getUID(), fpga2);
//        dccsdhsnd_obj[i] = new DCCTribSnd(index, tribstm_obj[i]->getUID(), fpga2);
//    }

    cardversionInfo = cpld.GetVerInfo();

    Am = new Trib8STM1AlarmModule(this);
    Am->initModule();
    cpld.enableDCC();
}

Card8STM1::~Card8STM1() {
    // TODO Auto-generated destructor stub
    delete Am;
    /* ����˿� */
//    cpld.disableDCC();
//
//    for (int i = 0; i < 8; ++i) {
//        delete dccsdhrcv_obj[i];
//        delete dccsdhsnd_obj[i];
//    }
    delete ccm_obj;
    for (int i = 0; i < 126; i++) {
        delete sltch_obj[i];
    }
    for (int i = 0; i < 8; i++) {
        delete tribstm_obj[i];
    }
}

int Card8STM1::GetCartTypeID() {
    return STM_8_CARD_TYPEID;
}

void Card8STM1::loadDefaultData(void) {
    CBaseCard::loadDefaultData();
    for (int i = 0; i < 8; ++i) {
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

PairRscSDHChannel* Card8STM1::getConnectedSDHChannel(uint32 puid) {
    UN_Info sltchinfo = UID::breakUID(puid);
    ST_Sdh_channel chinfo;
    chinfo.slot = sltchinfo.sltch.slot;
    chinfo.bus = sltchinfo.sltch.group;
    chinfo.channel = sltchinfo.sltch.channel;
    uint32 chUID = UID::makeUID(&chinfo);
    return PairRscSDHChannel::getInstance(chUID);
}

void Card8STM1::closeInterrupt(void) {
    cpld.disableDCC();
}

TASK void Do_Trib8_LED(void* pxc) {
    Card8STM1* card = (Card8STM1*) pxc;
    for (int i = 0; i < 8; ++i) {
        card->cpld.portLEDAlarmOn(i, false);
        card->fpga2.portLEDLosOn(i, false);
    }
    card->fpga2.cardLEDAlarmOn(false);
    card->fpga2.cardLEDWorkOn(true);

    while (1) {
        os_dly_wait(200);
        unsigned char AlarmLedOffCount = 0;
        for (int i = 0; i < 2; ++i) {
            /*
             * LOS 1~8
             */
            if (card->tmchip1.getLOS(i))
                card->fpga2.portLEDLosOn(i, true);
            else
                card->fpga2.portLEDLosOn(i, false);
            if (card->tmchip2.getLOS(i))
                card->fpga2.portLEDLosOn(i + 2, true);
            else
                card->fpga2.portLEDLosOn(i + 2, false);
            if (card->tmchip3.getLOS(i))
                card->fpga2.portLEDLosOn(i + 4, true);
            else
                card->fpga2.portLEDLosOn(i + 4, false);
            if (card->tmchip4.getLOS(i))
                card->fpga2.portLEDLosOn(i + 6, true);
            else
                card->fpga2.portLEDLosOn(i + 6, false);

            /*
             * ALM 1~8
             */
            if (card->tmchip1.ifAlarm(i)) {
                card->cpld.portLEDAlarmOn(i, true);
            } else {
                card->cpld.portLEDAlarmOn(i, false);
                ++AlarmLedOffCount;
            }
            if (card->tmchip2.ifAlarm(i)) {
                card->cpld.portLEDAlarmOn(i + 2, true);
            } else {
                card->cpld.portLEDAlarmOn(i + 2, false);
                ++AlarmLedOffCount;
            }
            if (card->tmchip3.ifAlarm(i)) {
                card->cpld.portLEDAlarmOn(i + 4, true);
            } else {
                card->cpld.portLEDAlarmOn(i + 4, false);
                ++AlarmLedOffCount;
            }
            if (card->tmchip4.ifAlarm(i)) {
                card->cpld.portLEDAlarmOn(i + 6, true);
            } else {
                card->cpld.portLEDAlarmOn(i + 6, false);
                ++AlarmLedOffCount;
            }
        }
        /*
         * card Alarm
         */
        if (AlarmLedOffCount == 0) {
            card->fpga2.cardLEDAlarmOn(false);
        } else {
            card->fpga2.cardLEDAlarmOn(true);
        }
    }
//     os_tsk_delete_self();
}

