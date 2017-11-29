/*
 * ChipRC7883.cpp
 *
 *  Created on: 2013-12-27
 *      Author: Administrator
 */

#include "ChipRC7883.h"
#include "CBaseSlot.h"
#include "RegisterAccess.h"
#include "CRC7.h"
#include <string.h>

BYTE    gVc4OhJ0Send[2] = {0};
BYTE    gVc4OhJ0Expect[2] = {0};
BYTE    gVc4OhJ1Send[2] = {0};
BYTE    gVc4OhJ1Expect[2] = {0};

ChipRC7883::ChipRC7883(CBaseSlot* slt, uint32 baseAddr, RC7883_Work_Mode_E mode) : BaseChip("ChipRC7883") {
    ra = slt->getRegisterAccess();
    RC7880_ADDR = baseAddr;
    os_mut_init(mut_7880);
    Wmode = mode;
}

ChipRC7883::~ChipRC7883() {
    // TODO Auto-generated destructor stub
}

Chip_INIT_RESULT ChipRC7883::ChipInit(void) {
    for( int i = 0; i < 24; i++ ) {
        /*
         * INIT V5 "010"
         */
        RC7880Tu12UnitV5SendWrite(i, 0x04);
        RC7880Tu12V5ExpectWrite(i, 0x04);
        /*
         * delete all default mapping
         */
        RC7880Tu12UnitTsSelectWrite(i, 63);
    }
    switch( Wmode ) {//7883��ģʽ��ӳ��ģʽ��PHYģʽ��TMģʽ
    case mode_MAP: {
        for( int i = 0; i < 24; i++ ) {
            RC7880Tu12UnitTsSelectWrite(i, i);
        }
        writeRc7880RegByte( RC7880_ADDR+RC7880REG_GLOBAL_CONFIG, 0x14 );
        for(int i=0;i<63;i++){

        	writeRc7880RegByte( RC7880_ADDR+0x04f0+i, (readRc7880RegByte(RC7880_ADDR+0x04f0+i))|0x08 );
        }

    }break;
    case mode_MAP_2: {
        for( int i = 0; i < 24; i++ ) {
            RC7880Tu12UnitTsSelectWrite(i, i+24);//tu12ʱ϶����63����ǰ24·ʱ϶����24��E1����24·ʱ϶����24��E1
        }
        writeRc7880RegByte( RC7880_ADDR+RC7880REG_GLOBAL_CONFIG, 0x14 );
        for(int i=0;i<63;i++){
               	writeRc7880RegByte( RC7880_ADDR+0x04f0+i, (readRc7880RegByte(RC7880_ADDR+0x04f0+i))|0x08 );
               }
    }break;
    case mode_PHY: {
        enum {
            D1 = 0x02, D2, D3,
            D4 = 0x09, D5 = 0x0C, D6 = 0x0f,
            D7 = 0x12, D8 = 0x15, D9 = 0x18,
            D10 = 0x1B, D11, D12,
            SO8MBase = 0x40,
            Boffset = 0x20
        };
        uint8 SO8M[] = {D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,D12};
        /* F1 connect OH-DXC; DCC from OH-DXC;
         * STB_C/D open
         * working in PHY
         */
        writeRc7880RegByte( RC7880_ADDR+RC7880REG_GLOBAL_CONFIG, 0x25 );

        /*
         * ����DCCͨ��Ϊ��׼DCC��ʹ��D1~D12����HDLC��˫���DCCʹ��
         */
        clearOHDXC();
        for( int i = 0; i < sizeof(SO8M); i++ ) {
            RC7880OHUnitOtherDxcWrite( SO8M[i], SO8M[i], SO8MBase+i);
            RC7880OHUnitOtherDxcWrite( SO8MBase+i, SO8MBase+i, SO8M[i] );

            RC7880OHUnitOtherDxcWrite( SO8M[i]+Boffset, SO8M[i]+Boffset, SO8MBase+i+Boffset);
            RC7880OHUnitOtherDxcWrite( SO8MBase+i+Boffset, SO8MBase+i+Boffset, SO8M[i]+Boffset );
        }

    }break;

    }
    return succeed;
}

bool ChipRC7883::setPortEnable(int port, int en) {
    int Rtn = 0;
    if( en == 1 ) {
        Rtn = RC7880Tu12UnitTsSelectWrite(port, port);
    }
    else {
        Rtn = RC7880Tu12UnitTsSelectWrite(port, 63);
    }

    if( Rtn == RC7880_OK ) {
        return true;
    }
    return false;
}

