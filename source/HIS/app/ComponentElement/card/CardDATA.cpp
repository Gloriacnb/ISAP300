/*
 * CardDATA.cpp
 *
 *  Created on: 2015年6月25日
 *      Author: Administrator
 */


#include "CardDATA.h"
#include "CardTypeID_define.h"
#include "UID.h"
//#include "GeneralLogic.h"
#include "SysError.h"
#include "PairTSChannel.h"


const uint8 CardDATA::port2ts[] ={1, 2, 3, 4, 17, 18, 19, 20, 5, 6, 7, 8, 21, 22, 23, 24 };//16port2ts

TASK void Do_16DATA_LED(void* pxc);
CardDATA::CardDATA(std::string& name, CBaseSlot* slt, uint8 pn) :
    CBaseCard(name,slt),drive(slt, pn),cpld(slt){
    portNumber = pn;
    Chip_INIT_RESULT rst = drive.ChipInit();
    if( rst == failed ) {
        throw SysError("!!!Card 16DATA E1Logic init error!!!");
    }
    if( !fetchConfig() ){
        throw SysError("!!!Card 16DATA config data error!!!");
    }
    C16DATA_LED =os_tsk_create_ex(Do_16DATA_LED,P_LED,this);
    ST_DATA info;
    info.slot = slt->GetSn();
    for(int i = 0; i < portNumber; i++){
        dataport[i] = 0;
        info.port = i;
        uint32 uid = UID::makeUID(&info);

        dataport[i] = new PortData(uid,this,getConnectedTS(uid),drive,&Config.port[i]);
    }
    cardversionInfo = cpld.GetVerInfo() + "," + drive.GetVerInfo();
}

CardDATA::~CardDATA() {
    os_tsk_delete(C16DATA_LED);
    for (int i = 0; i < portNumber; i++) {
        if(dataport[i])
            delete dataport[i];
    }
    // TODO Auto-generated destructor stub
}


PairTSChannel* CardDATA::getConnectedTS(uint32 datauid) {
    UN_Info datainfo = UID::breakUID(datauid);
    ST_Time_Slot tsinfo;
    tsinfo.slot = datainfo.data.slot;
    tsinfo.xcsn = 0;
    tsinfo.E1 = 0;
    tsinfo.TS = port2ts[datainfo.data.port];
    uint32 tsUID = UID::makeUID(&tsinfo);
    return PairTSChannel::getInstance(tsUID);
}

void CardDATA::loadDefaultData(void) {
    DATA_Config_T temp;
    memset(&temp, 0, sizeof(temp));
    temp.enable = 1;
    for (int i = 0; i < 16; ++i) {
        memcpy(&Config.port[i], &temp, sizeof(DATA_Config_T));
    }
}


TASK void Do_16DATA_LED(void* pxc){
    CardDATA* card = (CardDATA*)pxc;
    card->drive.ledWrk(true);
    card->drive.ledSta(false);
    while(1){
        os_dly_wait(50);
    }
//  os_tsk_delete_self();
}
