/****************************************Copyright (c)*****************************************************************
**                           
**                               Easy Broadband Technology Co., Ltd.                       
**                         
**                                  http://www.ebbnetwork.com  
**
**
**--------------File Info----------------------------------------------------------------------------------------------
** File name:			CLI_Config_cmd_group.c
** File path:			/ISAP100_Mainsoft/Src/EZSoft/CLI_Commands/CLI_Config_cmd_group.c
** Created by:          Administrator
** Created date:        2012-11-27
** Version:             V0.1
** Descriptions:        cmd_creat_dxc
**
**---------------------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:				
** Descriptions:        
**
** Rechecked by:        
***********************************************************************************************************************/

#include "s1l_cmds.h"
#include "s1l_line_input.h"
#include "CLI_Config_cmd_group.h"
#include "lpc_string.h"
#include <string.h>
#include "CPPTools.h"
#include <iostream>
#include "DeviceAttribute.h"
#include "DeviceComponent.h"
#include <fstream>
#include "net_api.h"
#include <stdio.h>

#include "CBaseSlot.h"
#include "CBaseCard.h"
#include "SlotModule.h"
#include <time.h>
#include "GeneralLogic.h"
#include "CardCPU.h"
#include "PortVF.h"
#include "UID.h"

/* private functions declaration*/
static BOOL_32 cmd_creat_dxc(void);
// static BOOL_32 cmd_setmap(void);

static BOOL_32 cmd_tuadd(void);
static BOOL_32 cmd_tudel(void);

static BOOL_32 cmd_utc(void);

static BOOL_32 cmd_syscfg(void);
// static BOOL_32 cmd_recover_config(void);

static BOOL_32 cmd_rgain(void);
static BOOL_32 cmd_tgain(void);

/*-----------------------Commands define-------------------------------------*/
/* DxcConfig command */
static UNS_32 cmd_creat_dxc_plist[] =
{
	(PARSE_TYPE_STR), /* The "dxcConfig" command */
	(PARSE_TYPE_DEC),
	(PARSE_TYPE_DEC | PARSE_TYPE_END)
};
static CMD_ROUTE_T creat_dxc_cmd =
{
	(UNS_8 *) "creatdxc",
	cmd_creat_dxc,
	(UNS_8 *) "Set the upSource(1:AB, 2:GH, 3:LOOP, 4:CD) of a tu12.",
	(UNS_8 *) "creatdxc [tu12ID][upSource]",
	cmd_creat_dxc_plist,
	0//NULL
};



/* RC6400 debug command */
static UNS_32 cmd_tuadd_plist[] =
{
	(PARSE_TYPE_STR), /* The "tuadd" command */
	(PARSE_TYPE_DEC),
	(PARSE_TYPE_DEC | PARSE_TYPE_END)
};
static CMD_ROUTE_T tuadd_cmd =
{
	(UNS_8 *) "tuadd",
	cmd_tuadd,
	(UNS_8 *) "add a tu12 to a VCG",
	(UNS_8 *) "tuadd [tu12][vcg]",
	cmd_tuadd_plist,
	0//NULL
};


static UNS_32 cmd_tudel_plist[] =
{
	(PARSE_TYPE_STR), /* The "tudel" command */
	(PARSE_TYPE_DEC | PARSE_TYPE_END)
};
static CMD_ROUTE_T tudel_cmd =
{
	(UNS_8 *) "tudel",
	cmd_tudel,
	(UNS_8 *) "delelt a tu12 from any VCG",
	(UNS_8 *) "tudel [tu12]",
	cmd_tudel_plist,
	0//NULL
};

static UNS_32 cmd_utc_plist[] =
{
    (PARSE_TYPE_STR ), /* The "utc" command */
    (PARSE_TYPE_STR | PARSE_TYPE_OPT),
    (PARSE_TYPE_DEC | PARSE_TYPE_END),
};
static CMD_ROUTE_T utc_cmd =
{
    (UNS_8 *) "utc",
    cmd_utc,
    (UNS_8 *) "get UTC time.",
    (UNS_8 *) "utc[operation]\r\n"
                "[get]\r\n"
                "[init]\r\n",
    cmd_utc_plist,
    0//NULL
};

static UNS_32 cmd_rgain_plist[] =
{
    (PARSE_TYPE_STR ), /* The "rgain" command */
	(PARSE_TYPE_DEC ), //slot
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT), //ch: 1~15 17~31, 0 is all
    (PARSE_TYPE_DEC | PARSE_TYPE_END), //gain sn
};
static CMD_ROUTE_T rgain_cmd =
{
    (UNS_8 *) "rgain",
	cmd_rgain,
    (UNS_8 *) "get/set rgain AD.",
    (UNS_8 *) "rgain [slot][channel][gvalue]\r\n"
                "[slot] \r\n"
                "[gvalue] -14 ~ +6 \r\n",
	cmd_rgain_plist,
    0//NULL
};

