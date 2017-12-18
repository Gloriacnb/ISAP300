/*
 * CardXC.cpp
 *
 *  Created on: 2013-11-21
 *      Author: Administrator
 */

#include "CardXC.h"
#include "ChipCP1121A.h"
#include "CBaseSlot.h"
#include "UID.h"
#include "SysError.h"
#include "RscSDHBus.h"
#include "CardTypeID_define.h"
#include "CInnerE1.h"
#include <iostream>
#include "CPPTools.h"
#include <stdio.h>
#include "RegisterAccess.h"
#include "XcOPTClockSource.h"
#include "XcExtClockSource.h"
#include "XcPCMClockSource.h"
#include "CardCPU.h"
//#include "DCCSdhRcv.h"
//#include "DCCSdhSnd.h"
//#include "DCCXe1Rcv.h"
//#include "DCCXe1Snd.h"
//#include "InnerDCCManager.h"
#include "CrossConnectionManager.h"
#include "CExtE1.h"

#include "XcLocalClockSource.h"
#include "ISAP100Led.h"
#include "ChipLM75A.h"
//#include "GeneralLogic.h"

#include "XCAlarmModule.h"
#include "XCPair.h"

#include "DeviceComponent.h"
#include "SlotModule.h"
#include "PortFE1.h"
#include "XCAlarmDataDefine.h"
#include "NMPort.h"
#include "ChannelSabit.h"
#include "ChannelTsDcn.h"
#include "ChannelDcc.h"

/* 数组下标sdhbus，数组元素，对应槽位 */
int shdbusSlotMapping[] = {0,0,2,2,3,3,5,5,6,6};

