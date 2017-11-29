/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CSnmpCommand.cpp
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

#include "CSnmpCommand.h"
#include "CErrorValueDefine.h"
#include "CSnmpCell.h"
#include "CClassPointer.h"
#include "CSnmpStatic.h"
#include "CSnmpDataInOut.h"

#include <string.h>
#include <rtl.h>
#include "TerminalUart.h"
#include "CommunicationModule.h"

//#include "CUDPInterface.h"
//extern CClassPointer objGlbClassPointer;
/**********************************************************************************************************************
** Function name:			CSnmpCommand
** Descriptions:			�๹�캯��
** input parameters:		objpIn��		SNMP��ݽ��ն���
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2011-08-01
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/

CSnmpCommand::CSnmpCommand(CSnmpDataInOut* objpIn)
{
	objpSnmpDataInOut = objpIn;				//SNMP��ݽ��ն���
//	objpUDPInterface = NULL;				//UDP��ݰ���Դ����
	uiRequestID = 0;								//SNMP����ID
	ucCurrentVersion = CSnmpConstDefine::ucConstVersion2c;							//�汾
	ucCurrentType = CSnmpConstDefine::ucConstGet;							//SNMP��������
	uiCurrentErrorStatus = 0;							//SNMP���״̬
	uiCurrentErrorIndex = 0;							//SNMP�������
}
/**********************************************************************************************************************
** Function name:			~CSnmpCommand
** Descriptions:			����������
** input parameters:		None
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2011-08-01
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/

CSnmpCommand::~CSnmpCommand()
{
}


/**********************************************************************************************************************
** Function name:			GetCurrentRequestType
** Descriptions:			���SNMP��������
** input parameters:		None
** output parameters:		None
** Returned value:			SNMP��������
** Created by:				WangChangRong
** Created date:			2011-08-04
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::GetCurrentRequestType()
{
	return ucCurrentType;
}

/**********************************************************************************************************************
** Function name:			SetCurrentRequestType
** Descriptions:			����SNMP��������
** input parameters:		ucType:	SNMP��������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::SetCurrentRequestType(uint8 ucType)
{
	ucCurrentType = ucType;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			GetCurrentRequestID
** Descriptions:			���SNMP����ID
** input parameters:		None
** output parameters:		None
** Returned value:			SNMP����ID
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::GetCurrentRequestID()
{
	return uiRequestID;
}

/**********************************************************************************************************************
** Function name:			SetCurrentRequestID
** Descriptions:			����SNMP����ID
** input parameters:		uiID:	SNMP����ID
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::SetCurrentRequestID(uint32 uiID)
{
	uiRequestID = uiID;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			GetCurrentRequestVersion
** Descriptions:			���SNMP�汾
** input parameters:		None
** output parameters:		None
** Returned value:			SNMP�汾
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::GetCurrentRequestVersion()
{
	return ucCurrentVersion;
}

/**********************************************************************************************************************
** Function name:			SetCurrentRequestVersion
** Descriptions:			����SNMP�汾
** input parameters:		ucVersion:	SNMP�汾
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::SetCurrentRequestVersion(uint8 ucVersion)
{
	ucCurrentVersion = ucVersion;
	return CErrorValueDefine::uiConstReturnSuccess;
}



/**********************************************************************************************************************
** Function name:			GetCurrentRequestErrorStatus
** Descriptions:			���SNMP���״̬
** input parameters:		None
** output parameters:		None
** Returned value:			SNMP���״̬
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::GetCurrentRequestErrorStatus()
{
	return uiCurrentErrorStatus;
}

/**********************************************************************************************************************
** Function name:			SetCurrentRequestErrorStatus
** Descriptions:			����SNMP���״̬
** input parameters:		ucVersion:	SNMP���״̬
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::SetCurrentRequestErrorStatus(uint32 uiStatus)
{
	uiCurrentErrorStatus = uiStatus;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			GetCurrentRequestErrorIndex
** Descriptions:			���SNMP�������
** input parameters:		None
** output parameters:		None
** Returned value:			SNMP�������
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::GetCurrentRequestErrorIndex()
{
	return uiCurrentErrorIndex;
}

/**********************************************************************************************************************
** Function name:			SetCurrentRequestErrorIndex
** Descriptions:			����SNMP�������
** input parameters:		ucVersion:	SNMP�������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::SetCurrentRequestErrorIndex(uint32 uiIndex)
{
	uiCurrentErrorIndex = uiIndex;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			GetUDPData
** Descriptions:			���UDP��ݰ����
** input parameters:		None
** output parameters:		None
** Returned value:			UDP��ݰ����
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CUDPData* CSnmpCommand::GetUDPData()
{
	return &objData;
}

/**********************************************************************************************************************
** Function name:			SetUDPData
** Descriptions:			����UDP��ݰ����
** input parameters:		objpData:	UDP��ݰ����
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::SetUDPData(CUDPData* objpData)
{
	if( objpData == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	return  objData.Clone(objpData);
}


/**********************************************************************************************************************
** Function name:			GetUDPInterface
** Descriptions:			���UDP��ݰ���Դ����
** input parameters:		None
** output parameters:		None
** Returned value:			UDP��ݰ���Դ����
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
//CUDPInterface* CSnmpCommand::GetUDPInterface()
//{
//	return objpUDPInterface;
//}

/**********************************************************************************************************************
** Function name:			SetUDPInterface
** Descriptions:			����UDP��ݰ���Դ����
** input parameters:		objpData:	UDP��ݰ���Դ����
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
//uint32 CSnmpCommand::SetUDPInterface(CUDPInterface* objpData)
//{
//	objpUDPInterface = objpData;
//	return  CErrorValueDefine::uiConstReturnSuccess;
//}


/**********************************************************************************************************************
** Function name:			InitialObject
** Descriptions:			��ʼ������
** input parameters:		objpIn��			UDP��ݰ���Դ����
** 							objpData��				UDP��ݰ����
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
/*uint32 CSnmpCommand::InitialObject(CUDPInterface* objpIn,CUDPData* objpData)
{
	if( objpData == NULL || objpIn == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	objpUDPInterface = objpIn;
	if( objData.Clone(objpData) != CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uint32 uiDataLength = 0;
	uint8* ucpBuffer = objData.GetData(&uiDataLength);
	if( ucpBuffer == NULL || uiDataLength == 0 ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//����
	if( Decode(ucpBuffer,uiDataLength) != CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	if( GetCurrentRequestType() != CSnmpConstDefine::ucConstGet
		&& GetCurrentRequestType() != CSnmpConstDefine::ucConstSet
		&& GetCurrentRequestType() != CSnmpConstDefine::ucConstGetNext ){
		return CErrorValueDefine::uiConstReturnFailed;
	}

	//ע������
	CTaskScheduling* objpTaskScheduling = objGlbClassPointer.GetTaskSchedulingPointer();
	if( objpTaskScheduling == NULL ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	CSnmpCell* objpCell = (CSnmpCell*)GetFirstDownLink();
	while(objpCell!=NULL){
		objpTaskScheduling->RegisterTask((COneTask*)objpCell,0);
		objpCell = (CSnmpCell*)GetNextDownLink((CEventTask*)objpCell);
	}
	//������ѯMIB��
	objpCell = (CSnmpCell*)GetFirstDownLink();
	while(objpCell!=NULL){
		objpCell->StartMibSearch();
		objpCell = (CSnmpCell*)GetNextDownLink(objpCell);
	}
	return CErrorValueDefine::uiConstReturnSuccess;
}
*/
/**********************************************************************************************************************
** Function name:			Decode
** Descriptions:			�����Buffer�н���
** input parameters:		ucpData��			UDP��ݾ�����Buffer
** 							uiLength��			UDP��ݾ�����Buffer����
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::Decode(uint8* ucpData,uint32 uiLength)
{
	if( ucpData == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	if( ucpData[0] != 0x30 ){//����SNMPЭ��ͷ
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uint32 uiOffset = 0;
	uint32 uiValue = 0;
	//�����ݰ��
	if( CSnmpStatic::DecodeLengthSection( &ucpData[1],uiLength-1,&uiValue,&uiOffset)
		!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//���Ȳ���ȷ
	if( uiValue == 0 || uiValue != (uiLength-1-uiOffset)){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uint32 uiPoint = uiOffset+1;//����λ�ü���
	//��ȡ�汾
	if( CSnmpStatic::DecodeInteger32( &ucpData[uiPoint],uiLength-uiPoint,&uiValue,&uiOffset)
		!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//ֻ����v1��v2c��snmp��ݰ�
	if( uiValue == CSnmpConstDefine::ucConstVersion1 || uiValue == CSnmpConstDefine::ucConstVersion2c ){
		ucCurrentVersion = uiValue;
	}
	else{
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiPoint += uiOffset;
	//�����ж�
	if( uiPoint >= uiLength ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//�ж�Community�ֶΣ��˴�Ĭ���ж��Ƿ���public
	{
		if( ucpData[uiPoint] != CSnmpConstDefine::ucConstOctetString 	//octetstring ����
		 && ucpData[uiPoint+1] != 6										//����
		 && ucpData[uiPoint+2] != 'p'										//p
		 && ucpData[uiPoint+3] != 'u'										//u
		 && ucpData[uiPoint+4] != 'b'										//b
		 && ucpData[uiPoint+5] != 'l'										//l
		 && ucpData[uiPoint+6] != 'i'										//i
		 && ucpData[uiPoint+7] != 'c' ){									//c
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiPoint += 8;
	}
	//�����ж�
	if( uiPoint >= uiLength ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//SNMP���������ж�
	if( ucpData[uiPoint] == CSnmpConstDefine::ucConstGet
	 || ucpData[uiPoint] == CSnmpConstDefine::ucConstSet
	 || ucpData[uiPoint] == CSnmpConstDefine::ucConstGetNext
	 || ucpData[uiPoint] == CSnmpConstDefine::ucConstResponse
	 || ucpData[uiPoint] == CSnmpConstDefine::ucConstTrap ){
		ucCurrentType = ucpData[uiPoint];
	}
	else{
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiPoint ++;
	//�����ж�
	if( uiPoint >= uiLength ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//�����ݰ��
	if( CSnmpStatic::DecodeLengthSection( &ucpData[uiPoint],uiLength-uiPoint,&uiValue,&uiOffset)
		!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//���Ȳ���ȷ
	if( uiValue == 0 || uiValue != (uiLength-uiPoint-uiOffset)){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiPoint += uiOffset;
	//�����ж�
	if( uiPoint >= uiLength ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//����ID
	if( CSnmpStatic::DecodeInteger32( &ucpData[uiPoint],uiLength-uiPoint,&uiValue,&uiOffset)
		!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiRequestID = uiValue;
	uiPoint += uiOffset;
	//�����ж�
	if( uiPoint >= uiLength ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//Error Status�ֶ�
	if( CSnmpStatic::DecodeInteger32( &ucpData[uiPoint],uiLength-uiPoint,&uiValue,&uiOffset)
		!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiCurrentErrorStatus = uiValue;
	uiPoint += uiOffset;
	//�����ж�
	if( uiPoint >= uiLength ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//Error Index�ֶ�
	if( CSnmpStatic::DecodeInteger32( &ucpData[uiPoint],uiLength-uiPoint,&uiValue,&uiOffset)
		!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiCurrentErrorIndex = uiValue;
	uiPoint += uiOffset;
	//�����ж�
	if( uiPoint >= uiLength ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//�������б�ͷ�ֶ�
	if( ucpData[uiPoint] != 0x30 ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiPoint ++;
	//�����ж�
	if( uiPoint >= uiLength ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	if( CSnmpStatic::DecodeLengthSection( &ucpData[uiPoint],uiLength-uiPoint,&uiValue,&uiOffset)
		!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//���Ȳ���ȷ
	if( uiValue == 0 || uiValue != (uiLength-uiPoint-uiOffset)){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiPoint += uiOffset;
	//�����ж�
	if( uiPoint >= uiLength ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//�����󶨵Ľ���
	while(true){
		//�������б�ͷ�ֶ�
		if( ucpData[uiPoint] != 0x30 ){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiPoint ++;
		//�����ж�
		if( uiPoint >= uiLength ){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		if( CSnmpStatic::DecodeLengthSection( &ucpData[uiPoint],uiLength-uiPoint,&uiValue,&uiOffset)
			!= CErrorValueDefine::uiConstReturnSuccess){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		//���Ȳ���ȷ
		if( uiValue == 0 ){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiPoint += uiOffset;
		//�����ж�
		if( uiPoint >= uiLength ){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		//OID�����жϣ�������OID
		if( ucpData[uiPoint] != CSnmpConstDefine::ucConstOID ){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		CSnmpCell* objpCell = new CSnmpCell(this);
		InsertDownLinkThreadTask(objpCell);
		if( objpCell->GetRequestVB()->GetOIDObject()->Decode(&ucpData[uiPoint],uiLength-uiPoint,&uiOffset)
			!= CErrorValueDefine::uiConstReturnSuccess){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiPoint += uiOffset;
		//�����ж�
		if( uiPoint >= uiLength ){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		if( objpCell->GetRequestVB()->GetValueObject()->Decode(&ucpData[uiPoint],uiLength-uiPoint,&uiOffset)
			!= CErrorValueDefine::uiConstReturnSuccess){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiPoint += uiOffset;
		//�����ж�,VB��������
		if( uiPoint >= uiLength ){
			return CErrorValueDefine::uiConstReturnSuccess;
		}
	}
// 	return CErrorValueDefine::uiConstReturnFailed;
}


/**********************************************************************************************************************
** Function name:			Encode
** Descriptions:			�Ӷ����б��뵽CSnmpDataInOut����Ӧ���Buffer
** input parameters:		None
** output parameters:		uipDataLength��	��ݵ���Ч����
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2011-08-04
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::Encode(uint32* uipDataLength)
{
	if( uipDataLength == NULL ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	CSnmpCell* objpCell = (CSnmpCell*)GetEndDownLink();
	if( objpCell == NULL ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uint32 uiMaxSize = CSnmpDataInOut::uiConstResponseBufferSize;
	uint8* ucpBuffer = objpSnmpDataInOut->GetResponseBuffer();
	uint32 uiLength = 0;
	uint32 uiOffsetTemp = 0;
	uint32 uiCellNumber = 0;
	uint8 ucErrorStatus = CSnmpConstDefine::ucConstSnmpErrorSuccess;
	uint32 uiErrorIndex = 0;
	while(objpCell!=NULL){
		CSnmpVariableBinding* objpVB = objpCell->GetResponseVB();
		if( CSnmpStatic::EncodeVariableBinding(ucpBuffer,uiMaxSize-uiLength,
				objpVB,&uiOffsetTemp)
			!= CErrorValueDefine::uiConstReturnSuccess){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiLength+=uiOffsetTemp;
		uint8 ucTemp =  objpCell->GetErrorStatus() ;
		if( ucTemp != CSnmpConstDefine::ucConstSnmpErrorSuccess ){
			ucErrorStatus = ucTemp;
			uiErrorIndex = uiCellNumber;
		}
		uiCellNumber++;
		objpCell = (CSnmpCell*)GetPrevDownLink(objpCell);
	}
	//���볤��
	if( CSnmpStatic::EncodeLengthSection(ucpBuffer,uiMaxSize-uiLength,uiLength,&uiOffsetTemp)
	!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiLength+=uiOffsetTemp;
	//�����ж�
	if( uiMaxSize <= uiLength){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//ͷ��0x30
	ucpBuffer[uiMaxSize-uiLength-1] = 0x30;
	uiLength ++;
	//�����ж�
	if( uiMaxSize <= uiLength){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	if( ucErrorStatus != CSnmpConstDefine::ucConstSnmpErrorSuccess ){
		uiErrorIndex = uiCellNumber - uiErrorIndex;
	}
	//����Error Index�ֶ�
	{
		CSnmpVariable objIntegerValue(uiErrorIndex);
		if( CSnmpStatic::EncodeInteger32(ucpBuffer,uiMaxSize-uiLength,&objIntegerValue,&uiOffsetTemp)
		!= CErrorValueDefine::uiConstReturnSuccess){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiLength+=uiOffsetTemp;
		//�����ж�
		if( uiMaxSize <= uiLength){
			return CErrorValueDefine::uiConstReturnFailed;
		}
	}
	//����Error status�ֶ�
	{
		CSnmpVariable objIntegerValue(ucErrorStatus);
		if( CSnmpStatic::EncodeInteger32(ucpBuffer,uiMaxSize-uiLength,&objIntegerValue,&uiOffsetTemp)
		!= CErrorValueDefine::uiConstReturnSuccess){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiLength+=uiOffsetTemp;
		//�����ж�
		if( uiMaxSize <= uiLength){
			return CErrorValueDefine::uiConstReturnFailed;
		}
	}
	//����RequestID�ֶ�
	{
		CSnmpVariable objIntegerValue(uiRequestID);
		if( CSnmpStatic::EncodeInteger32(ucpBuffer,uiMaxSize-uiLength,&objIntegerValue,&uiOffsetTemp)
		!= CErrorValueDefine::uiConstReturnSuccess){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiLength+=uiOffsetTemp;
		//�����ж�
		if( uiMaxSize <= uiLength){
			return CErrorValueDefine::uiConstReturnFailed;
		}
	}
	//���볤��
	if( CSnmpStatic::EncodeLengthSection(ucpBuffer,uiMaxSize-uiLength,uiLength,&uiOffsetTemp)
	!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiLength+=uiOffsetTemp;
	//�����ж�
	if( uiMaxSize <= uiLength){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//��Ӧ����
//	if( ucCurrentType == CSnmpConstDefine::ucConstTrap){
//		ucpBuffer[uiMaxSize-uiLength-1] = CSnmpConstDefine::ucConstTrap;
//	}
//	else{
//		ucpBuffer[uiMaxSize-uiLength-1] = CSnmpConstDefine::ucConstResponse;
//	}
	ucpBuffer[uiMaxSize-uiLength-1] =  ucCurrentType;

	uiLength ++;
	//�����ж�
	if( uiMaxSize <= uiLength){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//����Community������Ĭ��Ϊpublic
	uiLength += 8;
	//�����ж�
	if( uiMaxSize <= uiLength){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	ucpBuffer[uiMaxSize-uiLength] = CSnmpConstDefine::ucConstOctetString;
	ucpBuffer[uiMaxSize-uiLength+1] = 6;
	ucpBuffer[uiMaxSize-uiLength+2] = 'p';
	ucpBuffer[uiMaxSize-uiLength+3] = 'u';
	ucpBuffer[uiMaxSize-uiLength+4] = 'b';
	ucpBuffer[uiMaxSize-uiLength+5] = 'l';
	ucpBuffer[uiMaxSize-uiLength+6] = 'i';
	ucpBuffer[uiMaxSize-uiLength+7] = 'c';
	//����汾�ֶ�
	{
		CSnmpVariable objIntegerValue(ucCurrentVersion);
		if( CSnmpStatic::EncodeInteger32(ucpBuffer,uiMaxSize-uiLength,&objIntegerValue,&uiOffsetTemp)
		!= CErrorValueDefine::uiConstReturnSuccess){
			return CErrorValueDefine::uiConstReturnFailed;
		}
		uiLength+=uiOffsetTemp;
		//�����ж�
		if( uiMaxSize <= uiLength){
			return CErrorValueDefine::uiConstReturnFailed;
		}
	}
	//���볤��
	if( CSnmpStatic::EncodeLengthSection(ucpBuffer,uiMaxSize-uiLength,uiLength,&uiOffsetTemp)
	!= CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	uiLength+=uiOffsetTemp;
	//�����ж�
	if( uiMaxSize <= uiLength){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//SNMPͷ0x30
	ucpBuffer[uiMaxSize-uiLength-1] = 0x30;
	uiLength ++;
	*uipDataLength = uiLength;
	return CErrorValueDefine::uiConstReturnSuccess;
}
/**********************************************************************************************************************
** Function name:			EventCompleted
** Descriptions:			�¼���ɶ����鷽��
** input parameters:		None
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2011-08-04
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
/*uint32 CSnmpCommand::EventCompleted()
{
	uint32 uiLength = 0;
	//����
	if( Encode(&uiLength) != CErrorValueDefine::uiConstReturnSuccess ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//����Դ��Ŀ��
	objData.Switch();
	uint8* ucpBuffer = objpSnmpDataInOut->GetResponseBuffer();

	if( objData.SetData( &ucpBuffer[CSnmpDataInOut::uiConstResponseBufferSize-uiLength],uiLength)
			!= CErrorValueDefine::uiConstReturnSuccess ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//���������Ӧ
	if( objpUDPInterface != NULL ){
		objpUDPInterface->TransmitUDPPackage(&objData);
	}
	return CErrorValueDefine::uiConstReturnSuccess;
}
*/



