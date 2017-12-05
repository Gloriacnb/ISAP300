/*
 * NMPort.cpp
 *
 *  Created on: 2015年10月12日
 *      Author: Administrator
 */

#include "NMPort.h"
#include "RealPortBase.h"
#include "InnerDCCManager.h"
#include "NMChannel.h"
#include "PairTSChannel.h"
#include <string.h>
#include "SwitchPortInner.h"
#include "UID.h"
#include "CPPTools.h"
#include "SwitchCore.h"
#include <iostream>

std::map<uint32, NMPort*> NMPort::NMPortMap;

#ifdef EZ_DEBUG
#include "SwTrace.h"
#endif

NMPort::NMPort(RealPortBase* phy, FE1_NM_Config_Data_T* cfgdata) : Resource(phy->getUID()){

    upstreamUid = 0;
    memset( upstreamIP, 0, 4);
    memset( upstreamIpMask, 0 , 4);
    port = 0;
    phyPort = phy;
    WorkingNMCh = 0;
    ConfigData = cfgdata;
    topoAgingCounter = 300;

    NMPortMap.insert(std::pair<uint32, NMPort*>(phyPort->getUID(), this));
#ifdef EZ_DEBUG
    trace = new SwTrace(phyPort->getName());
#endif


}

NMPort::~NMPort() {
    NMPortMap.erase(phyPort->getUID());
}

bool NMPort::start(void) {
    //在此恢复配置，并加入到交换口
    if( !setType(ConfigData->type, false) ) {
        return false;
    }
    if( ConfigData->type == 0 ) {
        //sabit dcc
        WorkingNMCh->setTSMap(ConfigData->sabitCH.bitMap);
    }
    else {
        //dcn
        WorkingNMCh->setTSMap(ConfigData->dcnCH.bitMap);
    }
    setTopoDirection(ConfigData->topodir, false);
    setEnable(ConfigData->porten, false);

    return true;
}

bool NMPort::stop(void) {
    setEnable(0, false);
    if( getInnerPort() != 0 ) {
        InnerDCCManager::instance().leave(this);
    }
    return true;
}

void NMPort::sendData(uint8* d, uint32 len) {
    if( ConfigData->porten == 0 ) {
        return;
    }

    WorkingNMCh->sendData(d, len);
#ifdef EZ_DEBUG
        trace->sendOnePkg();
#endif
}

bool NMPort::receivData(void) {
    if( ConfigData->porten == 0 ) {
        return false;
    }

    if( WorkingNMCh->receivData() ) {
#ifdef EZ_DEBUG
        trace->receiveOnePkg();
#endif
        return true;
    }
    return false;
}

uint8 NMPort::getEnable(void) {
    return ConfigData->porten;
}

/*
 * 管理端口的使能，表示该端口正常加入交换体系
 * 首先注册为软交换端口下的一个管理接口，然后启动其包含的工作管理通道的
 * 禁能，需要关闭其工作通道，然后从交换端口中注销
 */
bool NMPort::setEnable(uint8 en, bool save) {
//    if( en == ConfigData->porten && save ) {
//        return true;
//    }
    if( en ) {
        WorkingNMCh->start();
    }
    else {
        WorkingNMCh->stop();
    }
    if( save ) {
        ConfigData->porten = en;
        return phyPort->saveConfig();
    }
    return true;
}

uint8 NMPort::getType(void) {
    return ConfigData->type;
}

/*
 * 设置管理端口类型，即设置工作管理通道。
 */
bool NMPort::setType(uint8 ntype, bool save) {
    if( ntype == ConfigData->type && save ) {
        return true;
    }
    /*
     * 如果是通道切换，需要先关闭原管理通道。
     */
    if( WorkingNMCh != 0 ) {
        WorkingNMCh->stop();
    }
    WorkingNMCh = useableCH[ntype];
    setEnable(ConfigData->porten, false);
    if( save ) {
        ConfigData->type = ntype;
        return phyPort->saveConfig();
    }
    return true;
}

