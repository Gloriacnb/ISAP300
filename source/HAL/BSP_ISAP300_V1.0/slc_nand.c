/*****************************Copyright(c)****************************************************************
**                    Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                           http://www.embedtools.com
**
**------File Info-----------------------------------------------------------------------------------------
** File Name:            slc_nand.c
** Latest modified Date: 2008-11-10        
** Latest Version:       V1.0    
** Description:          nand flash �������ӿں���
**                       
**--------------------------------------------------------------------------------------------------------
** Created By:           WangFaqiang 
** Created date:         2008-11-10    
** Version:              V1.0    
** Descriptions:         ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:          
** Modified date:        
** Version:             
** Description:          
**                      
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#include "slc_nand.h"

#include <LPC325x.H>

//#define FILE_SYSTEM_NAND_BLOCK_START 0
//#define PAGE_OFFS	(FILE_SYSTEM_NAND_BLOCK_START*64)
/*********************************************************************************************************
** Function name:       __SLCNANDInit
** Descriptions:        __SLC NAND Flash ��������ʼ��
** input parameters:    None
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/
void __SLCNANDInit(void)
{
    FLASHCLK_CTRL = 0x25;                                               /*  SLCʱ��ʹ��                 */
    SLC_CFG       = 0x00;                                               /*  ECC��ֹ��DMA��ֹ            */ 
//     SLC_TAC       = (uint32) ((15 << 0) |
//                     (15 << 4) |
//                     (15 << 8) |
//                     (15 << 12)|
//                     (15 << 16)|
//                     (15 << 20)|
//                     (15 << 24)|
//                     (0xf << 28) );                                         /*  ���ö�/дʱ��               */
    SLC_TAC = 0xffffffff;                
    SLC_CFG       = 0x00;                                               /*  ECC enable      */
    while( (SLC_STAT & 0x01) != 1);
    return;
     
}

/*********************************************************************************************************
** Function name:       __SetPageAddr
** Descriptions:        ����ҳ��ַ
** input parameters:    pageaddr : ҳ��ַ
                        index    : �ֽ���Ҳ�еĿ�ʼ��ַ
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/
static uint8  __SetPageAddr(uint32 index ,uint32 pageaddr)
{
//    pageaddr += PAGE_OFFS;
	SLC_ADDR = (uint8)(index);                                          /*  �ֽ���ʼ�ֽ�                */
    SLC_ADDR = (uint8)(index >> 8) & 0x0f;
    /*  ����ҳ��ַ                 */
    SLC_ADDR = (uint8)(pageaddr);
    SLC_ADDR = (uint8)(pageaddr >> 8);
    SLC_ADDR = (uint8)(pageaddr >> 16);
    
    return TRUE;                                                         
}

/*********************************************************************************************************
** Function name:       __SetBlockAddr
** Descriptions:        ���ÿ��ַ
** input parameters:    block : ���ַ
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/
static uint8  __SetBlockAddr(uint32 blockaddr)
{
//    blockaddr += PAGE_OFFS;
    SLC_ADDR = (uint8)(blockaddr&0xff);
    SLC_ADDR = (uint8)(blockaddr >> 8);
    SLC_ADDR = (uint8)(blockaddr >> 16);
    
    return TRUE;                                                         
}


/*********************************************************************************************************
** Function name:       __ReadByte
** Descriptions:        ��ĳһҳ������
** input parameters:    index    : ��ҳ�еĿ�ʼ��ַ
                        pageaddr : ҳ��ַ
                        num      ����̵��ֽ���
                        *buff    : �������ָ��
** output parameters:   None
** Returned value:      TRUE/FALSE
*********************************************************************************************************/
uint8 __ReadByte(uint32 index, uint32 pageaddr, uint32 num, uint8 *buff)
{
    unsigned int i;
    if(buff == NULL)
        return FALSE;
    while((SLC_STAT&0x01) != 0x01);                                     /*  �ȴ�NAND ׼����             */ 
    
    SLC_CMD = NAND_CMD_READ0;                                           /*  ���Ͷ�A����������           */
    __SetPageAddr(index, pageaddr);                                     /*  ����ҳ��ַ                  */
   
    SLC_CMD = NAND_CMD_READSTART;

    while((SLC_STAT&0x02 != 0x02));
    for(i=0;i<num;i++)
        *buff++ = SLC_DATA; 
        
    return TRUE;
    
}


/*********************************************************************************************************
** Function name:       __ReadID
** Descriptions:        ��Nand ID
** input parameters:     *buff    : �������ָ��
** output parameters:   None
** Returned value:      TRUE/FALSE
*********************************************************************************************************/
uint8 __ReadID(uint8 *buff)
{
    unsigned int i;
    if(buff == NULL)
        return FALSE;
    while((SLC_STAT&0x01) != 0x01);                                     /*  �ȴ�NAND ׼����             */ 
    SLC_CMD = NAND_CMD_READID;                                          /*  ���Ͷ�ID����                */
    __SetPageAddr(0, 0);                                                /*  ����ҳ��ַ                  */
    
    while((SLC_STAT&0x02 != 0x02));
    for(i=0;i<4;i++)
        *buff++ = SLC_DATA; 
        
    return TRUE;      
}

/*********************************************************************************************************
** Function name:       __ProgramByte
** Descriptions:        ��ĳһҳ������
** input parameters:    index    : ��ҳ�еĿ�ʼ��ַ
                        pageaddr : ҳ��ַ
                        num      ����̵��ֽ���
                        *buff    : �������ָ��
** output parameters:   None
** Returned value:      TRUE/FALSE
*********************************************************************************************************/
uint8 __ProgramByte(uint32 index, uint32 pageaddr, uint32 num, uint8 *buff)
{
    unsigned int i;
    if (buff == NULL)
        return FALSE;
    if(index > 2048)
        return FALSE;
//     if(num > 2048)
//         return FALSE;
//             
    while((SLC_STAT&0x01) != 0x01);                                     /*  �ȴ�NAND ׼����             */ 
    SLC_CMD = NAND_CMD_READ0;                                           /*  ���Ͷ�A����������           */
    SLC_CMD = NAND_CMD_SDIN;
    __SetPageAddr(index, pageaddr);                                     /*  ����ҳ��ַ                  */
    

    for(i=0;i<num;i++)
        SLC_DATA = *buff++; 
    
    SLC_CMD = NAND_CMD_PAGEPROG;  
    
    SLC_CMD = NAND_CMD_STATUS;
    
    while((SLC_STAT & 0x01) == 0x01);   
    
         
    return TRUE;
    
}

/*********************************************************************************************************
** Function name:       __EraseBlock
** Descriptions:        ����ĳһ����
** input parameters:    addr : ����������ҳ�ĵ�ַ
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/
uint8 __EraseBlock(uint32 addr)
{
     
    while((SLC_STAT&0x01) != 0x01);                                     /*  �ȴ�NAND ׼�����           */ 
    SLC_CMD = NAND_CMD_ERASE1ST;                                        /*  д�������ָ��              */
    __SetBlockAddr(addr);                                          /*  ����ҳ��ַ                  */
    
    SLC_CMD = NAND_CMD_ERASE2ND;
            
    while((SLC_STAT & 0x01) == 0x01);
        
    return TRUE;
       
}
