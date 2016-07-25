/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CMibImpl.h
 ** Created by:				WangChangRong
 ** Created date:			2011-8-1
 ** Version:				1.0
 ** Descriptions:			
 **---------------------------------------------------------------------------------------------------------------------
 ** Modified by:				
 ** Modified date:			
 ** Version:					
 ** Descriptions:			
 **********************************************************************************************************************/

#ifndef CMIBIMPL_H_
#define CMIBIMPL_H_
#include "EZ_types.h"
#include "CMibEntry.h"

class CSnmpMib;

class CMibImpl:public CMibEntry
{
private:
	CSnmpMib* objpSnmpMib;					//�ϼ�SNMP��MIB����ָ��
public:
	CMibImpl();
	virtual ~CMibImpl();

	uint32 InitialClassObject(CSnmpMib* objpIn);    //  ��ʼ�������
};

#endif /* CMIBIMPL_H_ */
