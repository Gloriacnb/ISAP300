/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CSnmpCell.h
 ** Created by:				WangChangRong
 ** Created date:			2011-8-2
 ** Version:				1.0
 ** Descriptions:			
 **---------------------------------------------------------------------------------------------------------------------
 ** Modified by:				
 ** Modified date:			
 ** Version:					
 ** Descriptions:			
 **********************************************************************************************************************/

#ifndef CSNMPCELL_H_
#define CSNMPCELL_H_
#include "EZ_types.h"
#include "CThreadTask.h"
#include "CSnmpVariable.h"
#include "CSnmpVariableBinding.h"
class CSnmpCommand;
class CSnmpCell: public CThreadTask
{
private:
	CSnmpCommand* objpCommand;						//������SNMP�������
	CSnmpVariableBinding objRequestVB;				//�����SNMP VB����
	CSnmpVariable objCurrentVariable;				//��ǰ���ڴ����OID����
	CSnmpVariableBinding objResponseVB;				//��ӦVB����
	uint8 ucErrorStatus;							//���״̬
public:
	CSnmpCell(CSnmpCommand* objpIn);
	virtual ~CSnmpCell();
	//���SNMP�������
	CSnmpCommand* GetSnmpCommand();
	//�������VB����
	CSnmpVariableBinding* GetRequestVB();
	//������ڴ����VB����
	CSnmpVariable* GetCurrentVariable();
	//�����ӦVB����
	CSnmpVariableBinding* GetResponseVB();
	//��ò��״̬
	uint8 GetErrorStatus();
	//���ò��״̬
	uint32 SetErrorStatus(uint8 ucError);
	//����MIB�����Ͳ�ѯ����ȡ��Ӧ
	uint32 StartMibSearch();
	//��������
	uint32 SetRequest(CSnmpVariable* objpOID,CSnmpVariable* objpValue);
	//������Ӧ
	uint32 SetResponse(CSnmpVariable* objpOID,CSnmpVariable* objpValue);
	//���ô�����Ӧ
	uint32 SetErrorResponse(CSnmpVariable* objpOID,uint8 ucError);
	//������,���������е���
//	virtual uint32 Notify(uint32 uiNotifyType,CThreadTask* objpSourceTask);
	//�����¡
	uint32 Clone(CSnmpCell* objpCell);

};

#endif /* CSNMPCELL_H_ */
