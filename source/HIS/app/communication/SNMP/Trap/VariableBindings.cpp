#ifndef VariableBindings_CPP
#define VariableBindings_CPP

#include "VariableBindings.h"
#include "SNMPPdu.h"
#include "SNMPDefine.h"

VariableBindings::VariableBindings()
{
	variableBinding = (VariableBinding *)NULL;
	vbsLength = 0;
	vbsmemsize = 0;
}
VariableBindings::~VariableBindings()
{
	if(vbsLength>0 && variableBinding!=NULL) delete []variableBinding;
	variableBinding = (VariableBinding *)NULL;
	vbsLength = 0;
	vbsmemsize = 0;
}
uint8 VariableBindings::getVBSLength()
{
	return vbsLength;
}

uint32 VariableBindings::getVBNumberFromBuf(unsigned char *buf,uint32 len)
{
	if(buf == NULL || len==0) return 0xffffffff;

	
	uint8 offset= 0;
	
	uint32 vblength = 0;
	uint32 point = 0;
	uint32 num = 0;
	uint32 etl = len;
	while(true)
	{
		if(buf[point]!=0x30) break;

		vblength = SNMPPdu::decodeLength(&(buf[point+1]),etl-1,&offset);	  //�����ݰ�Ȳ�У��
		if( vblength>3000 ) break;		
		point += offset+1+vblength;
		etl = etl -  (offset+1+vblength);
		num++;
		if(point>=len) break;
	}
	return num;
}
void VariableBindings::newVBS(int len)
{
	
	if(vbsLength>0)
	{
		 delete []variableBinding;
		 variableBinding = (VariableBinding *)NULL;
		 vbsLength = 0;
	}
	if(len<=0) return;
	variableBinding = new VariableBinding[len];
	vbsLength = (uint8)len;
}
VariableBinding* VariableBindings::getAt(int index)
{
	if(vbsLength == 0 || index<0 || index>=vbsLength) return (VariableBinding*)NULL;
	return &variableBinding[index];


}

int VariableBindings::getBerLength()
{
	if(vbsLength == 0) return 0;
	else
	{
		int len = 0;
		int vblen = 0;
		for(int i=0;i<vbsLength;i++)
		{
			vblen = variableBinding[i].getBerLength();
			len += vblen+1+SNMPPdu::getLengthBerLength(vblen);			
		}
		return len ;
	}
}
uint8 VariableBindings::encodeData(unsigned char *buf,int len)	//��PDU�л��buffer
{
	if(vbsLength == 0)
	{
		buf[0] = 0x30;
		buf[1] = 0x00;
		return 2;
	}
	buf[0] = 0x30;
	int offset = 0;
	offset = SNMPPdu::encodeLength(&(buf[1]),len-1,getBerLength());//�ܳ���
	int point = offset+1;
	for(int i=0;i<vbsLength;i++)
	{
		offset = variableBinding[i].encodeData(&(buf[point]),len-point);
		point += offset;
	}
	return true;
}
#endif
