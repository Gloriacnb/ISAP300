/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    NAND_File.c
 *      Purpose: File manipulation example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>                    /* standard I/O .h-file                */
#include <ctype.h>                    /* character functions                 */
#include <string.h>                   /* string and memory functions         */
#include <stdlib.h>
#include "File_Config.h"
#include "NAND_File.h"
#include "os.h"
#include "bsp_nand.h"

/* Local variables */
static char in_line[160];
extern int getkey (void);

/*----------------------------------------------------------------------------
 *        Format NAND Flash
 *---------------------------------------------------------------------------*/
static void cmd_format (char *par) {
    char *label;
    char arg[20];
//    U32 retv;

    label = 0;
    if (label == NULL) {
        label = "ISAP";
    }
    strcpy (arg, label);
    startformat:
    printf ("\nFormat NAND Flash Memory.\n");
//    retv = getkey();
//    if (retv == 'y' || retv == 'Y') {
        /* Format the Card with Label "KEIL". "*/
        if (fformat (arg) == 0) {
            printf ("NAND Flash Formatted with label %s\n", label);
        }
        else {
            printf ("Formatting failed.\n");
            goto startformat;
        }
//    }
}

/*----------------------------------------------------------------------------
 *        Initialize FAT File System on the NAND Flash
 *---------------------------------------------------------------------------*/
static void init_fs (void) {
    U32 retv;
    char c;
    //  finit("R:");
    //  fformat ("R:");


    while ((retv = finit ("N1:")) != 0) {
        if (retv == 1) {
            printf ("\nNAND Flash Init Failed\n");
            printf ("\nPress any key to retry...\n");
            c = getkey ();
            if( c == 'y' || c == 'Y') {
                continue;
            }
            else {
                break;
            }
        }
        else {
            printf ("\nWait for format Nandflash\n");
            strcpy (&in_line[0], "KEIL\r\n");
            cmd_format (&in_line[0]);
            break;
        }
    }
}


OS_SEM initComplete;

TASK void task_nandfile_init(void) {
    init_fs();
    os_sem_send(initComplete);
    os_tsk_delete_self ();

}
/*----------------------------------------------------------------------------
 *        Main: 
 *---------------------------------------------------------------------------*/
void NandFile_init (void) {
    U8* stk;
    size_t size = 8192;

    os_sem_init(initComplete, 0);

    stk = (U8*)malloc(size);
    OS_TASK_CREAT(task_nandfile_init, 200, stk, size);
    os_sem_wait(initComplete, 0xffff);
    free(stk);

}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
