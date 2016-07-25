/*
 * ChipE1Port16Logic.h
 *
 *  Created on: 2015年6月17日
 *      Author: Administrator
 */

#ifndef CHIPE1PORT16LOGIC_H_
#define CHIPE1PORT16LOGIC_H_

#include "FPGAChip.h"
#include "DriverPCM.h"
#include "os.h"

class CBaseSlot;
class ChipE1Port16Logic: public FPGAChip, public DriverPCM {
public:
    ChipE1Port16Logic(CBaseSlot* slt);
    virtual ~ChipE1Port16Logic();

    virtual Chip_INIT_RESULT ChipInit(void);

    virtual std::string GetChipVerInfo(const std::string ChipName,
            const uint16 Reg_Ver_Addr);

    virtual std::string GetVerInfo();


    /*SDH Clock*/
    virtual void resetClockChip(void){};
    virtual void selectClockSource(USEABLE_RCLOCK cs){};
    virtual void setExtClockMode(USEABLE_RCLOCK c, ExtClock_type_E mode){};

    virtual void selectPCMClock(uint8 src);

    /*64K cross connect*/
    virtual bool CreatCrossConnection(uint32 toCh, uint32 toTs, uint32 fromCh, uint32 fromTs){ return false;};
    virtual bool RemoveCrossConnection(uint32 Ch, uint32 Ts){ return false;};
    virtual uint16 getHID(uint32 uid){ return 0;};

    /*port state*/
    virtual bool itsLOS(int e1);
    virtual bool itsAIS(int e1);
    virtual bool itsLOF(int e1);
    virtual bool itsLOMF(int e1);
    /*
     * for DCC
     */
    virtual bool setDCCOH(int ch, uint16 data){return false;};

    virtual bool ifHavePacket(NM_CH_E ch){ return false;};
    virtual int getPacketLen(NM_CH_E ch){return 0;};
    virtual uint32 GetDCCPacket(NM_CH_E ch, uint8* buff, uint16 len, uint16 startPos = 0) { return 0;};
    virtual void DccSendPacket(NM_CH_E ch, uint8* data, uint16 len) {};
    virtual void clearDccRdBufForce(NM_CH_E ch){};

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
    virtual void setAlarm(uint8 remote, uint8 local){};
    virtual void protectFSMStart(void){};

    /*
     * for loop
     */
    virtual void setPortLoopOut(uint8 ch, uint8 loop);
    virtual void setPortLoopIn(uint8 ch, uint8 loop);

    /*
     * for led
     */
    virtual void turnOnLedWork(uint8 sn, bool on);
    virtual void turnOnLedAlm(uint8 sn, bool on);

    void sabitInterruptEnable(bool en);

    void resetRemote(uint8 port);
    virtual void setPortEnable(uint8 port, uint8 en);
private:
    OS_MUT mut_dcn_rcv;
    static const uint16 REG_VER_CPLD = 0x0010;
    static const uint16 REG_VER_FPGA_E1LOGIC = 0x1010;
    std::string ChipVerInfo;

    static const uint16 REG_INTRR_EN = 0x0002;
    static const uint16 LED     = 0x0100; //bit0 WORK; bit1 STA
    static const uint16 LED_ALM = 0x0101; // bit0~15 ALM1~16
    static const uint16 LED_WRK = 0x0102;

    static const uint16 REG_CLK_2M_SEL    = 0x1000; //bit4 0 off 1 on; bit3~0 port sn
    static const uint16 REG_E1_ALM_BASE   = 0x1410; //bit2 1 LOMF; bit3 1 RDI; bit4 0 LOF
    static const uint16 REG_E1_LOS        = 0x1460;
    static const uint16 REG_E1_AIS        = 0x1461;

    static const uint16 REG_LOOP_INNER      = 0x1465;
    static const uint16 REG_LOOP_OUTER      = 0x1466;
    static const uint16 REG_PORT_EN         = 0x1467;
    static const uint16 REG_REMOTE_RESET_L  = 0x146c;
    static const uint16 REG_REMOTE_RESET_H  = 0x146d;

    static const uint16 REG_SABIT_R_STA = 0x1403; //bit0 rcv ready
    static const uint16 REG_SABIT_R_CH  = 0x1404; //RCV channal sn
    static const uint16 REG_SABIT_R_LEN = 0x1405; //RCV len
    static const uint16 REG_SABIT_R_BUF = 0x1600; //RCV Data
    static const uint16 REG_SABIT_R_OVER= 0x140f; //RCV Over

    static const uint16 REG_SABIT_T_STA = 0x1400;
    static const uint16 REG_SABIT_T_BUF = 0x1500; //SND Data
    static const uint16 REG_SABIT_T_CH  = 0x1401; //SND channal sn
    static const uint16 REG_SABIT_T_LEN = 0x1402; //SND len
    static const uint16 REG_SABIT_T_OVER= 0x140c; //SND OVER 0x46 fix
    static const uint16 REG_SABIT_T_BROAD = 0x140d; //bit7 1
    static const uint16 REG_SABIT_T_CH_STA_L= 0x146a;
    static const uint16 REG_SABIT_T_CH_STA_H= 0x146b;

    static const uint16 REG_SABIT_INTRR_EN = 0x1750;
    static const uint16 REG_DCN_INTRR_EN = 0x1751;

    //for dcn
    uint16 REG_DCN_TS_SEL_L(uint8 ch) {
        return 0x1700 + (ch<<1);
    };
    uint16 REG_DCN_TS_SEL_H(uint8 ch) {
        return 0x1701 + (ch<<1);
    };

    static const uint16 REG_DCN_T_STA = 0x1720;
    static const uint16 REG_DCN_T_OVER = 0x1721;

    uint16 REG_DCN_T_BUF(uint8 ch) {
        return 0x1730 + ch;
    };

    static const uint16 REG_DCN_T_BROAD = 0x1760;

    static const uint16 REG_DCN_R_STA = 0x1722;
    static const uint16 REG_DCN_R_OVER = 0x1723;
    static const uint16 REG_DCN_R_SEL = 0x1740;
    static const uint16 REG_DCN_R_LEN = 0x1741;
    static const uint16 REG_DCN_R_BUF = 0x1742;

    static const uint16 REG_DCN_MONI_SEL = 0x1728;
    static const uint16 REG_DCN_PAGE_STA = 0x1727;
//    uint16 REG_DCN_R_BUF(uint8 ch) {
//        return 0x1740 + ch;
//    };
//    uint16 REG_DCN_R_LEN(uint8 ch) {
//        return 0x1770 + ch;
//    };

};

#endif /* CHIPE1PORT16LOGIC_H_ */
