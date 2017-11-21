/*
 * CardXC.h
 *
 *  Created on: 2013-11-21
 *      Author: Administrator
 */

#ifndef CARDXC_H_
#define CARDXC_H_


#include "CBaseCard.h"
#include <string>
#include "ChipCP1121A.h"
#include "ChipUOPTLogic.h"
#include "ChipPCMLogic.h"
#include "ChipSDHLogic.h"
#include "PortSTM4.h"
#include "os.h"
#include "DriverSDHXc.h"
#include "ChipXCCPLD.h"
#include "ClockZarlink30122.h"
#include "XCAlarmDataDefine.h"
#include "XCConfigDataDefine.h"

class BaseChip;
class RscSDHBus;
class CInnerE1;
class XcOPTClockSource;
class XcExtClockSource;
class CSTBus;
//class CrossConnectionManager;
class DCCSdhRcv;
class DCCSdhSnd;
class DCCXe1Rcv;
class DCCXe1Snd;
//class InnerDCCManager;
class CExtE1;
//class ChipLM75A;
class XCAlarmModule;
class XcLocalClockSource;
class XcPCMClockSource;
class PortFE1;
class ChannelDcc;
class NMPort;
class ChannelSabit;
class ChannelTsDcn;

class CardXC : public CBaseCard {
public:
    CardXC(std::string& name, CBaseSlot* slot);
    virtual ~CardXC();

    virtual int GetCartTypeID();

    void printRegsToFile(uint32 regAddrBegin, uint32 regCount, const char* fname, bool skipall0 = false);
    ChipUOPTLogic& getChipUOPTLogic(void) {
        return uoptLgc;
    };
    DriverPCM& getChipPCMLogic(void) {
        return pcmLgc;
    };

    virtual uint8* getConfigDataBuff(void) {
        return (uint8*)&AlarmData;
    };
    virtual uint32 getConfigDataLen(void) {
        return sizeof(AlarmData);
    }

    virtual void loadDefaultData(void) {
        ConfigCell::loadDefaultData();
        for(int i = 0; i < 2; i++) {
            for (int j = 0; j < STM_Signal_Count; ++j) {
                AlarmData.STMAlmData[i].Property[j].level = 1;
            }
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < VC4_Signal_Count; ++k) {
                    AlarmData.STMAlmData[i].VC4AlmData[j].Property[k].level = 2;
                }
            }
        }
        for( int i = 0; i < 4; i++ ) {
            for (int j = 0; j < XE1_Signal_Count; ++j) {
                AlarmData.XE1AlmData[i].Property[j].level = 1;
            }
        }
    }
    virtual std::string& GetCardVerInfo(){
    	return cardversionInfo;
    };

    virtual void closeInterrupt(void) {
        uoptLgc.dccInterruptEnable(false);
    };
    virtual void resetRemoteDevice(uint8 port);
    void setWorkLED(bool on);

    void changeWorkingSta(bool working) {
        if( working != staWorking ) {
            staWorking = working;
            if( working ) {
                changeToWorking();
            }
            else {
                changeToIdle();
            }
        }
    };
private:
    ClockZarlink30122 clock;
    ChipUOPTLogic uoptLgc;
    ChipPCMLogic pcmLgc;
    ChipSDHLogic sdhLgc;
    ChipCP1121A cp1121a;
    DriverSDHXc driver;
    friend TASK void check_opt_los(void);

    friend class XCAlarmModule;
    friend class XCPair;
    PortSTM4* stm4_obj[2];
    RscSDHBus* sdhbus_obj[10];
    CInnerE1* innere1_obj[4];
    CExtE1* exte1_obj[4];
    CSTBus* stbus_obj[5];
    PortFE1* fe1_obj[4];
    XcLocalClockSource* xclocalclock_obj;
    XcOPTClockSource* xcoptclock_obj[2];
    XcExtClockSource* xcextclock_obj[2];
    XcPCMClockSource* xcpcmLocal_obj;
    XcPCMClockSource* xcpcmclock_obj[8];
//    DCCSdhRcv* dccsdhrcv_obj[2];
//    DCCSdhSnd* dccsdhsnd_obj[2];
//
//    DCCXe1Rcv* dccxe1rcv_obj[4];
//    DCCXe1Snd* dccxe1snd_obj[4];

    std::string cardversionInfo;
    ChipXCCPLD cpld;

    Card_ConfigData_XC* ConfigData;
    XC_Alarm_Data_T AlarmData;

    XCAlarmModule* Am;

    bool staWorking;
    void changeToWorking(void);
    void changeToIdle(void);

    ChannelSabit* nmch_dcc[4];
    ChannelTsDcn* nmch_dcn[4];
    ChannelDcc* nmch_optdcc[2];
    NMPort* nmport[2];
};

#endif /* CARDXC_H_ */
