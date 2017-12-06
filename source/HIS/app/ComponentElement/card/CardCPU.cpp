/*
 * CardCPU.cpp
 *
 *  Created on: 2013-12-25
 *      Author: Administrator
 */

#include "CardCPU.h"
#include "lpc32xx_gpio_driver.h"
#include "EZ_types.h"
#include "CPPTools.h"

static const uint8 MainVerNumberX = 3;
static const uint8 MainVerNumberY = 1;
static const uint8 MainVerNumberZ = 20;

CardCPU::CardCPU() {
}

CardCPU::~CardCPU() {
    // TODO Auto-generated destructor stub
}

int CardCPU::itsSlot(void) {
    return (gpio_get_inppin_states() >> 10) & 1;
}

std::string CardCPU::GetVerInfo(void){
	return "MCU:" +
	        CPPTools::number2string(MainVerNumberX) + "." +
	        CPPTools::number2string(MainVerNumberY) + "." +
	        CPPTools::number2string(MainVerNumberZ);
}
