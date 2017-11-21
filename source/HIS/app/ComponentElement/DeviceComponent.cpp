/*
 * DeviceComponent.cpp
 *
 *  Created on: 2013-11-19
 *      Author: Administrator
 */

#include "DeviceComponent.h"
#include "DeviceAttribute.h"
#include "IsapCompFac.h"
#include "EmuxCompFac.h"
#include "SlotModule.h"
#include "lpc32xx_gpio_driver.h"
#include "CLoopModule.h"
#include "XCPair.h"
DeviceAttribute* DeviceComponent::pdevattr;
ComponentFactory* DeviceComponent::f;

DeviceComponent::DeviceComponent() {
    // TODO Auto-generated constructor stub
    pdevattr = 0;
//    xcpair = 0;
    f = 0;
}

DeviceComponent::~DeviceComponent() {
    // TODO Auto-generated destructor stub
}

int DeviceComponent::initDeviceComponent(void) {

    XCPair::instance().init(pdevattr->getDeviceSubType());
    return 0;
}

void DeviceComponent::run(void) {
    CLoopModule::instance().start();
    SlotModule::initSlotModuleWorking();

}

void DeviceComponent::clearDeviceCompnent(void) {

    SlotModule::clearSlotModuleWorking();
    CLoopModule::instance().stop();
    XCPair::instance().deInit();
}

int DeviceComponent::initDeviceComponentCommon(void) {
    pdevattr = new DeviceAttribute();
    if( !pdevattr ) {
        return -1;
    }
    std::string type;
    pdevattr->getBaseAttribute(ln_subtype, type);
    if( type == "ISAP300-155" || type == "ISAP300-622" ||
            type == "isap300-155" || type == "isap300-622" ) {
        f = new IsapCompFac();
    }
    else {
        f = new EmuxCompFac();
    }
    //����������ָ����󴴽���ģ��
    if( f ) {
        SlotModule::initSlotModule(f);
        return 0;
    }
#ifdef EZ_DEBUG
    std::cout << "!!!No componentFactory!!!" << std::endl;
#endif
    return -1;
}
