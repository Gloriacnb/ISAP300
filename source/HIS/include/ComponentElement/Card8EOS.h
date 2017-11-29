/*
 * Card8EOS.h
 *
 *  Created on: 2014-3-26
 *      Author: Administrator
 */

#ifndef CARD8EOS_H_
#define CARD8EOS_H_

#include "CBaseCard.h"
#include "Chip8EOSLogic.h"
#include "ChipRC6400.h"
#include "CHVC.h"
#include "VCG.h"
#include "Chip8EOSCPLD.h"
#include "EOSCardAlarmDataDefine.h"
#include "EosCardConfigDefine.h"


class EOS8CardAlarmModule;

class Card8EOS: public CBaseCard {
    static const uint16 REG_ADDR_RESET = 0x1101;
    Card8EOS();
public:
    Card8EOS(std::string& name, CBaseSlot* slot);
    virtual ~Card8EOS();

    virtual int GetCartTypeID();
    virtual uint8* getConfigDataBuff(void) {
        return (uint8*) &ConfigData;
    };
    virtual uint32 getConfigDataLen(void) {
        return sizeof(ConfigData);
    };
    virtual void loadDefaultData(void);
    virtual std::string& GetCardVerInfo(){
       		return cardversionInfo;
    };

private:
    friend class EOS8CardAlarmModule;
    Chip8EOSCPLD cpld;
    Chip8EOSLogic fpga;
    ChipRC6400 eos_1;
    ChipRC6400 eos_2;

    std::string cardversionInfo;

    CHVC* chvc_obj[126];
    VCG* vcg_obj[8];
    Card_ConfigData_8EOS ConfigData;
    EOS8CardAlarmModule* AM;
};

#endif /* CARD8EOS_H_ */
