/*
 * SrcAlarmVC4.h
 *
 *  Created on: 2014��9��5��
 *      Author: Administrator
 */

#ifndef SRCALARMVC4_H_
#define SRCALARMVC4_H_

#include "BAlarmSource.h"
#include "XCAlarmDataDefine.h"

class BAlarmModule;
class RscVC4;

class SrcAlarmVC4: public BAlarmSource {
    SrcAlarmVC4();
public:
    SrcAlarmVC4(BAlarmModule*, RscVC4*, SrcVC4Alarm_Data*);
    virtual ~SrcAlarmVC4();

    virtual void makeAlarmSignal(void); //�����澯�źţ�ͬʱΪ�澯�źŷ���澯���������У���
                                            //��Ҫ�ڸ澯Դ����ʱ���ã���Ϊ�澯Դ����ʱ���澯������ܻ�δ��ȷ���졣

    virtual bool ifSrcRestrain();
    virtual bool getAlarm(int type);
    virtual std::string getTypeName(int type);
    virtual uint32 itsID(void);
    virtual std::string& itsName(void);

private:
    SrcVC4Alarm_Data* pConfigData;
    RscVC4* port;

};

#endif /* SRCALARMVC4_H_ */
