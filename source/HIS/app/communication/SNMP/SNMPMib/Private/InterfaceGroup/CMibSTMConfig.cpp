/*
 * CMibSTMConfig.cpp
 *
 *  Created on: 2013-4-10
 *      Author: Administrator
 */

#include "CMibSTMConfig.h"
#include "CCommonDefine.h"
#include <string>
#include <string.h>
#include "PortSTM4.h"
#include "PairPortSTM.h"
#include "UID.h"
//#include "portAccess.h"

column_info_T CMibSTMConfig::colInfo[stm_cfg_size] = {
		{1, Mib_read_only, CSnmpConstDefine::ucConstInteger32},
		{2, Mib_read_only, CSnmpConstDefine::ucConstOctetString},
		{3, Mib_read_write, CSnmpConstDefine::ucConstInteger32},
		{4, Mib_read_write, CSnmpConstDefine::ucConstInteger32},
		{5, Mib_read_write, CSnmpConstDefine::ucConstInteger32},
		{6, Mib_read_write, CSnmpConstDefine::ucConstOctetString},
		{7, Mib_read_write, CSnmpConstDefine::ucConstInteger32},
        {8, Mib_read_write, CSnmpConstDefine::ucConstInteger32},

};



CMibSTMConfig::CMibSTMConfig(uint32* oid, uint32 oidLen, table_info_T* tInfo) : CMibTable(oid, oidLen, tInfo) {
	makeColumnOid();
}

CMibSTMConfig::~CMibSTMConfig() {
	// TODO Auto-generated destructor stub
}

column_info_T* CMibSTMConfig::getColInfo(int sn) {
	return &colInfo[sn];
}


/*
 * STMConfig table ��������
 */
CMibNodeObject* CMibSTMConfig::MakeColumn(int sn, uint32* oid, uint32 oidLen) {
	switch( sn ) {
	case stm_cfg_uid:
		return new CMibSTMConfigCol_uid(sn, oid, oidLen, this);
	case stm_cfg_name:
		return new CMibSTMConfigCol_name(sn, oid, oidLen, this);
	case stm_cfg_enable:
		return new CMibSTMConfigCol_enable(sn, oid, oidLen, this);
	case stm_cfg_als_enable:
		return new CMibSTMConfigCol_als(sn, oid, oidLen, this);
	case stm_cfg_port_backup:
		return new CMibSTMConfigCol_port_backup(sn, oid, oidLen, this);
	case stm_cfg_desc:
		return new CMibSTMConfigCol_desc(sn, oid, oidLen, this);
	case stm_cfg_als_mode:
		return new CMibSTMConfigCol_alsmode(sn, oid, oidLen, this);
	case stm_cfg_dcc_sel:
	    return new CMibSTMConfigCol_dccsel(sn, oid, oidLen, this);
	default:
		return 0;
	}
}


int CMibSTMConfig::getFirstIndex(index_info_T* index) {
	index->index_number = getTableInfo()->table_index_num;
	index->index[0] = CCommonDefine::uiConstDeviceIndex;
	PairPortSTM* p = PairPortSTM::getFirstInstance();
	if( p ) {
	   index->index[1] = (p->getUID() >> 24) + 1;
       index->index[2]	= p->getUID();
        return 1;
	}
	return -1;
}
int CMibSTMConfig::getNextIndex(index_info_T* index) {
	index->index_number = getTableInfo()->table_index_num;
	index->index[0] = CCommonDefine::uiConstDeviceIndex;
	uint8 slot = (index->index[2] >> 24) & 0xf;
	Resource* pp = 0;
	if( slot < 2 ) {
        pp = PairPortSTM::getNextInstance(index->index[2]);
        if( !pp ) {
            pp = PortSTM4::getNextInstance(UID::getPartnerUID(index->index[2]));
            if( !pp ) {
                pp = PortSTM4::getNextInstance(index->index[2]);
            }
        }

	}
	else {
        pp = PortSTM4::getNextInstance(index->index[2]);

	}
    if( pp ) {
        index->index[1] = (pp->getUID() >> 24) + 1;
        index->index[2] = pp->getUID();
        return 1;
    }
	return -1;

}


int CMibSTMConfigCol_uid::CallbackGet(const index_info_T& index) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm ) {
            return index.index[2];
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm ) {
            return index.index[2];
        }
    }
    return -1;
}

