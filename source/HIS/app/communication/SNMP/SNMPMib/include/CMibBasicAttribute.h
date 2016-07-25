/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CMibBasicAttribute.h
 ** Created by:				WangChangRong
 ** Created date:			2011-8-24
 ** Version:				1.0
 ** Descriptions:			
 **---------------------------------------------------------------------------------------------------------------------
 ** Modified by:				
 ** Modified date:			
 ** Version:					
 ** Descriptions:			
 **********************************************************************************************************************/

#ifndef CMIBBASICATTRIBUTE_H_
#define CMIBBASICATTRIBUTE_H_

#include "CMibEntry.h"
#include "CMibColumn.h"

class CMibBasicAttribute: public CMibEntry
{
public:
	CMibBasicAttribute();
	virtual ~CMibBasicAttribute();
	//��ʼ����׼���ж���
	virtual uint32 InitialGroup();
};

class CMibBasicAttributeColumn: public CMibColumn
{
public:
	CMibBasicAttributeColumn();
	virtual ~CMibBasicAttributeColumn();

	//Get����Ӧ����
	virtual uint32 GetResponse(CSnmpCell* objpCell);
	//Set����Ӧ����
	virtual uint32 SetResponse(CSnmpCell* objpCell);
	//GetNext���÷���1����õ�һ�������OID
	virtual uint32 GetFirstValidOID(CSnmpCell* objpCell);
	//GetNext���÷���2�������һ�������OID
	virtual uint32 GetNextValidOID(CSnmpCell* objpCell);
};

#endif /* CMIBBASICATTRIBUTE_H_ */
