/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CUDPData.h
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

#ifndef CUDPDATA_H_
#define CUDPDATA_H_
#include "EZ_types.h"
class CUDPData
{
private:
	uint8 ucSourceMAC[6];						//ԴMAC
	uint8 ucDestinationMAC[6];					//Ŀ��MAC
	uint8 ucSourceIP[4];						//ԴIP
	uint8 ucDestinationIP[4];					//Ŀ��IP
	uint16 usSourcePort;						//Դ�˿�
	uint16 usDestinationPort;					//Ŀ�Ķ˿�
	uint8* ucpUDPData;							//UDP����Buffer
	uint32 uiDataLength;						//UDP���ݳ���
public:
	CUDPData();
	CUDPData(uint8* remip, uint16 remport, uint8* pdata, uint16 len);
	virtual ~CUDPData();
	//��UDP���ݰ��н����UDP����
	uint32 Decode(uint8* ucpBuffer,uint32 uiLength);
	//����Դ��Ŀ��
	uint32 Switch();
	//���ԴMAC
	uint8* GetSourceMAC();
	//���Ŀ��MAC
	uint8* GetDestinationMAC();
	//���ԴIP
	uint8* GetSourceIP();
	//���Ŀ��IP
	uint8* GetDestinationIP();
	//���Դ�˿�
	uint16 GetSourcePort();
	//���Ŀ�Ķ˿�
	uint16 GetDestinationPort();
	//�������
	uint8* GetData(uint32* uipDataLength);
	//��¡UDP���ݶ���
	uint32 Clone(CUDPData* objpData);
	//��������
	uint32 SetData(uint8* ucpBuffer,uint32 uiLength);
	//��IP���н����UDP����
	uint32 DecodeFromIPPackage(uint8* ucpBuffer,uint32 uiLength);
    uint32 SetDestinationMAC(uint8* ucpDestMAC);
};

#endif /* CUDPDATA_H_ */
