/*
 * DeviceAttribute.h
 *
 *  Created on: 2013-11-19
 *      Author: Administrator
 */

#ifndef DEVICEATTRIBUTE_H_
#define DEVICEATTRIBUTE_H_

//#include "CMidFileTxt.h"
#include "ConfigCell.h"
#include <string>
typedef enum {
	SUBTYPE_EMUX = 32,
}Device_SubType_E;

typedef enum {
    ln_info,
    ln_subtype,
    ln_serialnumber,
    ln_factorydate,
    ln_ipaddress,
    ln_ipmask,
    ln_ipgateway,
    ln_description,
    ln_name,
    ln_user,
    ln_other,
    ln_contact,
    ln_location,
    ln_iptrapdest,
    ln_number
}Device_Attribute_ln_E;

typedef struct {
    char subtypename[16];
    char serialnumber[16];
    char factorydate[16];
    char ip[16];
    char ipmask[16];
    char ipgateway[16];
    char description[32];
    char name[32];
    char user[32];
    char other[32];
    char contact[32];
    char location[32];
    char trapdst[16];
}Device_Attribute_Config_T;

class DeviceAttribute : public ConfigCell {
    static const std::string fname;
public:
    DeviceAttribute();
    virtual ~DeviceAttribute();

    virtual uint8* getConfigDataBuff(void) {
        return (uint8*)&ConfigData;
    };
    virtual uint32 getConfigDataLen(void) {
        return sizeof(ConfigData);
    };
    virtual void loadDefaultData(void);

    void getBaseAttribute(Device_Attribute_ln_E ln, std::string&);

    uint32 getDeviceOPTMode(void);

    uint16 getDeviceSn(void);

    void getDeviceIP(std::string&);
    void getDeviceIP(uint8* ip);

    void getDeviceIPMask(uint8* mask);
    void getDeviceIPGateway(uint8* host);

    void getTrapDestIP(uint8* ip);

    void getDeviceMac(uint8* mac);
    int getDeviceSubType(void);

    int setBaseAttribute(Device_Attribute_ln_E ln, std::string&);

    uint32 optMode(void) {
        return mode;
    };

    /* for synfile */
    void getProtectMCUIP(uint8* ip);

private:
//    CMidFileTxt saveFile;
//    void restoreFile(void);
    Device_Attribute_Config_T ConfigData;
    uint32 mode;
    static int getSubTypeUID(const std::string& name);
};

#endif /* DEVICEATTRIBUTE_H_ */
