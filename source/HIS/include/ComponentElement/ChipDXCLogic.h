/*
 * ChipDXCLogic.h
 *
 *  Created on: 2015年6月15日
 *      Author: Administrator
 */

#ifndef CHIPDXCLOGIC_H_
#define CHIPDXCLOGIC_H_

#include "FPGAChip.h"
#include "DriverPCM.h"

class ChipDXCLogic: public FPGAChip, public DriverPCM {
public:
    ChipDXCLogic(CBaseSlot* slot);
    virtual ~ChipDXCLogic();

    virtual Chip_INIT_RESULT ChipInit(void);
    virtual std::string GetVerInfo(void);

    /*Clock*/
    virtual void resetClockChip(void) {};
    virtual void selectClockSource(USEABLE_RCLOCK cs) {};
    virtual void setExtClockMode(USEABLE_RCLOCK c, ExtClock_type_E mode) {};

    virtual void selectPCMClock(uint8 src);

    /*64K cross connect*/
    virtual bool CreatCrossConnection(uint32 toCh, uint32 toTs, uint32 fromCh, uint32 fromTs);
    virtual bool RemoveCrossConnection(uint32 Ch, uint32 Ts);
    virtual uint16 getHID(uint32 uid);

    /*port state*/
    virtual bool itsLOS(int e1);
    virtual bool itsAIS(int e1);
    virtual bool itsLOF(int e1);
    virtual bool itsLOMF(int e1);
    /*
     * for DCC
     */
    virtual bool setDCCOH(int ch, uint16 data) {return false;};

    virtual bool ifHavePacket(NM_CH_E ch) {return false;};
    virtual int getPacketLen(NM_CH_E ch) {return 0;};
    virtual uint32 GetDCCPacket(NM_CH_E ch, uint8* buff, uint16 len, uint16 startPos = 0) {return 0;};
    virtual void DccSendPacket(NM_CH_E ch, uint8* data, uint16 len) {};
    virtual void clearDccRdBufForce(NM_CH_E ch) {};

    /*
     * for Sabit
     */
    virtual void setSabitInterruptEnable(uint8 ch, uint8 en);
    virtual bool ifSaHavePacket(void);
    virtual uint8 getSourceCh(void);
    virtual int getSaPacketLen(void);
    virtual bool getSaPacket(uint8* buff, uint16 len, uint16 startPos = 0);
    virtual void readSaPacketOver(void);

    virtual void sendSaPacket(uint8 ch, uint8* data, uint16 len);


    /*
     * for dcn
     */
    virtual void setDcnInterruptEnable(uint8 ch, uint8 en);
    virtual bool selectDcnTs(uint8 ch, uint32 tsmap);
    virtual uint32 getDcnTsMap(uint8 ch);
    virtual bool ifDcnHavePacket(uint8 ch);
    virtual int getDcnPacketLen(uint8 ch);
    virtual bool getDcnPacket(uint8 ch, uint8* buff, uint16 len);
    virtual void readDcnPacketOver(uint8 ch);

    virtual void sendDcnPacket(uint8 ch, uint8* data, uint16 len);

    /*
     * for protected
     */
    virtual void setAlarm(uint8 remote, uint8 local) {};
    virtual void protectFSMStart(void);

    /*
     * for loop
     */
    virtual void setPortLoopOut(uint8 ch, uint8 loop);
    virtual void setPortLoopIn(uint8 ch, uint8 loop);

    virtual void setPortEnable(uint8 port, uint8 en);
    /**********for led*************/
    virtual void turnOnLedWork(uint8 sn, bool on);
    virtual void turnOnLedAlm(uint8 sn, bool on);
    void turnOnLedMaj(bool on);
    void turnOnLedMin(bool on);
    void turnOnledWorking(bool on);

    void sabitInterruptEnable(bool en);
    void resetRemote(uint8 port);



