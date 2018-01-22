/*
 * PortVFG.h
 *
 *  Created on: 2018年1月19日
 *      Author: Administrator
 */

#ifndef SOURCE_HIS_APP_BUSINESSRESOURCE_OTHERCARDRESOURCE_PORTVFG_H_
#define SOURCE_HIS_APP_BUSINESSRESOURCE_OTHERCARDRESOURCE_PORTVFG_H_

#include <PortVF.h>

class PortVFG: public PortVF {
public:
	PortVFG(uint32 uid, PairTSChannel* t, VFPort_Config_T* config, VFGroup_Config_T* gconfig, CardVF* card);
	virtual ~PortVFG();

    virtual uint8 getRcvGain();
    virtual uint8 getSndGain();
    virtual bool setRcvGain(uint8 g, bool save = true);
    virtual bool setSndGain(uint8 g, bool save = true);

};

#endif /* SOURCE_HIS_APP_BUSINESSRESOURCE_OTHERCARDRESOURCE_PORTVFG_H_ */
