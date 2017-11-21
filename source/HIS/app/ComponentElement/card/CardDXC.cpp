/*
 * CardDXC.cpp
 *
 *  Created on: 2015年6月17日
 *      Author: Administrator
 */

#include "CardDXC.h"
#include "DXCAlarmModule.h"
#include "XCPair.h"
#include "UID.h"
#include "SysError.h"
#include "CExtE1.h"
#include "CardTypeID_define.h"
#include "CardCPU.h"
#include "CrossConnectionManager.h"
//#include "DCCXe1Rcv.h"
//#include "DCCXe1Snd.h"
#include "XcPCMClockSource.h"
#include "PortFE1.h"
#include "SlotModule.h"
#include <stdio.h>
#include "ChannelSabit.h"
#include "ChannelTsDcn.h"
#include "NMPort.h"
#include "SyncFileAPI.h"
#include "GeneralLogic.h"
#include "ObjectReference.h"

CardDXC::CardDXC(std::string& name, CBaseSlot* slot) : CBaseCard(name, slot), fpga(slot) {

    staWorking = false;
    Chip_INIT_RESULT rst = fpga.ChipInit();
    if( rst == failed ) {
        throw SysError("!!!Card EMUXC fpga init error!!!");
    }

    SlotModule::startExtChecking(); //主fpga加载完成后，才开始检测业务板卡

    ST_Stbus sbInfo;
    sbInfo.slot = 0;
    sbInfo.xcsn = slot->GetSn();

    for (int i = 0; i < 4; ++i) {
        sbInfo.stbus = i;
        exte1_obj[i] = new CExtE1(UID::makeUID(&sbInfo), XCPair::instance().getConfigSaver(), &fpga, true);
    }

    for( int i = 0; i < 5; i++ ) {
        sbInfo.slot = i+2;
        sbInfo.stbus = 0;
        stbus_obj[i] = new CSTBus(UID::makeUID(&sbInfo), XCPair::instance().getConfigSaver(), &fpga, true);
        for (int j = 0; j < 16; ++j) {
            sbInfo.stbus = j + 1;
            expstbus_obj[i][j] = new CSTBus(UID::makeUID(&sbInfo), XCPair::instance().getConfigSaver(), &fpga, true);
        }
    }

    ConfigData = (Card_ConfigData_XC*)XCPair::instance().getConfigSaver()->getConfigDataBuff();
    ST_E1 e1info;
    e1info.slot = getSn();
    for (int i = 0; i < 4; ++i) {
         e1info.E1 = i;
         fe1_obj[i] = new PortFE1(UID::makeUID(&e1info), XCPair::instance().getConfigSaver(), &ConfigData->fe1port[i], fpga, true);
         nmport[i] = 0;
         nmch_dcc[i] = 0;
         nmch_dcn[i] = 0;
    }

    ST_Clock_source clkinfo;
    clkinfo.slot = slot->GetSn();
    clkinfo.type = UID::type_pcm_main_clock;
    clkinfo.sn = 0;
    uint32 index = UID::makeUID(&clkinfo);
    xcpcmLocal_obj = new XcPCMClockSource(index, 0, fpga);

    clkinfo.type = UID::type_pcm_recover_clock;
    for (int i = 0; i < 4; ++i) {
        clkinfo.sn = i;
        index = UID::makeUID(&clkinfo);
        xcpcmclock_obj[i] = new XcPCMClockSource(index, i+1, fpga);
    }

    XCPair::instance().getPccManager()->restoreCorssConnect();

    if( !fetchConfig() ) {
        throw SysError("!!!DXC Card config data error!!!");
    }
    am = new DXCAlarmModule(this);
    am->initModule();

    std::string fpgaversionInfo = fpga.GetVerInfo();
    std::string cpuversionInfo;
    if( CardCPU::itsSlot() == getSn() ) {
        cpuversionInfo += CardCPU::GetVerInfo();
    }
    else {
        cpuversionInfo += SyncFileAPI::instance().getBakMainVersion();
    }
    cardversionInfo = cpuversionInfo + "," + fpgaversionInfo;
}

CardDXC::~CardDXC() {
    // TODO Auto-generated destructor stub
	changeToIdle();//主备切换时，删除DXC卡需要清管理通道
    delete am;
    for( int i = 0; i < 4; i++ ) {
        delete xcpcmclock_obj[i];
    }
    delete xcpcmLocal_obj;
    changeToIdle();
    for (int i = 0; i < 4; ++i) {
        if( fe1_obj[i] ) {
            delete fe1_obj[i];
        }
    }
    for( int i = 0; i < 5; i++ ) {
        delete stbus_obj[i];
        for (int j = 0; j < 16; ++j) {
            delete expstbus_obj[i][j];
        }
    }
    for (int i = 0; i < 4; ++i) {
        delete exte1_obj[i];
    }

}

void CardDXC::changeToWorking(void) {
    /*
     * 建立管理接口
     */
    ST_NM_Channel nminfo;
    nminfo.slot = getSn();
    nminfo.subtype = subtype_sabit;
    for (int i = 0; i < 4; ++i) {
        nminfo.sn = i;
        uint32 index = UID::makeUID(&nminfo);

        nmch_dcc[i] = new ChannelSabit(index, fpga);
        nmch_dcn[i] = new ChannelTsDcn(index, fpga);

        nmport[i] = new NMPort(fe1_obj[i], &ConfigData->fe1port[i].nmportCfg);
        if( nmport[i] == 0 || nmch_dcc[i] == 0 || nmch_dcn[i] == 0 ) {
            throw SysError("!!!new object failed!!!");
        }

        nmport[i]->addNmChannel(nmch_dcc[i]);
        nmport[i]->addNmChannel(nmch_dcn[i]);
        nmport[i]->start();
    }
    fpga.sabitInterruptEnable(true);
}
void CardDXC::changeToIdle(void) {
    fpga.sabitInterruptEnable(false);
    for (int i = 0; i < 4; ++i) {
        if( nmport[i] ) {
            nmport[i]->stop();
            delete nmport[i];
            nmport[i] = 0;
        }
        if( nmch_dcn[i] ) {
            delete nmch_dcn[i];
            nmch_dcn[i] = 0;
        }
        if( nmch_dcc[i] ) {
            delete nmch_dcc[i];
            nmch_dcc[i] = 0;
        }
    }
}

void CardDXC::loadDefaultData(void) {
    ConfigCell::loadDefaultData();
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < UpE1_Signal_Count; ++j) {
            AlarmData.UpE1AlmData[i].Property[j].level = 1;
            AlarmData.UpE1AlmData[i].Property[j].mask = 0;
            AlarmData.UpE1AlmData[i].Property[j].trap = 0;
        }
    }
};

/*virtual CBaseCard*/
std::string& CardDXC::GetCardVerInfo() {
    return cardversionInfo;
}
void CardDXC::closeInterrupt(void) {
    fpga.sabitInterruptEnable(false);
}
int CardDXC::GetCartTypeID() {
    return DXC_CARD_TYPEID;
}
void CardDXC::resetRemoteDevice(uint8 port) {
    fpga.resetRemote(port);
}


void CardDXC::setWorkLED(bool on) {
    fpga.turnOnledWorking(on);
}

void CardDXC::openHstbus(uint8 slot, bool en) {
    if( en ) {
        fpga.onHstbus(slot-2);
    }
    else {
        fpga.offHstbus(slot-2);
    }
}