bool ChipRC7883::setSNCP(int port, int en) {
    uint8 mode = 0; // 0 is auto�� 2 is disable
    if( en == 0 ) {
        mode = 2;
    }
    RC7880Tu12UnitProtectModeWrite(port, mode);
	return true;
}


bool ChipRC7883::setPortLoopOutAndIn(int port) {
    RC7880Tu12UnitTu12LoopWrite(port, 1, 1);
    return true;
}
bool ChipRC7883::setPortLoopInOnly(int port) {
    RC7880Tu12UnitTu12LoopWrite(port, 1, 0);
    return true;
}
bool ChipRC7883::setPortLoopOutOnly(int port) {
    RC7880Tu12UnitTu12LoopWrite(port, 0, 1);
    return true;
}
bool ChipRC7883::setPortLoopClear(int port) {
    RC7880Tu12UnitTu12LoopWrite(port, 0, 0);
    return true;
}
/*
 * 7883����4STM1,24E1,48E1;���������STM�źŵ�LOS,��ALARM.
 */
bool ChipRC7883::getLOS(int STMsn) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(STMsn, &alm1, &alm2, &alm3);
    if( (alm1 & 0x80) == 0 ) {
        return false;
    }
    return true;
}

bool ChipRC7883::getLOF(int STMsn) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(STMsn, &alm1, &alm2, &alm3);
    if( (alm1 & 0x40) == 0 ) {
        return false;
    }
    return true;
}
bool ChipRC7883::getOOF(int STMsn) {
    return false;
}
bool ChipRC7883::getMSAIS(int STMsn) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(STMsn, &alm1, &alm2, &alm3);
    if( (alm3 & 0x80) == 0 ) {
        return false;
    }
    return true;
}
bool ChipRC7883::getMSRDI(int STMsn) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(STMsn, &alm1, &alm2, &alm3);
    if( (alm3 & 0x08) == 0 ) {
        return false;
    }
    return true;
}
bool ChipRC7883::getMSREI(int STMsn) {
    return false;
}


bool ChipRC7883::ifAlarm(int STMsn) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(STMsn, &alm1, &alm2, &alm3);
    if( alm1 & 0x80 ) {
        return false;
    }
    return ((alm1 & 0x40) | (alm3 & 0xC0)) != 0;
}



uint32 ChipRC7883::getB1(int sn) {
    uint16 counter = 0;
    RC7880DrvVC4UniteB1PerformRead(sn, &counter);
	return counter;
}
uint32 ChipRC7883::getB2(int sn) {
    return 0;
}
bool ChipRC7883::getJ0T(int sn, uint8* j0){
    Vc4UnitOverHead_t oh;
    RC7880DrvVC4UnitRcvOverHeadRead(sn, &oh);
    memcpy(j0, &oh.nj0[1], 16);
    return true;
}
bool ChipRC7883::getJ0E(int sn, uint8* j0){
	return false;
}
bool ChipRC7883::getJ0R(int sn, uint8* j0) {
    Vc4UnitOverHead_t oh;
    RC7880DrvVC4UnitRcvOverHeadRead(sn, &oh);
    memcpy(j0, &oh.nj0[1], 16);
    return true;
}

bool ChipRC7883::setJ0T(int sn, uint8* j0) {
    Vc4UnitOverHead_t oh;
    RC7880DrvVC4UnitRcvOverHeadRead(sn, &oh);
    memcpy(oh.nj0, j0, 16);
    if( RC7880DrvVC4UnitSendOverHeadWrite(sn, &oh, 1, 1) == RC7880_OK ) {
        return true;
    }
    return false;
}
bool ChipRC7883::setJ0E(int sn, uint8* j0) {
	return false;
}

bool ChipRC7883::setJ0LenTransmit(int sn, uint8 mode) {
	return false;
} //0 singleByte; 1 16 bytes
bool ChipRC7883::setJ0LenExpected(int sn, uint8 mode) {
	return false;
}

bool ChipRC7883::setSingleByteJ0T(int sn, uint8 j0) {
	return false;
}

uint32 ChipRC7883::getB3(int sn) {
	return false;
}
bool ChipRC7883::getJ1T(int sn, uint8* j1) {
	return false;
}
bool ChipRC7883::getJ1E(int sn, uint8* j1) {
	return false;
}
bool ChipRC7883::getJ1R(int sn, uint8* j1) {
	return false;
}