/**********************************************************************************************************************
** Function name:			StartMibSearch
** Descriptions:			������õ�CUDPDataֵ������MIB������
** input parameters:		None
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
**							����ִ�����񣬲���������ý�����ڴ�����CErrorValueDefine::uiConstReturnProcessing
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::StartMibSearch()
{
	uint32 uiDataLength = 0;
	uint8* ucpBuffer = objData.GetData(&uiDataLength);
	if( ucpBuffer == NULL || uiDataLength == 0 ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//����
	if( Decode(ucpBuffer,uiDataLength) != CErrorValueDefine::uiConstReturnSuccess){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	if( GetCurrentRequestType() != CSnmpConstDefine::ucConstGet
		&& GetCurrentRequestType() != CSnmpConstDefine::ucConstSet
		&& GetCurrentRequestType() != CSnmpConstDefine::ucConstGetNext ){
		return CErrorValueDefine::uiConstReturnFailed;
	}
	//������ѯMIB��
	CSnmpCell* objpCell = (CSnmpCell*)GetFirstDownLink();
	uint32 uiRet = 0;
	while(objpCell!=NULL){
		if(  objpCell->StartMibSearch() == CErrorValueDefine::uiConstReturnProcessing){//������ڴ����У���Ҫע��ȴ�����
			//�ȴ���Բ���
//			if(uiRet == 0){
//				Wait(0);
//			}
			uiRet = 1;
		}
		objpCell = (CSnmpCell*)GetNextDownLink(objpCell);
	}
	if(uiRet == 0){// �������
		//��������Ϊ��Ӧ����
		ucCurrentType = CSnmpConstDefine::ucConstResponse;
		//����Դ��Ŀ��
//		objData.Switch();
		SendSnmpCommand();
		ThreadTaskDestory();
	}
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			SendSnmpCommand
** Descriptions:			��������
** input parameters:		None
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
**							����ִ�����񣬲���������ý�����ڴ�����CErrorValueDefine::uiConstReturnProcessing
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::SendSnmpCommand()
{
	uint32 uiLength = 0;
	//����
	if( Encode(&uiLength) != CErrorValueDefine::uiConstReturnSuccess ){
		return CErrorValueDefine::uiConstReturnFailed;
	}

	uint8* ucpBuffer = objpSnmpDataInOut->GetResponseBuffer();

	//���������Ӧ
	uint8* sendbuf = udp_get_buf (uiLength);
	memcpy (sendbuf, &ucpBuffer[CSnmpDataInOut::uiConstResponseBufferSize-uiLength], uiLength);
	  /* Send 'Hello World!' to remote peer */
	udp_send (CommunicationModule::getSnmpSocket(), objData.GetSourceIP(), objData.GetSourcePort(), sendbuf, uiLength);

