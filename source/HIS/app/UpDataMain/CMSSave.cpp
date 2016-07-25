/****************************************Copyright (c)*****************************************************************
**                            Easy Broadband Technology Co., Ltd.
**                                      
**                                 http://www.ezwell-tech.com
**
**--------------File Info----------------------------------------------------------------------------------------------
** Created by:				Liu Weifeng
** Created date:			2011-06-07
** Version:				    1.0
** Descriptions:			��Flash����˫���Ĳ���
**
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Version:					
** Descriptions:						
**
**********************************************************************************************************************/
#include "CMSSave.h"
#include "CErrorValueDefine.h"
//#include "CNandFlash.h"
#include "string.h"
#include "CCheckData.h"
/**********************************************************************************************************************
** Function name:			CMSSave
** Descriptions:			���캯��
** input parameters:		None
** output parameters:		None
** Returned value:			None                               
** Created by:				Liu Weifeng                                            
** Created Date:			2011-06-07 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
CMSSave::CMSSave()
{
}

CMSSave::CMSSave(EZ_FLASH_DATA_T* stpInfo) {
	setInfo(*stpInfo);
}


/**********************************************************************************************************************
** Function name:			CMSSave
** Descriptions:			��������
** input parameters:		None
** output parameters:		None
** Returned value:			None                               
** Created by:				Liu Weifeng                                            
** Created Date:			2011-06-07 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
CMSSave::~CMSSave()
{
}


/**********************************************************************************************************************
** Function name:			ReadData
** Descriptions:			��ȡ���ݺ���
** input parameters:		uiLen:	��ȡ���ݳ���
** output parameters:		ucpBuf: ��ȡ���ݵĻ���
** Returned value:			CErrorValueDefine::uiConstReturnFailed:  ��ʧ��
**							CErrorValueDefine::uiConstReturnSuccess: ���ɹ�
**							CErrorValueDefine::uiConstReturnDeviceBusy:Flash��æ
**							��ȡ���ݵ�ʱ��һ��Ҫ�жϷ���ֵ���Է���ȡʧ��
** Created by:				Liu Weifeng                                            
** Created Date:			2011-06-07 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
uint32 CMSSave::ReadData(uint8* ucpBuf, uint32 uiLen)
{
    if( flash_data_load(&DataInfo) == TRUE ) {

    	return CErrorValueDefine::uiConstReturnSuccess;
    }

    return CErrorValueDefine::uiConstReturnFailed;
}

/**********************************************************************************************************************
** Function name:			SaveData
** Descriptions:			��ʼ��������
** input parameters:		uiDelayS: �ӳټ�����б��濪ʼ
** output parameters:		None
** Returned value:			CErrorValueDefine::uiConstReturnSuccess: д���ݳɹ�
** Created by:				Liu Weifeng                                            
** Created Date:			2011-06-07 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
uint32 CMSSave::SaveData(uint32 uiDelayS)
{
    if( flash_data_save(&DataInfo) == TRUE ) {
    	return CErrorValueDefine::uiConstReturnSuccess;
    }

    return CErrorValueDefine::uiConstReturnFailed;
}

bool CMSSave::ReadData(void) {
	if( flash_data_load(&DataInfo) ) {
		uint32 crcResult;
		CCheckData::CRCCheck1((uint8*)DataInfo.BufAddr, DataInfo.ByteNums-4, &crcResult);
		uint32* p = (uint32*)DataInfo.BufAddr;
		uint32 point = (DataInfo.ByteNums+3) >> 2;
		return crcResult == p[point-1];
	}
	return false;
}

bool CMSSave::SaveData(void) {
	uint32 crcResult;
	CCheckData::CRCCheck1((uint8*)DataInfo.BufAddr, DataInfo.ByteNums-4, &crcResult);
	uint32* p = (uint32*)DataInfo.BufAddr;
	uint32 point = (DataInfo.ByteNums+3) >> 2;
	p[point-1] = crcResult;
	return flash_data_save(&DataInfo);
}


/*
 * ���ô洢��Ϣ
 */
void CMSSave::setInfo(EZ_FLASH_DATA_T& info) {
	memcpy(&DataInfo, &info, sizeof(EZ_FLASH_DATA_T));
}