bool ChipRC7883::setJ1T(int sn, uint8* j1) {
	return false;
}
bool ChipRC7883::setJ1E(int sn, uint8* j1) {
	return false;
}

bool ChipRC7883::setJ1LenTransmit(int sn, uint8 mode) {
	return false;
} //0 singleByte; 1 16 bytes
bool ChipRC7883::setJ1LenExpected(int sn, uint8 mode) {
	return false;
}

bool ChipRC7883::getAUAIS(int vc4) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(vc4, &alm1, &alm2, &alm3);
    if( (alm3 & 0x40) == 0 ) {
        return false;
    }
    return true;

}
bool ChipRC7883::getAULOP(int vc4) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(vc4, &alm1, &alm2, &alm3);
    if( (alm1 & 0x10) == 0 ) {
        return false;
    }
    return true;
}
bool ChipRC7883::getHPRDI(int vc4) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(vc4, &alm1, &alm2, &alm3);
    if( (alm3 & 0x10) == 0 ) {
        return false;
    }
    return true;

}
bool ChipRC7883::getHPREI(int vc4) {
    return false;
}
bool ChipRC7883::getHPTIM(int vc4) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(vc4, &alm1, &alm2, &alm3);
    if( (alm1 & 0x02) == 0 ) {
        return false;
    }
    return true;

}
bool ChipRC7883::getHPSLM(int vc4) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(vc4, &alm1, &alm2, &alm3);
    if( (alm1 & 0x01) == 0 ) {
        return false;
    }
    return true;
}
bool ChipRC7883::getHPUNEQ(int vc4) {
    uint8 alm1; //los,lof,lom,aulop,rstim,hpuneq,hptim hpplm
    uint8 alm2; //msexc,msdeg,hpexc,hpdeg,-,-,-,-
    uint8 alm3; //msais,auais,hpais,hprdi,msrdi,-,-,-
    RC7880DrvVC4UnitAlarmRead(vc4, &alm1, &alm2, &alm3);
    if( (alm1 & 0x04) == 0 ) {
        return false;
    }
    return true;

}

bool ChipRC7883::getE1Los(int e1sn) {
    uint8 e1los;
    uint8 stbais;
    uint8 temp;
    RC7880DrvTU12UnitAlarmRead(e1sn, &e1los, &stbais, &temp);
    return (e1los & 0x80) != 0;
}


BYTE ChipRC7883::readRc7880RegByte(WORD addr)
{
    BYTE data = ra->readReg(addr) ;

    return data;
}
/*
 * ��7883д10��
 */
int ChipRC7883::writeRegByte(WORD addr , BYTE data)
{

    BYTE temp ;
    BYTE tempData;

    for(temp = 0 ; temp < 10; temp++)
    {
        tempData = ra->readReg(addr);
        if(tempData != data)
        {
            ra->writeReg(addr, data);
        }
        else
        {
            return RC7880_OK ;
        }
    }
    return RC7880_ERROR ;
}

int ChipRC7883::writeRc7880RegLock(void)
{
    WORD    regAddr ;
    BYTE    setData = 0 ;

    regAddr = RC7880_ADDR + RC7880REG_GLOBAL_WRITELOCK  ;
    setData = 0 ;
    if(writeRegByte(regAddr , setData) == RC7880_ERROR)
    {
        return RC7880_ERROR ;
    }
    return RC7880_OK ;
}

int  ChipRC7883::writeRc7880RegUnlock(void)
{
    WORD    regAddr ;
    BYTE    setData = 0 ;

    regAddr = RC7880_ADDR +  RC7880REG_GLOBAL_WRITELOCK ;
    setData = 0xAA ;
    if(writeRegByte(regAddr , setData) == RC7880_ERROR)
    {
        return RC7880_ERROR ;
    }
    return RC7880_OK ;
}

int ChipRC7883::writeRc7880RegByte(WORD addr , BYTE data)
{
    BYTE temp ;
    BYTE tempData;

    for(temp = 0 ; temp < 10; temp++)
    {
        //tempData = EXTERNAL_IO_BYTE[addr];
        tempData = readRc7880RegByte(addr);
        if(tempData != data)
        {
            os_mut_wait(mut_7880, 0xffff);
            writeRc7880RegUnlock() ;
            //EXTERNAL_IO_BYTE[addr] = data;
            writeRegByte(addr, data);
            writeRc7880RegLock() ;
            os_mut_release(mut_7880);
        }
        else
        {
            return RC7880_OK ;
        }
    }
    return RC7880_ERROR ;
}

