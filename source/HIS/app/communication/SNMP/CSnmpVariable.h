/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CSnmpVariable.h
 ** Created by:				WangChangRong
 ** Created date:			2011-8-3
 ** Version:				1.0
 ** Descriptions:			
 **---------------------------------------------------------------------------------------------------------------------
 ** Modified by:				
 ** Modified date:			
 ** Version:					
 ** Descriptions:			
 **********************************************************************************************************************/

#ifndef CSNMPVARIABLE_H_
#define CSNMPVARIABLE_H_
#include "EZ_types.h"
#include "CSnmpConstDefine.h"
class CSnmpVariable
{
private:
	uint32 uiIntegerValue;										//32λ����ֵ����
	uint32 uiIntegerValueLow;									//64λ����ֵ����ĵ�32λ
	uint32 uiLength;											//String��OID����Ч����
	uint8* ucpChangeString;										//����String���Bufferָ��
	uint32 uiOIDValue[CSnmpConstDefine::uiConstOidMaxSize];		//���OID��Buffer
	uint8 ucString[CSnmpConstDefine::uiConstStringMaxSize];		//���String��Buffer
	uint8 ucSyntax;												//SNMP��������

public:
	CSnmpVariable();
	CSnmpVariable(CSnmpVariable& objVariable);									//�������캯��
	CSnmpVariable(int iInteger32);												//���͹��캯��
	CSnmpVariable(uint32 uiUnsignedInteger32,uint8 ucType);						//�޷������͹��캯��
	CSnmpVariable(uint32 uiInteger64High,uint32 uiInteger64Low,uint8 ucType);	//64λ���͹��캯��
	CSnmpVariable(uint8* ucpBuffer,uint32 uiBufferLength,uint8 ucType);			//�ַ������캯��
	CSnmpVariable(uint32* uipOID,uint32 uiOIDLength);							//OID���캯��
	virtual ~CSnmpVariable();


	//����SNMP��������
	uint32 Clone(CSnmpVariable* objpVariable);

	//������Buffer�н��������
	uint32 Decode(uint8* ucpBuffer,uint32 uiBufferLength,uint32* uipOffset);
	//�Ӷ����б������ݵ�Buffer
	uint32 Encode(uint8* ucpBuffer,uint32 uiValidLength,uint32* uipOffset);

	//��ñ�������
	uint8 GetSyntax();
	//���ñ�������
	uint32 SetSyntax(uint8 ucType);
	//���32λ����ֵ
	uint32 GetInteger32Value();
	//���64λ����ֵ
	uint32 GetInteger64Value(uint32* uipValueHigh,uint32* uipValueLow);
	//����ַ���ֵ
	uint8* GetStringValue(uint32* uipLength);
	//���OIDֵ
	uint32* GetOIDValue(uint32* uipLength);

	//SNMP�����ȽϷ���
	int CompareTo(CSnmpVariable* objpInVariable);
	//OID�Ƿ��Ը���OID��ʼ
	uint32 StartWith(CSnmpVariable* objpInVariable);


};

#endif /* CSNMPVARIABLE_H_ */
