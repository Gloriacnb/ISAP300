/*
 * OMUStandby.cpp
 *
 *  Created on: 2014��11��27��
 *      Author: Administrator
 */

#include "OMUStandby.h"
#include "CommunicationModule.h"
#include "Lpc3250upload.h"
#include <string.h>
#include <stdio.h>
#include "CPPTools.h"
#include "GeneralLogic.h"
#include "SoftWDT.h"

static uint16 udp_callback(uint8 socket, uint8 *remip, uint16 remport, uint8 *buf, uint16 len);

OMUStandby::OMUStandby() : OMUStatus(200) {
    CommunicationModule::initStandby();
    lockIt(false);
    udp_cmd = udp_get_socket(0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, udp_callback);
    udp_open( udp_cmd, 10001 );
    GeneralLogic::instance().setWarmStartType();
}

OMUStandby::~OMUStandby() {
    SoftWDT::instance().stopSoftWDT(); //关闭软狗
    udp_close(udp_cmd);
    udp_release_socket(udp_cmd);
    CommunicationModule::clearStandby();
}

uint16 udp_callback(uint8 socket, uint8 *remip, uint16 remport, uint8 *buf, uint16 len) {
    static char Sync[] = "yes";
    static char DontSync[] = "no";
    printf("\nReceive %s %d\n", buf, len);
    if( memcmp("over", buf, len) == 0 ) {
        uint8* send_buff = udp_get_buf(10);
        if( send_buff == 0 ) {
            printf("\n!!!udp_get_buf error!!!\n");
            return 0;
        }
        strcpy( (char*)send_buff, "Roger" );
        udp_send(socket, remip, remport, send_buff, strlen((char*)send_buff)+1);
    }
    else if( memcmp("upmain", buf, len) == 0 ) {
        uint8* send_buff = udp_get_buf(10);
        if( updata_main("main.bit") < 0 ) {

            strcpy( (char*)send_buff, "failed" );
        }
        else {
            strcpy( (char*)send_buff, "succeed" );
        }
        udp_send(socket, remip, remport, send_buff, strlen((char*)send_buff)+1);
    }
    else if( memcmp("check", buf, 5) == 0 ) {
        printf("\nstart check\n");
        char* fileName = (char*)&buf[5];
        std::string tmpname(fileName);
        tmpname += ".temp";
        uint8* send_buff = udp_get_buf(10);
        if( frename(tmpname.c_str(), fileName) != 0 ) {
            strcpy( (char*)send_buff, "failed" );
        }
        else {
            strcpy( (char*)send_buff, "succeed" );
        }
        udp_send(socket, remip, remport, send_buff, strlen((char*)send_buff)+1);
    }
    else {//if need sync
        char fileName[30] = {0};
        uint32 UCRC = 0;
        sscanf((char*)buf, "0x%8x+%s", &UCRC, fileName);
        uint32 MyCRC = 0;
        uint8* send_buff = udp_get_buf(10);
        char* sendData = 0;
        if( CPPTools::getFileCRC(fileName, &MyCRC) ) {
            if( MyCRC != UCRC ) {
                sendData = Sync;
            }
            else {
                sendData = DontSync;
            }

        }
        else {
            printf("\n!!!CRC Error!!!\n");
            sendData = Sync;
        }
        strcpy( (char*)send_buff, sendData );
        udp_send(socket, remip, remport, send_buff, strlen(sendData)+1);
    }
    return 0;
}