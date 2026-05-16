//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------
// chipset
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// self
#include "flash_main.h"
// lib
#include "sys_def.h"
// lib
#include "dly_main.h"
// bsp
#include "app_main.h"
#include "font_def.h"
#include "lcd_main.h"
// app
#include "modbus_main.h"
#include "prtc_main.h"
#include "measure_main.h"
// disp
#include "disp_main.h"
#include "disp_string.h"
// menu
#include "menu_box.h"
#include "menu_quick.h"
#include "menu_system.h"
#include "menu_measure.h"
#include "menu_output.h"
#include "menu_data.h"
#include "menu_engineer.h"
#include "menu_factory.h"
#include "menu_test.h"
// screen
#include "screen_main.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
U08 code  * data pFl_read;		// Old: unsigned char code  * data FLASH_pread;     // read pointer
U08 xdata * data pFl_write;		// Old: unsigned char xdata * data FLASH_pwrite;    // write pointer


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------
void FlPrc_ErasePage(void)
{
    U08 SFRPAGE_SAVE = SFRPAGE;         // preserve SFRPAGE
    bit EA_SAVE = EA;                   // preserve EA

    EA = 0;                             // disable interrupts

    SFRPAGE = LEGACY_PAGE;

    FLSCL |= 0x01;                      // enable FLASH writes/erases
    PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1
    PSCTL |= 0x04;                      // set SFLE

    RSTSRC = 0x02;                      // enable VDDMON as reset source

    pFl_write  = (U08 xdata *) 0x0000;
    *pFl_write = 0x00;                   // initiate page erase
    pFl_write  = (U08 xdata *) 0x00ff;
    *pFl_write = 0x00;                   // initiate page erase

    PSCTL &= ~0x04;                     // clear SFLE
    PSCTL &= ~0x03;                     // PSWE = 0; PSEE = 0
    FLSCL &= ~0x01;                     // disable FLASH writes/erases

    SFRPAGE = SFRPAGE_SAVE;             // restore SFRPAGE
    EA = EA_SAVE;                       // restore interrupts

    DLY_002usec();
}

