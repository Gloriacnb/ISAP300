/*
 * DisplayEverySignal.cpp
 *
 *  Created on: 2014��10��21��
 *      Author: Administrator
 */

#include "DisplayEverySignal.h"
#include "AlarmInstance.h"

DisplayEverySignal::DisplayEverySignal(const char* name) : BAlarmDisplay(name) {
    // TODO Auto-generated constructor stub

}

DisplayEverySignal::~DisplayEverySignal() {
    // TODO Auto-generated destructor stub
}

void DisplayEverySignal::update(AlarmInstance* i) {
    display(i->currentState());
}
