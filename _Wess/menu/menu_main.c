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
#include <math.h>
#include <intrins.h>
#include <stdarg.h>
#include <string.h>
// chipset
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// self
#include "menu_main.h"
// bsp
#include "bsp_main.h"
#include "tmr_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "gpio_main.h"
#include "flash_main.h"
#include "data_main.h"
#include "rtc_ds1302.h"
// font
#include "font_def.h"
#include "font_idx_kor.h"
// app
#include "btn_main.h"
#include "dly_main.h"
#include "app_main.h"
#include "isr_main.h"
// disp
#include "disp_main.h"
#include "disp_menu.h"
#include "disp_string.h"
// menu
#include "menu_box.h"
#include "menu_pass.h"
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


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------
void MnProc_ExitSection(void)
{
	DISP_ClearScr();
	DpSTR_LineUpd(FT_EN_08x16, 0, 60, 55,"Storing data...");
	DLY_RunMsec(1000);

	FLS_WriteAll();

	TMR_CfgResume();
	if(MnDAT_GetFgSave())
	{
		RTC_GetTime();
		gLg_min=gRt.min;
		gLg_hour=gRt.hour;
	}
	gIsr_tm3_cnt = 20;
}	
#if 0
void MnDp_ListEng(U08 sel)
{
	U08 c  = MENU_S01_SYSTEM;
	U08 ft = FT_EN_08x16;
	U08 pSel[MENU_SCT_NUMB] = {0, };
	pSel[sel] = TRUE;
	#if 1
													  //0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, pSel[c++], MENU_S01_SYSTEM+1,  "System                         ");
	MnLST_DispStr(ft, pSel[c++], MENU_S02_MEASURE+1, "Measurement                    ");
	MnLST_DispStr(ft, pSel[c++], MENU_S03_OUTPUT+1,  "Output                         ");
	MnLST_DispStr(ft, pSel[c++], MENU_S04_DATA+1,	   "Data                           ");
	#else
													//0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, pSel[c++], MENU_S01_SYSTEM+1,	 "System                         ");
	MnLST_DispStr(ft, pSel[c++], MENU_S02_MEASURE+1, "Measurement                    ");
	MnLST_DispStr(ft, pSel[c++], MENU_S03_OUTPUT+1,	 "Output                         ");
	MnLST_DispStr(ft, pSel[c++], MENU_S04_DATA+1,	 "Data                           ");
	#endif
}

