/*
 * UID.cpp
 *
 *  Created on: 2013-2-26
 *      Author: Administrator
 */

#include "UID.h"
#include "SysError.h"
#include "ObjectReference.h"
#include "CPPTools.h"

UID::UID() {
	// TODO Auto-generated constructor stub

}

UID::~UID() {
	// TODO Auto-generated destructor stub
}

UN_Info UID::breakUID(uint32 id) {
	UN_Info info;
	uint8 type = (id >> 16) & 0xff;
	uint8 sub_type = (id>>6) & 0x3;
	uint8 slot = (id >> 24) & 0xf;
	switch( type ) {
	case type_SDH_155:
	case type_SDH_622:
		if(sub_type == subtype_STM) {
			info.stm.slot = slot;
	        info.stm.xcsn = (id >> 28) & 0xf;
			info.stm.stm = (id >> 12) & 0x0f;
		}
		else if( sub_type == subtype_vc4 ) {
		    info.vc4.slot = slot;
	        info.vc4.xcsn = (id >> 28) & 0xf;
		    info.vc4.stm = (id >> 12) & 0x0f;
		    info.vc4.hp = (id >>  8) & 0x0f;
		}
		else if( sub_type == subtype_vc12 ) {
			info.vc12.slot = slot;
	        info.vc12.xcsn = (id >> 28) & 0xf;
			info.vc12.stm = (id >> 12) & 0x0f;
			info.vc12.hp  = (id >>  8) & 0x0f;
			info.vc12.lp  = (id >>  0) & 0x3f;
		}
		else {
			throw SysError("the UID isn't a SDH resource!");
		}
		break;
	case type_nm_channel: {
	    info.nm.slot = slot;
	    info.nm.subtype = sub_type;
	    info.nm.sn = id & 0xf;
	}
	    break;

	case type_E1:
		info.E1.slot = slot;
		info.E1.E1 = (id >> 8) & 0xff;
		break;
	case type_VF:
	    info.vf.slot = slot;
	    info.vf.port = (id >> 8) & 0xff;
	    break;
	case type_DATA:
        info.data.slot = slot;
        info.data.port = (id >> 8) & 0xff;
        break;
	case type_VCG:
		if(sub_type == subtype_vcg) {
			info.vcg.slot = slot;
			info.vcg.vcg = (id >> 8) & 0xff;
		}
		else if( sub_type == subtype_member ) {
			info.member.slot = slot;
			info.member.vcg = (id >> 8) & 0xff;
			info.member.member = (id >> 0) & 0x3f;
		}
		else {
            throw SysError("the UID isn't a VCG resource!");
		}
		break;
	case type_chvc:
	    info.chvc.slot = slot;
	    info.chvc.group = (id >> 8) & 0xff;
	    info.chvc.channel = (id >> 0) & 0x3f;
	    break;
    case type_sltch:
        info.sltch.slot = slot;
        info.sltch.group = (id >> 8) & 0xff;
        info.sltch.channel = (id >> 0) & 0x3f;
        break;
	case type_stbus:
	    if( sub_type == subtype_stbus ) {
            info.stbus.slot = slot;
            info.stbus.stbus = (id >> 8) & 0xff;
            info.stbus.xcsn = (id >> 28) & 0xf;
	    }
	    else if( sub_type == subtype_ts ) {
	        info.ts.slot = slot;
	        info.ts.E1 = (id >> 8) & 0xff;
	        info.ts.TS = (id >> 0) & 0x3f;
            info.ts.xcsn = (id >> 28) & 0xf;
	    }
        else {
            throw SysError("the UID isn't a STBus resource!");
        }
		break;
	case type_sdhbus:
	    if( sub_type == subtype_sdhbus ) {
	        info.sdhbus.slot = slot;
	        info.sdhbus.sdhbus = (id >> 8) & 0xff;
	        info.sdhbus.xcsn = (id >> 28) & 0xf;
	    }
	    else if( sub_type == subtype_sdhch ) {
	        info.sdhch.slot = slot;
            info.sdhch.xcsn = (id >> 28) & 0xf;
	        info.sdhch.bus = (id >> 8) & 0xff;
	        info.sdhch.channel = (id >> 0) & 0x3f;
	    }
	    else {
            throw SysError("the UID isn't a SDHbus resource!");
	    }
	    break;
	case type_opt_clock:
        info.clock.slot = slot;
        info.clock.type = type_opt_clock;
        info.clock.sn = id & 0xff;
        break;
	case type_2m_clock:
        info.clock.slot = slot;
        info.clock.type = type_2m_clock;
        info.clock.sn = id & 0xff;
        break;
	case type_local_clock:
	    info.clock.slot = slot;
	    info.clock.type = type_local_clock;
	    info.clock.sn = id & 0xff;
	    break;
	case type_pcm_main_clock:
	    info.clock.slot = slot;
	    info.clock.type = type_pcm_main_clock;
	    info.clock.sn = id & 0xff;
	    break;
	case type_pcm_recover_clock:
	    info.clock.slot = slot;
	    info.clock.type = type_pcm_recover_clock;
	    info.clock.sn = id & 0xff;
	    break;
	default:
        throw SysError("the UID isn't a resource!");
//		break;
	}

	return info;
}

