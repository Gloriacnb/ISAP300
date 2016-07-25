/*
 * CardDXC.h
 *
 *  Created on: 2015年6月17日
 *      Author: Administrator
 */

#ifndef CARDDXC_H_
#define CARDDXC_H_

#include "CBaseCard.h"
#include "ChipDXCLogic.h"//#include "ChipEmuxLogic.h"
#include "DXCAlarmDataDefine.h"
#include "XCConfigDataDefine.h"

//typedef struct {
//    DXC_Alarm_Data_T AlarmData;
//    FE1_Config_Data_T fe1port[4];
//}Card_ConfigData_DXC;

class DXCAlarmModule;
class CExtE1;
class CSTBus;
class ChannelSabit;
class ChannelTsDcn;
class XcPCMClockSource;
class PortFE1;
class NMPort;

class CardDXC: public CBaseCard {
public:
    CardDXC(std::string& name, CBaseSlot* slot);
    virtual ~CardDXC();

    /*virtual ConfigCell*/
    virtual void loadDefaultData(void);
    virtual uint8* getConfigDataBuff(void) {
        return (uint8*)&AlarmData;
    };
    virtual uint32 getConfigDataLen(void) {
        return sizeof(DXC_Alarm_Data_T);
    };

    /*virtual CBaseCard*/
    virtual std::string& GetCardVerInfo();
    virtual void closeInterrupt(void);
    virtual int GetCartTypeID();
    virtual void resetRemoteDevice(uint8 port);

    void setWorkLED(bool on);

    DriverPCM& getChipPCMLogic(void) {
        return fpga;
    };

    void openHstbus(uint8 slot, bool en);

    bool ifWorking(void) {
        return staWorking;
    };
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
    friend class DXCAlarmModule;
    DXC_Alarm_Data_T AlarmData;
    Card_ConfigData_XC* ConfigData;

    ChipDXCLogic fpga;
    CExtE1* exte1_obj[4];
    CSTBus* stbus_obj[5];
    CSTBus* expstbus_obj[5][16];
    PortFE1* fe1_obj[4];
//    DCCXe1Rcv* dccxe1rcv_obj[4];
//    DCCXe1Snd* dccxe1snd_obj[4];
//    ChannelSabit* sabit_ch_obj[4];
//    ChannelTsDcn* dcn_ch_obj[4];
    XcPCMClockSource* xcpcmLocal_obj;
    XcPCMClockSource* xcpcmclock_obj[4];

    DXCAlarmModule* am;
    std::string cardversionInfo;

    bool staWorking;
    void changeToWorking(void);
    void changeToIdle(void);
    ChannelSabit* nmch_dcc[4];
    ChannelTsDcn* nmch_dcn[4];
    NMPort* nmport[4];

};
#endif /* CARDDXC_H_ */
