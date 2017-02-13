/*
 * IpPortMap.h
 *
 *  Created on: 2016年11月29日
 *      Author: Administrator
 */

#ifndef SOURCE_HIS_APP_COMMUNICATION_SOFTSWITCH_IPPORTMAP_H_
#define SOURCE_HIS_APP_COMMUNICATION_SOFTSWITCH_IPPORTMAP_H_

#include <map>
#include <string.h>
#include "EZ_types.h"
#include "PortRecord.h"
#include <string>

class PriPacket;
class IpPortMap {
	const static uint16 FLAG_FIX_IP = 0xffff;
public:
    const static uint16 LiveSecs = 3600; //ip port记录表生存时间为1小时

    IpPortMap();
    virtual ~IpPortMap();

    void learnAddress(PriPacket&);

    int findOutputPort(PriPacket&);

    void aging(void);

    void Print(void);

    std::string listIpsAtPort(uint8 psn);
    bool addFixIP(uint8* ip,uint8 swport);
    bool deleteAnIP(uint8* ip);
private:
    class IPAddr {
    public:
        IPAddr() {};
        IPAddr(uint8* d) {
            memcpy( ip, d, 4);
        };
        ~IPAddr() {};
        uint8 ip[4];

        bool operator < (const IPAddr& m) const {
            for( int i = 0; i < 4; i++ ) {
                if( ip[i] != m.ip[i] ) {
                    return ip[i] < m.ip[i];
                }
            }
            return false;
        };

    };

    std::map<IPAddr, PortRecord> ipPortTable;

};
#endif /* SOURCE_HIS_APP_COMMUNICATION_SOFTSWITCH_IPPORTMAP_H_ */
