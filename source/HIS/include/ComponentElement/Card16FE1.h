/*
 * Card16FE1.h
 *
 *  Created on: 2015年6月17日
 *      Author: Administrator
 */

#ifndef CARD16FE1_H_
#define CARD16FE1_H_

#include "CBaseCard.h"
#include "DXCAlarmDataDefine.h"
#include "ChipE1Port16Logic.h"
#include "FE1ConfigDataDefine.h"
typedef struct {
    FE116_Alarm_Data_T AlarmData;
    FE1_Config_Data_T fe1Data[16];
}Card_ConfigData_16FE1;

class PortFE1;
class PairSTBus;
class FE1CardAlarmModule;
//class DCCXe1Rcv;
//class DCCXe1Snd;
class F16E1PCMClockSource;
class NMPort;
class ChannelSabit;
class ChannelTsDcn;

class Card16FE1: public CBaseCard {
public:
    Card16FE1(std::string& name,CBaseSlot* slot);
    virtual ~Card16FE1();

    /*virtual ConfigCell*/
    virtual void loadDefaultData(void);
    virtual uint8* getConfigDataBuff(void) {
        return (uint8*)&ConfigData;
    };
    virtual uint32 getConfigDataLen(void) {
        return sizeof(ConfigData);
    };

    virtual std::string& GetCardVerInfo();
    virtual int GetCartTypeID();

    DriverPCM& getChipPCMLogic() {
        return fpga;
    };
    virtual void resetRemoteDevice(uint8 port);

    virtual void closeInterrupt(void);

private:
    void openHstbus(uint8 sn, bool open);
    friend class FE1CardAlarmModule;
    Card_ConfigData_16FE1 ConfigData;
    std::string cardversionInfo;
    ChipE1Port16Logic fpga;
    PortFE1* fe1_obj[16];
    PairSTBus* getConnectedStbus(uint32 uid);
    FE1CardAlarmModule* AM;
    ChannelSabit* nmch_dcc[16];
    ChannelTsDcn* nmch_dcn[16];
    NMPort* nmport[16];
    F16E1PCMClockSource* clockSource[16];
};

#endif /* CARD16FE1_H_ */
