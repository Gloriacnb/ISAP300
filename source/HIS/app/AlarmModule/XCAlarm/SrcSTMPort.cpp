/*
 * SrcSTMPort.cpp
 *
 *  Created on: 2014
 *      Author: Administrator
 */

#include "SrcSTMPort.h"
#include "PortSTM4.h"
#include "BAlarmModule.h"
#include "SrcAlarmVC4.h"


SrcSTMPort::SrcSTMPort(BAlarmModule* m, PortSTM4* p, SrcSTMAlarm_Data* d) : BAlarmSource(m), port(p) {
    pConfigData = d;
    for (int i = 0; i < port->itsVC4Number(); ++i) {
        AlarmSrcObj[i] = new SrcAlarmVC4(m, port->vc4_obj[i], &(pConfigData->VC4AlmData[i]));
        m->addAlmSrc( AlarmSrcObj[i] );
    }
}

SrcSTMPort::~SrcSTMPort() {
}

/*
 * ��չ�澯Դ�����ƹ�ϵ
 */
bool SrcSTMPort::ifSrcRestrain() {
    return false;
}

bool SrcSTMPort::ifAlarm(void) {
    for (int i = STM_Sig_BASE; i < STM_Sig_BASE+STM_Signal_Count; ++i) {
        if( getAlarm(i) ) {
            return true;
        }
    }
    return false;
}
/*
 * �澯�ɼ�
 * ��ʵ��Ӳ����ȡ�澯״̬
 */
bool SrcSTMPort::getAlarm(int type) {
    switch( type-STM_Sig_BASE ) {
    case R_LOS:
        return port->getLos();
    case R_OOF:
        if( port->getLos() ) {
            return false;
        }
        return port->getOof();
    case R_LOF:
        if( port->getLos() ) {
            return false;
        }
        return port->getLof();
    case MS_AIS:
        if( port->getLos() ) {
            return false;
        }
        return port->getMsAis();
    case MS_RDI:
        if( port->getLos() ) {
            return false;
        }
        return port->getMsRdi();
    case MS_REI:
        if( port->getLos() ) {
            return false;
        }
        return port->getMsRei();
    default:
        break;
    }
    return false;
}

std::string SrcSTMPort::getTypeName(int type) {
    switch( type-STM_Sig_BASE ) {
    case R_LOS: {
        std::string name("R_LOS");
        return name;
    }
    case R_OOF: {
        std::string name("R_OOF");
        return name;
    }
    case R_LOF: {
        std::string name("R_LOF");
        return name;
    }
    case MS_AIS: {
        std::string name("MS_AIS");
        return name;
    }
    case MS_RDI: {
        std::string name("MS_RDI");
        return name;
    }
    case MS_REI: {
        std::string name("MS_REI");
        return name;
    }
    }
    std::string name("No exist");
    return name;
}


uint32 SrcSTMPort::itsID(void) {
    return port->getUID();
}

std::string& SrcSTMPort::itsName(void) {
    return port->getName();
}
