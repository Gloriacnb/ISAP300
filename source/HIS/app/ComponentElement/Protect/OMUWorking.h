/*
 * OMUWorking.h
 *
 *  Created on: 2014��11��27��
 *      Author: Administrator
 */

#ifndef OMUWORKING_H_
#define OMUWORKING_H_

#include "OMUStatus.h"

class OMUWorking: public OMUStatus {
public:
    OMUWorking();
    virtual ~OMUWorking();

    virtual Working_STATE_E getStatus(void) {
        return OMU_Working;
    };
    OS_TID task_work;
private:
    OS_TID task_fast_alarm;
    OS_TID task_sync_files;
};

#endif /* OMUWORKING_H_ */
