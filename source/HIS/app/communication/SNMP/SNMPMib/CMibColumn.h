/****************************************Copyright (c)*****************************************************************
 **                           
 **                               Easy Broadband Technology Co., Ltd.                       
 **                         
 **                                  http://www.ebbnetwork.com  
 **
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** File name:			CMibColumn.h
 ** File path:			/EMUX300/Src/Main_Code/Communication/SNMPMib/CMibColumn.h
 ** Created by:          Shqp
 ** Created date:        2012-3-23
 ** Version:             V0.1
 ** Descriptions:        
 **
 **---------------------------------------------------------------------------------------------------------------------
 ** Modified by:         
 ** Modified date:       
 ** Version:				
 ** Descriptions:        
 **
 ** Rechecked by:        
 ***********************************************************************************************************************/

#ifndef CMIBCOLUMN_H_
#define CMIBCOLUMN_H_

#include "CMibNodeObject.h"
/*
 *
 */
class CMibColumn : public CMibNodeObject {
public:
	CMibColumn();
	CMibColumn(uint32* oid, uint32 oidLen);
	virtual ~CMibColumn();

	virtual uint32 GetResponse(CSnmpCell* objpCell) = 0;
	//Set����Ӧ����
	virtual uint32 SetResponse(CSnmpCell* objpCell) = 0;
	//GetNext���÷���1����õ�һ�������OID
	virtual uint32 GetFirstValidOID(CSnmpCell* objpCell) = 0;
	//GetNext���÷���2�������һ�������OID
	virtual uint32 GetNextValidOID(CSnmpCell* objpCell) = 0;
};

#endif /* CMIBCOLUMN_H_ */