int CMibSTMConfigCol_name::CallbackGet(const index_info_T& index, uint8* name, uint32* len) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm ) {
            std::string s = pstm->getName();
            *len = s.size();
            strcpy((char*)name, s.c_str());
            return 1;
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm ) {
            std::string s = pstm->getName();
            *len = s.size();
            strcpy((char*)name, s.c_str());
            return 1;
        }
    }
	return -1;
}

int CMibSTMConfigCol_enable::CallbackGet(const index_info_T& index) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm ) {
            return pstm->getPortEnable();
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm ) {
            return pstm->getPortEnable();
        }
    }
	return -1;
}

int CMibSTMConfigCol_enable::callbackSet( const index_info_T& index, uint32 en) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm ) {
            pstm->setPortEnable((uint8)en);
            return 0x5A;
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm ) {
            pstm->setPortEnable((uint8)en);
            return 0x5A;
        }
    }
	return -1;
}

int CMibSTMConfigCol_als::CallbackGet(const index_info_T& index) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm ) {
            return pstm->getAlsEnable();
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm ) {
            return pstm->getAlsEnable();
        }
    }
	return -1;
}
int CMibSTMConfigCol_als::callbackSet(  const index_info_T& index, uint32 data) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm ) {
            pstm->setAlsEnable((uint8)data);
            return 0x5A;
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm ) {
            pstm->setAlsEnable((uint8)data);
            return 0x5A;
        }
    }
	return -1;
}



int CMibSTMConfigCol_port_backup::CallbackGet(const index_info_T& index) {
	return 1;//DeviceLocal::instance().GetSdhObject().getPortBackup();
}

int CMibSTMConfigCol_port_backup::callbackSet( const index_info_T& index, uint32 en) {
//	DeviceLocal::instance().GetSdhObject().setPortBackup(en);
	return 0x5A;
}

int CMibSTMConfigCol_desc::CallbackGet(const index_info_T& index, uint8* desc, uint32* len) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm ) {
            std::string s = pstm->getDescription();
            *len = s.size();
            memcpy(desc, s.c_str(), *len);
            return 1;
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm ) {
            std::string s = pstm->getDescription();
            *len = s.size();
            memcpy(desc, s.c_str(), *len);
            return 1;
        }
    }
	return -1;
}
int CMibSTMConfigCol_desc::callbackSet(const index_info_T& index, uint8*desc, uint32 len) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm != 0 && desc != 0 ) {
            pstm->setDescription((char*)desc, (uint8)len);
            return 0x5A;
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm != 0 && desc != 0 ) {
            pstm->setDescription((char*)desc, (uint8)len);
            return 0x5A;
        }
    }
	return -1;
}


int CMibSTMConfigCol_alsmode::CallbackGet(const index_info_T& index) {
//    uint8 slot = (index.index[2] >> 24) & 0xf;
//    if( slot < 2 ) {
//        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
//        if( pstm ) {
//            return pstm->getAlsEnable();
//        }
//    }
//    else {
//        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
//        if( pstm ) {
//            return pstm->getAlsEnable();
//        }
//    }
	return 0;
}
int CMibSTMConfigCol_alsmode::callbackSet(  const index_info_T& index, uint32 data) {
//    uint8 slot = (index.index[2] >> 24) & 0xf;
//    if( slot < 2 ) {
//        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
//        if( pstm ) {
//            pstm->setAlsEnable((uint8)data);
//            return 0x5A;
//        }
//    }
//    else {
//        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
//        if( pstm ) {
//            pstm->setAlsEnable((uint8)data);
//            return 0x5A;
//        }
//    }
	return 0;
}

int CMibSTMConfigCol_dccsel::CallbackGet(const index_info_T& index) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm ) {
            return pstm->getDCCSel();
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm ) {
            return pstm->getDCCSel();
        }
    }
    return -1;
}
int CMibSTMConfigCol_dccsel::callbackSet( const index_info_T& index, uint32 data) {
    uint8 slot = (index.index[2] >> 24) & 0xf;
    if( slot < 2 ) {
        PairPortSTM* pstm = PairPortSTM::getInstance(index.index[2]);
        if( pstm->setDCCSel((uint16)data)) {
            return 0x5A;
        }
    }
    else {
        PortSTM4* pstm = PortSTM4::getInstance(index.index[2]);
        if( pstm->setDCCSel((uint16)data)) {
            return 0x5A;
        }
    }
    return -1;
}
