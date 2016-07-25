/****************************************Copyright (c)*****************************************************************
**                            Easy Broadband Technology Co., Ltd.
**                                      
**                                 http://www.ezwell-tech.com
**
**--------------File Info----------------------------------------------------------------------------------------------
** Created by:				Liu Weifeng
** Created date:			2011-07-06
** Version:				    1.0
** Descriptions:			��������þ���Ϊ�˻�ȡCMain�ȶ���ָ��
**
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Version:					
** Descriptions:						
**
**********************************************************************************************************************/
#include "CClassPointer.h"
#include "EZ_types.h"
//#include "CMain.h"
//#include "CLocalDevice.h"
//CMain* CClassPointer::objpMain = NULL;
//#include "CMibImpl.h"
#include "CSnmpMib.h"
extern CSnmpMib snmp;

/**********************************************************************************************************************
** Function name:			CClassPointer
** Descriptions:			���캯��
** input parameters:		None
** output parameters:		None
** Returned value:			None                               
** Created by:				Liu Weifeng                                            
** Created Date:			2011-07-06 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
CClassPointer::CClassPointer()
{
	return;
}


/**********************************************************************************************************************
** Function name:			~CClassPointer
** Descriptions:			��������
** input parameters:		None
** output parameters:		None
** Returned value:			None                               
** Created by:				Liu Weifeng                                            
** Created Date:			2011-07-06 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
CClassPointer::~CClassPointer()
{
	return;
}

/**********************************************************************************************************************
** Function name:			SetMainPointer
** Descriptions:			����CMain����ָ��
** input parameters:		None
** output parameters:		None
** Returned value:			None                             
** Created by:				Liu Weifeng                                            
** Created Date:			2011-07-06 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
// void CClassPointer::SetMainPointer(CMain* objpInMain)
// {
// 	if( objpInMain == NULL){
// 		return;
// 	}
//     objpMain = objpInMain;
// }



/**********************************************************************************************************************
** Function name:			GetMainPointer
** Descriptions:			��ȡCMain����ָ��
** input parameters:		None
** output parameters:		None
** Returned value:			Cmain����ָ��                              
** Created by:				Liu Weifeng                                            
** Created Date:			2011-07-06 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
// CMain* CClassPointer::GetMainPointer(void)
// {
//     return objpMain;
// }

/**********************************************************************************************************************
** Function name:			GetTaskSchedulingPointer
** Descriptions:			��ȡCTaskScheduling����ָ��
** input parameters:		None
** output parameters:		None
** Returned value:			CTaskScheduling����ָ��                              
** Created by:				Liu Weifeng                                            
** Created Date:			2011-07-06 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
// CTaskScheduling* CClassPointer::GetTaskSchedulingPointer(void)
// {
//     return objpMain->GetSystemObject()->GetTaskScheduling();
// }

/**********************************************************************************************************************
** Function name:			GetDataTimePointer
** Descriptions:			��ȡCDateTime����ָ��
** input parameters:		None
** output parameters:		None
** Returned value:			CDateTime����ָ��
** Created by:				Liu Weifeng                                            
** Created Date:			2011-07-06 
**---------------------------------------------------------------------------------------------------------------------
** Modified by:				
** Modified date:			
** Descriptions:			
**
**********************************************************************************************************************/
// CDateTime* CClassPointer::GetDataTimePointer(void)
// {
//     return objpMain->GetSystemObject()->GetDateTime();
// }
/*********************************************************************************************************************
** Function name:			GetLocalDevicePointer
** Descriptions:			  ��ȡCLocalDevicce����ָ��
** input parameters:		None
** output parameters:		None
** Returned value:			CLocalDevicce����ָ��
** Created by:				WangChangRong
** Created Date:			2011-07-26
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
*********************************************************************************************************************/
//CLocalDevice* CClassPointer::GetLocalDevicePointer()
//{
//	return objpMain->GetDeviceObject()->GetLocalDevice();
//}




/*********************************************************************************************************************
** Function name:			GetHardWarePointer
** Descriptions:			��ȡCHardWare����ָ��
** input parameters:		None
** output parameters:		None
** Returned value:			CHardWare����ָ��
** Created by:				WangChangRong
** Created Date:			2011-07-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
*********************************************************************************************************************/
// CHardWare* CClassPointer::GetHardWarePointer(void)
// {
//     return objpMain->GetHardWareObject();
// }
/**********************************************************************************************************************
** Function name:			GetMibImplPointer
** Descriptions:			��ȡCMibImpl����ָ��
** input parameters:		None
** output parameters:		None
** Returned value:			CMibImpl����ָ��
** Created by:				WangChangRong
** Created Date:			2011-08-08
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
*********************************************************************************************************************/
CMibImpl* CClassPointer::GetMibImplPointer()
{
	return snmp.GetMibImpl();
}

/*********************************************************************************************************************
** Function name:			GetCommunicationPointer
** Descriptions:			��ȡCCommunication����ָ��
** input parameters:		None
** output parameters:		None
** Returned value:			CCommunication����ָ��
** Created by:				WangChangRong
** Created Date:			2011-08-09
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
*********************************************************************************************************************/
// CCommunication* CClassPointer::GetCommunicationPointer()
// {
// 	return objpMain->GetCommunicationObject();
// }


