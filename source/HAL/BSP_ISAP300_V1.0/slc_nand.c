/*****************************Copyright(c)****************************************************************
**                    Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                           http://www.embedtools.com
**
**------File Info-----------------------------------------------------------------------------------------
** File Name:            slc_nand.c
** Latest modified Date: 2008-11-10        
** Latest Version:       V1.0    
** Description:          nand flash 控制器接口函数
**                       
**--------------------------------------------------------------------------------------------------------
** Created By:           WangFaqiang 
** Created date:         2008-11-10    
** Version:              V1.0    
** Descriptions:         初始版本
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
** Descriptions:        __SLC NAND Flash 控制器初始化
** input parameters:    None
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/
void __SLCNANDInit(void)
{
    FLASHCLK_CTRL = 0x25;                                               /*  SLC时钟使能                 */
    SLC_CFG       = 0x00;                                               /*  ECC禁止，DMA禁止            */ 
//     SLC_TAC       = (uint32) ((15 << 0) |
//                     (15 << 4) |
//                     (15 << 8) |
//                     (15 << 12)|
//                     (15 << 16)|
//                     (15 << 20)|
//                     (15 << 24)|
//                     (0xf << 28) );                                         /*  配置读/写时钟               */
    SLC_TAC = 0xffffffff;                
    SLC_CFG       = 0x00;                                               /*  ECC enable      */
    while( (SLC_STAT & 0x01) != 1);
    return;
     
}

/*********************************************************************************************************
** Function name:       __SetPageAddr
** Descriptions:        设置页地址
** input parameters:    pageaddr : 页地址
                        index    : 字节在也中的开始地址
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/
static uint8  __SetPageAddr(uint32 index ,uint32 pageaddr)
{
//    pageaddr += PAGE_OFFS;
	SLC_ADDR = (uint8)(index);                                          /*  字节起始字节                */
    SLC_ADDR = (uint8)(index >> 8) & 0x0f;
    /*  发送页地址                 */
    SLC_ADDR = (uint8)(pageaddr);
    SLC_ADDR = (uint8)(pageaddr >> 8);
    SLC_ADDR = (uint8)(pageaddr >> 16);
    
    return TRUE;                                                         
}

/*********************************************************************************************************
** Function name:       __SetBlockAddr
** Descriptions:        设置块地址
** input parameters:    block : 块地址
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
** Descriptions:        读某一页的数据
** input parameters:    index    : 在页中的开始地址
                        pageaddr : 页地址
                        num      ：编程的字节数
                        *buff    : 存放数据指针
** output parameters:   None
** Returned value:      TRUE/FALSE
*********************************************************************************************************/
uint8 __ReadByte(uint32 index, uint32 pageaddr, uint32 num, uint8 *buff)
{
    unsigned int i;
    if(buff == NULL)
        return FALSE;
    while((SLC_STAT&0x01) != 0x01);                                     /*  等待NAND 准备好             */ 
    
    SLC_CMD = NAND_CMD_READ0;                                           /*  发送读A区数据命令           */
    __SetPageAddr(index, pageaddr);                                     /*  发送页地址                  */
   
    SLC_CMD = NAND_CMD_READSTART;

    while((SLC_STAT&0x02 != 0x02));
    for(i=0;i<num;i++)
        *buff++ = SLC_DATA; 
        
    return TRUE;
    
}


/*********************************************************************************************************
** Function name:       __ReadID
** Descriptions:        读Nand ID
** input parameters:     *buff    : 存放数据指针
** output parameters:   None
** Returned value:      TRUE/FALSE
*********************************************************************************************************/
uint8 __ReadID(uint8 *buff)
{
    unsigned int i;
    if(buff == NULL)
        return FALSE;
    while((SLC_STAT&0x01) != 0x01);                                     /*  等待NAND 准备好             */ 
    SLC_CMD = NAND_CMD_READID;                                          /*  发送读ID命令                */
    __SetPageAddr(0, 0);                                                /*  发送页地址                  */
    
    while((SLC_STAT&0x02 != 0x02));
    for(i=0;i<4;i++)
        *buff++ = SLC_DATA; 
        
    return TRUE;      
}

/*********************************************************************************************************
** Function name:       __ProgramByte
** Descriptions:        读某一页的数据
** input parameters:    index    : 在页中的开始地址
                        pageaddr : 页地址
                        num      ：编程的字节数
                        *buff    : 存放数据指针
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
    while((SLC_STAT&0x01) != 0x01);                                     /*  等待NAND 准备好             */ 
    SLC_CMD = NAND_CMD_READ0;                                           /*  发送读A区数据命令           */
    SLC_CMD = NAND_CMD_SDIN;
    __SetPageAddr(index, pageaddr);                                     /*  发送页地址                  */
    

    for(i=0;i<num;i++)
        SLC_DATA = *buff++; 
    
    SLC_CMD = NAND_CMD_PAGEPROG;  
    
    SLC_CMD = NAND_CMD_STATUS;
    
    while((SLC_STAT & 0x01) == 0x01);   
    
         
    return TRUE;
    
}

/*********************************************************************************************************
** Function name:       __EraseBlock
** Descriptions:        擦除某一个块
** input parameters:    addr : 块所在任意页的地址
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/
uint8 __EraseBlock(uint32 addr)
{
     
    while((SLC_STAT&0x01) != 0x01);                                     /*  等待NAND 准备完成           */ 
    SLC_CMD = NAND_CMD_ERASE1ST;                                        /*  写入擦除块指令              */
    __SetBlockAddr(addr);                                          /*  发送页地址                  */
    
    SLC_CMD = NAND_CMD_ERASE2ND;
            
    while((SLC_STAT & 0x01) == 0x01);
        
    return TRUE;
       
}