static const char* SLOTNAME[] = {
        "DXC1", "DXC2",
        "EXT1", "EXT2", "EXT3", "EXT4", "EXT5",
        "PWR1", "PWR2",
        "FAN"
};

uint32 UID::getPartnerUID(uint32 id) {
    UN_Info info = breakUID(id);
    uint32 newID = id;
    uint8 type = (newID >> 16) & 0xff;
    switch( type ) {
    case type_SDH_155:
    case type_SDH_622:
    case type_sdhbus:
    case type_stbus:{
        if( (newID >> 28) == 0 ) {
            newID |= 0x10000000;
        }
        else {
        	newID &= 0x0fffffff;
        }
    }
    break;
    default: {
        if( (newID >> 24) == 0 ) {
            newID |= 0x01000000;
        }
        else {
        	newID &= 0x00ffffff;
        }
    }
    }
    return newID;
}


uint32 UID::getPairUID(uint32 id) {
    uint32 newID = id;
    UN_Info info = breakUID(newID);
    uint8 type = (newID >> 16) & 0xff;
    switch( type ) {
    case type_SDH_155:
    case type_SDH_622:
    case type_sdhbus:
    case type_stbus: {
        newID &= 0x0fffffff;
    }
    break;
    default: {
        newID &= 0x00ffffff;
    }
    }
    return newID;
}