void FlPrc_ReadAll(void)
{
	MnSYS_MemLoad();
	MnMSR_MemLoad();
	MnOUT_MemLoad();
	MnDAT_MemLoad();
	MnEGN_MemLoad();
	MnFTR_MemLoad();
	MnTST_MemLoad();
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Access Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Get
// Set


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
U08 FLS_Rd1Byte(U08 addr)
{
	U08 byte;
	U08 SFRPAGE_SAVE = SFRPAGE;         // preserve SFRPAGE
	bit EA_SAVE = EA;                   // preserve EA

	EA = 0;                             // disable interrupts

	SFRPAGE = LEGACY_PAGE;

	PSCTL |= 0x04;                      // set SFLE

	pFl_read = (U08 code *) addr;
	byte = *pFl_read;                  // read the byte

	PSCTL &= ~0x04;                     // clear SFLE

	SFRPAGE = SFRPAGE_SAVE;             // restore SFRPAGE
	EA = EA_SAVE;                       // restore interrupts

	DLY_002usec();

	return byte;
}

U16 FLS_Rd2Byte(U08 addr)
{
	U08 SFRPAGE_SAVE = SFRPAGE;         // preserve SFRPAGE
	bit EA_SAVE = EA;                   // preserve EA
	U08 byte_l, byte_h;
	U16 byte_2;

	EA = 0;                             // disable interrupts

	SFRPAGE = LEGACY_PAGE;

	PSCTL |= 0x04;                      // set SFLE

	pFl_read = (U08 code *) addr;
	byte_l = *pFl_read;                // read the byte
	pFl_read++;
	byte_h = *pFl_read;                // read the byte

	PSCTL &= ~0x04;                     // clear SFLE

	SFRPAGE = SFRPAGE_SAVE;             // restore SFRPAGE
	EA = EA_SAVE;                       // restore interrupts

	byte_2 = byte_h;
	byte_2 = (byte_2 << 8) | byte_l;

	DLY_002usec();

	return byte_2;
}

void FLS_Wr1Byte(U08 addr, U08 byte)
{
    U08 SFRPAGE_SAVE = SFRPAGE;         // preserve SFRPAGE
    bit EA_SAVE = EA;                   // preserve EA

    EA = 0;                             // disable interrupts

    SFRPAGE = LEGACY_PAGE;

    FLSCL |= 0x01;                      // enable FLASH writes/erases
    PSCTL |= 0x01;                      // PSWE = 1
    PSCTL |= 0x04;                      // set SFLE

    RSTSRC = 0x02;                      // enable VDDMON as reset source

    pFl_write  = (U08 xdata *) addr;
    *pFl_write = byte;               // write the byte

    PSCTL &= ~0x04;                     // clear SFLE
    PSCTL &= ~0x01;                     // PSWE = 0
    FLSCL &= ~0x01;                     // disable FLASH writes/erases

    SFRPAGE = SFRPAGE_SAVE;             // restore SFRPAGE
    EA = EA_SAVE;                       // restore interrupts

    DLY_002usec();
}

void FLS_Wr2Byte(U08 addr, U16 byte_2)
{
    U08 SFRPAGE_SAVE = SFRPAGE;         // preserve SFRPAGE
    bit EA_SAVE = EA;                   // preserve EA
    U08 byte_l, byte_h;

    EA = 0;                             // disable interrupts

    byte_l = byte_2 & 0x00ff;
    byte_h = (byte_2 & 0xff00) >> 8;

    SFRPAGE = LEGACY_PAGE;

    FLSCL |= 0x01;                      // enable FLASH writes/erases
    PSCTL |= 0x01;                      // PSWE = 1
    PSCTL |= 0x04;                      // set SFLE

    RSTSRC = 0x02;                      // enable VDDMON as reset source

    pFl_write  = (U08 xdata *) addr;
    *pFl_write = byte_l;                     // write the byte
    pFl_write++;
    *pFl_write = byte_h;                     // write the byte

    PSCTL &= ~0x04;                     // clear SFLE
    PSCTL &= ~0x01;                     // PSWE = 0
    FLSCL &= ~0x01;                     // disable FLASH writes/erases

    SFRPAGE = SFRPAGE_SAVE;             // restore SFRPAGE
    EA = EA_SAVE;                       // restore interrupts

    DLY_002usec();
}

void FLS_WriteAll(void)
{
	FlPrc_ErasePage();

	MnSYS_MemSave();
	MnMSR_MemSave();
	MnOUT_MemSave();
	MnDAT_MemSave();
	MnEGN_MemSave();
	MnFTR_MemSave();
	MnTST_MemSave();

	FlPrc_ReadAll();
}

void FLS_ResetFactory(void)
{
	DpSTR_LineUpd(FT_EN_08x16, 0, 52, 48, "Factory Setting");
	DpSTR_LineUpd(FT_EN_08x16, 0, 84, 64, " .......");			DLY_RunMsec(250);
	DpSTR_LineUpd(FT_EN_08x16, 0, 84, 64, ". ......");			DLY_RunMsec(250);
	DpSTR_LineUpd(FT_EN_08x16, 0, 84, 64, ".. .....");			DLY_RunMsec(250);
	DpSTR_LineUpd(FT_EN_08x16, 0, 84, 64, "... ....");			DLY_RunMsec(250);
	DpSTR_LineUpd(FT_EN_08x16, 0, 84, 64, ".... ...");			DLY_RunMsec(250);
	DpSTR_LineUpd(FT_EN_08x16, 0, 84, 64, "..... ..");			DLY_RunMsec(250);
	DpSTR_LineUpd(FT_EN_08x16, 0, 84, 64, "...... .");			DLY_RunMsec(250);
	DpSTR_LineUpd(FT_EN_08x16, 0, 84, 64, "....... ");			DLY_RunMsec(250);

	FlPrc_ErasePage();

	MnSYS_MemFtry();
	MnMSR_MemFtry();
	MnOUT_MemFtry();
	MnDAT_MemFtry();
	MnEGN_MemFtry();
	MnFTR_MemFtry();
	MnTST_MemFtry();

	FlPrc_ReadAll();
}

void FLS_InitBoot(void)
{
	SFRPAGE = LEGACY_PAGE;
	FLSCL = 0x30;			// Bits 5-4: FLRT: Flash Read Time. 11: SYSCLK < 100 MHz.
							// Bit 0: FLWE: Flash Write/Erase Enable. 1: Flash writes/erases enabled.
}

void FLS_InitMain(void)
{
	FlPrc_ReadAll();
}



//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

