/*
 * DeviceAttribute.cpp
 *
 *  Created on: 2013-11-19
 *      Author: Administrator
 */

#include "DeviceAttribute.h"
#include <string>
//#include <iostream>
#include "SysError.h"
//#include <vector>
#include "CPPTools.h"
#include "CCommonDefine.h"
#include <stdio.h>
#include "net_api.h"
#include <string.h>
#include "SysError.h"
//#include "ProtectAssistCell.h"

using namespace std;


const string DeviceAttribute::fname("DeviceAttributeNew.txt");

DeviceAttribute::DeviceAttribute() : ConfigCell(fname, -1) {

    if( !fetchConfig() ) {
        throw SysError("!!!DeviceAttribute config data error!!!");
    }
    mode = getDeviceOPTMode();
}

DeviceAttribute::~DeviceAttribute() {
    // TODO Auto-generated destructor stub
}


void DeviceAttribute::loadDefaultData(void) {
    FILE* fold = fopen("DeviceAttribute.txt", "r");
    if( fold ) {
        fread(&ConfigData, sizeof(ConfigData), 1, fold);
        strcpy(ConfigData.trapdst, "192.168.1.1");
    }
    else {
        strcpy(ConfigData.subtypename, "emux-30");
        strcpy(ConfigData.serialnumber, "65535");
        strcpy(ConfigData.factorydate, "2015-01-01");
        strcpy(ConfigData.ip, "192.168.1.121");
        strcpy(ConfigData.ipmask, "255.255.255.0");
        strcpy(ConfigData.ipgateway, "192.168.1.1");
        strcpy(ConfigData.description, "");
        strcpy(ConfigData.name, "");
        strcpy(ConfigData.user, "");
        strcpy(ConfigData.other, "");
        strcpy(ConfigData.contact, "");
        strcpy(ConfigData.location, "");
        strcpy(ConfigData.trapdst, "192.168.1.1");
    }
    saveConfig();
}
//void DeviceAttribute::restoreFile(void) {
//    saveFile.clearContent();
//    vector<string> v;
//    v.push_back(fname);
//    v.push_back("isap300-155");
//    v.push_back("65535");
//    v.push_back("2014-01-01");
//    v.push_back("192.168.100.100");
//    v.push_back("255.255.255.0");
//    v.push_back("192.168.100.1");
//    v.push_back("description.");
//    v.push_back("name");
//    v.push_back("user");
//    v.push_back("other");
//    v.push_back("contact");
//    v.push_back("location");
//
//    for( int i = 0; i < ln_number; i++ ) {
//        saveFile.appendTxtLine(v[i] );
//    }
////    ProtectAssistCell::instance().synFileTo(fname.c_str());
//
//}



uint32 DeviceAttribute::getDeviceOPTMode(void) {
    std::string s;
    getBaseAttribute(ln_subtype, s);
    if( s == "isap300-155" || s == "ISAP300-155" ) {
        return 155;
    }
    else if( s == "isap300-622" || s == "ISAP300-622" ) {
        return 622;
    }
    return 0;
}


uint16 DeviceAttribute::getDeviceSn(void) {
    std::string s;
    getBaseAttribute(ln_serialnumber, s);
    int sn = 0;
    sscanf(s.c_str(), "%d", &sn);
    return sn;
}

int DeviceAttribute::getDeviceSubType(void) {
    std::string s;
    getBaseAttribute(ln_subtype, s);
    return getSubTypeUID(s);

}

void DeviceAttribute::getDeviceIP(std::string& s) {
    getBaseAttribute(ln_ipaddress, s);
}

void DeviceAttribute::getDeviceIP(uint8* ip) {
    if( ip ) {
        std::string s;
        getDeviceIP(s);
        CPPTools::ezip_aton(s, ip);
    }
}

void DeviceAttribute::getTrapDestIP(uint8* ip) {
    if( ip ) {
        std::string s;
        getBaseAttribute(ln_iptrapdest, s);
        CPPTools::ezip_aton(s, ip);
    }
}

void DeviceAttribute::getDeviceIPMask(uint8* mask) {
    if( mask ) {
        std::string s;
        getBaseAttribute(ln_ipmask, s);
        CPPTools::ezip_aton(s, mask);
    }
}

void DeviceAttribute::getDeviceIPGateway(uint8* host) {
    if( host ) {
        std::string s;
        getBaseAttribute(ln_ipgateway, s);
        CPPTools::ezip_aton(s, host);
    }
}

void DeviceAttribute::getProtectMCUIP(uint8* ip) {
    if( ip ) {
        uint8 ipaddr[4] = {0};
        getDeviceIP(ipaddr);
        uint8 ipmask[4] = {0};
        getDeviceIPMask(ipmask);
        uint8 tempip[4] = {0};
        for (int i = 0; i < 4; ++i) {
            tempip[i] = ipaddr[i] & ipmask[i];
            tempip[i] |= ~ipmask[i];
        }
        tempip[3] &= ~1;
        for (int i = 0; i < 4; ++i) {
            ip[i] = tempip[i];
        }
    }
}

