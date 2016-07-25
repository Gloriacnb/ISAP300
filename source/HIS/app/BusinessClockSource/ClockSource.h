/*
 * ClockSource.h
 *
 *  Created on: 2014-1-13
 *      Author: Administrator
 */

#ifndef CLOCKSOURCE_H_
#define CLOCKSOURCE_H_

#include <string>
#include "EZ_types.h"
#include "BaseResource.h"

typedef enum {
    clock_ok,
    clock_failed,
    clock_19 = 0x50,
    clock_2,
    clock_free,
    clock_pcm_main,
    clock_pcm_recover,
}CLOCK_STATUS_E;

class ClockSource : public BaseResource {
public:
    ClockSource(uint32 id, bool paired = false);
    virtual ~ClockSource();

    /*
     * ��ȡʱ��Դ״̬
     */
    virtual CLOCK_STATUS_E getStatus(void) = 0;

    /*
     * �����ʱ��Դ
     */
    virtual bool active(void) = 0;
    virtual bool deActive(void) {
        return true;
    };
    /*
     * ��Ӧ�˿���ƣ�����У�
     */
    virtual std::string& getName(void) {
        Nname = Resource::getName() + getPortName();
        return Nname;
    };
    virtual std::string& getPortName(void) {
        return pname;
    };
private:
    std::string Nname;
    std::string pname;
};

#endif /* CLOCKSOURCE_H_ */