std::string UID::getNameByUID(uint32 id) {
    UN_Info info = breakUID(id);
    std::string name;
    uint8 type = (id >> 16) & 0xff;
    uint8 sub_type = (id>>6) & 0x3;
    switch( type ) {
    case type_SDH_155:
    case type_SDH_622:
        if(sub_type == 0) {
            name = "STM" + CPPTools::number2string(type*type) + "/" +
                    std::string(SLOTNAME[info.stm.slot]) + "/" +
                    CPPTools::number2string(info.stm.stm+1);
        }
        else if( sub_type == 1 ) {
            name = "VC4/" +
                std::string(SLOTNAME[info.vc4.slot]) + "/" +
                CPPTools::number2string(info.vc4.stm+1) + "/" +
                CPPTools::number2string(info.vc4.hp+1);
        }
        else if( sub_type == 2 ) {
            name = "VC12/" +
                    std::string(SLOTNAME[info.vc12.slot]) + "/" +
                CPPTools::number2string(info.vc12.stm+1) + "/" +
                CPPTools::number2string(info.vc12.hp+1) + "/" +
                CPPTools::number2string(info.vc12.lp+1);
        }
        else {
            throw SysError("!!!It's not a valid SDH UID!!!");
        }
        break;
    case type_nm_channel:
        switch( sub_type ) {
        case subtype_dcc:
            name = "SDHDCC/" +
                std::string(SLOTNAME[info.nm.slot]) + "/" +
                CPPTools::number2string(info.nm.sn+1);
            break;

        case subtype_sabit:
            name = "SABIT/" +
                std::string(SLOTNAME[info.nm.slot]) + "/" +
                CPPTools::number2string(info.nm.sn+1);
            break;
        }
        break;
    case type_E1:
        name = "E1/" +
                std::string(SLOTNAME[info.E1.slot]) + "/" +
                CPPTools::number2string(info.E1.E1+1);
        break;
    case type_VF:
        name = "VF/" +
        std::string(SLOTNAME[info.vf.slot]) + "/" +
                CPPTools::number2string(info.vf.port+1);
        break;
    case type_DATA:
        name = "DATA/" +
        std::string(SLOTNAME[info.data.slot]) + "/" +
                CPPTools::number2string(info.data.port+1);
        break;
    case type_VCG:
        if(sub_type == 0) {
            name = "VCG/" +
                    std::string(SLOTNAME[info.vcg.slot]) + "/" +
                    CPPTools::number2string(info.vcg.vcg+1);
        }
        else if( sub_type == 2 ) {
            name = "Member/" +
                    std::string(SLOTNAME[info.member.slot]) + "/" +
                    CPPTools::number2string(info.member.vcg+1) + "/" +
                    CPPTools::number2string(info.member.member+1);
        }
        else {
            throw SysError("!!!It's not a valid VCG UID!!!");
        }
        break;
    case type_chvc:
        name = "CHVC/" +
        std::string(SLOTNAME[info.chvc.slot]) + "/" +
                CPPTools::number2string(info.chvc.group+1) + "/" +
                CPPTools::number2string(info.chvc.channel+1);
        break;
    case type_sltch:
        name = "CHVC/" +
        std::string(SLOTNAME[info.sltch.slot]) + "/" +
                CPPTools::number2string(info.sltch.group+1) + "/" +
                CPPTools::number2string(info.sltch.channel+1);
        break;
    case type_stbus:
        if( sub_type == 1 ) {
            if( info.stbus.stbus < 4 ) {
                name = "EXTE1/";
            }
            else if( info.stbus.stbus < 8 ) {
                info.stbus.stbus -= 4;
                name = "INNRE1/";
            }
            else {
                info.stbus.stbus -= 8;
                name = "STBUS/";
            }
            name += std::string(SLOTNAME[info.stbus.slot]) + "/" +
                    CPPTools::number2string(info.stbus.stbus+1);
        }
        else if( sub_type == 2 ) {
            name = "TS/" +
                    std::string(SLOTNAME[info.ts.slot]) + "/" +
                    CPPTools::number2string(info.ts.E1+1) + "/" +
                    CPPTools::number2string(info.ts.TS+1);
        }
        else {
            throw SysError("!!!It's not a valid stbus UID!!!");
        }
        break;
    case type_sdhbus:
        if( sub_type == 1 ) {
            name = "SDHBUS/" +
                    std::string(SLOTNAME[info.sdhbus.slot]) + "/" +
                    CPPTools::number2string(info.sdhbus.sdhbus+1);
        }
        else if( sub_type == 2 ) {
            name = "SDHCH/" +
                    std::string(SLOTNAME[info.sdhch.slot]) + "/" +
                    CPPTools::number2string(info.sdhch.bus+1) + "/" +
                    CPPTools::number2string(info.sdhch.channel+1);
        }
        else {
            throw SysError("!!!It's not a valid sdhbus UID!!!");
        }
        break;
    case type_opt_clock:
        name = "CLOCKOPT/" +
                std::string(SLOTNAME[info.sdhch.slot]) + "/" +
                CPPTools::number2string(info.clock.sn+1);
        break;
    case type_2m_clock:
        if( info.clock.sn == 0 ) {
            name = "CLOCK2MHZ/" +
                    std::string(SLOTNAME[info.sdhch.slot]) + "/" +
                    CPPTools::number2string(info.clock.sn+1);
        }
        else {
            name = "CLOCK2MBIT/" +
                    std::string(SLOTNAME[info.sdhch.slot]) + "/" +
                    CPPTools::number2string(info.clock.sn+1);
        }
        break;
    case type_local_clock:
        name = "CLOCKFREE/" +
                std::string(SLOTNAME[info.sdhch.slot]) + "/" +
                CPPTools::number2string(info.clock.sn+1);
        break;
    case type_pcm_main_clock:
        name = "PCMCLOCKFREE/" +
                std::string(SLOTNAME[info.sdhch.slot]) + "/" +
                CPPTools::number2string(info.clock.sn+1);
        break;
    case type_pcm_recover_clock:
        name = "PCMCLOCKRECOVER/" +
                std::string(SLOTNAME[info.sdhch.slot]) + "/" +
                CPPTools::number2string(info.clock.sn+1);
        break;
    default:
        throw SysError("!!!It's not a valid UID!!!");
//      break;
    }
    return name;
}

