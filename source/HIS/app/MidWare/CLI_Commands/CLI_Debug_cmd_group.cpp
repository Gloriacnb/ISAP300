/****************************************Copyright (c)*****************************************************************
**                           
**                               Easy Broadband Technology Co., Ltd.                       
**                         
**                                  http://www.ebbnetwork.com  
**
**
**--------------File Info----------------------------------------------------------------------------------------------
** File name:			CLI_Debug_cmd_group.c
** File path:			/ISAP100_Mainsoft/Src/EZSoft/CLI_Commands/CLI_Debug_cmd_group.c
** Created by:          Administrator
** Created date:        2012-12-6
** Version:             V0.1
** Descriptions:        
**
**---------------------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:				
** Descriptions:        
**
** Rechecked by:        
***********************************************************************************************************************/
#include "lpc_string.h"
#include "s1l_cmds.h"
#include "s1l_line_input.h"
#include "CLI_Debug_cmd_group.h"
#include "TerminalUart.h"
#include <string.h>
#include <assert.h>
#include "CBaseSlot.h"
#include "CBaseCard.h"

#include "SlotModule.h"
#include "RegisterAccess.h"
#include <stdio.h>
#include "ChipPCMLogic.h"
#include "ObjectReference.h"
#include "CardXC.h"
#include "Card30VF.h"
#include <stdio.h>
#include "VFCmdRegRead.h"
#include "VFCmdRegWrite.h"
#include "VFCmdCoeRead.h"
#include "VFCmdCoeWrite.h"
#include "net_api.h"
#include "TopoManager.h"
#include "Uart1.h"
#include "ProtectAssistCell.h"

//#include "MidWatchDog.h"

#include "Card16FE1.h"
#include "CardDXC.h"
#include "SwTrace.h"
#include "SwitchCore.h"

#include "PriPacket.h"
//void out_trace( Trace_T * trace, uint8* sbuf);

static BOOL_32 cmd_bread(void);
static BOOL_32 cmd_bwrite(void);
static BOOL_32 cmd_dbread(void);
static BOOL_32 cmd_dbwrite(void);
static BOOL_32 cmd_dcc_test(void);
static BOOL_32 cmd_sabit_test(void);
static BOOL_32 cmd_dcn_test(void);
static BOOL_32 cmd_vfread(void);
static BOOL_32 cmd_vfwrite(void);
static BOOL_32 cmd_reset(void);
static BOOL_32 cmd_tx2fan(void);
static BOOL_32 cmd_ping(void);
static BOOL_32 cmd_udpsend(void);

static BOOL_32 cmd_omuswitch(void);

static BOOL_32 cmd_coeread(void);
static BOOL_32 cmd_coewrite(void);

static BOOL_32 cmd_swtrace(void);

//static BOOL_32 cmd_slotinfo(void);
/* Peek saved data */
static UNS_32 last_addr = 0;
static UNS_32 last_width = 4;





/* bread command */
static UNS_32 cmd_bread_plist[] =
{
    (PARSE_TYPE_STR), /* The "bread" command */
    (PARSE_TYPE_HEX | PARSE_TYPE_OPT),
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),
    (PARSE_TYPE_DEC | PARSE_TYPE_END)
};
static CMD_ROUTE_T dbg_bread_cmd =
{
    (UNS_8 *) "bread",
    cmd_bread,
    (UNS_8 *) "read a byte from cpld register",
    (UNS_8 *) "bread [regaddr][bytenum][number of replication]",
    cmd_bread_plist,
    (CMD_ROUTE_E*)NULL
};

/* bwrite command */
static UNS_32 cmd_bwrite_plist[] =
{
    (PARSE_TYPE_STR), /* The "bwrite" command */
    (PARSE_TYPE_HEX),
    (PARSE_TYPE_HEX | PARSE_TYPE_END)
};
static CMD_ROUTE_T dbg_bwrite_cmd =
{
    (UNS_8 *) "bwrite",
    cmd_bwrite,
    (UNS_8 *) "write a byte to register",
    (UNS_8 *) "bwrite [regaddr][hex value]",
    cmd_bwrite_plist,
    (CMD_ROUTE_E*)NULL
};


/* dbread command */
static UNS_32 cmd_dbread_plist[] =
{
    (PARSE_TYPE_STR), /* The "dbread" command */
    (PARSE_TYPE_DEC),
    (PARSE_TYPE_HEX | PARSE_TYPE_OPT),
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),
    (PARSE_TYPE_HEX | PARSE_TYPE_END)
};
static CMD_ROUTE_T dbg_dbread_cmd =
{
    (UNS_8 *) "dbr",
    cmd_dbread,
    (UNS_8 *) "read a word from chip register",
    (UNS_8 *) "dbr [CS][regaddr][wordsnum][number of replication][Compare Data]\r\n"
              "Note: �Ĵ�����ַ����EZBUS��ַ�滮������Ҫ���⴦��",
    cmd_dbread_plist,
    (CMD_ROUTE_E*)NULL
};

/* bwrite command */
static UNS_32 cmd_dbwrite_plist[] =
{
    (PARSE_TYPE_STR), /* The "dbwrite" command */
    (PARSE_TYPE_DEC),
    (PARSE_TYPE_HEX),                   //ADDR
    (PARSE_TYPE_HEX | PARSE_TYPE_OPT),  //DATA
    (PARSE_TYPE_DEC | PARSE_TYPE_END)   //LOOP check
};
static CMD_ROUTE_T dbg_dbwrite_cmd =
{
    (UNS_8 *) "dbw",
    cmd_dbwrite,
    (UNS_8 *) "write a byte to register",
    (UNS_8 *) "dbw [CS][regaddr][hex value][Repeat check]",
    cmd_dbwrite_plist,
    (CMD_ROUTE_E*)NULL
};


/* Peek command */
static BOOL_32 cmd_peek(void);
static UNS_32 cmd_peek_plist[] =
{
	(PARSE_TYPE_STR | PARSE_TYPE_OPT), /* The "peek" command */
	(PARSE_TYPE_HEX | PARSE_TYPE_OPT),
	(PARSE_TYPE_DEC | PARSE_TYPE_END)
};
static CMD_ROUTE_T core_peek_cmd =
{
	(UNS_8 *) "peek",
	cmd_peek,
	(UNS_8 *) "Displays a byte, half-word, or word value at an address",
	(UNS_8 *) "peek <hex address><1, 2, or 4 bytes>",
	cmd_peek_plist,
	(CMD_ROUTE_E*)NULL
};

/* Poke command */
static BOOL_32 cmd_poke(void);
static UNS_32 cmd_poke_plist[] =
{
	(PARSE_TYPE_STR), /* The "poke" command */
	(PARSE_TYPE_HEX),
	(PARSE_TYPE_HEX),
	(PARSE_TYPE_DEC | PARSE_TYPE_END)
};
static CMD_ROUTE_T core_poke_cmd =
{
	(UNS_8 *) "poke",
	cmd_poke,
	(UNS_8 *) "Places a byte, half-word, or word value at an address",
	(UNS_8 *) "poke [hex address][hex value][1, 2, or 4 bytes]",
	cmd_poke_plist,
	(CMD_ROUTE_E*)NULL
};