CardXC::CardXC(std::string& name, CBaseSlot* slot) : CBaseCard(name, slot),
        uoptLgc(slot), pcmLgc(slot), sdhLgc(slot), cp1121a(slot), driver(cp1121a, pcmLgc, uoptLgc),cpld(slot), clock(&pcmLgc) {
	staWorking = false;
	Chip_INIT_RESULT rst = donothing;
	    /*������������������������������*/
    rst = uoptLgc.ChipInit();
    if( rst == failed ) {
        throw SysError("!!!Chip uoptLgc init failed!!!");
    }

    if( rst != donothing ) {
        if( pcmLgc.ChipInit() == failed ) {
            throw SysError("!!!Chip pcmlogic init failed!!!");
        }
        pcmLgc.resetClockChip();
        clock.initChip();
    }
    if( rst != donothing ) {
        if( cp1121a.ChipInit() == failed ) {
            throw SysError("!!!Chip cp1121A init failed!!!");
        }
        sdhLgc.resetPLL(); //must after clock and cp1121a init over!!!
        SlotModule::resetPLL(slot->GetSn());
    }
    /*板载芯片初始化完成后，再开启业务槽的板卡发现功能*/
    SlotModule::startExtChecking();

	/* �����������������������������  */
	XCPair& pxc = XCPair::instance();
    ConfigData = (Card_ConfigData_XC*)pxc.getConfigSaver()->getConfigDataBuff();

    /* ����������������������������*/

    ST_SDH_STM info;
    info.slot = 0;
    info.xcsn = slot->GetSn();
    for( int i = 0; i < 2; i++ ) {
        info.stm = i;
        stm4_obj[i] = new PortSTM4(UID::makeUID(&info), pxc.getConfigSaver(), driver, &ConfigData->stmport[i], true);
#ifdef EZ_DEBUG
        printf("\nXC First new at:0x%4x\n", stm4_obj[i]);
#endif
    }

    ST_Sdhbus inf;
    for( int i = 0; i < sizeof(shdbusSlotMapping)/sizeof(shdbusSlotMapping[0]); i++ ) {
        if( shdbusSlotMapping[i] == 0 ) {
            inf.slot = 0;
        }
        else {
            inf.slot = shdbusSlotMapping[i];
        }
        inf.xcsn = slot->GetSn();
        inf.sdhbus = i % 2;
        sdhbus_obj[i] = new RscSDHBus(UID::makeUID(&inf), pxc.getConfigSaver(), cp1121a, true);
    }
    /* ����������4��������������E1����������������mapping ������#17 SDHbus ������ 0~3sdhchannel �������� */
    ST_Sdh_channel chinf;
    chinf.slot = 0;
    chinf.bus = 0;
    chinf.xcsn = slot->GetSn();

    ST_Stbus sbInfo;
    sbInfo.slot = 0;
    sbInfo.xcsn = slot->GetSn();
    for( int i = 0; i < 4; i++ ) {
        chinf.channel = i;
        sbInfo.stbus = i+4;
        innere1_obj[i] = new CInnerE1(UID::makeUID(&sbInfo), pxc.getConfigSaver(), UID::makeUID(&chinf), &pcmLgc, true);
    }

    for (int i = 0; i < 4; ++i) {
        sbInfo.stbus = i;
        exte1_obj[i] = new CExtE1(UID::makeUID(&sbInfo), pxc.getConfigSaver(), &pcmLgc, true);
    }

    for( int i = 0; i < 5; i++ ) {
        sbInfo.slot = i+2;
        sbInfo.stbus = 0;
        sbInfo.xcsn = slot->GetSn();
        stbus_obj[i] = new CSTBus(UID::makeUID(&sbInfo), pxc.getConfigSaver(), &pcmLgc, true);
    }

    ST_E1 e1info;
    e1info.slot = slot->GetSn();
    for (int i = 0; i < 4; ++i) {
         e1info.E1 = i;
         fe1_obj[i] = new PortFE1(UID::makeUID(&e1info), pxc.getConfigSaver(), &ConfigData->fe1port[i], pcmLgc, true);
    }

    /* XC卡时钟源 */
    ST_Clock_source clkinfo;
    clkinfo.slot = slot->GetSn();
    clkinfo.type = UID::type_local_clock;
    clkinfo.sn = 0;
    uint32 index = UID::makeUID(&clkinfo);
    xclocalclock_obj = new XcLocalClockSource(index, &pcmLgc);
    for( int i = 0; i < 2; i++ ) {

        /*������������19.44���������������������*/
        clkinfo.type = UID::type_opt_clock;
        clkinfo.sn = i;
        index = UID::makeUID(&clkinfo);
        xcoptclock_obj[i] = new XcOPTClockSource(index, &pcmLgc, &cp1121a, &clock, true);

        /* ������������2.048��������������*/
        clkinfo.type = UID::type_2m_clock;
        clkinfo.sn = i;
        index = UID::makeUID(&clkinfo);
        xcextclock_obj[i] = new XcExtClockSource(index, &pcmLgc, &clock, true);
    }

    clkinfo.type = UID::type_pcm_main_clock;
    clkinfo.sn = 0;
    index = UID::makeUID(&clkinfo);
    xcpcmLocal_obj = new XcPCMClockSource(index, 0, pcmLgc);

    clkinfo.type = UID::type_pcm_recover_clock;
    for (int i = 0; i < 8; ++i) {
        clkinfo.sn = i;
        index = UID::makeUID(&clkinfo);
        xcpcmclock_obj[i] = new XcPCMClockSource(index, i+1, pcmLgc);
    }
    /*  ��������������������������������������������  */
    pxc.getPccManager()->restoreCorssConnect();


    cardversionInfo = \
    		CardCPU::GetVerInfo()+","\
    		+cpld.GetVerInfo();

    /* Alarm Init */
    if( !fetchConfig() ) {
        throw SysError("!!!XC Card config data error!!!");
    }
    Am = new XCAlarmModule(this);
#ifdef EZ_DEBUG
    printf("\nXC last new at:0x%4x\n", Am);
#endif
    Am->initModule();

}

CardXC::~CardXC() {
    uoptLgc.dccInterruptEnable(false);
    delete Am;
//    for (int i = 0; i < 2; ++i) {
//        delete dccsdhrcv_obj[i];
//        delete dccsdhsnd_obj[i];
//    }
//    for (int i = 0; i < 1; ++i) {
//        delete dccxe1rcv_obj[i];
//        delete dccxe1snd_obj[i];
//    }
    for( int i = 0; i < 8; i++ ) {
        delete xcpcmclock_obj[i];
    }
    delete xcpcmLocal_obj;

    for( int i = 0; i < 2; i++ ) {
        delete xcoptclock_obj[i];
        delete xcextclock_obj[i];

    }
    delete xclocalclock_obj;

    for (int i = 0; i < 4; ++i) {
         delete fe1_obj[i];
    }
    for( int i = 0; i < 5; i++ ) {
        delete stbus_obj[i];
    }

    for( int i = 0; i < 4; i++ ) {
        delete exte1_obj[i];
        delete innere1_obj[i];
    }
    for( int i = 0; i < sizeof(shdbusSlotMapping)/sizeof(shdbusSlotMapping[0]); i++ ) {
        delete sdhbus_obj[i];
    }
    for( int i = 0; i < 2; i++ ) {
        delete stm4_obj[i];
    }
}