void DeviceAttribute::getDeviceMac(uint8* mac) {
    if( mac ) {
        mac[0] = 0;
        mac[1] = (uint8)CCommonDefine::ucConstDeviceTypeID;
        mac[2] = (uint8)CCommonDefine::ucConstDeviceTypeID >> 8;
        mac[3] = 0;
        uint16 sn = getDeviceSn();
        mac[4] = sn >> 8;
        mac[5] = sn & 0xff;
    }
}


void DeviceAttribute::getBaseAttribute(Device_Attribute_ln_E ln, std::string& s) {
//    saveFile.getTxtLine(ln, s);
    switch( ln ) {
    case ln_info:
        break;
    case ln_subtype:
        s.append(ConfigData.subtypename);
        break;
    case ln_serialnumber:
        s.append(ConfigData.serialnumber);
        break;
    case ln_factorydate:
        s.append(ConfigData.factorydate);
        break;
    case ln_ipaddress:
        s.append(ConfigData.ip);
        break;
    case ln_ipmask:
        s.append(ConfigData.ipmask);
        break;
    case ln_ipgateway:
        s.append(ConfigData.ipgateway);
        break;
    case ln_description:
        s.append(ConfigData.description);
        break;
    case ln_name:
        s.append(ConfigData.name);
        break;
    case ln_user:
        s.append(ConfigData.user);
        break;
    case ln_other:
        s.append(ConfigData.other);
        break;
    case ln_contact:
        s.append(ConfigData.contact);
        break;
    case ln_location:
        s.append(ConfigData.location);
        break;
    case ln_iptrapdest:
        s.append(ConfigData.trapdst);
        break;
    default:
        return;
    }

}

int DeviceAttribute::setBaseAttribute(Device_Attribute_ln_E ln, std::string& s) {
//    if( ln == ln_ipaddress || ln == ln_ipmask ) {
//        uint8 ip[4];
//        uint8 mask[4];
//        getDeviceIP(ip);
//        getDeviceIPMask(mask);
//        setNetIP(ip, mask);
//
//    }
//    else if( ln == ln_ipgateway ) {
//        uint8 gw[4];
//        getDeviceIPGateway(gw);
//        setNetGW(gw);
//    }
//    saveFile.modifyTxtLine(ln, s);
    switch( ln ) {
    case ln_info:
        break;
    case ln_subtype:
        if( s.size() > 15 ) {
            return -1;
        }
        strcpy(ConfigData.subtypename, s.c_str());
        break;
    case ln_serialnumber:
        if( s.size() > 15 ) {
            return -1;
        }
        strcpy(ConfigData.serialnumber, s.c_str());
        break;
    case ln_factorydate:
        if( s.size() > 15 ) {
            return -1;
        }
        strcpy(ConfigData.factorydate, s.c_str());
        break;
    case ln_ipaddress: {
        if( s.size() > 15 ) {
            return -1;
        }
        strcpy(ConfigData.ip, s.c_str());
        uint8 ip[4];
        uint8 mask[4];
        getDeviceIP(ip);
        getDeviceIPMask(mask);
        setNetIP(ip, mask);
    }
        break;
    case ln_ipmask: {
        if( s.size() > 15 ) {
            return -1;
        }
        strcpy(ConfigData.ipmask, s.c_str());
        uint8 ip[4];
        uint8 mask[4];
        getDeviceIP(ip);
        getDeviceIPMask(mask);
        setNetIP(ip, mask);
    }
        break;
    case ln_ipgateway: {
        if( s.size() > 15 ) {
            return -1;
        }
        strcpy(ConfigData.ipgateway, s.c_str());
        uint8 gw[4];
        getDeviceIPGateway(gw);
        setNetGW(gw);
    }
        break;
    case ln_iptrapdest:
        if( s.size() > 15 ) {
            return -1;
        }
        strcpy(ConfigData.trapdst, s.c_str());
        break;
    case ln_description:
        if( s.size() > 31 ) {
            return -1;
        }
        strcpy(ConfigData.description, s.c_str());
        break;
    case ln_name:
        if( s.size() > 31 ) {
            return -1;
        }
        strcpy(ConfigData.name, s.c_str());
        break;
    case ln_user:
        if( s.size() > 31 ) {
            return -1;
        }
        strcpy(ConfigData.user, s.c_str());
        break;
    case ln_other:
        if( s.size() > 31 ) {
            return -1;
        }
        strcpy(ConfigData.other, s.c_str());
        break;
    case ln_contact:
        if( s.size() > 31 ) {
            return -1;
        }
        strcpy(ConfigData.contact, s.c_str());
        break;
    case ln_location:
        if( s.size() > 31 ) {
            return -1;
        }
        strcpy(ConfigData.location, s.c_str());
        break;
    default:
        return -1;
    }
    if( saveConfig() ) {
        return 0x5A;
    }
    return -1;
}


int DeviceAttribute::getSubTypeUID(const std::string& name) {
    if( name == "ISAP300-155" || name == "isap300-155") {
        return 30;
    }
    else if( name == "ISAP300-622" || name == "isap300-622" ) {
        return 31;
    }
    else if( name == "EMUX-30" || name == "emux-30" ) {
        return 32;
    }
    else if( name == "MSAP300-155" || name == "msap300-155" ) {
        return 33;
    }
    else if( name == "MSAP300-622" || name == "masp300-622" ) {
        return 34;
    }
    return 32;
}