uint32 UID::makeUID(ST_SDH_VC12* info) {
    uint8 ctype = ObjectReference::getOPTMode() ? type_SDH_155 : type_SDH_622;
    if( info->slot > 1 ) {
        ctype = type_SDH_155;
    }
	return ( info->slot << 24) |
            (info->xcsn << 28) |
			(info->stm << 12) |
			(info->hp  <<  8) |
			(info->lp  <<  0) |
			(ctype     << 16) |
			(2         <<  6) ;
}
uint32 UID::makeUID(ST_SDH_STM* info) {
    uint8 ctype = ObjectReference::getOPTMode() ? type_SDH_155 : type_SDH_622;
    if( info->slot > 1 ) {
        ctype = type_SDH_155;
    }
	return (info->slot << 24) |
            (info->xcsn << 28) |
			(info->stm << 12) |
			(ctype     << 16) ;
}

uint32 UID::makeUID(ST_NM_Channel* info) {
    return (info->slot << 24) |
           (type_nm_channel << 16) |
           ((info->subtype & 3) << 6) |
           (info->sn) ;
}

uint32 UID::makeUID(ST_E1* info) {
	return (info->slot << 24) |
			(info->E1  <<  8) |
			(type_E1   << 16) ;

}
uint32 UID::makeUID(ST_VF* info) {
    return (info->slot << 24) |
            (info->port  <<  8) |
            (type_VF   << 16) ;

}
uint32 UID::makeUID(ST_DATA* info) {
    return (info->slot << 24) |
            (info->port  <<  8) |
            (type_DATA   << 16) ;
}

uint32 UID::makeUID(ST_Time_Slot* info) {
	return ( info->slot << 24) |
            (info->xcsn << 28) |
			(info->E1   <<  8) |
			(info->TS   <<  0) |
            (2          <<  6) |
			(type_stbus <<  16);
}
uint32 UID::makeUID(ST_VCG* info) {
	return ( info->slot << 24) |
			(info->vcg  << 8)  |
			(type_VCG   << 16) ;
}

uint32 UID::makeUID(ST_VCG_Member* info) {
	return ( info->slot  << 24) |
			(info->vcg   <<  8) |
			(info->member<<  0) |
			(2           <<  6) |
			(type_VCG    <<  16);
}

uint32 UID::makeUID(ST_Stbus* info) {
	return (info->slot << 24) |
	        (info->xcsn << 28) |
			(info->stbus  <<  8) |
            (1           <<  6) |
			(type_stbus   << 16) ;

}

uint32 UID::makeUID(ST_Sdhbus* info) {
    return ( info->slot << 24) |
            (info->xcsn << 28) |
            (info->sdhbus  << 8)  |
            (1           <<  6) |
            (type_sdhbus   << 16) ;
}
uint32 UID::makeUID(ST_Sdh_channel* info) {
    return ( info->slot   << 24) |
            (info->xcsn << 28) |
            (info->bus    <<  8) |
            (info->channel<<  0) |
            (2            <<  6) |
            (type_sdhbus  <<  16);
}
uint32 UID::makeUID(ST_SDH_VC4* info) {
    uint8 ctype = ObjectReference::getOPTMode() ? type_SDH_155 : type_SDH_622;
    if( info->slot > 1 ) {
        ctype = type_SDH_155;
    }
    return ( info->slot << 24) |
            (info->xcsn << 28) |
            (info->stm << 12) |
            (info->hp  <<  8) |
            (ctype     << 16) |
            (1         <<  6) ;
}

uint32 UID::makeUID(ST_Chvc_channel* info) {
    return ( info->slot   << 24) |
            (info->group  <<  8) |
            (info->channel<<  0) |
            (1            <<  6) |
            (type_chvc    <<  16);
}

uint32 UID::makeUID(ST_Sltch_channel* info) {
    return ( info->slot   << 24) |
            (info->group  <<  8) |
            (info->channel<<  0) |
            (1            <<  6) |
            (type_sltch    <<  16);
}

uint32 UID::makeUID(ST_Clock_source* info) {
    return (info->slot << 24) |
            (info->type  <<  16) |
            (info->sn   << 0) ;
}

uint8 UID::getPortTypeByUID(uint32 id) {
	return  (id >> 16) & 0xff;
}
