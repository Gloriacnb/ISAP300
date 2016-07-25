/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CSnmpMib.h
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

#ifndef CSNMPMIB_H_
#define CSNMPMIB_H_
#include "EZ_types.h"
#include "CSnmpDataInOut.h"
#include "CMibImpl.h"
class CCommunication;
class CSnmpMib
{
private:
//	CCommunication* objpCommunication;				//�ϼ�ͨ�Ŷ���ָ��
	CSnmpDataInOut objDataInOut;					//SNMP���������������
	CMibImpl objMibImpl;							//MIB��ʵ��
public:
	CSnmpMib();
	virtual ~CSnmpMib();

	uint32 InitialClassObject(void/*CCommunication* objpIn*/);    //  ��ʼ�������
	//����ϼ�ͨ�Ŷ���ָ��
//	CCommunication* GetCommunication();
	//���SNMP���������������
	CSnmpDataInOut* GetSnmpDataInOut();
	//���MIB��ʵ��
	CMibImpl* GetMibImpl();

};

#endif /* CSNMPMIB_H_ */
