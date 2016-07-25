/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CSnmpDataInOut.cpp
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

#include "CSnmpDataInOut.h"
#include "CErrorValueDefine.h"
#include "CSnmpCommand.h"
#include "CClassPointer.h"
//extern CClassPointer objGlbClassPointer;
/**********************************************************************************************************************
** Function name:			CSnmpDataInOut
** Descriptions:			�๹�캯��
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

CSnmpDataInOut::CSnmpDataInOut()
{
	objpSnmpMib = NULL;					//�ϼ�SNMP��MIB����ָ��
}
/**********************************************************************************************************************
** Function name:			~CSnmpDataInOut
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

CSnmpDataInOut::~CSnmpDataInOut()
{
}
/**********************************************************************************************************************
** Function name:			InitialClassObject
** Descriptions:			��ʼ���������ʹ��ǰ�������
** input parameters:		objpIn��			SNMP��MIB����ָ��
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2011-8-1
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpDataInOut::InitialClassObject(CSnmpMib* objpIn)
{
	if(objpIn == NULL){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	objpSnmpMib = objpIn;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			InitialClassObject
** Descriptions:			��������
** input parameters:		objpUDPInterface��	UDP���ݰ�����Դ����
** 							objData��			UDP���ݰ�����
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2011-8-1
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CSnmpDataInOut::ReceiverData(CUDPData* objData)
{
	if( objData == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	//��������
	CSnmpCommand* objpSnmp = new CSnmpCommand(this);
	if( objpSnmp == NULL){
		return CErrorValueDefine::uiConstReturnFailed;
	}
//	objpSnmp->SetUDPInterface(objpUDPInterface);
	objpSnmp->SetUDPData(objData);

	return objpSnmp->StartMibSearch();

//	//��ʼ������
//	if( objpSnmp->InitialObject(objpUDPInterface,objData) != CErrorValueDefine::uiConstReturnSuccess){
//		objpSnmp->EventDestory();
//		//delete objpSnmp;
//		return CErrorValueDefine::uiConstReturnFailed;
//	}
//	//ע������
//	CTaskScheduling* objpTaskScheduling = objGlbClassPointer.GetTaskSchedulingPointer();
//	if( objpTaskScheduling!= NULL ){
//		objpTaskScheduling->RegisterTask(objpSnmp,0);
//	}
//	return CErrorValueDefine::uiConstReturnSuccess;
}


/**********************************************************************************************************************
** Function name:			InitialClassObject
** Descriptions:			���SNMP��Ӧ���ݰ�����Buffer
** input parameters:		None
** output parameters:		None
** Returned value:			SNMP��Ӧ���ݰ�����Buffer
** Created by:				WangChangRong
** Created date:			2011-8-4
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint8* CSnmpDataInOut::GetResponseBuffer()
{
	return ucResponseBuffer;
}
