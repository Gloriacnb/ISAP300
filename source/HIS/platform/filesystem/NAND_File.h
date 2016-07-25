/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    NAND_File.h
 *      Purpose: File manipulation example definitions
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/
#ifndef NAND_FILE_H_
#define NAND_FILE_H_

#include "EZ_types.h"
#if defined (__cplusplus)
extern "C"
{
#endif

#define CNTLQ       0x11
#define CNTLS       0x13
#define DEL         0x7F
#define BACKSPACE   0x08
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

/* Command definitions structure. */
typedef struct scmd {
  char val[8];
  void (*func)(char *par);
} SCMD;

extern void NandFile_init (void);

#if defined (__cplusplus)
}
#endif
#endif
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
