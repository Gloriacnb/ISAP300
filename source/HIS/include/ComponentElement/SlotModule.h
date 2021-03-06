/*
 * SlotModule.h
 *
 *  Created on: 2013-11-19
 *      Author: Administrator
 */

#ifndef SLOTMODULE_H_
#define SLOTMODULE_H_

#include <vector>

class CBaseSlot;
class ComponentFactory;
class CCardOnlineChecker;

class SlotModule {
public:
//    SlotModule(ComponentFactory* f);
    SlotModule();
    virtual ~SlotModule();

    static void initSlotModule(ComponentFactory* f);
    static void initSlotModuleWorking(void);
    static void clearSlotModuleWorking(void);
//    bool addSlot(CBaseSlot* slt);
    static std::vector<CBaseSlot*>& getSlots(void) {
        return slots;
    };

    static CBaseSlot* getSlot(int n) {
        if( n > 9 ) {
            return 0;
        }
        return slots[n];
    };

    static void resetPLL(int sn);
    static void startExtChecking(void);
    static void stopExtChecking(void);
private:
    static std::vector<CBaseSlot*> slots;
    static ComponentFactory* fac;
    static CCardOnlineChecker* onch;
};

#endif /* SLOTMODULE_H_ */