uint32 NMPort::getTopoDirection(void) {
    if( getInnerPort() == 0 ) {
        return 0;
    }
    return getInnerPort()->getPortSn();
}
bool NMPort::setTopoDirection(uint32 dir, bool save) {
    if( (dir == ConfigData->topodir) && save ) {
        return true;
    }
    /*
     * 如果已经加入交换体系，则需要先注销，再注册
     */
    if( getInnerPort() != 0 ) {
        InnerDCCManager::instance().leave(this);
    }
    if( InnerDCCManager::instance().join(this, dir) ) {
        if( save ) {
            ConfigData->topodir = dir;
            return phyPort->saveConfig();
        }
        else {
            return true;
        }
    }
    return false;
}
std::string NMPort::getTsInfo(void) {
    uint32 bitmap = ConfigData->dcnCH.bitMap;
    std::string rtn;
    ST_Time_Slot tsinfo;
    tsinfo.slot = UID::breakUID(getUID()).E1.slot;
    if( tsinfo.slot < 2 ) {
        tsinfo.slot = 0;
    }
    tsinfo.xcsn = 0;
    for (int i = 0; i < 32; ++i) {
        uint8 bitval = (bitmap >> i) & 1;
        if( bitval == 1 ) {
            tsinfo.E1 = UID::breakUID(getUID()).E1.E1;
            if( tsinfo.slot != 0 ) {
                tsinfo.E1++;
            }
            tsinfo.TS = i;
            uint32 tsuid = UID::makeUID(&tsinfo);
            PairTSChannel* ts = PairTSChannel::getInstance(tsuid);
            if( ts != 0 ) {
                rtn = rtn + CPPTools::number2string(tsinfo.TS) + ",";
            }
        }
    }
    return rtn;
}

bool NMPort::addNMTs(uint32 tsuid, bool save) {
    if( WorkingNMCh->getMode() != dcn ) {

        return false;
    }
    //检查时隙是否存在，是否被占用
    PairTSChannel* ts = PairTSChannel::getInstance(tsuid);
    if( ts == 0 || ts->getConnectFrom() != 0 ) {
        return false;
    }

    //获取时隙序列号
    uint8 sn = UID::breakUID(tsuid).ts.TS;

    if( sn == 0 || sn == 16 ) {
        return false; //不能使用0和16时隙
    }
    uint32 bitmap = ConfigData->dcnCH.bitMap;
    bitmap |= (1 << sn);
    if( WorkingNMCh->setTSMap(bitmap) ) {
        if( save ) {
            ConfigData->dcnCH.bitMap = bitmap;
            return phyPort->saveConfig();
        }
        else {
            return true;
        }
    }
    return false;
}
bool NMPort::deleteNMTs(uint32 tsuid, bool save) {
    if( WorkingNMCh->getMode() != dcn ) {

        return false;
    }
    //检查时隙是否存在，是否被占用
    PairTSChannel* ts = PairTSChannel::getInstance(tsuid);
    if( ts == 0 || ts->getConnectFrom() != 0 ) {
        return false;
    }

    //获取时隙序列号
    uint8 sn = UID::breakUID(tsuid).ts.TS;
    uint32 bitmap = WorkingNMCh->getTSMap();
    bitmap &= ~(1 << sn);
    if( WorkingNMCh->setTSMap(bitmap) ) {
        if( save ) {
            ConfigData->dcnCH.bitMap = bitmap;
            return phyPort->saveConfig();
        }
        else {
            return true;
        }
    }
    return false;
}

uint8* NMPort::getPortDescription(uint8* len) {
    *len = ConfigData->descLen;
    return ConfigData->desc;
}
bool NMPort::setPortDescription(uint8* d, uint8 len, bool save) {
    ConfigData->descLen = len;
    memcpy( ConfigData->desc, d, len);
    phyPort->saveConfig();
    return true;
}

void NMPort::addNmChannel(NMChannel* c) {
    c->setBelongPort(this);
    useableCH.push_back(c);
};

uint32 NMPort::getTsMap(void) {
    return ConfigData->dcnCH.bitMap >> 1;
}
bool NMPort::setTsMap(uint32 map, bool save) {
//    if( WorkingNMCh->getMode() != dcn ) {
//        return false;
//    }
//    if( ((map<<1) == ConfigData->dcnCH.bitMap) && save ) {
//        return true;
//    }
    if( useableCH[1]->setTSMap(map << 1 ) ) {
        if( save ) {
            ConfigData->dcnCH.bitMap = (map << 1);
            return phyPort->saveConfig();
        }
    }
    return false;
}

bool NMPort::printIpInfo(std::string& os) {
	os = SwitchCore::instance().getIpPortTable().listIpsAtPort(getTopoDirection());
	return true;
}
bool NMPort::addAnIp(char* ip) {
	uint8 p[4] = {0};
	CPPTools::ezip_aton(ip, p);
	return SwitchCore::instance().getIpPortTable().addFixIP(p, getTopoDirection());

}
bool NMPort::deleteAIP(char* ip) {
	uint8 p[4] = {0};
	CPPTools::ezip_aton(ip, p);
	return SwitchCore::instance().getIpPortTable().deleteAnIP(p);

}
