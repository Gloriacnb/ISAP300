/*
 * STMConfigDataDefine.h
 *
 *  Created on: 2014��11��24��
 *      Author: Administrator
 */


#ifndef STMCONFIGDATADEFINE_H_
#define STMCONFIGDATADEFINE_H_

#include "FE1ConfigDataDefine.h"

#if defined (__cplusplus)
extern "C"
{
#endif


typedef struct {
    uint8 J1ModeTransmit; //0 is 16bytes, 1 is single byte
    uint8 J1ModeReceive;
    uint8 J1Transmit[64];
    uint8 J1Expect[64];
}VC4_Config_Data_T;

typedef struct {
    uint8 J0ModeTransmit; //0 is 16bytes, 1 is single byte
    uint8 J0ModeReceive;
    uint8 J0Transmit[16];
    uint8 J0Expect[16];
    VC4_Config_Data_T RscVC4[4];
    uint16 DCCSel;
    uint8 TXEN;
    uint8 ALSEnable;
    char Desc[32];
    uint8 DescLen;
    FE1_NM_Config_Data_T nmportCfg;
}STM_Config_Data_T;

#if defined (__cplusplus)
}
#endif
#endif /* STMCONFIGDATADEFINE_H_ */
