/*
 * Resource.cpp
 *
 *  Created on: 2014��11��26��
 *      Author: Administrator
 */

#include "Resource.h"
#include "UID.h"

Resource::Resource(uint32 uid) : itsUID(uid) {
    name = UID::getNameByUID(uid);

}

Resource::~Resource() {
    // TODO Auto-generated destructor stub
}

