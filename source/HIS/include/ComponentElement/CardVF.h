/*
 * CardVF.h
 *
 *  Created on: 2014.7.14
 *      Author: Administrator
 */

#ifndef CARDVF_H_
#define CARDVF_H_


#include "CBaseCard.h"
#include "Chip30VFLogic.h"
#include "PortVF.h"
#include "Chip30VFCPLD.h"
#include "os.h"

typedef struct {
    VFPort_Config_T port[30];
    VFGroup_Config_T group[8];
}Card_ConfigData_30VF;

class CBaseSlot;
class PairTSChannel;
class VFCommand;

class CardVF : public CBaseCard {
public:
    CardVF(std::string& name, CBaseSlot* slot, uint8 pn);
    virtual ~CardVF();

    virtual int GetCartTypeID() = 0;
    virtual std::string& GetCardVerInfo(){//
        return cardversionInfo;
    }
    int processVFCommand(VFCommand& cmd);

    virtual uint8* getConfigDataBuff(void) {
        return (uint8*) &ConfigData;
    };
    virtual uint32 getConfigDataLen(void) {
        return sizeof(ConfigData);
    };
    virtual void loadDefaultData(void);
    virtual bool Resetcard(){
    	cpld.Resetchip();
    	return true;
    }

private:
    uint8 PortNumber;
    OS_MUT mut_vf_cmd;
    Chip30VFCPLD cpld;
    Chip30VFLogic chip;
    PortVF* port_obj[30];
    Card_ConfigData_30VF ConfigData;
    PairTSChannel* getConnectedTS(uint32 vfuid);
    static const uint8 port2ts[30];

    std::string cardversionInfo;
    std::string getMcuVersion(void);


};

#endif /* CARD30VF_H_ */
