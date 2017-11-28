/*
 * ProtectAssistCell.cpp
 *
 *  Created on: 2014��11��27��
 *      Author: Administrator
 */

#include "ProtectAssistCell.h"
#include "OMUStandby.h"
#include "OMUWorking.h"
#include "GeneralLogic.h"
#include "CardCPU.h"
#include <iostream>
#include "DeviceComponent.h"
//#include "DeviceAttribute.h"
#include <string.h>
#include <stdio.h>
#include "CommunicationModule.h"

#include "CBaseSlot.h"
#include "SlotModule.h"
#include "SoftWDT.h"
#include "SysError.h"
#include "SyncFileAPI.h"

static void check_working(void*);


ProtectAssistCell ProtectAssistCell::pcell;

ProtectAssistCell::ProtectAssistCell() {
    omu = 0;
    working_state_check = 0;
    protectPause = false;
}

ProtectAssistCell::~ProtectAssistCell() {
    // TODO Auto-generated destructor stub
}

/*
 * 1.����豸����
 * 2.���IP���Զ��MCU��ֻ��Workingʱ���ã�
 * 2.��ʼ��Э��������ָ����λ��Ϣ
 * 3.����IDLE״̬
 * 4.��ʼ״̬������
 */
void ProtectAssistCell::start() {
    GeneralLogic::instance().assignSlot( CardCPU::itsSlot() );
    if( CardCPU::itsSlot() == 1 ) {
        os_dly_wait(300);
    }
    SoftWDT::instance().init(); //首先建立软狗，否则其它任务无法注册
    GeneralLogic::instance().FSMStart();
    DeviceComponent::initDeviceComponentCommon();
    CommunicationModule::initCommon();
    SoftWDT::instance().startSoftWDT(); //开启软狗
    if( getOMUWorkingStateByHardware() == OMU_Working ) {
        omu = new OMUWorking();
#ifdef EZ_DEBUG
    std::cout << "OMUWorking" << std::endl;
#endif
    }
    else {
        omu = new OMUStandby(); //
#ifdef EZ_DEBUG
    std::cout << "OMUStandby" << std::endl;
#endif
    }

    working_state_check = os_tsk_create_ex(check_working, P_Protect, this);
}

Working_STATE_E ProtectAssistCell::getOMUWorkingStateByHardware() {
    if( GeneralLogic::instance().ifOMUWorking() ) {
        return OMU_Working;
    }
    return OMU_Standby;
}

Working_STATE_E ProtectAssistCell::getOMUCurrentWorkingState() {
    return omu->getStatus();
}


void ProtectAssistCell::switchTo(Working_STATE_E st) {
    if( st == getOMUCurrentWorkingState() ) {
        return;
    }
//    CBaseSlot* slot = SlotModule::getSlot(CardCPU::itsSlot());
//    if( slot ) {
//        slot->reset(Warm_start);
//    }
    switch( st ) {
    case OMU_Working: {//因为从备用切换到working导致死机，所以切到主时采用复位方式
//        delete omu;
//        try {
//            omu = new OMUWorking();
//    #ifdef EZ_DEBUG
//        std::cout << "OMUWorking" << std::endl;
//    #endif
//        }
//        catch(SysError& e) {
//            std::cout << e.what() << std::endl;
//            while(1);
//        }
            CBaseSlot* rst = SlotModule::getSlot(0);
            if( rst ) {
                rst->reset(Warm_start);
            }
		}
        break;
    case OMU_Standby:
        if( omu->ifLock() ) {
            return;
        }
        if(omu) {
        	delete omu;
        }
        omu = new OMUStandby();
#ifdef EZ_DEBUG
    std::cout << "OMUStandby" << std::endl;
#endif
        break;
    default:
#ifdef EZ_DEBUG
    std::cout << "!!!ProtectAssistCell::switchTo ERROR!!!" << std::endl;
#endif
        break;
    }
}


TASK void check_working(void* pc) {
    ProtectAssistCell* pac = reinterpret_cast<ProtectAssistCell*>(pc);
    if( pac == 0 ) {
#ifdef EZ_DEBUG
        printf("\n!!!task check_working error!!!\n");
#endif
        os_tsk_delete_self();
    }
    static int count = 0;
    while( 1 ) {
        os_dly_wait(100);
        if( pac->protectPause ) {
            continue;
        }
        Working_STATE_E currentState = pac->getOMUWorkingStateByHardware();
        if( pac->getOMUCurrentWorkingState() != currentState ) {
            if( ++count > 3 ) {
                count = 0;
                pac->switchTo(currentState);
            }
        }
        else {
            count = 0; //保证必须连续三次采集到不同才会切换
        }
    }
}