//	term_dat_out_len(sendbuf, uiLength);

	return CErrorValueDefine::uiConstReturnSuccess;

}

/**********************************************************************************************************************
** Function name:			GetSnmpDataInOut
** Descriptions:			���SNMP��ݽ��ն���
** input parameters:		None
** output parameters:		None
** Returned value:			SNMP��ݽ��ն���
** Created by:				WangChangRong
** Created date:			2012-03-16
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CSnmpDataInOut* CSnmpCommand::GetSnmpDataInOut()
{
	return objpSnmpDataInOut;
}


/**********************************************************************************************************************
** Function name:			GetSnmpDataInOut
** Descriptions:			�������SNMPCell
** input parameters:		None
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-03-16
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::Clear()
{
	//�������α�߳��������
	if( GetFirstDownLink() !=NULL ){
		CThreadTask* obipThreadTask = GetFirstDownLink();
		CThreadTask* obipTmp = NULL;
		while( obipThreadTask != NULL ){
			obipTmp = obipThreadTask->GetThreadTaskNext();
			obipThreadTask->ThreadTaskDestory();
			obipThreadTask = obipTmp;
		}
	}
	return CErrorValueDefine::uiConstReturnSuccess;
}
/**********************************************************************************************************************
** Function name:			Notify
** Descriptions:			������
** 							ֻ��α�߳�������Ч���Զ�ʱ���񡢸����ȼ�����͵����ȼ�������Ч
** input parameters:		uiNotifyType:	��������
** 							objpSourceTask:	���ѵ�Դα�߳��������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-16
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
//uint32 CSnmpCommand::Notify(uint32 uiNotifyType,CThreadTask* objpSourceTask)
//{
//	//��������Ϊ��Ӧ����
//	ucCurrentType = CSnmpConstDefine::ucConstResponse;
//	//����Դ��Ŀ��
//	objData.Switch();
//	//���ͷ�������
//	SendSnmpCommand();
//	//��ٶ���
//	ThreadTaskDestory();
//
//	return CErrorValueDefine::uiConstReturnSuccess;
//}

/**********************************************************************************************************************
** Function name:			Notify
** Descriptions:			�����¡
** input parameters:		objpSnmp:	��¡����
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							�����ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-18
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpCommand::Clone(CSnmpCommand* objpSnmp)
{
	if(objpSnmp == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
//	objpUDPInterface = objpSnmp->GetUDPInterface();				//UDP��ݰ���Դ����
	objData.Clone(objpSnmp->GetUDPData());								//UDP��ݰ����
	uiRequestID = objpSnmp->GetCurrentRequestID();								//SNMP����ID
	ucCurrentVersion = objpSnmp->GetCurrentRequestVersion();							//�汾
	ucCurrentType = objpSnmp->GetCurrentRequestType();							//SNMP��������
	uiCurrentErrorStatus = objpSnmp->GetCurrentRequestErrorStatus();							//SNMP���״̬
	uiCurrentErrorIndex = objpSnmp->GetCurrentRequestErrorIndex();							//SNMP�������

	CSnmpCell* objTempTask = (CSnmpCell*)objpSnmp->GetFirstDownLink();
	while(objTempTask != NULL){
		CSnmpCell* objpCell = new CSnmpCell(this);
		objpCell->Clone(objTempTask);
		InsertDownLinkThreadTask(objpCell);
		objTempTask =  (CSnmpCell*)objTempTask->GetThreadTaskNext();
	}
	return CErrorValueDefine::uiConstReturnSuccess;
}
