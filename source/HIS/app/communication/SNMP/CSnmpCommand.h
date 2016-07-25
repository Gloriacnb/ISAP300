/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CSnmpCommand.h
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

#ifndef CSNMPCOMMAND_H_
#define CSNMPCOMMAND_H_

#include "CThreadTask.h"
#include "CUDPData.h"
class CSnmpDataInOut;
//class CUDPInterface;
class CSnmpCommand: public CThreadTask
{
private:
	CSnmpDataInOut* objpSnmpDataInOut;				//SNMP���ݽ��ն���
//	CUDPInterface* objpUDPInterface;				//UDP���ݰ���Դ����
	CUDPData objData;								//UDP���ݰ�����
	uint32 uiRequestID;								//SNMP����ID
	uint8 ucCurrentVersion;							//�汾
	uint8 ucCurrentType;							//SNMP��������
	uint32 uiCurrentErrorStatus;							//SNMP���״̬
	uint32 uiCurrentErrorIndex;							//SNMP�������

public:
	CSnmpCommand(CSnmpDataInOut* objpIn);
	virtual ~CSnmpCommand();
	//��ʼ�����󣬷���
	//uint32 InitialObject(CUDPInterface* objpIn,CUDPData* objpData);
	//������Buffer�н���
	uint32 Decode(uint8* ucpData,uint32 uiLength);
	//�Ӷ����б��뵽CSnmpDataInOut����Ӧ����Buffer
	uint32 Encode(uint32* uipDataLength);
	//���SNMP��������
	uint32 GetCurrentRequestType();
	//����SNMP��������
	uint32 SetCurrentRequestType(uint8 ucType);
	//���SNMP����ID
	uint32 GetCurrentRequestID();
	//����SNMP����ID
	uint32 SetCurrentRequestID(uint32 uiID);
	//���SNMP����汾
	uint32 GetCurrentRequestVersion();
	//����SNMP����汾
	uint32 SetCurrentRequestVersion(uint8 ucVersion);
	//���SNMP������״̬
	uint32 GetCurrentRequestErrorStatus();
	//����SNMP������״̬
	uint32 SetCurrentRequestErrorStatus(uint32 uiStatus);
	//���SNMP����������
	uint32 GetCurrentRequestErrorIndex();
	//����SNMP����������
	uint32 SetCurrentRequestErrorIndex(uint32 uiIndex);
	//���UDP���ݰ�����
    CUDPData* GetUDPData();
    //����UDP���ݰ�����
    uint32 SetUDPData(CUDPData* objpData);
    //���UDP���ݰ���Դ����
//    CUDPInterface* GetUDPInterface();
    //����UDP���ݰ���Դ����
//    uint32 SetUDPInterface(CUDPInterface* objpInterface);

    //�������õ�CUDPDataֵ������MIB������
    uint32 StartMibSearch();
    //��������
    uint32 SendSnmpCommand();
    //���SNMP���ݽ��ն���
    CSnmpDataInOut* GetSnmpDataInOut();

    //�������SNMPCell
    uint32 Clear();

	//������,���������е���
//	virtual uint32 Notify(uint32 uiNotifyType,CThreadTask* objpSourceTask);

	//�����¡
	uint32 Clone(CSnmpCommand* objpSnmp);
	//�¼���ɶ����鷽��
//	virtual uint32 EventCompleted();
};

#endif /* CSNMPCOMMAND_H_ */