    void offHstbus(uint8 extslot);
    void onHstbus(uint8 extslot);

private:
    static const uint16 LED = 0x2fff; // bit0 MAJ; bit1 MIN
                                      // bit4~7 WORK1~4
                                      // bit8~12 ALM1~4
                                      // bit15 work
    static const uint16 VER = 0x9002;
    static const uint16 REG_Alarm_STA   = 0x9003; //bit15~8 remote; bit7~0 local
    static const uint16 REG_FSM_Start   = 0x9004; //bit0 start(1)
    static const uint16 REG_HSTBUS_SWITCH = 0x900c;
    std::string ChipVerInfo;
    virtual std::string GetChipVerInfo(const std::string, const uint16);

    static const uint16 REG_VER_CPLD_GENERALLOGIC_XY   = 0x0010;
    static const uint16 REG_VER_CPLD_GENERALLOGIC_Z   = 0x0011;
    static const uint16 REG_VER_CPLD_UOPTLOGIC   = 0x2010;
    static const uint16 REG_INTRR_EN = 0x2040;

    static const uint16 REG_64K_CROSS_BASE = 0x8000;
    static uint16 getCrossRegAddr(uint8 ch, uint8 ts) {
        ch &= 0x7f;
        ts &= 0x1f;
        return (ch << 5) | ts;
    };

    static const uint16 REG_PCM_CLOCK_SEL = 0x9100;
    static const uint16 REG_PCM_CLOCK_START = 0x9105;

    static const uint16 REG_E1_Module_BASE = 0xa000;
    static uint16 getE1RegAddr(uint8 ch) {
        return REG_E1_Module_BASE | (ch << 9);
    };
    static const uint16 REG_E1_ALARM_OFFSET = 0x00; //bit4 RDI; bit3 MFI; bit2 LOF(0); bit1 AIS; bit0 LOS(0);
    static const uint16 REG_E1_LOOPIN_OFFSET = 0x06;//bit0 1 loop;
    static const uint16 REG_E1_LOOPOUT_OFFSET = 0x07;
    static const uint16 REG_E1_CLOSE = 0x08;        //bit0 1 close; bit 15 far reset

    //for sabit 0x6100
    static const uint16 REG_SABIT_R_STA = 0xa014; //bit0 rcv ready
    static const uint16 REG_SABIT_R_CH  = 0xa015; //RCV channal sn
    static const uint16 REG_SABIT_R_LEN = 0xa016; //RCV len
    static const uint16 REG_SABIT_R_BUF = 0xa100; //RCV Data
    static const uint16 REG_SABIT_R_OVER= 0xa00f; //RCV Over

//    static const uint16 REG_SABIT_T_STA = 0x8400;
    static const uint16 REG_SABIT_T_BUF = 0xa100; //SND Data
    static const uint16 REG_SABIT_T_CH  = 0xa012; //SND channal sn
    static const uint16 REG_SABIT_T_LEN = 0xa013; //SND len
    static const uint16 REG_SABIT_T_OVER= 0xa00c; //SND OVER 0x46 fix
    static const uint16 REG_SABIT_T_BROAD = 0xa00d; //bit7 1
    static const uint16 REG_SABIT_T_CH_STA= 0xa06a;

    static const uint16 REG_SABIT_INTRR_EN = 0xa0a0;
    static const uint16 REG_DCN_INTRR_EN = 0xa0a1;

    //for dcn
    uint16 REG_DCN_TS_SEL_L(uint8 ch) {
        return 0xa074 + (ch<<1);
    };
    uint16 REG_DCN_TS_SEL_H(uint8 ch) {
        return 0xa075 + (ch<<1);
    };

    static const uint16 REG_DCN_T_STA = 0xa070;
    static const uint16 REG_DCN_T_OVER = 0xa071;
    uint16 REG_DCN_T_BUF(uint8 ch) {
        return 0xa080 + ch;
    };

    static const uint16 REG_DCN_R_STA = 0xa072;
    static const uint16 REG_DCN_R_OVER = 0xa073;
    uint16 REG_DCN_R_BUF(uint8 ch) {
        return 0xa090 + ch;
    };
    uint16 REG_DCN_R_LEN(uint8 ch) {
        return 0xa094 + ch;
    };
};

#endif /* CHIPDXCLOGIC_H_ */
