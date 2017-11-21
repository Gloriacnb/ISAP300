/*
 * XCPair.cpp
 *
 *  Created on: 2014��11��19��
 *      Author: Administrator
 */

#include "XCPair.h"
//#include "InnerDCCManager.h"
#include "ClockSourceManager.h"
#include "SysError.h"
#include "CrossConnectionManager.h"
#include "ChipLM75A.h"
//#include "PairXcLocalClockSource.h"

#include "UID.h"
#include "PairPortSTM.h"
#include "PairRscSDHBus.h"
#include "PairSTBus.h"
//#include "PairXcOPTClockSource.h"
//#include "PairXcExtClockSource.h"
#include "clockSource.h"
#include "PairClockSource.h"
#include "PairPortFE1.h"

#include <stdio.h>
extern int shdbusSlotMapping[10];
XCPair XCPair::pairCard;

XcPairSaver::XcPairSaver() {
    if( !fetchConfig() ) {
        throw SysError("!!!XC Pair config data error!!!");
    }
}

void XcPairSaver::loadDefaultData(void) {
    ConfigCell::loadDefaultData();
    for (int i = 0; i < 2; ++i) {
        ConfigData.stmport[i].DCCSel = 0xfff;
        ConfigData.stmport[i].TXEN = 1;
        ConfigData.stmport[i].ALSEnable = 0;
        ConfigData.stmport[i].DescLen = 0;
    }
    for (int i = 0; i < 4; ++i) {
        ConfigData.fe1port[i].TXEN = 1;
        ConfigData.fe1port[i].DescLen = 0;
        ConfigData.fe1port[i].nmportCfg.dcnCH.bitMap = (1 << 30);
        ConfigData.fe1port[i].nmportCfg.porten = 1;
        ConfigData.fe1port[i].nmportCfg.topodir = 3+i;
        ConfigData.fe1port[i].nmportCfg.type = 1;
    }
}

bool XCPair::addResource(BaseResource* r) {
    if( r ) {
        uint32 pairuid = UID::getPairUID(r->getUID());
        PairResource* pair = PairResource::getInstance(pairuid);
        if( pair ) {
            return pair->foundResource(r);
        }
        else {
    		throw SysError("!!!XC Pair add resource found failed!!!");
        }
    }
	else {
		throw SysError("!!!XC Pair add input null!!!");
	}
    return false;
}

bool XCPair::removeResource(BaseResource* r) {
    if( r ) {
        uint32 pairuid = UID::getPairUID(r->getUID());
        PairResource* pair = PairResource::getInstance(pairuid);
        if( pair ) {
            return pair->loseResource(r);
        }
        else {
    		throw SysError("!!!XC Pair remove resource found failed!!!");
        }
    }
    else {
		throw SysError("!!!XC Pair remove input null!!!");
    }
    return false;
}