int CardXC::GetCartTypeID() {
    return XC_CARD_TYPEID;
}

void CardXC::printRegsToFile(uint32 regAddrBegin, uint32 regCount, const char* fname, bool skipall0) {

    if( fname == 0 ) {
        std::string defaultname =   "XC_" + CPPTools::number2string(getSn()) +
                                    "Regs_" + CPPTools::hex2string(regAddrBegin) +
                                    "-" + CPPTools::hex2string(regAddrBegin+regCount) + ".txt";
        fname = defaultname.c_str();
    }
    FILE* regFile = fopen(fname, "w");
    if( regFile ) {
        fprintf( regFile, "\nThe registers list:[%4x, %4x)\n", regAddrBegin, regAddrBegin+regCount);
        for( int addr = 0; addr < regCount; addr++ ) {
            uint16 value = getItsSlot()->getRegisterAccess()->readReg(addr+regAddrBegin);
            if( skipall0 && value == 0 ) {
                continue;
            }
            fprintf( regFile, "RegAddr: 0x%4x\t, RegValue: 0x%04x\n", addr+regAddrBegin, value);
        }
        fclose(regFile);
        std::cout << "Regs value print completed! Please check file " << fname << " use dir n comand." << std::endl;
    }
    else {
        std::cout << "!!!open file " << fname << " failed!!!" << std::endl;
    }
}


void CardXC::resetRemoteDevice(uint8 port) {
    uoptLgc.resetRemoteDevice(port);
}

//void CardXC::enableDCCIntr() {
//    uoptLgc.dccInterruptEnable(true);
//}
//void CardXC::disableDCCIntr() {
//    uoptLgc.dccInterruptEnable(false);
//}
void CardXC::setWorkLED(bool on) {
    cpld.turnLedWorkOn(on);
}

//void CardXC::loadDefaultData(void) {
//    CBaseCard::loadDefaultData();
//    for (int i = 0; i < 2; ++i) {
//        ConfigData.stmport[i].DCCSel = 0xfff;
//        for (int j = 0; j < 6; ++j) {
//            ConfigData.AlarmData.STMAlmData[i].Property[j].level = 1;
//        }
//        for (int j = 0; j < 4; ++j) {
//            for (int k = 0; k < 7; ++k) {
//                ConfigData.AlarmData.STMAlmData[i].VC4AlmData[j].Property[k].level = 2;
//            }
//        }
//    }
//
//}
void CardXC::changeToWorking(void) {
    /*
     * 建立管理接口
     */
	ST_NM_Channel nminfo;
	nminfo.slot = getSn();

	nminfo.subtype = subtype_dcc;
	for (int i = 0; i < 2; ++i) {
		nminfo.sn = i;
		uint32 index = UID::makeUID(&nminfo);

		nmch_optdcc[i] = new ChannelDcc(index, pcmLgc);

		nmport[i] = new NMPort(stm4_obj[i],
				&ConfigData->stmport[i].nmportCfg);
		if (nmport[i] == 0 || nmch_optdcc[i] == 0) {
			throw SysError("!!!new object failed!!!");
		}

		nmport[i]->addNmChannel(nmch_optdcc[i]);
		nmport[i]->start();
	}

	uoptLgc.dccInterruptEnable(true);
}
void CardXC::changeToIdle(void) {
    uoptLgc.dccInterruptEnable(false);
    for (int i = 0; i < 2; ++i) {
        if( nmport[i] ) {
            nmport[i]->stop();
            delete nmport[i];
            nmport[i] = 0;
        }
        if( nmch_optdcc[i] ) {
            delete nmch_optdcc[i];
        }
    }

}