static UNS_32 cmd_tgain_plist[] =
{
    (PARSE_TYPE_STR ), /* The "tgain" command */
	(PARSE_TYPE_DEC ), //slot
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT), //ch: 1~15 17~31, 0 is all
    (PARSE_TYPE_DEC | PARSE_TYPE_END), //gain sn
};
static CMD_ROUTE_T tgain_cmd =
{
    (UNS_8 *) "tgain",
	cmd_tgain,
    (UNS_8 *) "get/set tgain DA.",
    (UNS_8 *) "tgain [slot][channel][gvalue]\r\n"
                "[slot] \r\n"
                "[gvalue] -14 ~ +6 \r\n",
	cmd_tgain_plist,
    0//NULL
};

static UNS_32 cmd_syscfg_plist[] =
{
	(PARSE_TYPE_STR), /* The "syscfg" command */
	(PARSE_TYPE_DEC | PARSE_TYPE_OPT),
	(PARSE_TYPE_STR | PARSE_TYPE_END),
};
static CMD_ROUTE_T syscfg_cmd =
{
	(UNS_8 *) "syscfg",
	cmd_syscfg,
	(UNS_8 *) "config simple system work mode.",
	(UNS_8 *) "syscfg <option> <string>\r\n"
			  	"\t\toption 0 Get factory infomation.\r\n"
				"\t\toption 1 set device subtype.\r\n"
				"\t\toption 2 set serial number.\r\n"
                "\t\toption 3 set tProductionDate.\r\n"
				"\t\toption 4 set ip address.\r\n"
				"\t\toption 5 set net mask.\r\n"
				"\t\toption 6 set net gateway.\r\n"
				"\t\toption 8 get MAC address.\r\n"
				"\t\toption 9 get version.\r\n"
				"\t\toption 13 set trap dest ip\r\n",
	cmd_syscfg_plist,
	0//NULL
};


//static UNS_32 cmd_recover_cfg_plist[] =
//{
//    (PARSE_TYPE_STR), /* The "syscfg" command */
//    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),
//    (PARSE_TYPE_STR | PARSE_TYPE_END),
//};
//static CMD_ROUTE_T recover_cfg_cmd =
//{
//    (UNS_8 *) "recovercfg",
//    cmd_recover_config,
//    (UNS_8 *) "recover config data to default.",
//    (UNS_8 *) "recovercfg <slot>\r\n"
//                "\t\slot 0 Get factory infomation.\r\n"
//                "\t\toption 1 set device subtype.\r\n"
//                "\t\toption 2 set serial number.\r\n"
//                "\t\toption 3 set tProductionDate.\r\n"
//                "\t\toption 4 set ip address.\r\n"
//                "\t\toption 5 set net mask.\r\n"
//                "\t\toption 6 set net gateway.\r\n"
//                "\t\toption 7 get MAC address.\r\n",
//    cmd_recover_cfg_plist,
//    0//NULL
//};

/* .......................config group................................ */
GROUP_LIST_T config_group =
{
	(UNS_8 *) "config", /* NAND group */
	(UNS_8 *) "Config command group",
	0,//NULL,
	0//NULL
};


/* ---------------------private functions ---------------------------------------*/
BOOL_32 cmd_creat_dxc(void) {
	return TRUE;
}


// BOOL_32 cmd_setmap(void) {
// 	return TRUE;
// }




BOOL_32 cmd_tuadd(void) {
	return TRUE;
}
BOOL_32 cmd_tudel(void) {
	return TRUE;
}


BOOL_32 cmd_utc(void) {
//    char* option = (char*)cmd_get_field_val(1);
//    if( strcmp(option, "get") == 0 ) {
//        time_t rawtime;
//        struct tm * ptm;
//
//        time ( &rawtime );
//
//        ptm = gmtime ( &rawtime );
//        printf("UTC time is: %s\n",asctime(ptm));
//        return TRUE;
//    }
//    else if( strcmp(option, "init") == 0 ) {
//        bootTime = cmd_get_field_val(2);
//        printf("bootTime:%16d\n",  bootTime);
//        return TRUE;
//    }
    return FALSE;
}