/* Fill command */
static BOOL_32 cmd_fill(void);
static UNS_32 cmd_fill_plist[] =
{
	(PARSE_TYPE_STR), /* The "fill" command */
	(PARSE_TYPE_HEX),
	(PARSE_TYPE_HEX),
	(PARSE_TYPE_HEX),
	(PARSE_TYPE_DEC | PARSE_TYPE_END)
};
static CMD_ROUTE_T core_fill_cmd =
{
	(UNS_8 *) "fill",
	cmd_fill,
	(UNS_8 *) "Fills a range of memory with a value",
	(UNS_8 *) "fill [hex address][bytes to fill(hex)][hex value][1, 2, or 4 bytes]",
	cmd_fill_plist,
	(CMD_ROUTE_E*)NULL
};

/*
 * rprint command ��ӡ�Ĵ���ֵ
 */
static BOOL_32 cmd_rprint(void);
static UNS_32 cmd_rprint_plist[] =
{
    (PARSE_TYPE_STR), /* The "rprint" command */
    (PARSE_TYPE_DEC),                       //slot
    (PARSE_TYPE_HEX | PARSE_TYPE_OPT),      //reg addr
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),      //number
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),      //stdio or file
    (PARSE_TYPE_STR | PARSE_TYPE_END)       //file name
};
static CMD_ROUTE_T debug_rprint_cmd =
{
    (UNS_8 *) "rprint",
    cmd_rprint,
    (UNS_8 *) "print a range of register's value(fix 16bit width).",
    (UNS_8 *)   "rprint [slot][regaddr begin][number to print][stdio/file][file name]\r\n"
                "\t[slot]              : 0 =  main XC, 1 = backup XC, 2~6 = extend card 1~5\r\n"
                "\t[[regaddr begin]    : must hex value\r\n"
                "\t[number to print]   : it's optional, default 1, must dec value\r\n"
                "\t[stdio/file]        : where print the register's value to.\r\n"
                "\t\t\t it's optional, default stdio, 0 = stdio, 1 = file\r\n"
                "\t[file name]         : if select [stdio/file] = file, define file name\r\n"
                "\t\t\t it's optional, default name fomat slot_n_reg_record.txt\r\n",
    cmd_rprint_plist,
    (CMD_ROUTE_E*)NULL
};

/*
 * rcfg command config register from config file
 */
static BOOL_32 cmd_rcfg(void);
static UNS_32 cmd_rcfg_plist[] =
{
    (PARSE_TYPE_STR), /* The "rprint" command */
    (PARSE_TYPE_DEC),                       //slot
    (PARSE_TYPE_STR | PARSE_TYPE_END)       //file name
};
static CMD_ROUTE_T debug_rcfg_cmd =
{
    (UNS_8 *) "rcfg",
    cmd_rcfg,
    (UNS_8 *) "config register from config file.",
    (UNS_8 *)   "rcfg [slot][file name]\r\n"
                "\t[slot]              : 0 =  main XC, 1 = backup XC, 2~6 = extend card 1~5\r\n"
                "\t[file name]         : config file format w 6000 0002\r\n",
    cmd_rcfg_plist,
    (CMD_ROUTE_E*)NULL
};


static UNS_32 cmd_dcc_test_plist[] =
{
    (PARSE_TYPE_STR ), /* The "dcctest" command */
    (PARSE_TYPE_STR ), /* read or write*/
    (PARSE_TYPE_DEC ), /* slot */
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT), /* ch */
    (PARSE_TYPE_DEC | PARSE_TYPE_END) /* packet len */
};
static CMD_ROUTE_T cmd_dcc_test_cmd =
{
    (UNS_8 *) "dcctest",
    cmd_dcc_test,
    (UNS_8 *) "send a fix packet at selected dcc channel.",
    (UNS_8 *) "dcctest <read/write><dcc channel>",
    cmd_dcc_test_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_sabit_test_plist[] =
{
    (PARSE_TYPE_STR ), /* The "sabittest" command */
    (PARSE_TYPE_STR ), /* read or write*/
    (PARSE_TYPE_DEC ), /* slot */
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT), /* ch */
    (PARSE_TYPE_DEC | PARSE_TYPE_END) /* packet len */
};
static CMD_ROUTE_T cmd_sabit_test_cmd =
{
    (UNS_8 *) "sabittest",
    cmd_sabit_test,
    (UNS_8 *) "send a fix packet at selected sabit channel.",
    (UNS_8 *) "sabittest <read/write><dcc channel>",
    cmd_sabit_test_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_dcn_test_plist[] =
{
    (PARSE_TYPE_STR ), /* The "dcntest" command */
    (PARSE_TYPE_STR ), /* read or write*/
    (PARSE_TYPE_DEC ), /* slot */
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT), /* ch */
    (PARSE_TYPE_DEC | PARSE_TYPE_END) /* packet len */
};
static CMD_ROUTE_T cmd_dcn_test_cmd =
{
    (UNS_8 *) "dcntest",
    cmd_dcn_test,
    (UNS_8 *) "send a fix packet at selected dcn channel.",
    (UNS_8 *) "dcntest <read/write><dcc channel>",
    cmd_dcn_test_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_vfread_plist[] =
{
    (PARSE_TYPE_STR ), /* The "vfread" command */
    (PARSE_TYPE_DEC ), /* slot */
    (PARSE_TYPE_DEC ), /* chip */
    (PARSE_TYPE_DEC ), /* ch */
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),/* start register addr*/
    (PARSE_TYPE_DEC | PARSE_TYPE_END) /* register number */
};
static CMD_ROUTE_T cmd_vfread_cmd =
{
    (UNS_8 *) "vfread",
    cmd_vfread,
    (UNS_8 *) "read the 821054's register. ",
    (UNS_8 *) "vfread <slot><chip><channel><regaddr><number>\r\n"
                "<slot>     : [2, 6]\r\n"
                "<chip>     : [0, 7]\r\n"
                "<channel>  : [0, 6], 0 is global, 1~4 local, 5 AD, 6 DA\r\n"
                "<regaddr>  : \r\n"
                "<number>   : it's optional\r\n",
    cmd_vfread_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_vfwrite_plist[] =
{
    (PARSE_TYPE_STR ), /* The "vfwrite" command */
    (PARSE_TYPE_DEC ), /* slot */
    (PARSE_TYPE_DEC ), /* chip */
    (PARSE_TYPE_DEC ), /* ch */
    (PARSE_TYPE_DEC ),/* start register addr*/
    (PARSE_TYPE_HEX | PARSE_TYPE_END) /* register value */
};
static CMD_ROUTE_T cmd_vfwrite_cmd =
{
    (UNS_8 *) "vfwrite",
    cmd_vfwrite,
    (UNS_8 *) "write the 821054's register. ",
    (UNS_8 *) "vfwrite <slot><chip><channel><regaddr><regVal>\r\n"
                "<slot>     : [2, 6]\r\n"
                "<chip>     : [0, 7]\r\n"
                "<channel>  : [0, 6], 0 is global, 1~4 local\r\n"
                "<regaddr>  : \r\n"
                "<regVal>   : it's optional\r\n",
    cmd_vfwrite_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_coeread_plist[] =
{
    (PARSE_TYPE_STR ), /* The "coeread" command */
    (PARSE_TYPE_DEC ), /* slot */
    (PARSE_TYPE_DEC ), /* chip */
    (PARSE_TYPE_DEC ), /* idt ch */
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),/* coeRam block*/
    (PARSE_TYPE_DEC | PARSE_TYPE_END) /* word count default 8 */
};

static CMD_ROUTE_T coeread_cmd =
{
    (UNS_8 *) "coeread",
    cmd_coeread,
    (UNS_8 *) "Read a block of 82V1054 coe ram.",
    (UNS_8 *) "coeread <slot><chip><idtch><block><wordcnt>\r\n"
              " [slot] :    2~6\r\n"
              " [chip] :    0~7\r\n"
              " [idtch]:    0~3\r\n"
              " [block]:    0~4\r\n"
              " [wordcnt]:  1~8 option default:8 \r\n",
   cmd_coeread_plist,
    (CMD_ROUTE_E*)NULL
};


static UNS_32 cmd_coewrite_plist[] =
{
    (PARSE_TYPE_STR), /* The "coewrite" command */
    (PARSE_TYPE_DEC),   //slot
    (PARSE_TYPE_DEC),   //chip
    (PARSE_TYPE_DEC),   //idt ch
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),  //coeRam block
    (PARSE_TYPE_STR | PARSE_TYPE_END)
};

