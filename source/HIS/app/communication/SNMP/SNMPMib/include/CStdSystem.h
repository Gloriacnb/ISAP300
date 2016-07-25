/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CStdSystem.h
 ** Created by:				WangChangRong
 ** Created date:			2011-8-8
 ** Version:				1.0
 ** Descriptions:			
 **---------------------------------------------------------------------------------------------------------------------
 ** Modified by:				
 ** Modified date:			
 ** Version:					
 ** Descriptions:			
 **********************************************************************************************************************/

#ifndef CSTDSYSTEM_H_
#define CSTDSYSTEM_H_
#include "CMibColumn.h"
#include "EZ_types.h"

class CStdSystem : public CMibColumn
{
public:
	CStdSystem();
	virtual ~CStdSystem();
	//��ʼ����׼���ж���
	uint32 InitialGroup();

	//Get����Ӧ����
	virtual uint32 GetResponse(CSnmpCell* objpCell);
	//Set����Ӧ����
	virtual uint32 SetResponse(CSnmpCell* objpCell);
	//GetNext���÷���1����õ�һ�������OID
	virtual uint32 GetFirstValidOID(CSnmpCell* objpCell);
	//GetNext���÷���2�������һ�������OID
	virtual uint32 GetNextValidOID(CSnmpCell* objpCell);
};

#endif /* CSTDSYSTEM_H_ */
