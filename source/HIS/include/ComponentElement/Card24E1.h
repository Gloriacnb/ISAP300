/*
 * Card24E1.h
 *
 *  Created on: 2013-12-17
 *      Author: Administrator
 */

#ifndef CARD24E1_H_
#define CARD24E1_H_

#include "ChipE1Logic.h"
#include "CBaseCard.h"
#include "ChipRC7883.h"
#include "PortE1.h"
#include "Chip24E1CPLD.h"
#include "E1CardAlarmDataDefine.h"

class CBaseSlot;
class PortE1;
class PairRscSDHChannel;
class E1CardAlarmModule;

typedef struct {
    UE1_Config_Data_T port[24];//24��E1�ڵ�ʹ�ܡ�SNCP����������������
    SrcE1Alarm_Data E1AlmData[24];
}Card_ConfigData_24E1;

class Card24E1 : public CBaseCard {
    enum{ E1_Port_Num = 24};
public:
    Card24E1(std::string& name, CBaseSlot* slot);
    virtual ~Card24E1();

    virtual int GetCartTypeID();
    virtual std::string& GetCardVerInfo(){
    	return cardversionInfo;
   	}
    ChipE1Logic& getChipE1Logic(void) {
        return chipE1;
    };

    virtual uint8* getConfigDataBuff(void) {
        return (uint8*) &ConfigData;
    };
    virtual uint32 getConfigDataLen(void) {
        return sizeof(ConfigData);
    };

    virtual void loadDefaultData(void);

private:
//    friend TASK void Do_24E1_LED(void* pxc);
    friend class E1CardAlarmModule;
    Chip24E1CPLD cpld;
    ChipE1Logic chipE1;//FPGAchip
    ChipRC7883 chipE1Map;//7883chip
    PortE1* e1port_obj[E1_Port_Num];
    Card_ConfigData_24E1 ConfigData;
    PairRscSDHChannel* getConnectedSDHChannel(uint32 puid);

    std::string cardversionInfo;

//    OS_TID C24E1_LED;
    E1CardAlarmModule* AM;

};

#endif /* CARD24E1_H_ */