BOOL_32 cmd_syscfg(void) {
 	UNS_8 option = cmd_get_field_val(1);
 	if( option > ln_number ) {
        std::cout << "input parse error!" << std::endl;
 	    return FALSE;
 	}
 	else if( option == 9 ) {//version
 		std::cout << CardCPU::GetVerInfo() << std::endl;
 		if( GeneralLogic::instance().ifOMUWorking() ) {
			for (int i = 0; i < 10; ++i) {
				CBaseSlot* objpSlot = SlotModule::getSlot(i);
				if (objpSlot) {
					CBaseCard* objpCard = objpSlot->GetCardObject();
					if (objpCard) {
						std::cout << "Slot-" << i << ": " << objpCard->GetCardVerInfo() << std::endl;
					}

				}
			}
 		}
 	    return TRUE;

 	}
 	else if( option == 8 ) {
 	    /* mac is read only */
        uint8 mac[6] = {0};
        DeviceComponent::getDeviceAttribute().getDeviceMac(mac);
        printf("%02x-%02x-%02x-%02x-%02x-%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return TRUE;
 	}
 	else if( parse_get_entry_count() > 2 ) {
 	    /* set option */
        char* s = (char*)cmd_get_field_val(2);
        std::string str(s);
        DeviceComponent::getDeviceAttribute().setBaseAttribute((Device_Attribute_ln_E)option, str);
        switch( option ) {
            case 2: //set serial number
            {
                uint8 mac[6] = {0};
                DeviceComponent::getDeviceAttribute().getDeviceMac(mac);
                setNetMAC(mac);
                std::cout << "Device MAC address changed, it will become effective after reset!" << std::endl;
            }
                break;
            case 4: // set ip
            case 5: // set ip mask
            {
                uint8 ip[4] = {0};
                uint8 mask[4] = {0};
                DeviceComponent::getDeviceAttribute().getDeviceIP(ip);
                DeviceComponent::getDeviceAttribute().getDeviceIPMask(mask);
                setNetIP(ip, mask);
            }
                break;
            case 6: // set ip gateway
            {
                uint8 ip[4] = {0};
                DeviceComponent::getDeviceAttribute().getDeviceIPGateway(ip);
                setNetGW(ip);
            }
                break;
        }
 	}

 	/* read option */
    std::string sn;
    DeviceComponent::getDeviceAttribute().getBaseAttribute((Device_Attribute_ln_E)option, sn);
    std::cout << sn << std::endl;
 	return TRUE;
}

static BOOL_32 cmd_rgain(void) {
 	UNS_8 slot = cmd_get_field_val(1);
 	UNS_8 ch = cmd_get_field_val(2);
	ST_VF info;
	info.slot = slot;
	info.port = ch;
	PortVF* p = PortVF::getInstance(UID::makeUID(&info));
	if(p == 0) {
        std::cout << "Error input!" << std::endl;
        return FALSE;
	}

 	if( parse_get_entry_count() > 3 ) {
 		UNS_8 g = cmd_get_field_val(3);
		p->setRcvGain(g, true);
        std::cout << "Rgain config complete!" << std::endl;
 	}
 	else {
		std::cout << (int)p->getRcvGain() << std::endl;
 	}
 	return TRUE;
}
static BOOL_32 cmd_tgain(void) {
 	UNS_8 slot = cmd_get_field_val(1);
 	UNS_8 ch = cmd_get_field_val(2);
	ST_VF info;
	info.slot = slot;
	info.port = ch;
	PortVF* p = PortVF::getInstance(UID::makeUID(&info));
	if(p == 0) {
        std::cout << "Error input!" << std::endl;
        return FALSE;
	}

 	if( parse_get_entry_count() > 3 ) {
 		UNS_8 g = cmd_get_field_val(3);
		p->setSndGain(g, true);
        std::cout << "Rgain config complete!" << std::endl;
 	}
 	else {
		std::cout << (int)p->getSndGain() << std::endl;
 	}
 	return TRUE;
}

/*--------------------------public function----------------------------*/
void CLI_config_add_commands(void) {
	/* Add NAND group */
	cmd_add_group(&config_group);

	/* Add commands to the config group */
	cmd_add_new_command(&config_group, &creat_dxc_cmd);
	cmd_add_new_command(&config_group, &tuadd_cmd);
	cmd_add_new_command(&config_group, &tudel_cmd);
	cmd_add_new_command(&config_group, &syscfg_cmd);
    cmd_add_new_command(&config_group, &utc_cmd);
    cmd_add_new_command(&config_group, &rgain_cmd);
    cmd_add_new_command(&config_group, &tgain_cmd);


}//Nand�洢