bool XCPair::init(int subtype) {
    saver = new XcPairSaver();
    ccm_obj = new CrossConnectionManager(0);
    temperatureSensor = new ChipLM75A();
    uint8 frameE1Number = 4;

    SubType = subtype;
    if( subtype < 32 ) {
        //SDH 设备独有
        frameE1Number = 8;
        ST_SDH_STM info;
        info.slot = 0;
        for( int i = 0; i < 2; i++ ) {
            info.stm = i;
            stm4_obj[i] = new PairPortSTM(UID::makeUID(&info));
        }

        ST_Sdhbus inf;
        for( int i = 0; i < sizeof(shdbusSlotMapping)/sizeof(shdbusSlotMapping[0]); i++ ) {
            inf.slot = shdbusSlotMapping[i];
            inf.xcsn = 0;
            inf.sdhbus = i % 2;
            sdhbus_obj[i] = new PairRscSDHBus(UID::makeUID(&inf));
        }
        /* ����XC������SDH������ */
        std::string sdhname("SDHClock");
        sdh_clock = new ClockSourceManager(sdhname);
        ST_Clock_source clkinfo;
        clkinfo.slot = 0;
        clkinfo.type = UID::type_local_clock;
        clkinfo.sn = 0;
        uint32 index = UID::makeUID(&clkinfo);
        xclocalclock_obj = new PairClockSource(index, 0);
        sdh_clock->addClockSource(xclocalclock_obj);
        for( int i = 0; i < 2; i++ ) {

            /*����19.44������������*/
            clkinfo.type = UID::type_opt_clock;
            clkinfo.sn = i;
            index = UID::makeUID(&clkinfo);
            xcoptclock_obj[i] = new PairClockSource(index, stm4_obj[i]);
            sdh_clock->addClockSource(xcoptclock_obj[i]);

            /* ����2.048������*/
            clkinfo.type = UID::type_2m_clock;
            clkinfo.sn = i;
            index = UID::makeUID(&clkinfo);
            xcextclock_obj[i] = new PairClockSource(index, framee1_obj[i+2]);
            sdh_clock->addClockSource(xcextclock_obj[i]);
        }
    }
    else if( subtype == 32 ) {
        //EMUX 设备独有
        ST_Stbus sbInfo;
        sbInfo.xcsn = 0;
        for( int i = 0; i < 5; i++ ) {
            sbInfo.slot = i+2;
            for (int j = 0; j < 16; ++j) {
                sbInfo.stbus = j + 1;
                expstbus_obj[i][j] = new PairSTBus(UID::makeUID(&sbInfo));
            }
        }
    }

    /* SDH 与 EMUX 设备共有 */
    ST_Stbus sbInfo;
    sbInfo.slot = 0;
    sbInfo.xcsn = 0;
    /*  ����8������E1 */
    for( int i = 0; i < frameE1Number; i++ ) {
        sbInfo.stbus = i;
        framee1_obj[i] = new PairSTBus(UID::makeUID(&sbInfo));
    }

    /* ��������������stbus */
    for( int i = 0; i < 5; i++ ) {
        sbInfo.slot = i+2;
        sbInfo.stbus = 0;
        uint32 pairuid = UID::makeUID(&sbInfo);
        stbus_obj[i] = new PairSTBus(pairuid);
    }

    ST_E1 e1Info;
    e1Info.slot = 0;
    for (int i = 0; i < 4; ++i) {
        e1Info.E1 = i;
        fe1_obj[i] = new PairPortFE1(UID::makeUID(&e1Info));
    }
    /* ����XC����PCM������ */
    std::string pcmname("PCMClock");
    pcm_clock = new ClockSourceManager(pcmname);
    ST_Clock_source clkinfo;
    clkinfo.slot = 0;
    clkinfo.type = UID::type_pcm_main_clock;
    clkinfo.sn = 0;
    uint32 index = UID::makeUID(&clkinfo);
    pcmLocalClock_obj = new PairClockSource(index, 0);
    pcm_clock->addClockSource(pcmLocalClock_obj);

    clkinfo.type = UID::type_pcm_recover_clock;
    for (int i = 0; i < frameE1Number; ++i) {
        clkinfo.sn = i;
        index = UID::makeUID(&clkinfo);
        pcmRecoverClock_obj[i] = new PairClockSource(index, framee1_obj[i]);
        pcm_clock->addClockSource(pcmRecoverClock_obj[i]);
    }
    return true;
}

void XCPair::deInit(void) {
    uint8 frameE1Number = 4;

    if( SubType < 32 ) {
        frameE1Number = 8;
        for( int i = 0; i < 2; i++ ) {
            delete xcextclock_obj[i];
            delete xcoptclock_obj[i];
        }
        delete sdh_clock;
        delete xclocalclock_obj;
        for( int i = 0; i < sizeof(shdbusSlotMapping)/sizeof(shdbusSlotMapping[0]); i++ ) {
            delete sdhbus_obj[i];
        }
        for( int i = 0; i < 2; i++ ) {
            delete stm4_obj[i];
        }

    }
    else {
        for( int i = 0; i < 5; i++ ) {
            for (int j = 0; j < 16; ++j) {
                delete expstbus_obj[i][j];
            }
        }
    }
    for (int i = 0; i < frameE1Number; ++i) {
        delete pcmRecoverClock_obj[i];
    }
    delete pcmLocalClock_obj;
    delete pcm_clock;

    for( int i = 0; i < 5; i++ ) {
        delete stbus_obj[i];
    }
    for( int i = 0; i < frameE1Number; i++ ) {
        delete framee1_obj[i];
    }
    delete temperatureSensor;
    delete ccm_obj;
    delete saver;
}
