/*
 * SoftWDT.cpp
 *
 *  Created on: 2016年4月12日
 *      Author: Administrator
 */

#include "SoftWDT.h"
#include "GeneralLogic.h"
#include <stdio.h>

SoftWDT SoftWDT::wdt;

TASK void T_SoftWDT(void);

OS_MUT mut_softwdt;

void SoftWDT::init(void) {
    os_mut_init(mut_softwdt);

}

void SoftWDT::startSoftWDT(void) {
    task_soft_wdt = os_tsk_create(T_SoftWDT, P_WDT);
}

void SoftWDT::stopSoftWDT(void) {
    GeneralLogic::instance().stopWTD();
    os_tsk_delete(task_soft_wdt);
    workingWdt.clear();
}

void SoftWDT::pause(void) {
    GeneralLogic::instance().stopWTD();
}
void SoftWDT::resume(void) {
    GeneralLogic::instance().startWTD();
}

void SoftWDT::add(OS_TID t, WDT_Para_T* d) {
    if( d ) {
        d->current_count = OUT_TIME_COUNT;
    }
    workingWdt.insert(std::pair<OS_TID, WDT_Para_T*>(t, d));
}
void SoftWDT::feed(OS_TID t) {
    std::map<OS_TID, WDT_Para_T*>::iterator it = workingWdt.find(t);
    if( it != workingWdt.end() ) {
//        os_mut_wait(mut_softwdt, 0xffff);
        it->second->current_count = OUT_TIME_COUNT;
//        os_mut_release(mut_softwdt);
    }
}


TASK void T_SoftWDT(void) {
    GeneralLogic::instance().startWTD();
    while(1) {
        bool outtime = false;
        os_dly_wait(100);
        std::map<OS_TID, WDT_Para_T*>::iterator it = SoftWDT::instance().workingWdt.begin();
        while( it != SoftWDT::instance().workingWdt.end() ) {
            if( it->second->current_count == 0 ) {
                outtime = true;
#ifdef EZ_DEBUG
                printf("task:%d timeout\n", it->first);
#endif
                break;
            }
            else {
//                os_mut_wait(mut_softwdt, 0xffff);
                it->second->current_count--;
//                os_mut_release(mut_softwdt);
            }
			it++;
        }
        if( !outtime ) {
            GeneralLogic::instance().feedWTD();
        }
    }

}
