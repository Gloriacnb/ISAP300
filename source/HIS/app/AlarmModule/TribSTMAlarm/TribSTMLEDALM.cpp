/*
 * TribSTMLEDALM.cpp
 *
 *  Created on: 2014��9��18��
 *      Author: Administrator
 */

#include "TribSTMLEDALM.h"
#include "Chip8STM1CPLD.h"

TribSTMLEDALM::TribSTMLEDALM(int sn, Chip8STM1CPLD& d) : DisplayMultiSignalOR("TribSTM_LED_LOS"), Driver(d) {

    itsSn = sn;
    Driver.portLEDAlarmOn(itsSn, false);
}

TribSTMLEDALM::~TribSTMLEDALM() {
    // TODO Auto-generated destructor stub
}

void TribSTMLEDALM::display(bool alarm) {
    Driver.portLEDAlarmOn(itsSn, alarm);
}
