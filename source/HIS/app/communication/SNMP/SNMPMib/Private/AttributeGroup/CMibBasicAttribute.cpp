/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CMibBasicAttribute.cpp
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

#include "CMibBasicAttribute.h"
#include "CErrorValueDefine.h"
#include "CSnmpCell.h"
#include "CCommonDefine.h"
//#include "CClassPointer.h"
//#include "CMain.h"
//#include "DeviceLocal.h"
//#include "Task_define.h"
#include "DeviceAttribute.h"
#include <string.h>
#include "DeviceComponent.h"
#include <stdio.h>

//extern CClassPointer objGlbClassPointer;
/**********************************************************************************************************************
** Function name:			CMibBasicAttribute
** Descriptions:			�๹�캯��
** input parameters:		None
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2011-08-24
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CMibBasicAttribute::CMibBasicAttribute()
{
}
/**********************************************************************************************************************
** Function name:			~CMibBasicAttribute
** Descriptions:			����������
** input parameters:		None
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2011-08-24
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CMibBasicAttribute::~CMibBasicAttribute()
{
}
/**********************************************************************************************************************
** Function name:			InitialGroup
** Descriptions:			��ʼ����׼���ж���
** input parameters:		None
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2011-8-24
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CMibBasicAttribute::InitialGroup()
{
	//��ʼ��
	uint32 uiInOID[] = {1,3,6,1,4,1,25449,1,1,9,1,1,1};
	CSnmpVariable objMib(uiInOID,11);
	InitialNode(&objMib);

	{//��ʼ�������Ա�
		for( uint32 i=0;i<12;i++ ){//10����Ա
			CMibBasicAttributeColumn* objpColumn = new CMibBasicAttributeColumn();
			uiInOID[12] = i+1;
			CSnmpVariable objMib(uiInOID,13);
			objpColumn->InitialNode(&objMib);
			InsertDownLinkNode(objpColumn);
		}
	}

	return CErrorValueDefine::uiConstReturnSuccess;


}

/**********************************************************************************************************************
** Function name:			CMibBasicAttributeColumn
** Descriptions:			�๹�캯��
** input parameters:		None
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2011-08-24
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/

CMibBasicAttributeColumn::CMibBasicAttributeColumn()
{
}
/**********************************************************************************************************************
** Function name:			~CMibElementColumn
** Descriptions:			����������
** input parameters:		None
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2011-08-24
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/

CMibBasicAttributeColumn::~CMibBasicAttributeColumn()
{
}


/**********************************************************************************************************************
** Function name:			GetResponse
** Descriptions:			Get����Ӧ����
** input parameters:		objpCell��		SNMP�������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** 							��ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							���ڴ��?��CErrorValueDefine::uiConstReturnProcessing;
** Created by:				WangChangRong
** Created date:			2011-08-24
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CMibBasicAttributeColumn::GetResponse(CSnmpCell* objpCell)
{
	if( objpCell == NULL ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	CSnmpVariable* objpOID = objpCell->GetCurrentVariable();
	uint32 uiOIDLen = 0;
	uint32* uipInOID = objpOID->GetOIDValue(&uiOIDLen);
	if( uiOIDLen != 14 || uipInOID[13] != CCommonDefine::uiConstDeviceIndex ){//���Ⱥ�oidУ��
		return CErrorValueDefine::uiConstReturnFailed;
	}
 	switch( uipInOID[12] ){//����
 		case 1://eciDPBasicDeviceName
 		{
 		    std::string s;
 			DeviceComponent::getDeviceAttribute().getBaseAttribute(ln_name, s);
 			uint8* ucBuf = (uint8*)s.c_str();
 			uint32 uiLen  = s.size();
 			CSnmpVariable objResponse(ucBuf,uiLen,CSnmpConstDefine::ucConstOctetString);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 2://eciDPBasicUser
 		{
            std::string s;
            DeviceComponent::getDeviceAttribute().getBaseAttribute(ln_user, s);
            uint8* ucBuf = (uint8*)s.c_str();
            uint32 uiLen  = s.size();
 			CSnmpVariable objResponse(ucBuf,uiLen,CSnmpConstDefine::ucConstOctetString);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 3://eciDPBasicDesc
 		{
            std::string s;
            DeviceComponent::getDeviceAttribute().getBaseAttribute(ln_description, s);
            uint8* ucBuf = (uint8*)s.c_str();
            uint32 uiLen  = s.size();
 			CSnmpVariable objResponse(ucBuf,uiLen,CSnmpConstDefine::ucConstOctetString);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 4://eciDPBasicOther
 		{
            std::string s;
            DeviceComponent::getDeviceAttribute().getBaseAttribute(ln_other, s);
            uint8* ucBuf = (uint8*)s.c_str();
            uint32 uiLen  = s.size();
 			CSnmpVariable objResponse(ucBuf,uiLen,CSnmpConstDefine::ucConstOctetString);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 5://eciDPBasicContact
 		{
            std::string s;
            DeviceComponent::getDeviceAttribute().getBaseAttribute(ln_contact, s);
            uint8* ucBuf = (uint8*)s.c_str();
            uint32 uiLen  = s.size();
 			CSnmpVariable objResponse(ucBuf,uiLen,CSnmpConstDefine::ucConstOctetString);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 6://eciDPBasicLocation
 		{
            std::string s;
            DeviceComponent::getDeviceAttribute().getBaseAttribute(ln_location, s);
            uint8* ucBuf = (uint8*)s.c_str();
            uint32 uiLen  = s.size();
 			CSnmpVariable objResponse(ucBuf,uiLen,CSnmpConstDefine::ucConstOctetString);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 7://eciDPBasicSoftwareReset
 		case 8://eciDPBasicHardwareReset
 		case 9://eciDPBasicDeviceReset
 			break;
 		case 10://eciDPBasicDefaultSetting
 		{
 			CSnmpVariable objResponse(0);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
//  		break;
 		case 11: {
            std::string s;
            DeviceComponent::getDeviceAttribute().getBaseAttribute(ln_factorydate, s);
 			CSnmpVariable objResponse((uint8*)s.c_str(),s.size(),CSnmpConstDefine::ucConstOctetString);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 12: {
            std::string s;
            DeviceComponent::getDeviceAttribute().getBaseAttribute(ln_serialnumber, s);
            int sn;
            sscanf(s.c_str(), "%d", &sn);
 			CSnmpVariable objResponse(sn);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;

 		}
 	}
	return CErrorValueDefine::uiConstReturnFailed;
}


/**********************************************************************************************************************
** Function name:			SetResponse
** Descriptions:			Set����Ӧ����
** input parameters:		objpCell��		SNMP�������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** 							��ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							���ڴ��?��CErrorValueDefine::uiConstReturnProcessing;
** Created by:				WangChangRong
** Created date:			2011-08-24
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CMibBasicAttributeColumn::SetResponse(CSnmpCell* objpCell)
{
	if( objpCell == NULL ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	CSnmpVariable* objpOID = objpCell->GetCurrentVariable();
	uint32 uiOIDLen = 0;
	uint32* uipInOID = objpOID->GetOIDValue(&uiOIDLen);
	if( uiOIDLen != 14 || uipInOID[13] != CCommonDefine::uiConstDeviceIndex ){//���Ⱥ�oidУ��
		return CErrorValueDefine::uiConstReturnFailed;
	}
 	switch( uipInOID[12] ){//����
 		case 1://eciDPBasicDeviceName
 		{
 			CSnmpVariable* objInvalue = objpCell->GetRequestVB()->GetValueObject();
 			if( objInvalue->GetSyntax() != CSnmpConstDefine::ucConstOctetString ){//���ʹ���
 				objpCell->SetErrorResponse(objpOID,CSnmpConstDefine::ucConstSnmpErrorWrongType);
 				return CErrorValueDefine::uiConstReturnSuccess;
 			}
 			uint32 uiTempLen = 0;
 			uint8* ucpTempBuf = objInvalue->GetStringValue(&uiTempLen);
 			std::string s((char*)ucpTempBuf,uiTempLen);
 			DeviceComponent::getDeviceAttribute().setBaseAttribute(ln_name, s);
 			CSnmpVariable objResponse(0x5a);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 2://eciDPBasicUser
 		{
 			CSnmpVariable* objInvalue = objpCell->GetRequestVB()->GetValueObject();
 			if( objInvalue->GetSyntax() != CSnmpConstDefine::ucConstOctetString ){//���ʹ���
 				objpCell->SetErrorResponse(objpOID,CSnmpConstDefine::ucConstSnmpErrorWrongType);
 				return CErrorValueDefine::uiConstReturnSuccess;
 			}
 			uint32 uiTempLen = 0;
 			uint8* ucpTempBuf = objInvalue->GetStringValue(&uiTempLen);
            std::string s((char*)ucpTempBuf,uiTempLen);
            DeviceComponent::getDeviceAttribute().setBaseAttribute(ln_user, s);
            CSnmpVariable objResponse(0x5a);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 3://eciDPBasicDesc
 		{
 			CSnmpVariable* objInvalue = objpCell->GetRequestVB()->GetValueObject();
 			if( objInvalue->GetSyntax() != CSnmpConstDefine::ucConstOctetString ){//���ʹ���
 				objpCell->SetErrorResponse(objpOID,CSnmpConstDefine::ucConstSnmpErrorWrongType);
 				return CErrorValueDefine::uiConstReturnSuccess;
 			}
 			uint32 uiTempLen = 0;
 			uint8* ucpTempBuf = objInvalue->GetStringValue(&uiTempLen);
            std::string s((char*)ucpTempBuf,uiTempLen);
            DeviceComponent::getDeviceAttribute().setBaseAttribute(ln_description, s);
            CSnmpVariable objResponse(0x5a);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 4://eciDPBasicOther
 		{
 			CSnmpVariable* objInvalue = objpCell->GetRequestVB()->GetValueObject();
 			if( objInvalue->GetSyntax() != CSnmpConstDefine::ucConstOctetString ){//���ʹ���
 				objpCell->SetErrorResponse(objpOID,CSnmpConstDefine::ucConstSnmpErrorWrongType);
 				return CErrorValueDefine::uiConstReturnSuccess;
 			}
 			uint32 uiTempLen = 0;
 			uint8* ucpTempBuf = objInvalue->GetStringValue(&uiTempLen);
            std::string s((char*)ucpTempBuf,uiTempLen);
            DeviceComponent::getDeviceAttribute().setBaseAttribute(ln_other, s);
            CSnmpVariable objResponse(0x5a);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 5://eciDPBasicContact
 		{
 			CSnmpVariable* objInvalue = objpCell->GetRequestVB()->GetValueObject();
 			if( objInvalue->GetSyntax() != CSnmpConstDefine::ucConstOctetString ){//���ʹ���
 				objpCell->SetErrorResponse(objpOID,CSnmpConstDefine::ucConstSnmpErrorWrongType);
 				return CErrorValueDefine::uiConstReturnSuccess;
 			}
 			uint32 uiTempLen = 0;
 			uint8* ucpTempBuf = objInvalue->GetStringValue(&uiTempLen);
            std::string s((char*)ucpTempBuf,uiTempLen);
            DeviceComponent::getDeviceAttribute().setBaseAttribute(ln_contact, s);
            CSnmpVariable objResponse(0x5a);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 6://eciDPBasicLocation
 		{
 			CSnmpVariable* objInvalue = objpCell->GetRequestVB()->GetValueObject();
 			if( objInvalue->GetSyntax() != CSnmpConstDefine::ucConstOctetString ){//���ʹ���
 				objpCell->SetErrorResponse(objpOID,CSnmpConstDefine::ucConstSnmpErrorWrongType);
 				return CErrorValueDefine::uiConstReturnSuccess;
 			}
 			uint32 uiTempLen = 0;
 			uint8* ucpTempBuf = objInvalue->GetStringValue(&uiTempLen);
            std::string s((char*)ucpTempBuf,uiTempLen);
            DeviceComponent::getDeviceAttribute().setBaseAttribute(ln_location, s);
            CSnmpVariable objResponse(0x5a);
 			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
 		}
 		case 9://eciDPBasicDeviceReset
 			break;
 		case 7: {//eciDPBasicSoftwareReset
 			CSnmpVariable objResponse(0x5A);
			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
		}

 		case 8: {//eciDPBasicHardwareReset
 			CSnmpVariable objResponse(0x5A);
			objpCell->SetResponse(objpOID,&objResponse);
 			return CErrorValueDefine::uiConstReturnSuccess;
		}
 		case 10://eciDPBasicDefaultSetting
 			objpCell->SetErrorResponse(objpOID,CSnmpConstDefine::ucConstSnmpErrorNotWriteable);
 			return CErrorValueDefine::uiConstReturnSuccess;
//  		break;
 	}

	return CErrorValueDefine::uiConstReturnFailed;
}
/**********************************************************************************************************************
** Function name:			GetFirstValidOID
** Descriptions:			GetNext���÷���1����õ�һ�������OID
** input parameters:		objpCell��		SNMP�������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** 							��ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							���ڴ��?��CErrorValueDefine::uiConstReturnProcessing;
** Created by:				WangChangRong
** Created date:			2011-08-24
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CMibBasicAttributeColumn::GetFirstValidOID(CSnmpCell* objpCell)
{
	if( objpCell == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	CSnmpVariable* objpSelf = GetNodeOIDVariable();
	if( objpSelf == NULL ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uint32 uiSelfOIDLength = 0;
	uint32* uipOID = objpSelf->GetOIDValue(&uiSelfOIDLength);


	uint32 uiInOID[] = {1,3,6,1,4,1,25449,1,1,9,1,1,1,0};
	uiInOID[12] = uipOID[12];
	uiInOID[13] = CCommonDefine::uiConstDeviceIndex;
	CSnmpVariable objFirst(uiInOID,14);
	objpCell->GetCurrentVariable()->Clone(&objFirst);

	return CErrorValueDefine::uiConstReturnSuccess;
}
/**********************************************************************************************************************
** Function name:			GetNextValidOID
** Descriptions:			GetNext���÷���2�������һ�������OID
** input parameters:		objpCell��		SNMP�������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** 							��ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							���ڴ��?��CErrorValueDefine::uiConstReturnProcessing;
** Created by:				WangChangRong
** Created date:			2011-08-24
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CMibBasicAttributeColumn::GetNextValidOID(CSnmpCell* objpCell)
{
	if( objpCell == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	CSnmpVariable objTemp;
	objTemp.Clone(objpCell->GetCurrentVariable());
	uint32 uiBaseLen = 0;
	uint32* uipBaseOID = objTemp.GetOIDValue(&uiBaseLen);
	for(uint32 i=uiBaseLen;i<14;i++){
		uipBaseOID[i] = 0;
	}
	if( uipBaseOID[13] < CCommonDefine::uiConstDeviceIndex){
		uipBaseOID[13] = CCommonDefine::uiConstDeviceIndex;
		CSnmpVariable objNext(uipBaseOID,14);
		objpCell->GetCurrentVariable()->Clone(&objNext);
		return CErrorValueDefine::uiConstReturnSuccess;
	}
	return CErrorValueDefine::uiConstReturnFailed;

}
