/*
 * Card8EOSFXN.h
 *
 *  Created on: 2014��12��23��
 *      Author: Administrator
 */

#ifndef CARD8EOSFXN_H_
#define CARD8EOSFXN_H_

#include "Card8EOSN.h"

class Card8EOSFXN : public Card8EOSN {
public:
    Card8EOSFXN(std::string& name, CBaseSlot* slot);
    virtual ~Card8EOSFXN();

    virtual int GetCartTypeID();
};

#endif /* CARD8EOSFXN_H_ */