#if defined (_FT_LANG_KR)
void MnDp_ListKor(U08 sel)
{
#if 1
	U08 c  = MENU_S01_SYSTEM;
	U08 ft = FT_LC_16x16;
	U08 pSel[MENU_SCT_NUMB] = {0, };

	pSel[sel] = TRUE;

	MnLST_DispStr(ft, pSel[c++], MENU_S01_SYSTEM+1,  _F_STR(FKR_SI_,	 FKR_SEU,	FKR_TEM, 1,1,1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft, pSel[c++], MENU_S02_MEASURE+1, _F_STR(FKR_CHEUG, FKR_JEONG,	1,		 1,1,1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft, pSel[c++], MENU_S03_OUTPUT+1,  _F_STR(FKR_CHUL,	 FKR_LYEOG,	1,		 1,1,1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft, pSel[c++], MENU_S04_DATA+1,	   _F_STR(FKR_DE_,	 FKR_I__,	FKR_TEO, 1,1,1,1,1,1,1,1,1,1,1,1));

#else
	U08 i;
	U08 pSel[MENU_SCT_NUMB] = {0, };

	pSel[sel] = TRUE;

	for(i=(MENU_S00_IDLE+1); i<MENU_SCT_NUMB; i++)
	{
		switch(i)
		{
			case MENU_S01_SYSTEM:		_F_SS(FKR_SI_,	FKR_SEU,	FKR_TEM,	1,		1,1,1,1,1,1,1,1,1,1,1);		break;
			case MENU_S02_MEASURE:		_F_SS(FKR_CHEUG,FKR_JEONG,	1,			1,		1,1,1,1,1,1,1,1,1,1,1);		break;
			case MENU_S03_OUTPUT:		_F_SS(FKR_CHUL,	FKR_LYEOG,	1,			1,		1,1,1,1,1,1,1,1,1,1,1);		break;
			case MENU_S04_DATA:			_F_SS(FKR_DE_,	FKR_I__,	FKR_TEO,	1,		1,1,1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
#endif
}

#elif defined (_FT_LANG_CN)
void MnDp_ListChn(U08 sel)
{
	U08 i;
	U08 pSel[MENU_SCT_NUMB] = {0, };

	pSel[sel] = TRUE;

	for(i=(MENU_S00_IDLE+1); i<MENU_SCT_NUMB; i++)
	{
		switch(i)
		{
			case MENU_S01_SYSTEM:		_F_SS(106, 108, 121, 147,   1, 110, 118, 121, 87, 1,1,1,1,1,1);			break;
			case MENU_S02_MEASURE:		_F_SS(104, 170, 134, 108,   1, 110, 118, 121, 87, 1,1,1,1,1,1);			break;
			case MENU_S03_OUTPUT:		_F_SS(160,  95, 170,   1, 110, 118, 121, 87, 1,   1,1,1,1,1,1);			break;
			case MENU_S04_DATA:			_F_SS(157, 159, 170,   1, 110, 118, 121, 87, 1,   1,1,1,1,1,1);			break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
}


#elif defined (_FT_LANG_JP)
void MnDp_ListJpn(U08 sel)
{
	U08 i;
	U08 pSel[MENU_SCT_NUMB] = {0, };

	pSel[sel] = TRUE;

	for(i=(MENU_S00_IDLE+1); i<MENU_SCT_NUMB; i++)
	{
		switch(i)
		{
			case MENU_S01_SYSTEM: 		_F_SS(79, 80, 91, 82,	0,1,1,1,1,1,1,1,1,1,1);		break;
			case MENU_S02_MEASURE:		_F_SS(55, 16, 0,  1,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MENU_S03_OUTPUT:		_F_SS(49, 45, 90, 41,	0,1,1,1,1,1,1,1,1,1,1);		break;
			case MENU_S04_DATA: 		_F_SS(81, 69, 24, 0,	1,1,1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
}

#endif
#endif
void MnuMain_DspList(U08 idx)
{
#if 1
	U08 i;
	U08 c  = MENU_S01_SYSTEM;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;
	U08 pSel[MENU_SCT_NUMB] = {0, };

	if(idx > 8)
		return;

	pSel[idx] = TRUE;


	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			#if defined (_FT_LANG_KR)
			MnLST_DispClr(1);
			DpSTR_DrawLin(ft_loc, FALSE,	 MNU_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(FKR_ME,  FKR_NYU, 1,1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S01_SYSTEM+1,  _F_STR(FKR_SI_,   FKR_SEU,	  FKR_TEM, 1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S02_MEASURE+1, _F_STR(FKR_CHEUG, FKR_JEONG, 1,	   1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S03_OUTPUT+1,  _F_STR(FKR_CHUL,  FKR_LYEOG, 1,	   1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S04_DATA+1,    _F_STR(FKR_DE_,   FKR_I__,   FKR_TEO, 1,1,1,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
			DpSTR_DrawLin(ft_loc, FALSE,	 MNU_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(180, 18, 1, 1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S01_SYSTEM+1,  _F_STR(79,  80,  91,  82,   1, 1, 1, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S02_MEASURE+1, _F_STR(66, 107,   187,   82,   1, 1, 1, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S03_OUTPUT+1,  _F_STR(101, 12,   1,   1,   1, 1, 1, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S04_DATA+1,    _F_STR(49,  45,  90,  41,   1, 1, 1, 1, 1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
                                      			         	 //0123456789abcdef0123456789abcde
			DpSTR_DrawLin(ft_loc, FALSE,	 MNU_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(95, 24, 33, 42,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S01_SYSTEM+1,  _F_STR(13, 15, 20, 31, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S02_MEASURE+1, _F_STR(81, 73,  1,  1, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S03_OUTPUT+1,  _F_STR(48, 51, 1, 1, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MENU_S04_DATA+1,    _F_STR(21, 42, 17,  1, 1,1,1,1,1,1,1,1,1,1,1));
			#endif
			break;
		case MnSYS_LANG_ENG:
		default:
                                      			    		 //0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE, 	 MENU_S00_IDLE+1, "            MENU               ");															   // 0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, pSel[c++], MENU_S01_SYSTEM+1,  "System                         ");
			MnLST_DispStr(ft_eng, pSel[c++], MENU_S02_MEASURE+1, "Measurement                    ");
			MnLST_DispStr(ft_eng, pSel[c++], MENU_S03_OUTPUT+1,  "Output                         ");
			MnLST_DispStr(ft_eng, pSel[c++], MENU_S04_DATA+1,    "Data                           ");
			break;
	}
#else
	U08 i;

	if(idx > 8)
		return;
													// 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, F_F, 0, DpMNU_STR_Y01, "            MENU               ");
	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			MnDp_ListKor(idx);

#elif defined (_FT_LANG_CN)
			MnDp_ListChn(idx);

#elif defined (_FT_LANG_JP)
			MnDp_ListJpn(idx);

#endif
			break;

		case MnSYS_LANG_ENG:
		default:
			MnDp_ListEng(idx);
			break;
	}
#endif

	for(i=MENU_SCT_NUMB+1; i<=(MNU_LIST_NUM+1); i++)
		MnLST_DispClr(i);

    LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Access Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Get
// Set


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MENU_InitVari(void)
{
	MnSYS_InitVari();
	
	MnFTR_InitVari();
	
	MnMSR_InitVari();
	MnOUT_InitVari();
	MnDAT_InitVari();

	MnPAS_InitVari();
	MnEGN_InitVari();
	MnTST_InitVari();
}

void MENU_ProcMain(void)
{
	U08 sel = 1;
	U08 fDp = FALSE;

	DISP_ClearScr();
	MnuMain_DspList(sel);
	BTN_DelayMsec(BTN_WAIT_SET);

	do
	{
		if(!_SBIT_BTN_DOWN)
		{
		    if(sel < (MENU_SCT_NUMB-1))		sel++;
		    else                            sel = MENU_S00_IDLE +1;
			fDp = TRUE;
		}

		if(!_SBIT_BTN_UP)
		{
		    if(sel > (MENU_S00_IDLE+1))		sel--;
		    else                            sel = MENU_SCT_NUMB - 1;
			fDp = TRUE;
		}

		if(!_SBIT_BTN_SET)
		{
			switch(sel)
			{
				case MENU_S01_SYSTEM:		MnSYS_ProcMain();		break;
				case MENU_S02_MEASURE:    	MnMSR_ProcMain();		break;
				case MENU_S03_OUTPUT:		MnOUT_ProcMain();		break;
				case MENU_S04_DATA:			MnDAT_ProcMain();		break;
			}

			fDp = TRUE;
		}

		if(fDp)
		{
			fDp = FALSE;
			MnuMain_DspList(sel);
			BTN_DelayMsec(BTN_WAIT_SET);
		}
	}
	while(_SBIT_BTN_EXIT);

	MnProc_ExitSection();
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

