/*
 * SlotFan.h
 *
 *  Created on: 2013-12-5
 *      Author: Administrator
 */

#ifndef SLOTFAN_H_
#define SLOTFAN_H_

#include "CBaseSlot.h"
#include "CardTypeID_define.h"
#include "FANPOWModule.h"

class ComponentFactory;
class RegisterAccess;
class SlotFan : public CBaseSlot {
public:
    SlotFan(int sn, RegisterAccess*, ComponentFactory* f);
    virtual ~SlotFan();


    virtual int GetCardTypeIDFromHardware();
    virtual bool supportThisCard(int cardType);
//     virtual bool reset(uint32){
//     	return true;
//     }
private:
    FANPOWModule mod;
};

#endif /* SLOTFAN_H_ */
