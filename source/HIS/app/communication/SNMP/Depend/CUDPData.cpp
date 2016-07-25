/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CUDPData.cpp
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

#include "CUDPData.h"
#include "string.h"
#include "CErrorValueDefine.h"
/**********************************************************************************************************************
** Function name:			CUDPData
** Descriptions:			�๹�캯��
** input parameters:		None
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2011-8-2
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/

CUDPData::CUDPData()
{

	memset(ucSourceMAC,0x00,6);						//ԴMAC
	memset(ucDestinationMAC,0x00,6);					//Ŀ��MAC
	memset(ucSourceIP,0x00,4);						//ԴIP
	memset(ucDestinationIP,0x00,6);					//Ŀ��IP
	usSourcePort = 0;						//Դ�˿�
	usDestinationPort = 0;					//Ŀ�Ķ˿�
	ucpUDPData = NULL;							//UDP����Buffer
	uiDataLength = 0;						//UDP���ݳ���
}

CUDPData::CUDPData(uint8* remip, uint16 remport, uint8* pdata, uint16 len) {
	usSourcePort = remport;
	memcpy(ucSourceIP,remip, 4);
	ucpUDPData = pdata;
	uiDataLength = len;
}

/**********************************************************************************************************************
** Function name:			~CUDPData
** Descriptions:			����������
** input parameters:		None
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CUDPData::~CUDPData()
{
}


/**********************************************************************************************************************
** Function name:			Decode
** Descriptions:			��UDP���ݰ��н����UDP����
** input parameters:		ucpBuffer��			����MAC��̫������Buffer
** 							uiLength��			����MAC��̫�����ݳ���
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** 							��ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							���ݲ������󷵻�CErrorValueDefine::uiConstReturnInputDataInvalid;
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CUDPData::Decode(uint8* ucpBuffer,uint32 uiLength)
{
	if( ucpBuffer == NULL){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	if( uiLength < 42){//MAC���ݰ�����С��14(MACͷ��)+20(IPͷ��)+8(UDPͷ��)
		return CErrorValueDefine::uiConstReturnInputDataInvalid;
	}
	if( ucpBuffer[12] != 0x80 && ucpBuffer[13] != 0x00 ){//У���Ƿ���IP���ݰ�
		return CErrorValueDefine::uiConstReturnInputDataInvalid;
	}
	if( ucpBuffer[23] != 0x11 ){//У���Ƿ���UDPЭ��
		return CErrorValueDefine::uiConstReturnInputDataInvalid;
	}
	memcpy(ucDestinationMAC,ucpBuffer,6 );		//Ŀ��MAC
	memcpy(ucSourceMAC,&ucpBuffer[6],6 );		//ԴMAC
	memcpy(ucSourceIP,&ucpBuffer[26],4 );		//ԴIP
	memcpy(ucDestinationIP,&ucpBuffer[30],4 );	//Ŀ��IP
	usSourcePort = (uint16)((ucpBuffer[34] << 8) |ucpBuffer[35]);//Դ�˿�
	usDestinationPort = (uint16)((ucpBuffer[36] << 8) |ucpBuffer[37]);//Ŀ�Ķ˿�

	ucpUDPData = &ucpBuffer[42];							//UDP����Buffer
	//uiDataLength = ((ucpBuffer[38] << 8) |ucpBuffer[39]);	//UDP���ݳ���
	uiDataLength = uiLength - 46;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			DecodeFromIPPackage
** Descriptions:			��IP���н����UDP���ݣ�û��MAC��ַ
** input parameters:		ucpBuffer��			IP������Buffer
** 							uiLength��			IP�����ݳ���
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** 							��ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							���ݲ������󷵻�CErrorValueDefine::uiConstReturnInputDataInvalid;
** Created by:				WangChangRong
** Created date:			2011-08-26
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CUDPData::DecodeFromIPPackage(uint8* ucpBuffer,uint32 uiLength)
{
	if( ucpBuffer == NULL){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}

	if( uiLength < 28){//IP������С��20(IPͷ��)+8(UDPͷ��)
		return CErrorValueDefine::uiConstReturnInputDataInvalid;
	}
	if( ucpBuffer[9] != 0x11 ){//У���Ƿ���UDPЭ��
		return CErrorValueDefine::uiConstReturnInputDataInvalid;
	}
	memcpy(ucSourceIP,&ucpBuffer[12],4 );		//ԴIP
	memcpy(ucDestinationIP,&ucpBuffer[16],4 );	//Ŀ��IP
	usSourcePort = (uint16)((ucpBuffer[20] << 8) |ucpBuffer[21]);//Դ�˿�
	usDestinationPort = (uint16)((ucpBuffer[22] << 8) |ucpBuffer[23]);//Ŀ�Ķ˿�

	ucpUDPData = &ucpBuffer[28];							//UDP����Buffer
	//uiDataLength = ((ucpBuffer[38] << 8) |ucpBuffer[39]);	//UDP���ݳ���
	uiDataLength = uiLength - 28;
	return CErrorValueDefine::uiConstReturnSuccess;
}
/**********************************************************************************************************************
** Function name:			Switch
** Descriptions:			����Դ��Ŀ��
** input parameters:		None
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CUDPData::Switch()
{
	uint8 ucTemp[6];
	memcpy(ucTemp,ucSourceMAC,6);
	memcpy(ucSourceMAC,ucDestinationMAC,6);
	memcpy(ucDestinationMAC,ucTemp,6);

	memcpy(ucTemp,ucSourceIP,4);
	memcpy(ucSourceIP,ucDestinationIP,4);
	memcpy(ucDestinationIP,ucTemp,4);

	uint16 usTemp = usSourcePort;
	usSourcePort = usDestinationPort;
	usDestinationPort = usTemp;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			Clone
** Descriptions:			��¡UDP���ݶ���
** input parameters:		objpData��			UDP���ݶ���
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** 							��ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer;
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CUDPData::Clone(CUDPData* objpData)
{
	if( objpData == NULL){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}

	memcpy(ucDestinationMAC,objpData->GetDestinationMAC(),6 );		//Ŀ��MAC
	memcpy(ucSourceMAC,objpData->GetSourceMAC(),6 );		//ԴMAC
	memcpy(ucSourceIP,objpData->GetSourceIP(),4 );		//ԴIP
	memcpy(ucDestinationIP,objpData->GetDestinationIP(),4 );	//Ŀ��IP
	usSourcePort = objpData->GetSourcePort();//Դ�˿�
	usDestinationPort = objpData->GetDestinationPort();//Ŀ�Ķ˿�

	ucpUDPData = objpData->GetData(&uiDataLength);	//UDP����Buffer,UDP���ݳ���
	return CErrorValueDefine::uiConstReturnSuccess;
}
/**********************************************************************************************************************
** Function name:			GetSourceMAC
** Descriptions:			���ԴMAC
** input parameters:		None
** output parameters:		None
** Returned value:			ԴMAC��Buffer��ַָ��
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint8* CUDPData::GetSourceMAC()
{
	return ucSourceMAC;
}
/**********************************************************************************************************************
** Function name:			GetDestinationMAC
** Descriptions:			���Ŀ��MAC
** input parameters:		None
** output parameters:		None
** Returned value:			Ŀ��MAC��Buffer��ַָ��
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint8* CUDPData::GetDestinationMAC()
{
	return ucDestinationMAC;
}

/**********************************************************************************************************************
** Function name:			GetSourceIP
** Descriptions:			���ԴIP
** input parameters:		None
** output parameters:		None
** Returned value:			ԴIP��Buffer��ַָ��
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint8* CUDPData::GetSourceIP()
{
	return ucSourceIP;
}

/**********************************************************************************************************************
** Function name:			GetDestinationIP
** Descriptions:			���Ŀ��IP
** input parameters:		None
** output parameters:		None
** Returned value:			Ŀ��IP��Buffer��ַָ��
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint8* CUDPData::GetDestinationIP()
{
	return ucDestinationIP;
}

/**********************************************************************************************************************
** Function name:			GetSourcePort
** Descriptions:			���Դ�˿�
** input parameters:		None
** output parameters:		None
** Returned value:			Դ�˿�ֵ
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint16 CUDPData::GetSourcePort()
{
	return usSourcePort;
}

/**********************************************************************************************************************
** Function name:			GetDestinationPort
** Descriptions:			���Ŀ�Ķ˿�
** input parameters:		None
** output parameters:		None
** Returned value:			Ŀ�Ķ˿�ֵ
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint16 CUDPData::GetDestinationPort()
{
	return usDestinationPort;
}

/**********************************************************************************************************************
** Function name:			GetData
** Descriptions:			�������
** input parameters:		uipDataLength��	���ݳ���
** output parameters:		None
** Returned value:			���ݵ�Buffer��ַָ��
** Created by:				WangChangRong
** Created date:			2011-08-02
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint8* CUDPData::GetData(uint32* uipDataLength)
{
	if( uipDataLength != NULL){
		*uipDataLength = uiDataLength;
	}
	return ucpUDPData;
}

/**********************************************************************************************************************
** Function name:			SetData
** Descriptions:			��������
** input parameters:		ucpBuffer��	����Buffer��ַ
** 							uiLength��	���ݳ���
** output parameters:		None
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
uint32 CUDPData::SetData(uint8* ucpBuffer,uint32 uiLength)
{
	if( ucpBuffer == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	ucpUDPData = ucpBuffer;
	uiDataLength = uiLength;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			SetDestinationMAC
** Descriptions:			����Ŀ��MAC��ַ
** input parameters:		ucpDestMAC: Ŀ��MAC��ַָ��
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
**							���������ָ�����CErrorValueDefine::uiConstReturnInputNULLPointer;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				Liu Weifeng
** Created date:			2011-12-20
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CUDPData::SetDestinationMAC(uint8* ucpDestMAC)
{
	if(ucpDestMAC == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}
	memcpy(ucDestinationMAC, ucpDestMAC, 6);
	return CErrorValueDefine::uiConstReturnSUCCESS;
}


