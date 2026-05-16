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
// std
#include <string.h>
// lib
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// bsp
#include "bsp_main.h"
#include "flash_main.h"
#include "uart_main.h"
#include "gpio_main.h"
#include "adc_main.h"
#include "mem_main.h"
#include "btn_main.h"
#include "rtc_ds1302.h"
#include "dac_main.h"
#include "dly_main.h"
#include "tmr_main.h"
// app
#include "disp_main.h"
#include "rly_main.h"
#include "pcd_main.h"
#include "isr_main.h"
#include "prtc_main.h"
#include "modbus_main.h"
#include "data_main.h"
// menu
#include "menu_main.h"
#include "menu_measure.h"
#include "menu_output.h"
#include "menu_pass.h"
#include "menu_quick.h"
#include "menu_engineer.h"
#include "menu_test.h"
// msr
#include "measure_main.h"
#include "measure_temperature.h"
#include "measure_analyze.h"
// screen
#include "screen_main.h"
// self
#include "app_main.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
U08 lApp_pStr[APP_STR_LENGTH];
U32 test_global_cnt;	


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------
void AppProc_EnterMenu(void)
{
	if(!BTN_ChkLong(BTN_IDX_PROG, 200)) 
		return;

	ES0     = 0;
	SFRPAGE = TMR3_PAGE;
	TMR3CN  = 0x00;
	SFRPAGE = CONFIG_PAGE;

	gPas_fChk = FALSE;

	MnPAS_ProcMain();

	if(!gPas_fChk)
	{
		DISP_ClearScr();
		SCRN_ProcDisp();
		TMR_CfgResume();
    	gIsr_tm3_cnt = 0;
		return;
	}
	MENU_ProcMain();
}

void AppProc_EnterQuick(void)
{
	if(!BTN_ChkLong(BTN_IDX_EXIT, BTN_WAIT_SET)) 
		return;

	ES0 = 0;
	MnQUI_ProcMain();
}

void AppProc_MoveScrn(U08 key)
{
	SCRN_ProcKey(key);
	SCRN_ProcDisp();
	BTN_DelayMsec(500);
}

void AppInit_Vari(void)
{
	// BSP
	UAT_InitVari();
	RLY_InitVari();
	PCD_InitVari();
	ADC_InitVari();
	// ISR
	ISR_InitVari();
	// MSR
	MEAS_InitVari();
	MsTPR_InitVari();
	MsANL_InitVari();

	DISP_InitVari();
	MENU_InitVari();
	SCRN_InitVari();
	
	test_global_cnt = 0;

	
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Operation
//------------------------------------------------------------------------------------------------------------------------------
U08* APP_NumTest2Str(U32 num, U08* p_dst, U16 digit, U08 decimal, U08 f_fill_zero)
{
	U08 i, cnt;
	U32 pow = 1;

	for(i=0; i<(digit-1); i++)
		pow *= 10;

	if(decimal == 0)		cnt = digit;
	else					cnt = digit + 1;

	for(i=0; i<cnt; i++)
	{
		if(i == (digit-decimal))
			lApp_pStr[i] = '.';
		else
		{
			if(num < pow)
			{
				if(f_fill_zero == TRUE)
					lApp_pStr[i] = '0';
				else
				{
					if(i >= (digit-decimal-1))		lApp_pStr[i] = '0';
					else							lApp_pStr[i] = ' ';
				}
			}
			else
				lApp_pStr[i] = (num / pow) % 10 + '0';

			pow /= 10;
		}
	}

	lApp_pStr[i] = 0;

	strncpy(p_dst, lApp_pStr, APP_STR_LENGTH);

	return lApp_pStr;
}






U08* APP_Num2Str(S32 num, U08* p_dst, U16 digit, U08 decimal, U08 f_fill_zero)
{
	U08 i, cnt;
	U32 pow = 1;

	for(i=0; i<(digit-1); i++)
		pow *= 10;

	if(decimal == 0)		cnt = digit;
	else					cnt = digit + 1;

	for(i=0; i<cnt; i++)
	{
		if(i == (digit-decimal))
			lApp_pStr[i] = '.';
		else
		{
			if(num < pow)
			{
				if(f_fill_zero == TRUE)
					lApp_pStr[i] = '0';
				else
				{
					if(i >= (digit-decimal-1))		lApp_pStr[i] = '0';
					else							lApp_pStr[i] = ' ';
				}
			}
			else
				lApp_pStr[i] = (num / pow) % 10 + '0';

			pow /= 10;
		}
	}

	lApp_pStr[i] = 0;

	strncpy(p_dst, lApp_pStr, APP_STR_LENGTH);

	return lApp_pStr;
}

void APP_InitBoot(void)
{
	int i;

	if(BTN_GetStatus(BTN_IDX_SET) && BTN_GetStatus(BTN_IDX_EXIT))
	{
		FLS_ResetFactory();
	}

	DISP_IntroScr();

	MEM_EraseMram();
	FLS_InitMain();
	PRT_InitBoot();
	BSP_InitPort();

	AppInit_Vari();

	RTC_GetTime();
	LOG_InitAddr();

	for(i=0; i<4; i++)
		MsTPR_CalcVal();

	ANZ_SetEmptyPct(110);
}

void APP_ProcEvery(void)
{
	PRT_ProcMain();
	MDB_PrcMain();

	if(!BTN_ChkPush())
		return;

	ScTRD_ProcMain();

	switch(BTN_GetIdx())
	{
		case BTN_IDX_PROG:			AppProc_EnterMenu();				break;
		case BTN_IDX_EXIT:			AppProc_EnterQuick();				break;
		case BTN_IDX_SET:			ScTRD_ProcEnter();					break;
		case BTN_IDX_RIGHT:			AppProc_MoveScrn(BTN_IDX_RIGHT);	break;
		case BTN_IDX_LEFT:			AppProc_MoveScrn(BTN_IDX_LEFT);		break;
		default:					break;
	}

	switch(SCRN_GetScrnIdx())
	{
		case SCR_I02_GRP_LIVE:		BTN_DelayMsec(200); 		break;
		case SCR_I03_GRP_AVRG:		MEAS_CfgThreshold();		break;
		default:					break;
	}
}

void APP_Proc1Sec(void)
{
	S16 sg_thick = MEAS_GetSgThick();

	if(gIsr_tm3_cnt < 100)		// 100 = 1sec
		return;
	gIsr_tm3_cnt = 0;
	if(MnTST_GetGloval_fCnt())
	{
		test_global_cnt++;
		if(test_global_cnt==0xFFFFFFFF)	//4294967295 
			test_global_cnt=0;
	}
	else
		test_global_cnt=0;
	
	MEM_Reset_Status();
	MsANL_Proc1Sec();
	
	MEAS_ProcMain();
	MEAS_SetOpr();

	PCD_Prc1Sec();
	MDB_PrcMain();
	DAT_ProcSave();
	MDB_PrcMain();

	DAC_OutCurr(sg_thick, DAC_CURR_IDX_NORMAL);
	RLY_ProcOut(sg_thick);
	UT0_PrcTx(MnOUT_GetProtocol());
	MDB_PrcMain();

	RTC_GetTime();
	MDB_PrcMain();
	SCRN_ProcDisp();
	MDB_PrcMain();
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

