/*
 * BAlarmModule.h
 *
 *  Created on: 2014
 *      Author: Administrator
 */

#ifndef BALARMMODULE_H_
#define BALARMMODULE_H_

#include <map>
#include <string>
#include "os.h"
#include "AlarmDataDefine.h"


class BAlarmSource;
class BAlarmDisplay;
class CBaseCard;
class AlarmInstance;

typedef std::map<uint32, BAlarmSource*> SRCMap;

class BAlarmModule {
    BAlarmModule();
public:
    BAlarmModule(CBaseCard*);
    virtual ~BAlarmModule();

    void initModule(void); //ÿ���忨�����ڸ澯ģ�鹹����ɺ����ȵ��ô˷���������

    void initSource(void);
    void processAlarm(void);

    virtual void creatAlarmSource() = 0;
    void addAlmSrc(BAlarmSource*);
    void delAlmSrc(BAlarmSource*);

    bool saveConfigData(void);

    virtual bool ifMCUModule(void) {
        return false;
    };

    bool ifHaveMAJ(void);
    bool ifHaveMin(void);
    bool ifAlarm(void);
    uint32 itsSn(void) { return slotSn; };

    /* �澯����������� */
    bool setAlarmPropertyTo(uint32 alarmType, Alarm_Property_T*);

    /*�澯����ȫ�ַ������*/
    static AlarmInstance* getAlarmInstance(uint32 slot, uint32 srcid, uint32 type);
    static AlarmInstance* getFirstAlarmInstance();
    static AlarmInstance* getNextAlarmInstance(uint32 slot, uint32 srcid, uint32 type);
    static AlarmInstance* getNextAlarmInstance(AlarmInstance*);


protected:
    CBaseCard* card;

private:
    SRCMap SrcGroup;
    OS_TID tsk_alm_process;
    uint32 slotSn;
    uint8* task_stack;
//    std::map<std::string, BAlarmDisplay*> AlmDisplay;
    BAlarmSource* getFirstAlarmSource(void);
    BAlarmSource* getAlarmSource(uint32 uid);
    BAlarmSource* getNextAlarmSource(BAlarmSource*);


private:
    static std::map<uint32, BAlarmModule*> group;
public:
    /* group option */
    static BAlarmModule* getInstance(uint32 uid) {
        std::map<uint32, BAlarmModule*>::iterator it = group.find(uid);
        if( it != group.end() ) {
            return it->second;
        }
        return 0;
    };

    static BAlarmModule* getFirstInstance(void) {
        std::map<uint32, BAlarmModule*>::iterator it = group.begin();
        if( it != group.end() ) {
            return it->second;
        }
        return 0;
    };

    static BAlarmModule* getNextInstance(uint32 suid) {
        std::map<uint32, BAlarmModule*>::iterator it = group.find(suid);
        if( it != group.end() ) {
            it++;
            if( it != group.end() ) {
                return it->second;
            }
        }
        return 0;
    };
};

#endif /* BALARMMODULE_H_ */
