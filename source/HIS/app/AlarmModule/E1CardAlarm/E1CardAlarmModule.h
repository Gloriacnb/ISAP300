/*
 * E1CardAlarmModule.h
 *
 *  Created on: 2014��9��9��
 *      Author: Administrator
 */

#ifndef E1CARDALARMMODULE_H_
#define E1CARDALARMMODULE_H_

#include "BAlarmModule.h"
#include "E1LEDs.h"
class Card24E1;

class E1CardAlarmModule: public BAlarmModule {
public:
    E1CardAlarmModule(Card24E1* c);
    virtual ~E1CardAlarmModule();

    virtual void creatAlarmSource();

private:
    Card24E1* card;
    E1LEDs led;
};

#endif /* E1CARDALARMMODULE_H_ */
