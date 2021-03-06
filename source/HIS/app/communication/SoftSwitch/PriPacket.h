/*
 * PriPacket.h
 *
 *  Created on: 2013-6-25
 *      Author: Administrator
 */

#ifndef PRIPACKET_H_
#define PRIPACKET_H_

#include "Layer2Tag.h"
#include <string>
#include "SoftSwitch_define.h"
#include "EthMemory.h"
#include <map>

typedef enum {
	unicast,
	broadcast,
	multicast,
	error_type = -1

}packet_type;

class NMPort;

class PriPacket {
	PriPacket();
	virtual ~PriPacket() {};
	static void calculateLifeTime(uint32 start, uint32 end);
	static uint32 MIN_DURATION;       	//最长生存时间
	static uint32 MAX_DURATION;			//最长生存时间
	static uint32 AVR_DURATION;			//总平均生存时间
	static uint32 RECT_AVR_DURATION;	//最近20次平均生存时间
	static uint32 TOTAL_TIMES;			//总包数

	static int TRACE;
public:
	const static uint8 multicastAddress[6];
	const static uint8 broadcastAddress[6];
	PriPacket(Layer2Tag&, uint32 swport, LAYER2FRAME* f);
	void deletePacket(void);

	int packetType(void);

	Layer2Tag& getPrivateTag(void) {
		return priTag;
	};

	int getPriLen(void) {
		return stdDataLen+4;
	};

	int getStdLen(void) {
		return stdDataLen;
	};

	uint8* getStdStream(uint16* len) {
		*len = stdDataLen;
		return stdData;
	};

	uint32 getSourcePort(void) {
		return srcPort;
	};

	uint8* getSrcMAC(void) {
		return priTag.srcMAC;
	};
	uint8* getDesMAC(void) {
		return priTag.decMAC;
	};

	uint8 setRefenceCounter(uint8 cnt) {
	    refenceCounter = cnt;
	    return refenceCounter;
	};

    NMPort* getRcvNMPort(void) {
        return nmport;
    };
    void setRcvNMPort(NMPort* p) {
        nmport = p;
    };


    /*
     * function for ip layer
     */
    bool ifValid(void) {
        return ifARP() || ifUDP() || (packetType()==2);
    };

    uint8* getSrcIP(void) {
        if( ifARP() ) {
            return &stdData[arpsenderIP];
        }
        else if( ifUDP() ) {
            return &stdData[ipsrcIP];

        }
        return 0;
    };
    uint8* getDesIP(void) {
        if( ifARP() ) {
            return &stdData[arptagetIP];
        }
        else if( ifUDP() ) {
            return &stdData[ipdesIP];

        }
        return 0;

    };

    static void printTotalLifeTimeInfo(void);
    static void openTraceContinue(void) {
    	TRACE = -1;
    };
    static void closeTrace(void) {
    	TRACE = 0;
    };
    static void openTraceNLoop(uint16 x) {
    	TRACE = x;
    };
//    void recordProcessInfo(std::string& info) {
//    	processinfo = info;
//    };
//    void recordProcessInfo(const char* s) {
//    	std::string info(s);
//    	recordProcessInfo(info);
//    };
    void recordProcessInfo(int cnt) {
    	processLastCnt = cnt;
    };
    void printLivingInfo(void);
private:
    NMPort* nmport;
	uint32 srcPort;
	Layer2Tag priTag;
	uint8* stdData;
	uint16 stdDataLen;
	LAYER2FRAME* frame;
	uint8 refenceCounter;

	uint32 TicksOfbirth;
	uint32 TicksOfDeath;
	std::string processinfo;
	int processLastCnt;

    enum {
        ethType = 0x0c,
        arpsenderIP = 0x1c,
        arptagetIP = 0x26,
        ipsrcIP = 0x1a,
        ipdesIP = 0x1e,
    };
	bool ifARP(void) {
	    return (stdData[ethType] == 0x08) && (stdData[ethType+1] == 0x06);
	};
	bool ifUDP(void) {
        return (stdData[ethType] == 0x08) && (stdData[ethType+1] == 0x00);
	};

};

#endif /* PRIPACKET_H_ */
