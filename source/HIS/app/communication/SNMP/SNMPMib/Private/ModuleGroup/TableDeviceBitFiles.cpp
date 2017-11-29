/*
 * TableDeviceBitFiles.cpp
 *
 *  Created on: 2016年6月16日
 *      Author: Administrator
 */

#include "TableDeviceBitFiles.h"

#include "TableDeviceBitFiles.h"
#include "CCommonDefine.h"
#include "os.h"
#include <string.h>
#include "FileFind.h"
#include "CPPTools.h"
#include "SyncFileAPI.h"

column_info_T TableDeviceBitFiles::colInfo[bfile_size] = {
        {1, Mib_read_only, CSnmpConstDefine::ucConstInteger32},     //文件序号
        {2, Mib_read_only, CSnmpConstDefine::ucConstOctetString},   //文件名称
        {3, Mib_read_only, CSnmpConstDefine::ucConstInteger32},     //文件大小
        {4, Mib_read_only, CSnmpConstDefine::ucConstInteger32},     //文件CRC校验是否正确
        {5, Mib_read_only, CSnmpConstDefine::ucConstInteger32},     //与备盘文件比对是否一致
        {6, Mib_write_only, CSnmpConstDefine::ucConstInteger32},    //将文件同步到备盘
        {7, Mib_write_only, CSnmpConstDefine::ucConstOctetString},  //将文件同步到指定IP
};


TableDeviceBitFiles::TableDeviceBitFiles(uint32* oid, uint32 oidLen, table_info_T* tInfo) : CMibTable(oid, oidLen, tInfo) {
    makeColumnOid();
}

TableDeviceBitFiles::~TableDeviceBitFiles() {
    // TODO Auto-generated destructor stub
}

column_info_T* TableDeviceBitFiles::getColInfo(int sn) {
    return &colInfo[sn];
}

CMibNodeObject* TableDeviceBitFiles::MakeColumn(int sn, uint32* oid, uint32 oidLen) {
    switch( sn ) {
    case bfile_sn:
        return new TableDeviceBitFiles_index(sn, oid, oidLen, this);
    case bfile_name:
        return new TableDeviceBitFiles_name(sn, oid, oidLen, this);
    case bfile_bytes:
        return new TableDeviceBitFiles_size(sn, oid, oidLen, this);
    case bfile_crcsta:
        return new TableDeviceBitFiles_crcsta(sn, oid, oidLen, this);
    case bfile_cmp:
        return new TableDeviceBitFiles_cmp(sn, oid, oidLen, this);
    case bfile_sync2B:
        return new TableDeviceBitFiles_sync2b(sn, oid, oidLen, this);
    case bfile_syncP:
        return new TableDeviceBitFiles_syncp(sn, oid, oidLen, this);
    default:
        return 0;
    }
}

int TableDeviceBitFiles::getFirstIndex(index_info_T* index) {
    index->index_number = getTableInfo()->table_index_num;
    index->index[0] = CCommonDefine::uiConstDeviceIndex;
    FINFO info;
    info.fileID  = 0;
    char* mask = "*.bit";
    if( ffind (mask,&info) == 0 ) {
     index->index[1] = info.fileID;
     return 1;
    }
    return -1;
}

int TableDeviceBitFiles::getNextIndex(index_info_T* index) {
    index->index_number = getTableInfo()->table_index_num;
    index->index[0] = CCommonDefine::uiConstDeviceIndex;
    FINFO info;
    info.fileID  = 0;
    char* mask = "*.bit";
    while( ffind (mask,&info) == 0 ) {
        if( index->index[1] == info.fileID ) {
            if( ffind (mask,&info) == 0 ) {
                index->index[1] = info.fileID;
                return 1;
            }
        }
    }
    return -1;
}

int TableDeviceBitFiles_index::CallbackGet(const index_info_T& index) {
    return index.index[1];
}

int TableDeviceBitFiles_name::CallbackGet(const index_info_T& index, uint8* name, uint32* len) {
    FINFO info;
    if( FileFind::getBitFileInfoByID(index.index[1], info) ) {
        *len = strlen((char*)info.name);
        strcpy((char*)name, (char*)info.name);
        return 1;
    }
    return -1;
}

int TableDeviceBitFiles_size::CallbackGet(const index_info_T& index) {
    FINFO info;
    if( FileFind::getBitFileInfoByID(index.index[1], info) ) {
        return info.size;
    }
    return -1;

}

int TableDeviceBitFiles_crcsta::CallbackGet(const index_info_T& index) {
    FINFO info;
    if( FileFind::getBitFileInfoByID(index.index[1], info) ) {
        uint32 expectcrc = 0;
        uint32 actualcrc = 0;
        CPPTools::getFileCRC((char*)info.name, &expectcrc);
        CPPTools::calculateFileCRC((char*)info.name, &actualcrc);
        if( expectcrc != actualcrc ) {
            return 0;
        }
        return 1;
    }
    return -1;

}

int TableDeviceBitFiles_cmp::CallbackGet(const index_info_T& index) {
    FINFO info;
    if( FileFind::getBitFileInfoByID(index.index[1], info) ) {
        return SyncFileAPI::instance().ifSame((char*)info.name);
    }
    return -1;
}

int TableDeviceBitFiles_sync2b::callbackSet( const index_info_T& index, uint32 ) {
    FINFO info;
    if( FileFind::getBitFileInfoByID(index.index[1], info) ) {
        SyncFileAPI::instance().sendAfileToRemote((char*)info.name);
        return 0x5a;
    }
    return -1;
}

int TableDeviceBitFiles_syncp::callbackSet( const index_info_T& index, uint8*, uint32 len) {
    return -1;
}