int ChipRC7883::RC7880Tu12UnitTsSelectWrite(BYTE    tu12Id , BYTE select) {
    WORD    addr ;
    BYTE    tempData = 0 ;
    int result ;

    if(select > 63)
    {
        return RC7880_ERROR ;
    }
    addr = RC7880_ADDR + RC7880REG_TU12REG_BASEADDR(tu12Id) + RC7880REG_TS_SELECT ;
    tempData = select ;
    result = writeRc7880RegByte(addr , tempData) ;
    return result ;
}
int ChipRC7883::RC7880Tu12UnitV5SendWrite(BYTE tu12Id , BYTE sendData) {
    WORD    addr ;
    BYTE    tempData = 0 ;
    int   result ;


    addr = RC7880_ADDR + RC7880REG_TU12REG_BASEADDR(tu12Id) + RC7880REG_LPRDI_ENABLE ;
    tempData = readRc7880RegByte(addr) ;
    tempData &= 0xF1 ;
    tempData |= (sendData & 0x0E) ;
    result = writeRc7880RegByte(addr , tempData) ;
    return result ;
}
int ChipRC7883::RC7880Tu12V5ExpectWrite(BYTE tu12Id , BYTE v5Expect) {
    WORD    addr ;
    BYTE    tempData = 0 ;
    int   result ;


    addr = RC7880_ADDR + RC7880REG_TU12REG_BASEADDR(tu12Id) + RC7880REG_ALARM_BUSAV5EXPECT ;
    tempData = (v5Expect & 0x0E) ;
    result = writeRc7880RegByte(addr , tempData) ;
    addr = RC7880_ADDR + RC7880REG_TU12REG_BASEADDR(tu12Id) + RC7880REG_ALARM_BUSBV5EXPECT ;
    tempData = (v5Expect & 0x0E) ;
    result = writeRc7880RegByte(addr , tempData) ;
    return result ;

}
int ChipRC7883::RC7880Tu12UnitTu12LoopWrite(BYTE  tu12Id , BYTE lineLoop , BYTE devLoop) {
    WORD    addr ;
    BYTE    tempData = 0 ;
    int result ;

    if((lineLoop > 1) || (devLoop > 1))
    {
        return RC7880_ERROR ;
    }
    addr = RC7880_ADDR + RC7880REG_TU12REG_BASEADDR(tu12Id) + RC7880REG_RC7880E1_CONFIG ;
    tempData = readRc7880RegByte(addr) ;
    if(lineLoop == 1)       /*out Loop*/
    {
        tempData |= 0x01 ;
        tempData |= 0x08 ;          /*insert AIS*/
    }
    else
    {
        tempData &= 0xF6 ;
    }
    if( devLoop == 1)       /*inside Loop*/
    {
        tempData |= 0x02 ;
        tempData |= 0x04 ;          /*insert AIS*/
    }
    else
    {
        tempData &= 0xF9 ;
    }
    if((devLoop == 1) && (lineLoop == 1))
    {
        tempData &= 0xF3 ;
    }
    result = writeRc7880RegByte(addr , tempData) ;
    return result ;

}

int ChipRC7883::RC7880Tu12UnitProtectModeWrite(BYTE tu12Id , BYTE protectMode /*, BYTE startFlag*/)
{
    WORD    addr ;
    BYTE    tempData = 0 ;
    BYTE    tempData2 = 0 ;
    int result = RC7880_OK ;

    if((protectMode !=  AUTO_PROTECT)
        && (protectMode !=  MANUAL_PROTECT) && (protectMode !=  DISABLE_PROTECT))
    {
        return RC7880_ERROR ;
    }
    addr = RC7880_ADDR + RC7880REG_TU12REG_BASEADDR(tu12Id) + RC7880REG_TS_PROTECT ;
    tempData = readRc7880RegByte(addr) ;
    tempData2 = tempData ;
    if(protectMode == AUTO_PROTECT)
    {
        tempData2 &= 0xFB ; /*clear force expect */
        tempData2 &= 0xE7 ;
    }
    else if(protectMode == MANUAL_PROTECT)
    {
        tempData2 |= 0x18 ;
        if((tempData & 0x01) == 0x01)   /*B*/
        {
            tempData2 |= 0x02 ; /*B*/
        }
        else        /*A*/
        {
            tempData2 &= 0xFD ; /*A*/
        }
    }
    else if(protectMode == DISABLE_PROTECT)
    {
        tempData2 &= 0xE7 ;
        tempData2 |= 0x08 ;
    }
    if(tempData2 != tempData)
    {
        result = writeRc7880RegByte(addr , tempData2) ;
    }
    return result ;
}

