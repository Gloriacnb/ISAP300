/*
 * SlotPower.h
 *
 *  Created on: 2013-12-5
 *      Author: Administrator
 */

#ifndef SLOTPOWER_H_
#define SLOTPOWER_H_


#include "CBaseSlot.h"
#include "CardTypeID_define.h"
#include "FANPOWModule.h"

class ComponentFactory;
class RegisterAccess;
class SlotPower : public CBaseSlot {
public:
    SlotPower(int sn, RegisterAccess*, ComponentFactory* f);
    virtual ~SlotPower();


    virtual int GetCardTypeIDFromHardware();
    virtual bool supportThisCard(int cardType);
//     virtual bool reset(uint32){
// 		return true;
// 	};
private:
    FANPOWModule mod;
};

#endif /* SLOTPOWER_H_ */
