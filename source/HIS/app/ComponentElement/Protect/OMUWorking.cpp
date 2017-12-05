/*
 * OMUWorking.cpp
 *
 *  Created on: 2014��11��27��
 *      Author: Administrator
 */

#include "OMUWorking.h"
#include "CommunicationModule.h"
#include "InnerDCCManager.h"
#include "GeneralLogic.h"
#include "ObjectReference.h"

#include "CardXC.h"
#include "DeviceComponent.h"
#include "net_api.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "DeviceAttribute.h"
#include "CardDXC.h"
#include "SysError.h"
#include "SyncFileAPI.h"

TASK static void process_Work_LED(void);
TASK static void check_opt_los(void);
TASK static void Sync_all_files(void);

OMUWorking::OMUWorking() : OMUStatus(50){
    //ϵͳ��ʼ���������豸���
    if( DeviceComponent::initDeviceComponent() < 0 ) {
#ifdef EZ_DEBUG
    std::cout << "!!!DeviceComponent init failed!!!" << std::endl;
#endif
        throw SysError("OMUWorking creat error");
    }
    CommunicationModule::initWorking();
    GeneralLogic::instance().openEZBUS();

    task_work = os_tsk_create(process_Work_LED, P_LED);
    if(DeviceComponent::getDeviceAttribute().getDeviceSubType() != SUBTYPE_EMUX) {
    	task_fast_alarm = os_tsk_create(check_opt_los, P_FAST_CHECK);
    }
    task_sync_files = os_tsk_create(Sync_all_files, P_FILE_SYNC - 1);

    lockIt(false);
    SyncFileAPI::instance().initSyncFileAPI();

    DeviceComponent::run();
}

OMUWorking::~OMUWorking() {
#ifdef EXT_DEBUG
    printf("\n~OMUWorking()\n");
#endif
    CommunicationModule::clearWorking();
    SyncFileAPI::instance().clearSyncFileAPI();
    os_tsk_delete(task_sync_files);
    os_tsk_delete(task_fast_alarm);
    os_tsk_delete(task_work);
    DeviceComponent::clearDeviceCompnent();
    GeneralLogic::instance().closeEZBUS();
}

TASK void process_Work_LED(void) {
    os_evt_wait_and(0x8000, 0xffff);//等待槽位初始化完成
    bool ifinit = false;
    bool started = false;
    while( 1 ) {
        os_dly_wait(500);
        if( ifinit && !started ) {
        	started = true;
        	InnerDCCManager::instance().start(true);
        }
        for (int i = 0; i < 2; ++i) {
            bool actWorking = GeneralLogic::instance().ifXCWorking(i);
            if(DeviceComponent::getDeviceAttribute().getDeviceSubType() != SUBTYPE_EMUX) {
                CardXC* c = ObjectReference::getPXCCard(i);
                if( c ) {
                    try {
                        c->changeWorkingSta(actWorking);
                    }
                    catch( SysError& e ) {
                        std::cout << e.what() << std::endl;
                    }
                    c->setWorkLED(actWorking);
                }
            }
            else {
                CardDXC* c = ObjectReference::getDXCCard(i);
                if( c ) {
                    try {
                        c->changeWorkingSta(actWorking);
                    }
                    catch( SysError& e ) {
                        std::cout << e.what() << std::endl;
                    }
                    c->setWorkLED(actWorking);
                }
            }
        }
        ifinit = true;

    }
}

TASK void check_opt_los(void) {
    os_dly_wait(500);
    while(1) {
        CardXC* xc0 = ObjectReference::getPXCCard(0);
        CardXC* xc1 = ObjectReference::getPXCCard(1);
        uint8 state0 = 0xff;
        uint8 state1 = 0xff;
        if( xc0 ) {
            state0 = xc0->stm4_obj[0]->getLos() || xc0->stm4_obj[1]->getLos();
        }
        if( xc1 ) {
            state1 = xc1->stm4_obj[0]->getLos() || xc1->stm4_obj[1]->getLos();
        }
        if( xc0 ) {
            xc0->pcmLgc.setAlarm(state1, state0);
        }
        if( xc1 ) {
            xc1->pcmLgc.setAlarm(state0, state1);
        }
        os_dly_wait(100);
    }
}

TASK void Sync_all_files(void) {
    //syn all cfg Files
    bool RemoteExist = false;
    uint8 synCounter = 3;
    os_dly_wait(3000);
    while( 1 ) {
        os_dly_wait(200);
        if( !GeneralLogic::instance().ifRemoteExist() ) {
            if( synCounter > 3 ) {
                --synCounter;
            }
            if( synCounter == 3 ) {
                if( RemoteExist ) {
                    printf("\n!!!Remote OMU disconnect!!!\n");
                }
                RemoteExist = false;
            }
            continue;
        }
        else {
            if( synCounter < 7 ) {
                ++synCounter;
            }
            if( synCounter == 7 ) {
                if( !RemoteExist ) {
                    printf("\nRemote OMU connect!\n");
                }
               RemoteExist = true;
            }
        }
        if( RemoteExist != SyncFileAPI::instance().ifProtected() ) {
            SyncFileAPI::instance().setProtectSignal(RemoteExist);
            if( RemoteExist ) {
                SyncFileAPI::instance().sendAllFiles();
            }
        }
    }
}