int ChipRC7883::RC7880DrvVC4UnitAlarmRead (BYTE vc4Id , BYTE *iA0P , BYTE *iA1P , BYTE *iA2P)
{
    WORD    addr ;
    volatile BYTE   tempData = 0 ;

    if(((vc4Id != 0) && (vc4Id != 1))
        ||((iA0P == RC7880_NULL) ||(iA1P == RC7880_NULL) || (iA2P == RC7880_NULL)))
    {
        return RC7880_ERROR ;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_ALARM_IA0 ;
    tempData = readRc7880RegByte(addr) ;
    *iA0P = tempData ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_ALARM_IA1 ;
    tempData = readRc7880RegByte(addr) ;
    *iA1P = tempData ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_ALARM_IA2 ;
    tempData = readRc7880RegByte(addr) ;
    *iA2P = tempData ;
    return RC7880_OK ;
}

int ChipRC7883::RC7880DrvVC4UniteB1PerformRead(BYTE vc4Id , WORD *counterB1P)
{
    WORD    addr ;
    BYTE    tempData1 = 0 ;
    BYTE    tempData2 = 0 ;

    if((vc4Id > 1) ||(counterB1P == RC7880_NULL))
    {
        return  RC7880_ERROR;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_PERFORM_B1L ;  /*B1*/
    tempData1 = readRc7880RegByte(addr) ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_PERFORM_B1H ;
    tempData2 = readRc7880RegByte(addr) ;
    *counterB1P = ((tempData2 << 8) |(tempData1)) ;
    return  RC7880_OK ;
}

int ChipRC7883::RC7880OHUnitOtherDxcWrite(BYTE  startAddr , BYTE  endAddr , BYTE     startSrcAddr)
{
    WORD    addr ;
    WORD    addrId = 0 ;
    BYTE    tempData = 0 ;

    for(addrId = startAddr ; addrId <= endAddr ; addrId++ )
    {
        addr = RC7880_ADDR + RC7880REG_OHDXC_BASE + addrId ;
        tempData = startSrcAddr ;
        if(writeRc7880RegByte(addr , tempData) == RC7880_ERROR)
        {
            return RC7880_ERROR ;
        }
        startSrcAddr += 1 ;
    }
    return RC7880_OK ;
}

int ChipRC7883::clearOHDXC(void) {
    for( uint16 i = 0x200; i < 0x300; i++ ) {
        writeRc7880RegByte(RC7880_ADDR + i , 0);
    }
    return RC7880_OK ;
}


int ChipRC7883::RC7880DrvVC4UnitRcvOverHeadRead(
                            BYTE    vc4Id ,
                            Vc4UnitOverHead_t *overHeadP)
{
    WORD    addr ;
    BYTE    tempData = 0 ;
    BYTE    len ;
    BYTE    newJ0Byte[16] ;

    if(((vc4Id != 0) && (vc4Id != 1)) ||(overHeadP == RC7880_NULL))
    {
        return RC7880_ERROR ;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_J0_RCV ;
    tempData = readRc7880RegByte(addr) ;
    overHeadP->j0 = tempData ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_K1_RCV  ;
    tempData = readRc7880RegByte(addr) ;
    overHeadP->k1 = tempData ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_K2_RCV ;
    tempData = readRc7880RegByte(addr) ;
    overHeadP->k2 = tempData ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_S1_RCV ;
    tempData = readRc7880RegByte(addr) ;
    overHeadP->s1 = tempData ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_X1_RCV ;
    tempData = readRc7880RegByte(addr) ;
    overHeadP->x1 = tempData ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_X2_RCV  ;
    tempData = readRc7880RegByte(addr) ;
    overHeadP->x2 = tempData ;
    memset(newJ0Byte , 0x20 , sizeof(newJ0Byte)) ;
    for(len = 0 ; len < 16 ; len++)
    {
        addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_J1_RCV  + len ;
        tempData = readRc7880RegByte(addr) ;
        newJ0Byte[len] = tempData ;
    }
    RC7880DrvWriteNewJxBytes(overHeadP->j1 , newJ0Byte , 16) ;

    memset(newJ0Byte , 0x20 , sizeof(newJ0Byte)) ; //set newJ0Byte to �� ��
    for(len = 0 ; len < 16 ; len++)
    {
        addr = RC7880_ADDR + RC7880REG_NEWJ0_RCV + vc4Id * 0x0200 + len  ;
        tempData = readRc7880RegByte(addr) ;
        newJ0Byte[len] = tempData ; //get register to newJ0Byte
    }

    RC7880DrvWriteNewJxBytes(overHeadP->nj0 , newJ0Byte , 16) ; //translate to nj0 noCRC


    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_C2_RCV ;
    tempData = readRc7880RegByte(addr) ;
    overHeadP->c2 = tempData ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_K3_RCV ;
    tempData = readRc7880RegByte(addr) ;
    overHeadP->k3 = tempData ;
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_N1_RCV  ;
    tempData = readRc7880RegByte(addr) ;
    overHeadP->n1 = tempData ;
    return  RC7880_OK ;
}

int ChipRC7883::RC7880DrvVC4UnitSendOverHeadWrite(
                BYTE vc4Id, Vc4UnitOverHead_t *overHeadP,
                BYTE j0CrcEnable,
                BYTE j1CrcEnable) {
    WORD addr;
    BYTE tempData = 0;
    BYTE len;
    BYTE newJ0Byte[16];
    BYTE jxLen = 0;

    if (((vc4Id != 0) && (vc4Id != 1)) || (overHeadP == RC7880_NULL)) {
        return RC7880_ERROR;
    }
    /* ��bit J0 ��ַ0x040B*/
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_J0_SEND;
    tempData = overHeadP->j0;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }

    /*16 byte J0*/
    jxLen = strlen((const char*) overHeadP->nj0);
    if (jxLen > 15) {
        jxLen = 15;
    }
    memset(newJ0Byte, 0x20, sizeof(newJ0Byte));
    if (gVc4OhJ0Send[vc4Id] == 0) {
        RC7880DrvReadNewJxBytes(overHeadP->nj0, newJ0Byte, jxLen);
        if (j0CrcEnable != 1) {
            newJ0Byte[0] = 0x80;
        }
    } else {
        memcpy(newJ0Byte, overHeadP->nj0, 16);
    }
    for (len = 0; len < 16; len++) {
        addr = RC7880_ADDR + RC7880REG_NEWJ0_SEND + vc4Id * 0x0200 + len;
        tempData = newJ0Byte[len];
        if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
            return RC7880_ERROR;
        }
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_K1_SEND;
    tempData = overHeadP->k1;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_K2_SEND;
    tempData = overHeadP->k2;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_S1_SEND;
    tempData = overHeadP->s1;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_X1_SEND;
    tempData = overHeadP->x1;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_X2_SEND;
    tempData = overHeadP->x2;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    jxLen = strlen((const char*) overHeadP->j1);
    if (jxLen > 15) {
        jxLen = 15;
    }
    memset(newJ0Byte, 0x20, sizeof(newJ0Byte));
    if (gVc4OhJ1Send[vc4Id] == 0) {
        RC7880DrvReadNewJxBytes(overHeadP->j1, newJ0Byte, jxLen);
        if (j1CrcEnable != 1) {
            newJ0Byte[0] = 0x80;
        }
    } else {
        memcpy(newJ0Byte, overHeadP->j1, 16);
    }
    for (len = 0; len < 16; len++) {
        addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_J1_SEND
                + len;
        tempData = newJ0Byte[len];
        if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
            return RC7880_ERROR;
        }
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_C2_SEND;
    tempData = overHeadP->c2;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_K3_SEND;
    tempData = overHeadP->k3;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_N1_SEND;
    tempData = overHeadP->n1;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    return RC7880_OK;
}

int ChipRC7883::RC7880DrvVC4UnitExceptOverHeadWrite(
            BYTE vc4Id, Vc4UnitExceptOverHead_t *overHeadP,
            BYTE j0CrcEnable,
            BYTE j1CrcEnable) {
    WORD addr;
    BYTE tempData = 0;
    BYTE len;
    BYTE newJ0Byte[16];
    BYTE jxLen = 0;

    if (((vc4Id != 0) && (vc4Id != 1)) || (overHeadP == RC7880_NULL)) {
        return RC7880_ERROR;
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_J0_EXCEPT;
    tempData = overHeadP->j0;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    jxLen = strlen((const char*) overHeadP->nj0);
    if (jxLen > 15) {
        jxLen = 15;
    }
    memset(newJ0Byte, 0x20, sizeof(newJ0Byte));
    if (gVc4OhJ0Expect[vc4Id] == 0) {
        RC7880DrvReadNewJxBytes(overHeadP->nj0, newJ0Byte, jxLen);
        if (j0CrcEnable != 1) {
            newJ0Byte[0] = 0x80;
        }
    } else {
        memcpy(newJ0Byte, overHeadP->nj0, 16);
    }
    for (len = 0; len < 16; len++) {
        addr = RC7880_ADDR + RC7880REG_NEWJ0_EXCEPT + 0x200 * vc4Id + len;
        tempData = newJ0Byte[len];
        if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
            return RC7880_ERROR;
        }
    }
    jxLen = strlen((const char*) overHeadP->j1);
    if (jxLen > 15) {
        jxLen = 15;
    }
    memset(newJ0Byte, 0x20, sizeof(newJ0Byte));
    if (gVc4OhJ1Expect[vc4Id] == 0) {
        RC7880DrvReadNewJxBytes(overHeadP->j1, newJ0Byte, jxLen);
        if (j1CrcEnable != 1) {
            newJ0Byte[0] = 0x80;
        }
    } else {
        memcpy(newJ0Byte, overHeadP->j1, 16);
    }
    for (len = 0; len < 16; len++) {
        addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id)
                + RC7880REG_J1_EXCEPT + len;
        tempData = newJ0Byte[len];
        if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
            return RC7880_ERROR;
        }
    }
    addr = RC7880_ADDR + RC7880REG_LPORT_BASEADDR(vc4Id) + RC7880REG_C2_EXCEPT;
    tempData = overHeadP->c2;
    if (writeRc7880RegByte(addr, tempData) == RC7880_ERROR) {
        return RC7880_ERROR;
    }
    return RC7880_OK;
}

