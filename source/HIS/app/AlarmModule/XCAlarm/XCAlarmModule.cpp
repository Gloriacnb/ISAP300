/*
 * XCAlarmModule.cpp
 *
 *  Created on: 2014
 *      Author: Administrator
 */

#include "XCAlarmModule.h"
#include "CardXC.h"
#include "SrcGroupSTMPort.h"
#include "SrcAlarmUpE1.h"
#include "XCAlarmDataDefine.h"
#include "PortFE1.h"

TASK void Do_LED(void* xc);

XCAlarmModule::XCAlarmModule(CardXC* c) : BAlarmModule(c) {
    XCCard = c;
    xc_alarm = os_tsk_create_ex(Do_LED, P_LED, this);
}

XCAlarmModule::~XCAlarmModule() {
    os_tsk_delete(xc_alarm);
}


void XCAlarmModule::creatAlarmSource() {
    for (int i = 0; i < 2; ++i) {
        addAlmSrc( new SrcGroupSTMPort(this, XCCard->stm4_obj[i], &(XCCard->AlarmData.STMAlmData[i]), XCCard->cpld) );
    }
    for (int i = 0; i < 4; ++i) {
        addAlmSrc( new SrcAlarmUpE1(i, this, static_cast<RealPortBase*>(XCCard->fe1_obj[i]),
                &(XCCard->AlarmData.XE1AlmData[i]), XCCard->pcmLgc ) );
    }
}

void XCAlarmModule::turnLedMajOn(bool on) {
    XCCard->cpld.turnLedMajOn(on);
}
void XCAlarmModule::turnLedMinOn(bool on) {
    XCCard->cpld.turnLedMinOn(on);
}


TASK void Do_LED(void* pxc) {
    XCAlarmModule* xc = (XCAlarmModule*)pxc;
    while(1) {
        os_dly_wait(300);
        bool maj = false;
        bool min = false;
        BAlarmModule* mm = BAlarmModule::getFirstInstance();
        while( mm ) {
            if( !maj ) {
                maj = mm->ifHaveMAJ();
            }
            if( !min ) {
                min = mm->ifHaveMin();
            }
            mm = BAlarmModule::getNextInstance(mm->itsSn());
        }
        if( maj ) {
            //�������澯��
            xc->turnLedMajOn(true);
        }
        else {
            //�����澯��
            xc->turnLedMajOn(false);
       }
        if( min ) {
            //������Ҫ�澯��
            xc->turnLedMinOn(true);
        }
        else {
            //���Ҫ�澯��
            xc->turnLedMinOn(false);
       }

    }
}
