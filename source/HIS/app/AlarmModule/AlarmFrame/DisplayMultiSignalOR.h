/*
 * DisplayMultiSignalOR.h
 *
 *  Created on: 2014��10��21��
 *      Author: Administrator
 */

#ifndef DISPLAYMULTISIGNALOR_H_
#define DISPLAYMULTISIGNALOR_H_

#include "BAlarmDisplay.h"

class DisplayMultiSignalOR: public BAlarmDisplay {
public:
    DisplayMultiSignalOR(const char* name);
    virtual ~DisplayMultiSignalOR();

    virtual void update(AlarmInstance*);
    virtual void display(bool alarm) = 0;
};

#endif /* DISPLAYMULTISIGNALOR_H_ */