BYTE ChipRC7883::RC7880DrvCRC7Func(const BYTE *pSrc , DWORD cnt)
{
   return CRC7::getCRC7(pSrc, cnt);
}



void ChipRC7883::RC7880DrvReadNewJxBytes(BYTE *jxCharP , BYTE *jxByteP , BYTE jxLen)
{
   BYTE    len ;

   for(len = 1 ; len <= jxLen ; len++)
   {
       jxByteP[len] = jxCharP[len - 1] ;
   }
   jxByteP[0] = 0x80 ;
   jxByteP[0] = RC7880DrvCRC7Func(&jxByteP[0] , 16) ;
   jxByteP[0]  |= 0x80 ;
}


void ChipRC7883::RC7880DrvWriteNewJxBytes(BYTE *jxCharP , BYTE *jxByteP , BYTE jxLen)
{
   BYTE    len ;

   memset(jxCharP , 0 , 16) ;
   for(len = 1; len < jxLen ; len++)
   {
       jxCharP[len] = jxByteP[len] ;
       if(jxByteP[len] == '\0')
       {
           return ;
       }
       jxCharP[len] = jxByteP[len] ;
   }
/*  jxByteP[15] = '\0' ;  */
}

int ChipRC7883::RC7880DrvTU12UnitAlarmRead(BYTE tu12Id, BYTE *e1P, BYTE *dropA, BYTE *DropB) {
    WORD addr;
    BYTE tempData = 0;

    if ((dropA == RC7880_NULL) || (DropB == RC7880_NULL)) {
        return RC7880_ERROR;
    }
    addr = RC7880_ADDR
            + RC7880REG_TU12REG_BASEADDR(tu12Id) + RC7880REG_ALARM_E1PORT;
    tempData = readRc7880RegByte(addr);
    *e1P = tempData;
    addr = RC7880_ADDR
            + RC7880REG_TU12REG_BASEADDR(tu12Id) + RC7880REG_ALARM_DROPA;
    tempData = readRc7880RegByte(addr);
    *dropA = tempData;
    addr = RC7880_ADDR
            + RC7880REG_TU12REG_BASEADDR(tu12Id) + RC7880REG_ALARM_DROPB;
    tempData = readRc7880RegByte(addr);
    *DropB = tempData;
    return RC7880_OK;
}