static CMD_ROUTE_T coewrite_cmd =
{
    (UNS_8 *) "coewrite",
    cmd_coewrite,
    (UNS_8 *) "write a block of 82V1054 coe ram.",
    (UNS_8 *) "coewrite [slot][chip][idtch][block]\r\n"
              " [slot] :    0~3\r\n"
              " [chip] :    0~1\r\n"
              " [idtch]:    0~3\r\n"
              " [block]:    0~4\r\n"
              " [file] :    config file name\r\n",
    cmd_coewrite_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_reset_plist[] =
{
    (PARSE_TYPE_STR ), // The "reset" command 
    (PARSE_TYPE_DEC | PARSE_TYPE_OPT),// choose which slot to reset
    (PARSE_TYPE_DEC | PARSE_TYPE_END) // warm(0) or cold(1default)
};
static CMD_ROUTE_T cmd_reset_cmd =
{
    (UNS_8 *) "reset",
    cmd_reset,
    (UNS_8 *) "reset a card of slot. ",
    (UNS_8 *) "reset <slot><way>\r\n"
                "<slot> : [0, 6]\r\n"
                "<way>  : it's optional,1 is default cold start,0 is warm start.\r\n",
    cmd_reset_plist,
    (CMD_ROUTE_E*)NULL
};
static UNS_32 cmd_tx2fan_plist[] =
{
    (PARSE_TYPE_STR),
    (PARSE_TYPE_STR| PARSE_TYPE_END)
};
static CMD_ROUTE_T cmd_tx2fan_cmd =
{
    (UNS_8 *) "tx2fan",
    cmd_tx2fan,
    (UNS_8 *) "Send string to Fan,receive same string from fan ",
    (UNS_8 *) "tx2fan <sendinfo>\r\n",
    cmd_tx2fan_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_ping_plist[] =
{
    (PARSE_TYPE_STR ), // The "ping" command
    (PARSE_TYPE_STR | PARSE_TYPE_END) // ipaddress
};
static CMD_ROUTE_T cmd_ping_cmd =
{
    (UNS_8 *) "ping",
    cmd_ping,
    (UNS_8 *) "ping a host. ",
    (UNS_8 *) "ping <host>\r\n"
                "<host>  : ip address.\r\n",
    cmd_ping_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_udp_plist[] =
{
    (PARSE_TYPE_STR ), // The "udp" command
    (PARSE_TYPE_STR ),
    (PARSE_TYPE_STR | PARSE_TYPE_END) // ipaddress
};
static CMD_ROUTE_T cmd_udp_cmd =
{
    (UNS_8 *) "udpsend",
    cmd_udpsend,
    (UNS_8 *) "send a udp packet. ",
    (UNS_8 *) "udpsend <host><data>\r\n"
                "<host>  : ip address.\r\n"
                "<data>  : data to sending",
    cmd_udp_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_omuswitch_plist[] =
{
    (PARSE_TYPE_STR ), // The "omuswitch" command
    (PARSE_TYPE_DEC | PARSE_TYPE_END) // ipaddress
};
static CMD_ROUTE_T cmd_omuswitch_cmd =
{
    (UNS_8 *) "omuswitch",
    cmd_omuswitch,
    (UNS_8 *) "change omu work mode. ",
    (UNS_8 *) "omuswitch <mode>\r\n"
              "<mode>  : 0 work, 1 standby, 2 auto",
    cmd_omuswitch_plist,
    (CMD_ROUTE_E*)NULL
};

static UNS_32 cmd_swtrace_plist[] =
{
    (PARSE_TYPE_STR ), // The "swtrace" command
    (PARSE_TYPE_STR ),
    (PARSE_TYPE_HEX | PARSE_TYPE_END)
};
static CMD_ROUTE_T cmd_swtrace_cmd =
{
    (UNS_8 *) "swtrace",
    cmd_swtrace,
    (UNS_8 *) "trace the soft switch input & output.",
    (UNS_8 *) "swtrace [operation][hex port bitmap]\r\n"
                "\t[operation] read/clear/reset/macport/ipport/maxbroadcast\r\n"
                "\t            sysclk/totalinfo/actinfo\r\n"
                "\t[port] bit0~19 = port0~19.\r\n",
    cmd_swtrace_plist,
    (CMD_ROUTE_E*)NULL
};


/* Debug group */
GROUP_LIST_T debug_group =
{
	(UNS_8 *) "debug", /* Debug group */
	(UNS_8 *) "ISAP100 debug command group",
	(CMD_ROUTE_E*)NULL,
	(GROUP_LIST_E*)NULL
};


/***********************************************************************
 *
 * Function: hex_dump
 *
 * Purpose: Dumps a series of data values
 *
 * Processing:
 *     Format and dump data based on address and width.
 *
 * Parameters:
 *     addr     : Starting address of dump
 *     width    : Width of the dump in bytes (1, 2, or 4)
 *     numtrans : Number of items to dump
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void hex_dump(UNS_32 addr,
					 UNS_32 width,
					 UNS_32 numtrans)
{
	int dumpsperline, ldspl, idx;
	UNS_32 tmp, laddr;
	UNS_8 daddr [16];

	/* Based on width, determine number of dumps per line */
	switch (width)
	{
		case 4:
			dumpsperline = 4;
			break;

		case 2:
			dumpsperline = 8;
			break;

		case 1:
		default:
			width = 1;
			dumpsperline = 8;
			break;
	}
	laddr = addr + (width * numtrans);

	/* Start dump */
	while (addr < laddr)
	{
		/* Dump address first */
		str_makehex(daddr, addr, 8);
		term_dat_out(daddr);
		term_dat_out((UNS_8 *) " : ");

		ldspl = dumpsperline;
		if (ldspl > numtrans)
		{
			ldspl = numtrans;
		}
		numtrans = numtrans - ldspl;

		/* Dump data in the line */
		for (idx = 0; idx < ldspl; idx++)
		{
			if (width == 4)
			{
				tmp = * (UNS_32 *) addr;
				addr = addr + 4;
			}
			else if (width == 2)
			{
				tmp = (UNS_32) (* (UNS_16 *) addr);
				addr = addr + 2;
			}
			else
			{
				tmp = (UNS_32) (* (UNS_8 *) addr);
				addr = addr + 1;
			}

			str_makehex(daddr, tmp, (width * 2));
			term_dat_out(daddr);
			term_dat_out((UNS_8 *) " ");
		}

		term_dat_out_crlf((UNS_8 *) "");
	}
}

    /***********************************************************************
 *
 * Function: cmd_peek
 *
 * Purpose: Peek command
 *
 * Processing:
 *     Parse the string elements for the peek command and display the
 *     peeked value.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: TRUE if the command was processed, otherwise FALSE
 *
 * Notes: None
 *
 **********************************************************************/
static BOOL_32 cmd_peek(void)
{
	UNS_32 newaddr, newwidth;

	/* If only 1 parsed command, then print at current address and
	   width */
	if (parse_get_entry_count() > 1)
	{
		/* Get address */
		newaddr = cmd_get_field_val(1);

		/* Width */
		newwidth = last_width;
		if (parse_get_entry_count() >= 3)
		{
			newwidth = cmd_get_field_val(2);
			if (!((newwidth == 1) || (newwidth == 2) ||
				(newwidth == 4)))
			{
				newwidth = 1;
			}
		}

		last_addr = newaddr;
		last_width = newwidth;
	}

	/* Based on width, limit address start */
	last_addr = last_addr & ~(last_width - 1);

	/* Dump data */
	hex_dump(last_addr, last_width, 1);

	return TRUE;
}

/***********************************************************************
 *
 * Function: cmd_poke
 *
 * Purpose: Poke command
 *
 * Processing:
 *     Parse the string elements for the poke command and update
 *     requested memory.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: TRUE if the command was processed, otherwise FALSE
 *
 * Notes: None
 *
 **********************************************************************/
static BOOL_32 cmd_poke(void)
{
	UNS_32 addr, width, fillval;

	/* Get address */
	addr = cmd_get_field_val(1);

	/* Hex fill value */
	fillval = cmd_get_field_val(2);

	/* Width */
	width = cmd_get_field_val(3);

	if (!((width == 1) || (width == 2) || (width == 4)))
	{
		width = 1;
	}

	/* Limit addressing size */
	addr = addr & ~(width - 1);

	if (width == 1)
	{
		UNS_8 *faddr = (UNS_8 *) addr;
		*faddr = (UNS_8) fillval;
	}
	else if (width == 2)
	{
		UNS_16 *faddr = (UNS_16 *) addr;
		*faddr = (UNS_16) fillval;
	}
	else
	{
		UNS_32 *faddr = (UNS_32 *) addr;
		*faddr = fillval;
	}

	return TRUE;
}

/***********************************************************************
 *
 * Function: cmd_fill
 *
 * Purpose: Fill command
 *
 * Processing:
 *     Parse the string elements for the fill command and fill the
 *     requested data range with data.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: TRUE if the command was processed, otherwise FALSE
 *
 * Notes: None
 *
 **********************************************************************/
static BOOL_32 cmd_fill(void) {
	UNS_32 addr, width, bytes, fillval;

	/* Get address */
	addr = cmd_get_field_val(1);

	/* Number of bytes */
	bytes = cmd_get_field_val(2);

	/* Hex fill value */
	fillval = cmd_get_field_val(3);

	/* Width */
	width = cmd_get_field_val(4);

	if (!((width == 1) || (width == 2) || (width == 4)))
	{
		width = 1;
	}

	/* Limit number of bytes based on addressing size */
	bytes = bytes & ~(width - 1);
	addr = addr & ~(width - 1);

	if (width == 1)
	{
		UNS_8 *faddr = (UNS_8 *) addr;
		while (bytes > 0)
		{
			*faddr = (UNS_8) fillval;
			faddr++;
			bytes--;
		}
	}
	else if (width == 2)
	{
		UNS_16 *faddr = (UNS_16 *) addr;
		while (bytes > 0)
		{
			*faddr = (UNS_16) fillval;
			faddr++;
			bytes -= 2;
		}
	}
	else
	{
		UNS_32 *faddr = (UNS_32 *) addr;
		while (bytes > 0)
		{
			*faddr = fillval;
			faddr++;
			bytes -= 4;
		}
	}

	return TRUE;
}



/***********************************************************************
 *
 * Function: cmd_bread
 *
 * Purpose: Read static memory CS2 register command
 *
 * Processing:
 *     read the registers value of generalLogic
  *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: TRUE if the command was processed, otherwise FALSE
 *
 * Notes: None
 *
 **********************************************************************/
BOOL_32 cmd_bread(void) {
    UNS_32 iters, hexaddr, bytes;

    hexaddr = cmd_get_field_val(1) + 0xe2000000;
    if( hexaddr >= 0xe3000000 ) {
        return FALSE;
    }
    bytes = 1;
    iters = 1;
    if( parse_get_entry_count() > 2 ) {
        bytes = cmd_get_field_val(2);
        if( parse_get_entry_count() > 3 ) {
            iters = cmd_get_field_val(3);
        }
    }
    while( iters > 0 ) {
        hex_dump(hexaddr, 1, bytes);
        iters--;
    }

    return TRUE;
}


/***********************************************************************
 *
 * Function: cmd_bwrite
 *
 * Purpose: Write EMC register command
 *
 * Processing:
 *     write the register value of general logic
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: TRUE if the command was processed, otherwise FALSE
 *
 * Notes: None
 *
 **********************************************************************/
BOOL_32 cmd_bwrite(void) {
    UNS_32 hexaddr, value;
    hexaddr = cmd_get_field_val(1) + 0xe2000000;
    if( hexaddr >= 0xe3000000 ) {
        return FALSE;
    }
   value = cmd_get_field_val(2) & 0xff;

    *((UNS_8 *) hexaddr) = (UNS_8)value;

    return TRUE;
}

/***********************************************************************
 *
 * Function: cmd_dbread
 *
 * Purpose: Read static memory CS3 register command
 *
 * Processing:
 *     read the registers value of EZBUS
  *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: TRUE if the command was processed, otherwise FALSE
 *
 * Notes: None
 *
 **********************************************************************/
BOOL_32 cmd_dbread(void) {
    UNS_32 iters, hexaddr, words, csaddr, cmp;
    UNS_16 rval;
    UNS_8 cs = cmd_get_field_val(1);

    hexaddr = (cmd_get_field_val(2) << 1) + 0xe3000000;
    if( hexaddr > 0xe3ffffff ) {
        return FALSE;
    }
    words = 1;
    iters = 1;
    cmp = 0xffffffff;
    if( parse_get_entry_count() > 3 ) {
        words = cmd_get_field_val(3);
        if( parse_get_entry_count() > 4 ) {
            iters = cmd_get_field_val(4);
            if( parse_get_entry_count() > 5 ) {
                cmp = cmd_get_field_val(5);
            }
        }
    }

    csaddr = 0xe2000000;
//    tsk_lock();
    *((UNS_8*)csaddr) = cs;//address select
    while( iters > 0 ) {
        if( cmp == 0xffffffff ) {
            hex_dump(hexaddr, 2, words);
        }
        else {
            rval = *((UNS_16*)hexaddr);
            if( rval != cmp ) {
                printf("!!!Not equal at Addr: %04x, Val : %04x!!!\n", hexaddr, rval);
            }
        }
        iters--;
    }
//    tsk_unlock();

    return TRUE;
}


/***********************************************************************
 *
 * Function: cmd_dbwrite
 *
 * Purpose: Write EMC cs3 static memory register command
 *
 * Processing:
 *     write the register value of EZBUS
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: TRUE if the command was processed, otherwise FALSE
 *
 * Notes: None
 *
 **********************************************************************/
BOOL_32 cmd_dbwrite(void) {
    UNS_32 hexaddr, value, csaddr;
    UNS_8 cs = cmd_get_field_val(1);
    hexaddr = (cmd_get_field_val(2) << 1) + 0xe3000000;
    if( hexaddr > 0xe3ffffff ) {
        return FALSE;
    }
    value = cmd_get_field_val(3) & 0xffff;

    csaddr = 0xe2000000;

//    tsk_lock();
    *((UNS_8*)csaddr) = cs;//address select
    *((UNS_16 *) hexaddr) = (UNS_16)value;
//    tsk_unlock();

    if( parse_get_entry_count() > 4 ) {
        UNS_32 loop = cmd_get_field_val(4);
        for (int i = 0; i < loop; ++i) {
//            tsk_lock();
            *((UNS_8*)csaddr) = cs;//address select
            *((UNS_16 *) hexaddr) = (UNS_16)value;
            UNS_16 real = *((UNS_16 *) hexaddr);
//            tsk_unlock();
            if( value != real ) {
                printf("\n!!!loop %d failed!!! expect: %2x, real: %2x\n", i, value, real);
            }
        }
    }

    return TRUE;
}


BOOL_32 cmd_rprint(void) {
    UNS_8 slot = cmd_get_field_val(1);
    UNS_32 regBegin = cmd_get_field_val(2);
    UNS_32 regNum = 1;
    uint8 outSel = 0;
    char* fname = 0;
    if( parse_get_entry_count() > 3 ) {
        regNum = cmd_get_field_val(3);
        if( parse_get_entry_count() > 4 ) {
            outSel = cmd_get_field_val(4);
            if( parse_get_entry_count() > 5 && outSel == 1 ) {
                fname = (char*)cmd_get_field_val(5);
           }
        }
    }

    CBaseSlot* pslt = SlotModule::getSlot(slot);
    if( !pslt || !pslt->cardExist() ) {
		printf("!!!No Card!!!.\n");
        return FALSE;
    }
    if( outSel == 0 ) {
        for( int i = 0; i < regNum; i++ ) {
            uint16 currentAddr = i+regBegin;
            uint16 currentVal  = pslt->getRegisterAccess()->readReg(regBegin+i);
            printf("Addr: %04x, Val : %04x\n", currentAddr, currentVal);
        }
    }
    else {
        FILE* f = 0;
        if( fname ) {
            f = fopen(fname, "w");
        }
        else {
            char name[30];
            sprintf( name, "slot_%d_reg_record.txt\n", slot);
            f = fopen(name, "w");
        }
        if( f ) {
            for( int i = 0; i < regNum; i++ ) {
                uint16 currentAddr = i+regBegin;
                uint16 currentVal  = pslt->getRegisterAccess()->readReg(regBegin+i);
                fprintf(f, "Addr: %04x, Val : %04x\n", currentAddr, currentVal);
            }
            fclose(f);
        }
    }

    printf("\nRegister value print over, all of result above!!\n");
    return TRUE;
}


BOOL_32 cmd_rcfg(void) {
    UNS_8 slot = cmd_get_field_val(1);

    CBaseSlot* pslt = SlotModule::getSlot(slot);
    if( !pslt || !pslt->cardExist() ) {
        printf("!!!No Card!!!.\n");
        return FALSE;
    }

    char* fname = (char*)cmd_get_field_val(2);
    FILE* f = fopen(fname, "r");
    int regAddr = 0;
    int regValue = 0;
    if( f ) {
        char line[20];
//        while( fscanf(f, "w %4x %4x\n", &regAddr, &regValue) != 0 ) {
//            pslt->getRegisterAccess()->writeReg(regAddr, regValue);
//        }
        while( fgets(line, sizeof(line), f) != 0 ) {
            sscanf(line, "w %4x %4x", &regAddr, &regValue);
            pslt->getRegisterAccess()->writeReg(regAddr, regValue);
        }
        fclose(f);
        return TRUE;
    }
    return FALSE;
}



static BOOL_32 cmd_dcc_test(void) {
    char* option = (char*)cmd_get_field_val(1);
    UNS_8 slot = cmd_get_field_val(2);
    UNS_8 ch = cmd_get_field_val(3);
    NM_CH_E channel;
    if( ch == 0 ) {
        channel = DCC_A;
    }
    else {
        channel = DCC_B;
    }
    int rdLen;
    static const uint8 col = 8;
    static UNS_32 head = 0;
    UNS_8 line;
    int i,j,point;
    UNS_8 daddr[10];
    UNS_8 rdbuf[1024];
//    static UNS_8 packet[] = {
//            0x00, 0x00, 0x00, 0x18, 0x00, 0x2c, 0x01, 0x00,
//            0x00, 0x0d, 0x50, 0xe5, 0x49, 0x83, 0x11, 0xf0,
//            0x08, 0x00, 0x45, 0x00, 0x00, 0x3c, 0x62, 0xc5,
//            0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xC0, 0xA8,
//            0x64, 0x13, 0xC0, 0xA8, 0x64, 0x0d, 0x08, 0x00,
//            0x6c, 0x79, 0x00, 0x01, 0x3B, 0x2F, 0x30, 0x30,
//            0x02, 0x01, 0x00, 0x04, 0x06, 0x70, 0x75, 0x62,
//            0x6C, 0x69, 0x63, 0xA2, 0x23, 0x02, 0x04, 0x5D,
//            0x83, 0x28, 0x25, 0x02, 0x01, 0x00, 0x02, 0x01,
//            0x00, 0x30, 0x15, 0x30, 0x13, 0x06, 0x08, 0x2B,
//            0x06, 0x01, 0x02, 0x01, 0x01, 0x01, 0x00, 0x04,
//            0x07, 0x49, 0x53, 0x41, 0x50, 0x31, 0x30, 0x30,
////          0xaa, 0x55, 0x7e, 0x7d
//    };
    if( strcmp(option, "read") == 0 ) {
        CardXC* pxc = ObjectReference::getPXCCard(slot);
        if( !pxc ) {
            return FALSE;
        }
        if( !pxc->getChipPCMLogic().ifHavePacket(channel) ) {
            printf("\nno packet received!\n");
        }
        else {
            rdLen = pxc->getChipPCMLogic().getPacketLen(channel);
            if( rdLen <= 0 || rdLen > sizeof(rdbuf) ) {
                printf("\noutof range, length is %d\n", rdLen);
                pxc->getChipPCMLogic().clearDccRdBufForce(channel);
                return FALSE;
            }
            pxc->getChipPCMLogic().GetDCCPacket(channel, rdbuf, rdLen);
            pxc->getChipPCMLogic().clearDccRdBufForce(channel);
            printf("\nRecieved Packet: %4x, length is %d\n",
                    (rdbuf[0]<<24)|(rdbuf[1]<<16)|(rdbuf[2]<<8)|(rdbuf[3]<<0), rdLen);

            if( parse_get_entry_count() > 4 ) {
               if( rdLen % col ) {
                    line = rdLen / col + 1;
                }
                else {
                    line = rdLen / col;
                }
                for( i = 0; i < line; i++ ) {
                    for( j = 0; j < col; j++ ) {
                        point = i*col + j;
                        if( point >= rdLen ) {
                            return TRUE;
                        }
                        str_makehex(daddr, rdbuf[point], 2);
                        term_dat_out(daddr);
                        term_dat_out((UNS_8 *) " ");
                    }
                    term_dat_out_crlf((UNS_8*)"");
                }
            }
        }

    }
    else if( strcmp(option, "write") == 0 ) {
        UNS_16 len = 250;
        UNS_8 loop = 1;
        if( parse_get_entry_count() > 4 ) {
            loop = cmd_get_field_val(4);
        }
//        else {
//            len = 96;
//        }
		CardXC* pxc = ObjectReference::getPXCCard(slot);
		if( !pxc ) {
			return FALSE;
		}

		for (int i = 0; i < loop; ++i) {
            rdbuf[0] = (head >> 24) & 0xff;
            rdbuf[1] = (head >> 16) & 0xff;
            rdbuf[2] = (head >> 8) & 0xff;
            rdbuf[3] = (head >> 0) & 0xff;
            for (int j = 0; j < len; ++j) {
                rdbuf[j+4] = j;
            }
            pxc->getChipPCMLogic().DccSendPacket(channel, rdbuf, len+4);
            printf("\n Packet %4x send succeed!\n", head);
            head++;
        }
    }
    else if( strcmp(option, "topo") == 0 ) {
        TopoManager::startTopoSearch();
    }
//    else if ( strcmp(option, "trace") == 0 ) {
//        if( ch == 88 ) {
//            int_disable_eth_real();
//            Dcc_interrupt_disable();
//            tsk_lock();
//            memset(&eth_trace, 0, sizeof(eth_trace));
//            memset(&dcc_trace, 0, sizeof(eth_trace));
//            stack_res_eth = 0;
//            stack_res_dcc = 0;
//            tsk_unlock();
//            Dcc_interrupt_enable();
//            int_enable_eth_real();
//        }
//        term_dat_out_crlf((UNS_8*)"eth trace out:");
//        out_trace(&eth_trace, rdbuf);
//
//        term_dat_out_crlf((UNS_8*)"");
//        term_dat_out_crlf((UNS_8*)"dcc trace out:");
//        out_trace(&dcc_trace, rdbuf);
//
//        term_dat_out_crlf((UNS_8*)"");
//        term_dat_out((UNS_8*)"stack up: ");
//        str_makedec(rdbuf, stack_res_eth);
//        term_dat_out_crlf(rdbuf);
//
//        term_dat_out((UNS_8*)"stack down: ");
//        str_makedec(rdbuf, stack_res_dcc);
//        term_dat_out_crlf(rdbuf);
//
//}
    else {
        return FALSE;
    }
    return TRUE;
}

BOOL_32 cmd_sabit_test(void) {
    char* option = (char*)cmd_get_field_val(1);
    UNS_8 slot = cmd_get_field_val(2);
    UNS_8 channel = cmd_get_field_val(3);
    UNS_8 rdbuf[1024];
    static const uint8 col = 8;

    if( strcmp(option, "read") == 0 ) {
        Card16FE1* pxc = ObjectReference::get16FE1Card(slot);
        if( !pxc ) {
            return FALSE;
        }
        if( !pxc->getChipPCMLogic().ifSaHavePacket(channel) ) {
            printf("\nno packet received!\n");
        }
        else {
            int rdLen = pxc->getChipPCMLogic().getSaPacketLen();
            if( rdLen <= 0 || rdLen > sizeof(rdbuf) ) {
                printf("\noutof range, length is %d\n", rdLen);
                pxc->getChipPCMLogic().readSaPacketOver();
                return FALSE;
            }
            pxc->getChipPCMLogic().getSaPacket(rdbuf, rdLen);
            pxc->getChipPCMLogic().readSaPacketOver();
            printf("\nRecieved Packet: %4x, length is %d\n",
                    (rdbuf[0]<<24)|(rdbuf[1]<<16)|(rdbuf[2]<<8)|(rdbuf[3]<<0), rdLen);

            if( parse_get_entry_count() > 4 ) {
                uint8 line = 0;
               if( rdLen % col ) {
                    line = rdLen / col + 1;
                }
                else {
                    line = rdLen / col;
                }
                for( int i = 0; i < line; i++ ) {
                    for( int j = 0; j < col; j++ ) {
                        int point = i*col + j;
                        if( point >= rdLen ) {
                            return TRUE;
                        }
                        UNS_8 daddr[10];
                        str_makehex(daddr, rdbuf[point], 2);
                        term_dat_out(daddr);
                        term_dat_out((UNS_8 *) " ");
                    }
                    term_dat_out_crlf((UNS_8*)"");
                }
            }
        }

    }
    else if( strcmp(option, "write") == 0 ) {
        UNS_16 len = 250;
        UNS_8 loop = 1;
        static UNS_32 head = 0;
        if( parse_get_entry_count() > 4 ) {
            loop = cmd_get_field_val(4);
        }
//        else {
//            len = 96;
//        }
        Card16FE1* pxc = ObjectReference::get16FE1Card(slot);
        if( !pxc ) {
            return FALSE;
        }

        for (int i = 0; i < loop; ++i) {
            rdbuf[0] = (head >> 24) & 0xff;
            rdbuf[1] = (head >> 16) & 0xff;
            rdbuf[2] = (head >> 8) & 0xff;
            rdbuf[3] = (head >> 0) & 0xff;
            for (int j = 0; j < len; ++j) {
                rdbuf[j+4] = j;
            }
            pxc->getChipPCMLogic().sendSaPacket(channel, rdbuf, len+4);
            printf("\n Packet %4x send succeed!\n", head);
            head++;
        }
    }
    else {
        return FALSE;
    }
    return TRUE;
}

BOOL_32 cmd_dcn_test(void) {
    char* option = (char*)cmd_get_field_val(1);
    UNS_8 slot = cmd_get_field_val(2);
    UNS_8 channel = cmd_get_field_val(3);
    UNS_8 rdbuf[1024];
    static const uint8 col = 8;

    if( strcmp(option, "read") == 0 ) {
        //CardDXC* pxc = ObjectReference::getDXCCard(slot);
        Card16FE1* pxc = ObjectReference::get16FE1Card(slot);
        if( !pxc ) {
            return FALSE;
        }
        if( !pxc->getChipPCMLogic().ifDcnHavePacket(channel) ) {
            printf("\nno packet received!\n");
        }
        else {
            int rdLen = pxc->getChipPCMLogic().getDcnPacketLen(channel);
            if( rdLen <= 0 || rdLen > sizeof(rdbuf) ) {
                printf("\noutof range, length is %d\n", rdLen);
                pxc->getChipPCMLogic().readDcnPacketOver(channel);
                return FALSE;
            }
            pxc->getChipPCMLogic().getDcnPacket(channel, rdbuf, rdLen);
            pxc->getChipPCMLogic().readDcnPacketOver(channel);
            printf("\nRecieved Packet: %4x, length is %d\n",
                    (rdbuf[0]<<24)|(rdbuf[1]<<16)|(rdbuf[2]<<8)|(rdbuf[3]<<0), rdLen);

            if( parse_get_entry_count() > 4 ) {
                uint8 line = 0;
               if( rdLen % col ) {
                    line = rdLen / col + 1;
                }
                else {
                    line = rdLen / col;
                }
                for( int i = 0; i < line; i++ ) {
                    for( int j = 0; j < col; j++ ) {
                        int point = i*col + j;
                        if( point >= rdLen ) {
                            return TRUE;
                        }
                        UNS_8 daddr[10];
                        str_makehex(daddr, rdbuf[point], 2);
                        term_dat_out(daddr);
                        term_dat_out((UNS_8 *) " ");
                    }
                    term_dat_out_crlf((UNS_8*)"");
                }
            }
        }

    }
    else if( strcmp(option, "write") == 0 ) {
        UNS_16 len = 250;
        UNS_8 loop = 1;
        static UNS_32 head = 0;
        if( parse_get_entry_count() > 4 ) {
            loop = cmd_get_field_val(4);
        }
//        else {
//            len = 96;
//        }
//        CardDXC* pxc = ObjectReference::getDXCCard(slot);
        Card16FE1* pxc = ObjectReference::get16FE1Card(slot);
        if( !pxc ) {
            return FALSE;
        }

        for (int i = 0; i < loop; ++i) {
            rdbuf[0] = (head >> 24) & 0xff;
            rdbuf[1] = (head >> 16) & 0xff;
            rdbuf[2] = (head >> 8) & 0xff;
            rdbuf[3] = (head >> 0) & 0xff;
            for (int j = 0; j < len; ++j) {
                rdbuf[j+4] = j;
            }
            pxc->getChipPCMLogic().sendDcnPacket(channel, rdbuf, len+4);
            printf("\n Packet %4x send succeed!\n", head);
            head++;
        }
    }
    else {
        return FALSE;
    }
    return TRUE;
}

BOOL_32 cmd_vfread(void) {
    UNS_8 slot = cmd_get_field_val(1);
    UNS_8 chip = cmd_get_field_val(2);
    UNS_8 ch = cmd_get_field_val(3);
    UNS_8 regAddr = cmd_get_field_val(4);
    UNS_8 num = 1;
    if( parse_get_entry_count() > 5 ) {
        num = cmd_get_field_val(5);
    }
    CardVF* vfc = ObjectReference::getVFCard(slot);
    if( vfc == 0 ) {
        return FALSE;
    }
    for( int i = 0; i < num; i++ ) {
        VFCmdRegRead cmd(chip, ch, regAddr);
        vfc->processVFCommand( cmd );
        uint8* pval = cmd.getResultBuff();
        assert(pval != 0);
        printf("\nreg %2x:\t", regAddr);
        for (int i = 0; i < pval[0]; ++i) {
            printf("%02x ", pval[i+1]);
        }
        printf("\n");
    }
    return TRUE;
}

BOOL_32 cmd_vfwrite(void) {
    UNS_8 slot = cmd_get_field_val(1);
    UNS_8 chip = cmd_get_field_val(2);
    UNS_8 ch = cmd_get_field_val(3);
    UNS_8 regAddr = cmd_get_field_val(4);
    UNS_8 regVal = cmd_get_field_val(5);
    CardVF* vfc = ObjectReference::getVFCard(slot);
    if( vfc == 0 ) {
        return FALSE;
    }
    VFCmdRegWrite cmd(chip, ch, regAddr, regVal);
    vfc->processVFCommand( cmd );
    uint8* pval = cmd.getResultBuff();
    assert(pval != 0);
    printf("\nreg %2x:\t", regAddr);
    for (int i = 0; i < pval[0]; ++i) {
        printf("%02x ", pval[i+1]);
    }
    printf("\n");
    return TRUE;
}

//reset ? get the card which can reset
BOOL_32 cmd_reset(void) {
    UNS_8 slot = cmd_get_field_val(1);
    UNS_8 way = 1;
    if( parse_get_entry_count() >2 ) {
    	way = cmd_get_field_val(2);
    }
	CBaseSlot* rst = SlotModule::getSlot(slot);
//    if(slot<= slt.size()){
	if( rst == 0 ) {
	    while(1); //测试看门狗用
// 	    return FALSE;
	}
	if( slot < 2 ) {
        printf("\nreset the %d slot\n", slot);
             rst->reset((Start_Type_E)way);
	}
	else if(rst->cardExist()){//getCurrentCardTypeID
    	printf("\nreset the %d slot\n", slot);
    		 rst->reset((Start_Type_E)way);
    }
    else{
    	printf("\nthe %d slot has no card to reset!\n", slot);
    	return FALSE;
    }


    return TRUE;
}
BOOL_32 cmd_tx2fan(void) {
	char* sendinfo =(char*) cmd_get_field_val(1);
	huart_dat_out((UNS_8*)sendinfo,strlen(sendinfo));
    os_dly_wait(200);
    char buff[48] = {0};
    uint8* p = (uint8*)buff;
    while(huart_dat_in(p++, 100) > 0) {
    }
	printf("\nreceive from fan :%s\n",buff);
    
	return TRUE;
}

uint8 ip[4] = {0};
uint32 sndPkt = 0;
uint32 rcvPkt = 0;
uint32 losPkt = 0;

TASK void ping_a_host(void) {
    while(1) {
        ++sndPkt;
        if( ifIPExist(ip) == 0 ) {
            rcvPkt++;
            printf("Ping %d.%d.%d.%d OK!\n", ip[0], ip[1], ip[2], ip[3]);
            os_dly_wait(100);
        }
        else {
            losPkt++;
            printf("!!!Ping %d.%d.%d.%d Failed!!!!\n", ip[0], ip[1], ip[2], ip[3]);
            os_dly_wait(300);
        }
    }
}

BOOL_32 cmd_ping(void) {
    sndPkt = 0;
    rcvPkt = 0;
    losPkt = 0;
    memset(ip, 0, 4);
    std::string s((char*)cmd_get_field_val(1));
    CPPTools::ezip_aton(s, ip);
    bool hold = true;
    OS_TID tsk_ping = os_tsk_create(ping_a_host, 30);

    while ( hold ) {
        char c = getkey();
        if( term_break() ) {
            os_tsk_delete(tsk_ping);
            break;
        }
    }
    printf("\nsndPkt = %d, rcvPkt = %d, losPkt = %d\n", sndPkt, rcvPkt, losPkt);
    return TRUE;
}

uint16 listenfunc(uint8 socket, uint8 *remip, uint16 remport, uint8 *buf, uint16 len) {
    return 0;
}

BOOL_32 cmd_udpsend(void) {
    std::string s((char*)cmd_get_field_val(1));
    uint8 ip[4] = {0};
    CPPTools::ezip_aton(s, ip);

    char* s2 = (char*)cmd_get_field_val(2);
    uint8 soc = udp_get_socket(0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, listenfunc);
    uint8* sendbuf = (uint8*)udp_get_buf(strlen(s2));
    if( soc == 0 || sendbuf == 0 ) {
        printf("\n!!!udp socket NULL!!!\n");
        return 0;
    }
    strcpy((char*)sendbuf, s2);
    udp_send(soc, ip, 10001, sendbuf, strlen(s2));
	return 0;
}

BOOL_32 cmd_omuswitch(void) {
    uint8 mode = cmd_get_field_val(1);
    if( mode == 2 ) {
        ProtectAssistCell::instance().autoStart();
    }
    else {
        ProtectAssistCell::instance().autoPause();
        ProtectAssistCell::instance().switchTo((Working_STATE_E)mode);
    }
    return 0;
}


static BOOL_32 cmd_coeread(void) {
    if( parse_get_entry_count() < 5 ) {
        return FALSE;
    }
    UNS_8 slot = cmd_get_field_val(1);
    UNS_8 chip = cmd_get_field_val(2);
    UNS_8 channel = cmd_get_field_val(3);
    UNS_8 block = cmd_get_field_val(4);
    UNS_8 wordCnt = 8;
    if( parse_get_entry_count() > 5 ) {
        wordCnt = cmd_get_field_val(5);
    }
    if( slot < 2 || slot > 6 || chip > 7 || channel > 3 || block > 4 ) {
        printf("\nPlease check input data.\n");
        return FALSE;
    }

    CardVF* vfc = ObjectReference::getVFCard(slot);
    if( vfc == 0 ) {
        return FALSE;
    }
    VFCmdCoeRead cmd(chip, channel, block, wordCnt);
    vfc->processVFCommand( cmd );
    uint8* pval = cmd.getResultBuff();
    assert(pval != 0);
    printf("\nblock: %d:\t", block);
    for (int i = 0; i < pval[0]; ++i) {
        printf("%02x ", pval[i+1]);
    }
    printf("\n");
    return TRUE;
}
static BOOL_32 cmd_coewrite(void) {
    if( parse_get_entry_count() < 5 ) {
        return FALSE;
    }
    UNS_8 slot = cmd_get_field_val(1);
    UNS_8 chip = cmd_get_field_val(2);
    UNS_8 channel = cmd_get_field_val(3);
    UNS_8 block = cmd_get_field_val(4);
    if( slot < 2 || slot > 6 || chip > 7 || channel > 3 || block > 4 ) {
        printf("\nPlease check input data.\n");
        return FALSE;
    }
    uint16 temp[8] = {0};

    if( parse_get_entry_count() == 5 ) {
        printf("\ninput format data like 0x1234.\n");
        static char buff[64] = "0x1111 0x2222 0x3333 0x4444";
        int bytes = 0;
        memset(buff, '\0', sizeof(buff));
        while ( 1 ) {
            char c = getkey();
            if( c == 27 || c == 13) {
                break;
            }

            sendchar(c);

            buff[bytes++] = c;
        }
        int data[8];
        int result = sscanf(buff, "%x %x %x %x %x %x %x %x",
                &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7] );
        for (int i = 0; i < 8; ++i) {
            temp[i] = (uint16)data[i];
        }
    }
    else { //读指定配置文件数据到temp
        FILE* f;
        char* fname = (char*)cmd_get_field_val(5);
        f = fopen(fname, "r");
        if( f ) {
            int data[8];
            int result = fscanf(f, "%x %x %x %x %x %x %x %x",
                    &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7] );
            for (int i = 0; i < 8; ++i) {
                temp[i] = (uint16)data[i];
            }
        }
        else {
            printf("\nFile isn't exist\n");
            return FALSE;
        }
    }
    CardVF* vfc = ObjectReference::getVFCard(slot);
    if( vfc == 0 ) {
        return FALSE;
    }
    VFCmdCoeWrite cmd(chip, channel, block, temp, 8);
    vfc->processVFCommand( cmd );
    uint8* pval = cmd.getResultBuff();
    assert(pval != 0);
    printf("\nblock: %d:\t", block);
    for (int i = 0; i < pval[0]; ++i) {
        printf("%02x ", pval[i+1]);
    }
    printf("\n");
    return FALSE;
}

BOOL_32 cmd_swtrace(void) {
    char* op = (char*)cmd_get_field_val(1);
    uint32 bitmap = cmd_get_field_val(2);
    if( strcmp(op, "read") == 0 ) {
        for (int i = 0; i < 20; ++i) {
            if( (bitmap >> i) & 1 ) {
                SwTrace::print(i);
            }
        }
    }
    else if( strcmp(op, "clear") == 0 ) {
        for (int i = 0; i < 20; ++i) {
            if( (bitmap >> i) & 1 ) {
                SwTrace::clear(i);
            }
        }
    }
    else if( strcmp(op, "reset") == 0 ) {
    	SwitchCore::instance().resetSwitchCore();
    	printf("\nSwitchCore reset over\n");
    }
    else if( strcmp(op, "macport") == 0 ) {
        SwitchCore::instance().showMacPortMap();
    }
    else if(strcmp(op, "ipport") == 0 ) {
        SwitchCore::instance().showIpPortMap();
    }
    else if( strcmp(op, "maxbroadcast") == 0 ) {
        if( bitmap != 0 ) {
            SwitchCore::instance().setMaxBroadcastPerSecond(bitmap);
        }
        printf("\nThe MAX broadcast is %d per second\n", SwitchCore::instance().getMaxBroadcastPerSecond());
    }
    else if( strcmp(op, "sysclk") == 0 ) {
    	printf("\nThe system clock is: %d", os_time_get());
    }
    else if( strcmp(op, "totalinfo") == 0 ) {
    	PriPacket::printTotalLifeTimeInfo();
    }
    else if( strcmp(op, "actinfo") == 0 ) {
    	PriPacket::openTraceNLoop(bitmap);
    }


    return TRUE;
}
/***********************************************************************
 *
 * Function: cmd_core_add_commands
 *
 * Purpose: Initialize this command block
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void CLI_debug_add_commands(void)
{
	/* Add core group */
	cmd_add_group(&debug_group);

	/* Add commands to the core group */
	cmd_add_new_command(&debug_group, &core_fill_cmd);
	cmd_add_new_command(&debug_group, &core_peek_cmd);
	cmd_add_new_command(&debug_group, &core_poke_cmd);

    cmd_add_new_command(&debug_group, &dbg_bread_cmd);
    cmd_add_new_command(&debug_group, &dbg_bwrite_cmd);
	cmd_add_new_command(&debug_group, &dbg_dbread_cmd);
    cmd_add_new_command(&debug_group, &dbg_dbwrite_cmd);
    cmd_add_new_command(&debug_group, &debug_rprint_cmd);
    cmd_add_new_command(&debug_group, &debug_rcfg_cmd);
    cmd_add_new_command(&debug_group, &cmd_dcc_test_cmd);
    cmd_add_new_command(&debug_group, &cmd_sabit_test_cmd);
    cmd_add_new_command(&debug_group, &cmd_dcn_test_cmd);
    cmd_add_new_command(&debug_group, &cmd_vfread_cmd);
    cmd_add_new_command(&debug_group, &cmd_reset_cmd);
    cmd_add_new_command(&debug_group, &cmd_tx2fan_cmd);
    cmd_add_new_command(&debug_group, &cmd_ping_cmd);
    cmd_add_new_command(&debug_group, &cmd_udp_cmd);
    cmd_add_new_command(&debug_group, &cmd_omuswitch_cmd);
    cmd_add_new_command(&debug_group, &cmd_vfwrite_cmd);
    cmd_add_new_command(&debug_group, &coeread_cmd);
    cmd_add_new_command(&debug_group, &coewrite_cmd);
    cmd_add_new_command(&debug_group, &cmd_swtrace_cmd);



}//ÿ����һ��������Ϣ����Ҫ��ӵ�debug_group�С�
