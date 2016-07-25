/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CThreadTask.cpp
 ** Created by:				WangChangRong
 ** Created date:			2012-3-8
 ** Version:				1.0
 ** Descriptions:			
 **---------------------------------------------------------------------------------------------------------------------
 ** Modified by:				
 ** Modified date:			
 ** Version:					
 ** Descriptions:			
 **********************************************************************************************************************/

#include "CThreadTask.h"
//#include "CTaskScheduling.h"
#include "CErrorValueDefine.h"
#include "CClassPointer.h"
/**********************************************************************************************************************
** Function name:			CThreadTask
** Descriptions:			�๹�캯��
** input parameters:		None
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2012-3-8
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CThreadTask::CThreadTask() {
	objpDownLinkBegin = NULL;			//����α�߳�����ͷ
	objpDownLinkEnd = NULL;			//����α�߳�����β
	objpThreadTaskNext = NULL;				//��һ��α�̣߳����ڹ���˫������
	objpThreadTaskPrev = NULL;				//��һ��α�̣߳����ڹ���˫������
	objpUpLinkThreadTask = NULL;			//����α�̶߳���

//	objpTimeoutTaskItem = NULL;
//	SetTaskType( CTaskScheduling::uiTaskTypePseudoThread );				//��������

	uiThreadTaskStatus = uiThreadTaskStatusProcess;
}

/**********************************************************************************************************************
** Function name:			~CThreadTask
** Descriptions:			����������
** input parameters:		None
** output parameters:		None
** Returned value:			None
** Created by:				WangChangRong
** Created date:			2012-3-8
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CThreadTask::~CThreadTask() {
}

/**********************************************************************************************************************
** Function name:			SetTimeoutTaskItem
** Descriptions:			���ó�ʱα�߳�������Ŀ�����ָ��
** input parameters:		objpItem��	��ʱα�߳�������Ŀ�����ָ��
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-8
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
//uint32 CThreadTask::SetTimeoutTaskItem(CTaskItem* objpItem)
//{
//	objpTimeoutTaskItem = objpItem;
//	return CErrorValueDefine::uiConstReturnSuccess;
//}
/**********************************************************************************************************************
** Function name:			GetTimeoutTaskItem
** Descriptions:			��ó�ʱα�߳�������Ŀ�����ָ��
** input parameters:		None
** output parameters:		None
** Returned value:			��ʱα�߳�������Ŀ�����ָ��
** Created by:				WangChangRong
** Created date:			2012-3-12
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
//CTaskItem* CThreadTask::GetTimeoutTaskItem( )
//{
//	return objpTimeoutTaskItem;
//}

/**********************************************************************************************************************
** Function name:			SetThreadTaskStatus
** Descriptions:			����α�߳�����״̬
** input parameters:		uiStatus��	α�߳�����״̬
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::SetThreadTaskStatus(uint32 uiStatus)
{
	uiThreadTaskStatus = uiStatus;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			GetThreadTaskStatus
** Descriptions:			���α�߳�����״̬
** input parameters:		None
** output parameters:		None
** Returned value:			α�߳�����״̬
** Created by:				WangChangRong
** Created date:			2012-3-12
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::GetThreadTaskStatus()
{
	return uiThreadTaskStatus;
}

/**********************************************************************************************************************
** Function name:			Wait
** Descriptions:			������𣬽������Ƶ��ȴ������С�������Ҫ����ʱ�������������Ƶ���ʱ�����С�
** 							ֻ��α�߳�������Ч���Զ�ʱ���񡢸����ȼ�����͵����ȼ�������Ч
** input parameters:		uiTimeout:		��ʱʱ�䣬��λ���룬1000��ʾ1�����볬ʱ����
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-8
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
//uint32 CThreadTask::Wait(uint32 uiTimeout)
//{
//	CClassPointer::GetTaskSchedulingPointer()->UnRegisterThreadTask(this);//��α�̶߳�����ע��
//	if( uiTimeout == 0){//ֻ���𣬲����г�ʱ������
//		//�ӳ�ʱ������ע��
//		CClassPointer::GetTaskSchedulingPointer()->UnRegisterTimeoutThreadTask(this);
//		//ע��������
//		CClassPointer::GetTaskSchedulingPointer()->RegisterThreadWaitTask(this,0);
//	}
//	else{
//		uint32 uiPara = CClassPointer::GetTaskSchedulingPointer()->GetSystemTimeTicks()+uiTimeout;
//		//��ע�ᳬʱ�������
//		CClassPointer::GetTaskSchedulingPointer()->RegisterTimeoutThreadTask(this,uiPara);
//		//��ע��������
//		CClassPointer::GetTaskSchedulingPointer()->RegisterThreadWaitTask(this,uiPara);
//	}
//	return CErrorValueDefine::uiConstReturnFailed;
//}
//

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
** Created date:			2012-3-8
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::Notify(uint32 uiNotifyType,CThreadTask* objpSourceTask)
{
	//֪ͨ�ϼ��������
	CThreadTask* objpUpTask = GetUpLinkThreadTask();
	if( objpUpTask != NULL ){
		objpUpTask->Notify(uiNotifyType,this);
	}
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			TimeoutNotify
** Descriptions:			��ʱ����
** 							ֻ��α�߳�������Ч���Զ�ʱ���񡢸����ȼ�����͵����ȼ�������Ч
** input parameters:		None
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-8
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
//uint32 CThreadTask::TimeoutNotify(uint32 uiTimeout)
//{
//	CClassPointer::GetTaskSchedulingPointer()->UnRegisterTimeoutThreadTask(this);
//	return CErrorValueDefine::uiConstReturnFailed;
//}

/**********************************************************************************************************************
** Function name:			IsTopThreadTask
** Descriptions:			�Ƿ������ϼ�α�߳��������
** input parameters:		None
** output parameters:		None
** Returned value:			�����Ƿ������ϼ�α�߳������־��	ȡֵtrue����ʾ�����ϼ�α�߳�����
** 															ȡֵfalse����ʾ�������ϼ�α�߳�����
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::IsTopThreadTask()
{
	if(objpUpLinkThreadTask == NULL){
		return true;
	}
	else{
		return false;
	}
}
/**********************************************************************************************************************
** Function name:			IsBottomThreadTask
** Descriptions:			�Ƿ������¼�α�߳��������
** input parameters:		None
** output parameters:		None
** Returned value:			�����Ƿ������¼�α�߳������־��	ȡֵtrue����ʾ�����¼�α�߳�����
** 															ȡֵfalse����ʾ�������¼�α�߳�����
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::IsBottomThreadTask()
{
	if(objpDownLinkBegin == NULL){
		return true;
	}
	else{
		return false;
	}
}

/**********************************************************************************************************************
** Function name:			GetFirstDownLink
** Descriptions:			�������α�߳�����ĵ�һ��
** input parameters:		None
** output parameters:		None
** Returned value:			���ص�һ������α�߳�����Ķ���ָ�룬���ΪNULL����ʾû������α�߳�����
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CThreadTask* CThreadTask::GetFirstDownLink()
{
	return objpDownLinkBegin;
}

/**********************************************************************************************************************
** Function name:			GetNextDownLink
** Descriptions:			�����һ������α�߳�������GetFirstDownLink�������ɵ�������
** input parameters:		objpDownLink:		����α�߳��������ָ�룬���ص�����һ������α�߳��������ָ��
** output parameters:		None
** Returned value:			������һ������α�߳�����Ķ���ָ�룬���ΪNULL����ʾû������α�߳����������������
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CThreadTask* CThreadTask::GetNextDownLink(CThreadTask* objpDownLink)
{
	if(objpDownLink == NULL){
		return NULL;
	}
	return objpDownLink->objpThreadTaskNext;
}

/**********************************************************************************************************************
** Function name:			GetEndDownLink
** Descriptions:			�������α�߳���������һ��
** input parameters:		None
** output parameters:		None
** Returned value:			�������һ������α�߳�����Ķ���ָ�룬���ΪNULL����ʾû������α�߳�����
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CThreadTask* CThreadTask::GetEndDownLink()
{
	return objpDownLinkEnd;
}

/**********************************************************************************************************************
** Function name:			GetPrevDownLink
** Descriptions:			���ǰһ������α�߳�������GetEndDownLink�������ɵ�������
** input parameters:		objpDownLink:		����α�߳��������ָ�룬���ص���ǰһ������α�߳��������ָ��
** output parameters:		None
** Returned value:			����ǰһ������α�߳�����Ķ���ָ�룬���ΪNULL����ʾû��ǰһ������α�߳����������������
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CThreadTask* CThreadTask::GetPrevDownLink(CThreadTask* objpDownLink)
{
	if(objpDownLink == NULL){
		return NULL;
	}
	return objpDownLink->objpThreadTaskPrev;
}
/**********************************************************************************************************************
** Function name:			SetUpLinkThreadTask
** Descriptions:			��������α�߳��������
** input parameters:		objpUpLink��		����α�߳��������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::SetUpLinkThreadTask(CThreadTask* objpUpLink)
{
	this->objpUpLinkThreadTask = objpUpLink;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			GetUpLinkEvent
** Descriptions:			�������α�߳��������
** input parameters:		None
** output parameters:		None
** Returned value:			����α�߳��������
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CThreadTask* CThreadTask::GetUpLinkThreadTask()
{
	return objpUpLinkThreadTask;
}

/**********************************************************************************************************************
** Function name:			InsertDownLinkEvent
** Descriptions:			��������α�߳��������
** input parameters:		objpDownLink��		����α�߳��������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							���˲���Ϊ��ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::InsertDownLinkThreadTask(CThreadTask* objpDownLink)
{
	if( objpDownLink == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}

	if( objpDownLinkBegin == NULL || objpDownLinkEnd == NULL){	//�����¼�Ϊ��
		objpDownLinkBegin = objpDownLink;
		objpDownLinkEnd = objpDownLink;
		objpDownLink->SetThreadTaskNext(NULL);
		objpDownLink->SetThreadTaskPrev(NULL);
		objpDownLink->SetUpLinkThreadTask(this);
	}
	else{	//�����¼��ǿգ���������
		objpDownLinkEnd->SetThreadTaskNext(objpDownLink);
		objpDownLink->SetThreadTaskNext(NULL);
		objpDownLink->SetThreadTaskPrev(objpDownLinkEnd);
		objpDownLinkEnd = objpDownLink;
		objpDownLink->SetUpLinkThreadTask(this);
	}
	return CErrorValueDefine::uiConstReturnSuccess;
}
/**********************************************************************************************************************
** Function name:			RemoveDownLinkThreadTask
** Descriptions:			�Ƴ�����α�߳��������
** input parameters:		objpDownLink��		�Ƴ�α�߳��������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							���˲���Ϊ��ָ�뷵��CErrorValueDefine::uiConstReturnInputNULLPointer
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::RemoveDownLinkThreadTask(CThreadTask* objpDownLink)
{
	if( objpDownLink == NULL ){
		return CErrorValueDefine::uiConstReturnInputNULLPointer;
	}

	if( objpDownLinkBegin == objpDownLink && objpDownLinkEnd == objpDownLink){
		objpDownLinkBegin = NULL;
		objpDownLinkEnd = NULL;
		objpDownLink->SetThreadTaskNext(NULL);
		objpDownLink->SetThreadTaskPrev(NULL);
		objpDownLink->SetUpLinkThreadTask(NULL);
	}
	else if( objpDownLinkBegin == objpDownLink ){

		objpDownLinkBegin = objpDownLinkBegin->GetThreadTaskNext();
		objpDownLinkBegin->SetThreadTaskPrev(NULL);
		objpDownLink->SetThreadTaskNext(NULL);
		objpDownLink->SetThreadTaskPrev(NULL);
		objpDownLink->SetUpLinkThreadTask(NULL);
	}
	else if( objpDownLinkEnd == objpDownLink ){

		objpDownLinkEnd = objpDownLinkEnd->GetThreadTaskPrev();
		objpDownLinkEnd->SetThreadTaskNext(NULL);
		objpDownLink->SetThreadTaskNext(NULL);
		objpDownLink->SetThreadTaskPrev(NULL);
		objpDownLink->SetUpLinkThreadTask(NULL);
	}
	else{
		objpDownLink->GetThreadTaskNext()->SetThreadTaskPrev(objpDownLink->GetThreadTaskPrev());
		objpDownLink->GetThreadTaskPrev()->SetThreadTaskNext(objpDownLink->GetThreadTaskNext());
	
		objpDownLink->SetThreadTaskNext(NULL);
		objpDownLink->SetThreadTaskPrev(NULL);
		objpDownLink->SetUpLinkThreadTask(NULL);
	}
	return CErrorValueDefine::uiConstReturnSuccess;
}


/**********************************************************************************************************************
** Function name:			SetThreadTaskNext
** Descriptions:			������һ��α�߳�����
** input parameters:		objpThreadTask��		α�߳��������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::SetThreadTaskNext(CThreadTask* objpThreadTask)
{
	objpThreadTaskNext = objpThreadTask;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			SetThreadTaskNext
** Descriptions:			������һ��α�߳�����
** input parameters:		objpThreadTask��		α�߳��������
** output parameters:		None
** Returned value:			ִ�гɹ�����CErrorValueDefine::uiConstReturnSuccess;
** 							ִ��ʧ�ܷ���CErrorValueDefine::uiConstReturnFailed;
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
uint32 CThreadTask::SetThreadTaskPrev(CThreadTask* objpThreadTask)
{
	objpThreadTaskPrev = objpThreadTask;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			GetThreadTaskNext
** Descriptions:			�����һ��α�߳�����
** input parameters:		None
** output parameters:		None
** Returned value:			��һ��α�߳�����
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CThreadTask* CThreadTask::GetThreadTaskNext()
{
	return objpThreadTaskNext;
}
/**********************************************************************************************************************
** Function name:			GetThreadTaskPrev
** Descriptions:			�����һ��α�߳�����
** input parameters:		None
** output parameters:		None
** Returned value:			��һ��α�߳�����
** Created by:				WangChangRong
** Created date:			2012-3-13
**---------------------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Descriptions:
**
**********************************************************************************************************************/
CThreadTask* CThreadTask::GetThreadTaskPrev()
{
	return objpThreadTaskPrev;
}

/**********************************************************************************************************************
** Function name:			ThreadTaskDestory
** Descriptions:			α�߳����������鷽�������������²���Ҫ����
** input parameters:		None
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
uint32 CThreadTask::ThreadTaskDestory()
{
	//��������α�߳��������
	if( objpDownLinkBegin!=NULL ){
		CThreadTask* obipThreadTask = objpDownLinkBegin;
		CThreadTask* obipTmp = NULL;
		while( obipThreadTask != NULL ){
			obipTmp = obipThreadTask->GetThreadTaskNext();
			obipThreadTask->ThreadTaskDestory();
			obipThreadTask = obipTmp;
		}
	}
	//������������Ƴ�
//	CClassPointer::GetTaskSchedulingPointer()->UnRegisterThreadTask(this);
//	CClassPointer::GetTaskSchedulingPointer()->UnRegisterThreadWaitTask(this);
//	CClassPointer::GetTaskSchedulingPointer()->UnRegisterTimeoutThreadTask(this);
	//�����Լ�
	delete this;
	return CErrorValueDefine::uiConstReturnSuccess;
}

/**********************************************************************************************************************
** Function name:			NotifyToUpThread
** Descriptions:			֪ͨ�ϼ�����
** input parameters:		uiNotifyType:	֪ͨ����
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
uint32 CThreadTask::NotifyToUpThread(uint32 uiNotifyType)
{
	CThreadTask* objpUpTask = GetUpLinkThreadTask();
	if( objpUpTask != NULL ){
		objpUpTask->Notify(uiNotifyType,this);
	}
	return CErrorValueDefine::uiConstReturnSuccess;

}

/**********************************************************************************************************************
** Function name:			ThreadTaskUndo
** Descriptions:			α�߳�����������Undo�鷽��
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
uint32 CThreadTask::ThreadTaskUndo()
{
	CThreadTask* objpDown = GetFirstDownLink();
	while(objpDown != NULL){
		objpDown->ThreadTaskUndo();
		objpDown = objpDown->GetNextDownLink(objpDown);
	}
	return CErrorValueDefine::uiConstReturnSuccess;
}
