//--------------------------------------------------------------------
//
//  Copyright (C) 2020, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of
//  WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement
//  with WESS-Global Co.,LTD
//
//  $Id: templet_format.c 1 2020-07-13 12:00:00Z eddie $
//  $Date: 2020-07-13 12:00:00 +0900 
//  $Author: eddie $
//  $Rev: 1.0 $
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------
// lib
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// self
#include "pcd_main.h"
// bsp
#include "flash_main.h"
#include "gpio_main.h"
#include "bsp_main.h"
#include "rly_main.h"
// app
#include "app_main.h"
#include "isr_main.h"
// menu
#include "menu_measure.h"

#include "modbus_main.h"
//--------------------------------------------------------------------
//  Local Macros
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Local Type definitions
//--------------------------------------------------------------------
// PCD: Probe Cleaning Device
typedef struct {
	U08 f_run;		 // OLD: unsigned char Flag_PCD_run_UC_G;
	U16 c_sec;		 // OLD: unsigned int  Cnt_sec_UI_G;
	U16 c_min;		 // OLD: unsigned int  Cnt_min_UI_G;
	U16 c_run;
} LV_PC;


//--------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------
LV_PC lPc;			// Probe Cleaning


//--------------------------------------------------------------------
// Funtion Definitions
//--------------------------------------------------------------------
U08 PCD_GetFgRun(void)		{	return lPc.f_run;		}

// PCD: Probe Cleaning Device
void PCD_InitVari(void)
{
	lPc.f_run = FALSE;
	lPc.c_sec = 0;
	lPc.c_min = 0;
	lPc.c_run = 0;
}

// OLD: void Out_PCD(void)
void PCD_Prc1Sec(void)		// PCD: Probe Cleaning Device
{
	U16 pcd_intv = MnMSR_GetPcdIntv();

	if(pcd_intv < 1)
		return;

	if(lPc.f_run==FALSE)
	{
		lPc.c_sec++;

		if(lPc.c_sec >= 60)
		{
			lPc.c_min++;
			lPc.c_sec = 0;
		}

		if(pcd_intv < 60)
		{
			if(lPc.c_sec >= pcd_intv)
			{
				lPc.f_run = TRUE;
				lPc.c_sec = 0;
			}
		}
		else
		{
			if(lPc.c_min >= (pcd_intv / 60))
			{
				lPc.f_run = TRUE;
				lPc.c_min = 0;
				lPc.c_sec = 0;
			}
		}

		if(lPc.f_run == FALSE)
			return;

		lPc.c_run =0;
		SBIT_PCD_OUT = RLY_STT_ON;
	}
	else
	{
		if(++lPc.c_run>=MnMSR_GetPcdTerm())
		{
			lPc.f_run = FALSE;
			lPc.c_run =0;
			SBIT_PCD_OUT = RLY_STT_OFF;
		}
	}
}


// EOF

