/*
 * UID.h
 *
 *  Created on: 2013-2-26
 *      Author: Administrator
 */

#ifndef UID_H_
#define UID_H_

#include "EZ_types.h"
#include <string>
//typedef unsigned char uint8;
//typedef unsigned int  uint32;
typedef struct {
	uint8 slot;
    uint8 xcsn;
	uint8 stm;
	uint8 hp;
	uint8 lp;
}ST_SDH_VC12;

typedef struct {
    uint8 slot;
    uint8 xcsn;
    uint8 stm;
    uint8 hp;
}ST_SDH_VC4;

typedef struct {
	uint8 slot;
    uint8 xcsn;
	uint8 stm;
}ST_SDH_STM;

typedef struct {
	uint8 slot;
	uint8 vcg;
}ST_VCG;

typedef struct {
	uint8 slot;
	uint8 vcg;
	uint8 member;
}ST_VCG_Member;

typedef struct {
	uint8 slot;
	uint8 E1;
}ST_E1;

typedef struct {
    uint8 slot;
    uint8 port;
}ST_VF;

typedef struct {
    uint8 slot;
    uint8 port;
}ST_DATA;

typedef struct {
	uint8 slot;
    uint8 xcsn;
	uint8 E1;
	uint8 TS;
}ST_Time_Slot;

typedef struct {
	uint8 slot;
	uint8 xcsn;
	uint8 stbus;
}ST_Stbus;

typedef struct {
    uint8 slot;
    uint8 xcsn;
    uint8 sdhbus;
}ST_Sdhbus;

typedef struct {
    uint8 slot;
    uint8 xcsn;
    uint8 bus;
    uint8 channel;
}ST_Sdh_channel;

typedef struct {
    uint8 slot;
    uint8 group;
    uint8 channel;
}ST_Chvc_channel;

typedef struct {
    uint8 slot;
    uint8 group;
    uint8 channel;
}ST_Sltch_channel;

typedef struct {
    uint8 slot;
    uint8 type;
    uint8 sn;
}ST_Clock_source;

typedef struct {
    uint8 slot;
    uint8 subtype;
    uint8 sn;
}ST_NM_Channel;

typedef union {
	ST_SDH_VC12 vc12;
	ST_SDH_VC4 vc4;
	ST_SDH_STM stm;
	ST_VCG vcg;
	ST_VCG_Member member;
	ST_E1 E1;
	ST_Time_Slot ts;
	ST_Stbus stbus;
	ST_Sdhbus sdhbus;
	ST_Sdh_channel sdhch;
	ST_VF vf;
	ST_DATA data;
	ST_Chvc_channel chvc;
	ST_Sltch_channel sltch;
	ST_Clock_source clock;
	ST_NM_Channel nm;
}UN_Info;

enum {
    subtype_dcc = 0,
    subtype_sabit,
    subtype_tsdcn,
    subtype_STM = 0,
    subtype_vc4,
    subtype_vc12,
    subtype_vcg = 0,
    subtype_member = 2,
    subtype_stbus = 1,
    subtype_ts,
    subtype_sdhbus = 1,
    subtype_sdhch,
};

class UID {
public:
	UID();
	virtual ~UID();
	static std::string getNameByUID(uint32 id);
	static UN_Info breakUID(uint32 id);
	static uint32 makeUID(UN_Info* info);
	static uint32 makeUID(ST_SDH_VC12*);
    static uint32 makeUID(ST_SDH_VC4*);
	static uint32 makeUID(ST_SDH_STM*);
	static uint32 makeUID(ST_E1*);
    static uint32 makeUID(ST_VF*);
    static uint32 makeUID(ST_DATA*);
	static uint32 makeUID(ST_Time_Slot*);
	static uint32 makeUID(ST_VCG*);
	static uint32 makeUID(ST_VCG_Member*);
	static uint32 makeUID(ST_Stbus*);
    static uint32 makeUID(ST_Sdhbus*);
    static uint32 makeUID(ST_Sdh_channel*);
    static uint32 makeUID(ST_Chvc_channel*);
    static uint32 makeUID(ST_Sltch_channel*);
    static uint32 makeUID(ST_Clock_source*);
    static uint32 makeUID(ST_NM_Channel*);

	static uint8 getPortTypeByUID(uint32 id);
	static uint32 getPartnerUID(uint32 id);
	static uint32 getPairUID(uint32 id);

	static const uint8 type_SDH_622 = 2;
	static const uint8 type_SDH_155 = 1;
	static const uint8 type_VCG = 0x40;
	static const uint8 type_chvc= 0x41;
	static const uint8 type_sltch = 0x42;
	static const uint8 type_E1 = 0x20;
	static const uint8 type_FE = 0x30;
	static const uint8 type_FX = 0x31;
	static const uint8 type_stbus = 0x60;
	static const uint8 type_sdhbus = 0x10;
	static const uint8 type_VF = 0x80;
	static const uint8 type_DATA = 0x81;
	static const uint8 type_opt_clock = 0x50;
	static const uint8 type_2m_clock = 0x51;
	static const uint8 type_local_clock = 0x52;
	static const uint8 type_pcm_main_clock = 0x53;
	static const uint8 type_pcm_recover_clock = 0x54;
	static const uint8 type_nm_channel = 0x55;

};



#endif /* UID_H_ */
