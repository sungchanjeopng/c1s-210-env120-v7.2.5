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
// sys
#include "sys_def.h"
// self
#include "menu_data.h"
// bsp
#include "rtc_ds1302.h"
#include "flash_main.h"
#include "gpio_main.h"
#include "bsp_main.h"
#include "btn_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
// app
#include "dly_main.h"
#include "data_main.h"
// disp
#include "disp_main.h"
#include "disp_menu.h"
#include "disp_string.h"
// menu
#include "menu_main.h"
#include "menu_box.h"
#include "menu_system.h"
// font
#include "font_def.h"
#include "font_idx_kor.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
LS_MnDAT lMnDat;


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------
void MnDat_SaveEn(void)
{
	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X07, MnBOX_Y01, F_F, "Enable?");
	BTN_DelayMsec(BTN_WAIT_UD);
	lMnDat.fSave = MnBOX_SelArg4(MnSYS_LANG_ENG, lMnDat.fSave, MnBOX_X11, "NO", MnBOX_X10, "YES", 0, "", 0, "", 2);

	#if 0
	if(lMnDat.fSave)
    {
        RTC_GetTime();
		gLg_min = gRt.min;
		gLg_hour = gRt.hour;
	}
	#endif
}

void MnDat_Download(void)
{
	U08 fDn = FALSE;

	MnBOX_DispBox();


	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 14, 70,  25, 20, 0,	 0,   0,   0, 0,0,0,0,0,0,0));
			break;


#elif defined (_FT_LANG_CN)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 49, 45,	2, 100, 0,	 0,   0,	0, 0,0,0,0,0,0,0));
			break;

#elif defined (_FT_LANG_JP)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X03, MnBOX_Y01, F_F, _F_STR( 18, 5,  41, 39, 42,  23,	0,	 0, 0,0,0,0,0,0,0));
			break;
#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, MnBOX_Y01, F_F, "Download?");
			break;
	}

	

	BTN_DelayMsec(BTN_WAIT_UD);
	fDn = MnBOX_SelArg4(MnSYS_LANG_ENG, fDn, MnBOX_X11, "NO", MnBOX_X10, "YES", 0, "", 0, "", 2);

	if(!_SBIT_BTN_SET && fDn)
		LOG_OutData();

	SFRPAGE = CONFIG_PAGE;
}

void MnDat_Delete(void)
{
	U08 fDn = FALSE;

	MnBOX_DispBox();


	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X10, MnBOX_Y01, F_F, _F_STR( 46, 86,  0, 0, 0,	 0,   0,   0, 0,0,0,0,0,0,0));
			break;


#elif defined (_FT_LANG_CN)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 49, 45,	14, 112, 0,	 0,   0,	0, 0,0,0,0,0,0,0));
			break;

#elif defined (_FT_LANG_JP)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X10, MnBOX_Y01, F_F, _F_STR( 72, 55,  0, 0, 0,  0,	0,	 0, 0,0,0,0,0,0,0));
			break;
#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X07, MnBOX_Y01, F_F, "Delete?");
			break;
	}

	
	BTN_DelayMsec(BTN_WAIT_UD);
	fDn = MnBOX_SelArg4(MnSYS_LANG_ENG, fDn, MnBOX_X11, "NO", MnBOX_X10, "YES", 0, "", 0, "", 2);

	if(!_SBIT_BTN_SET && fDn)
	{
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X07, MnBOX_Y01, F_F, "         ");
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X04, MnBOX_Y02, F_F, "Delete...");
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y03, F_F, "         ");
		DLY_RunMsec(1000);
		LOG_ResetAddr();
	}

	if(lMnDat.fSave)
	{
		RTC_GetTime();
		gLg_min  = gRt.min;
		gLg_hour = gRt.hour;

		LOG_ProcData();
		LOG_SaveAddr1();
		LOG_SaveAddr2();
	}
}

void MnDat_Interval(void)
{
	S16 old = lMnDat.sav_intv;
	S16 sel = old;

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "Time");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y02, F_F, "(min)");
	BTN_DelayMsec(BTN_WAIT_UD);
	sel = MnBOX_SelArg4(MnSYS_LANG_ENG, sel, MnBOX_X13, "1", MnBOX_X12, "10", MnBOX_X12, "60", 0, "", 3);

	if(!_SBIT_BTN_EXIT)
		return;

	if(sel != old)
		lMnDat.sav_intv = sel;

	DISP_ClearScr();
	DpSTR_LineUpd(FT_EN_08x16, F_F, 20, 0,  "The logged data");
	DpSTR_LineUpd(FT_EN_08x16, F_F, 20, 16, "will be deleted");
	DpSTR_LineUpd(FT_EN_08x16, F_F, 32, 48, "OK - SAVE");
	DpSTR_LineUpd(FT_EN_08x16, F_F, 32, 80, "NO - EXIT");
	BTN_DelayMsec(BTN_WAIT_SET);

	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if(!_SBIT_BTN_EXIT)
	{
		lMnDat.sav_intv = old;
		return;
	}
	
	DISP_ClearScr();
	DpSTR_LineUpd(FT_EN_08x16, F_F, 28, 16,"Max. Log Time");

	switch(lMnDat.sav_intv)
	{
		case MnDAT_INTV_01M:	DpSTR_LineUpd(FT_EN_08x16, F_F, 4, 48,"  6 days from now on");		break;
		case MnDAT_INTV_10M:	DpSTR_LineUpd(FT_EN_08x16, F_F, 4, 48," 60 days from now on");		break;
		case MnDAT_INTV_60M:	DpSTR_LineUpd(FT_EN_08x16, F_F, 4, 48,"400 days from now on");		break;
	}

	switch(lMnDat.sav_intv)
	{
		case MnDAT_INTV_01M:	lMnDat.disp_01m = 0;		break;
		case MnDAT_INTV_10M:	lMnDat.disp_10m = 0;		break;
		case MnDAT_INTV_60M:	lMnDat.disp_60m = 0;		break;
	}

	DLY_RunMsec(1000);
	LOG_ResetAddr();

	if(lMnDat.fSave)
	{
		RTC_GetTime();
		gLg_min  = gRt.min;
		gLg_hour = gRt.hour;
		LOG_ProcData();

		LOG_SaveAddr1();
		LOG_SaveAddr2();
	}
}

void MnDat_DpTerm(void)
{
	U08 *pS0, *pS1;
	U08 old, sel;
	U08 f_dsp = FALSE;

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "Time");
	BTN_DelayMsec(BTN_WAIT_UD);

	switch(lMnDat.sav_intv)
	{
		case MnDAT_INTV_01M:	old = lMnDat.disp_01m;		break;
		case MnDAT_INTV_10M:	old = lMnDat.disp_10m;		break;
		case MnDAT_INTV_60M:	old = lMnDat.disp_60m;		break;
	}

	sel = old;

	do
	{
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{
			LDT_ClrBuff(MnBOX_LEFT, MnBOX_TOP+2+32, MnBOX_RIGHT-2, MnBOX_BOTTOM-2);
			BTN_DelayMsec(BTN_WAIT_UD);

			if(++sel > 1)
				sel = 0;
		}

		switch(lMnDat.sav_intv)
		{
			case MnDAT_INTV_01M:
				pS0 = "(hrs)";

				switch(sel)
				{
					case MnDAT_DISP_I0: 	pS1 = "12";		break;
					case MnDAT_DISP_I1: 	pS1 = "24";		break;
				}

				break;

			case MnDAT_INTV_10M:
				pS0 = "(days)";
				switch(sel)
				{
					case MnDAT_DISP_I0: 	pS1 = "1";		break;
					case MnDAT_DISP_I1: 	pS1 = "7";	    break;
				}
				break;

			case MnDAT_INTV_60M:
				pS0 = "(days)";
				switch(sel)
				{
					case MnDAT_DISP_I0: 	pS1 = "14";		break;
					case MnDAT_DISP_I1: 	pS1 = "30";		break;
				}
				break;
		}

		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y02, F_F, pS0);
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X12, MnBOX_Y03, F_F, pS1);
		LDT_UpdBuff(LCD_Y_START,LCD_Y_END);
	}
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if(!_SBIT_BTN_SET)
	{
		switch(lMnDat.sav_intv)
		{
			case MnDAT_INTV_01M:	lMnDat.disp_01m = sel;		break;
			case MnDAT_INTV_10M:	lMnDat.disp_10m = sel;		break;
			case MnDAT_INTV_60M:	lMnDat.disp_60m = sel;		break;
		}
	}
}

void DatDp_ListP1(U08* pSel)
{
	U08 *pStr;
	U08 c = MnDAT_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			#if defined (_FT_LANG_KR)			
			MnLST_DispClr(1);
			DpSTR_DrawLin(ft_loc, FALSE, MnDAT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(FKR_DE_, FKR_I__, FKR_TEO,1,1,1,1,1,1,1,1,1,1,1,1));
			
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L01_F_SAVE+1,   _F_STR(FKR_JEO, FKR_JANG,	1,		 1,		 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L02_DOWNLOAD+1, _F_STR(FKR_DA_, FKR_UN_,	FKR_RO_, FKR_DEU,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L03_DELETE+1,   _F_STR(FTR_SAG, FKR_JE_,	1,		 1,		 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L04_SAV_INTV+1, _F_STR(FKR_JEO, FKR_JANG,	FKR_JU_, FKR_GI_,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L05_DP_TERM+1,  _F_STR(FKR_HWA, FKR_MYEON,	FKR_JU_, FKR_GI_,1,1,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
                                                                 // 0123456789abcdef0123456789abcde
			DpSTR_DrawLin(ft_loc, FALSE, MnDAT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(49, 45,  90,41,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L01_F_SAVE+1,   _F_STR( 6,  183, 1,  1,   1,	1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L02_DOWNLOAD+1, _F_STR( 49,  45,  2, 100,   1,	1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L03_DELETE+1,   _F_STR( 49,  45, 14, 112,   1,   1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L04_SAV_INTV+1, _F_STR(105, 115, 49,  45, 110, 113,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L05_DP_TERM+1,  _F_STR( 53,  72, 85,  25,   1,   1,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
														         // 0123456789abcdef0123456789abcde
			DpSTR_DrawLin(ft_loc, FALSE, MnDAT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(21, 42,  17,1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L01_F_SAVE+1,   _F_STR(46,  60, 1, 1, 1, 1, 1,  1,   1,   1,  1, 1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L02_DOWNLOAD+1, _F_STR(18,  5, 41, 39, 42, 23, 1,  1,   1,   1,  1, 1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L03_DELETE+1,   _F_STR(72,  55, 1, 1, 1, 1, 1,  1,   1,   1,  1, 1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L04_SAV_INTV+1,_F_STR(46,  60, 87, 88, 1, 1, 1,  1,   1,   1,  1, 1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnDAT_L05_DP_TERM+1,  _F_STR(79,  76, 68, 87, 78, 58, 1,  1,   1,   1,  1, 1,1,1,1));
			#endif
			break;
		case MnSYS_LANG_ENG:
		default:
                                                                 // 0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE,     MnDAT_L00_IDLE+1,	   "            DATA               ");
			MnLST_DispStr(ft_eng, pSel[c++], MnDAT_L01_F_SAVE+1,   "Save                           ");
			MnLST_DispStr(ft_eng, pSel[c++], MnDAT_L02_DOWNLOAD+1, "Download                       ");
			MnLST_DispStr(ft_eng, pSel[c++], MnDAT_L03_DELETE+1,   "Delete                         ");
			MnLST_DispStr(ft_eng, pSel[c++], MnDAT_L04_SAV_INTV+1, "Save Interval                  ");
			MnLST_DispStr(ft_eng, pSel[c++], MnDAT_L05_DP_TERM+1,  "Display Term                   ");
			break;
	}

	if	(lMnDat.fSave)	pStr = "ON";
	else				pStr = "OFF";

	DpSTR_DrawLin(ft_eng, pSel[MnDAT_L01_F_SAVE], 180, MnSTR_Y_HEIGHT*1, pStr);	
}
#if 0
void DatDp_ListEngP1(U08* pSel)
{
	U08 *pStr;
	U08 c  = MnDAT_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
													//   0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,	 MnDAT_L00_IDLE+1,		"            DATA               ");
	MnLST_DispStr(ft, pSel[c++], MnDAT_L01_F_SAVE+1,	"Save                           ");
	MnLST_DispStr(ft, pSel[c++], MnDAT_L02_DOWNLOAD+1,	"Download                       ");
	MnLST_DispStr(ft, pSel[c++], MnDAT_L03_DELETE+1,	"Delete                         ");
	MnLST_DispStr(ft, pSel[c++], MnDAT_L04_SAV_INTV+1,	"Save Interval                  ");
	MnLST_DispStr(ft, pSel[c++], MnDAT_L05_DP_TERM+1,	"Display Term                   ");

	if(lMnDat.fSave)	pStr = "ON";
	else				pStr = "OFF";

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnDAT_L01_F_SAVE], 180, MnSTR_Y_HEIGHT*1, pStr);
}

#if defined (_FT_LANG_KR)
void DatDp_ListKorP1(U08* pSel)
{
	U08 i;
	U08 *pStr;

	MnLST_DispClr(1);
	_F_SS(FKR_DE_,	FKR_I__,	FKR_TEO,	1,1,1,1,1,1,1,1,1,1,1,1);
	DpSTR_DrawLin(FT_LC_16x16, FALSE, MnDAT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, gFt_str);

	for(i=(MnDAT_L00_IDLE+1); i<MnDAT_LST_MAX; i++)
	{
		switch(i)
		{
			case MnDAT_L01_F_SAVE:		_F_SS(FKR_JEO,  FKR_JANG,	1,			1,			1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnDAT_L02_DOWNLOAD:	_F_SS(FKR_DA_,	FKR_UN_,	FKR_RO_,	FKR_DEU,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnDAT_L03_DELETE:		_F_SS(FTR_SAG,	FKR_JE_,	1,			1,			1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnDAT_L04_SAV_INTV:	_F_SS(FKR_JEO,	FKR_JANG,	FKR_JU_,	FKR_GI_,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnDAT_L05_DP_TERM:		_F_SS(FKR_HWA,	FKR_MYEON,	FKR_JU_,	FKR_GI_,	1,1,1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}

	if(lMnDat.fSave)	pStr = "ON";
	else				pStr = "OFF";

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnDAT_L01_F_SAVE], 180, MnSTR_Y_HEIGHT*MnDAT_L01_F_SAVE, pStr);
}

#elif defined (_FT_LANG_CN)
void DatDp_ListChnP1(U08* pSel)
{
	U08 i;
	U08 *pStr;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "            DATA               ");

	for(i=(MnDAT_L00_IDLE+1); i<MnDAT_LST_MAX; i++)
	{
		switch(i)
		{
			case MnDAT_L01_F_SAVE:		_F_SS(49,	45,		90,		41,		1,		1,		1,1,1,1,1,1,1,1,1);		break;
			case MnDAT_L02_DOWNLOAD:	_F_SS(49,	45,		2,		100,	1,		1,		1,1,1,1,1,1,1,1,1);		break;
			case MnDAT_L03_DELETE:		_F_SS(49,	45,		14,		112,	1,		1,		1,1,1,1,1,1,1,1,1);		break;
			case MnDAT_L04_SAV_INTV:	_F_SS(105,	115,	49,		45,		110,	113,	1,1,1,1,1,1,1,1,1);		break;
			case MnDAT_L05_DP_TERM:		_F_SS(53,	72,		85,		25,		1,		1,		1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}

	if(lMnDat.fSave)	pStr = "ON";
	else				pStr = "OFF";

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnDAT_L01_F_SAVE], 180, MnSTR_Y_HEIGHT*MnDAT_L01_F_SAVE, pStr);
}

#elif defined (_FT_LANG_JP)
void DatDp_ListJpnP1(U08* pSel)
{
	U08 i;
	U08 *pStr;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "            DATA               ");

	for(i=(MnDAT_L00_IDLE+1); i<MnDAT_LST_MAX; i++)
	{
		switch(i)
		{
			case MnDAT_L01_F_SAVE:		_F_SS(160,	95,	170,171,110,118,121,87,	0,	1,	1,	1,1,1,1);		break;
			case MnDAT_L02_DOWNLOAD: 	_F_SS(115,	89,	166,160,170,124,0,  1,	1,	1,	1,	1,1,1,1);		break;
			case MnDAT_L03_DELETE:		_F_SS(122,	157,170,123,0,  1,  1,  1,	1,	1,	1,	1,1,1,1);		break;
			case MnDAT_L04_SAV_INTV: 	_F_SS(104,	166,138,157,166,99, 171,106,152,89, 96,	0,1,1,1);		break;
			case MnDAT_L05_DP_TERM:		_F_SS(122,	86,	108,138,159,87, 171,107,94, 166,0,	1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}

	if(lMnDat.fSave)	pStr = "ON";
	else				pStr = "OFF";

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnDAT_L01_F_SAVE], 180, MnSTR_Y_HEIGHT*MnDAT_L01_F_SAVE, pStr);
}

#endif
#endif
void DatDp_ProcList(U08 sel)
{
	U08 i;
	U08 page = MNU_PAGE_01;
	U08 font = FT_EN_08x16;
	U08 pSel[MnDAT_LST_MAX] = {0, };

	pSel[sel] = TRUE;

	if		(sel > (MNU_LIST_NUM*2))		page = MNU_PAGE_03;
	else if (sel > (MNU_LIST_NUM*1))		page = MNU_PAGE_02;
	else									page = MNU_PAGE_01;

	if		(sel > (MNU_LIST_NUM*2))		pSel[sel-(MNU_LIST_NUM*2)] = TRUE;
	else if	(sel > (MNU_LIST_NUM*1))		pSel[sel-(MNU_LIST_NUM*1)] = TRUE;
	else									pSel[sel-(MNU_LIST_NUM*0)] = TRUE;

#if 1
	switch(page)
	{		
		case MNU_PAGE_01:	DatDp_ListP1(pSel);	break;
		default:
			return;
	}
#else
	if(page > MNU_PAGE_01)
		return;

	switch(MnSYS_GetLang())
	{
#if 1
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			DatDp_ListKorP1(pSel);

#elif defined (_FT_LANG_CN)
			DatDp_ListChnP1(pSel);

#elif defined (_FT_LANG_JP)
			DatDp_ListJpnP1(pSel);

#endif
			break;

#else
		case MnSYS_LANG_KOR:		DatDp_ListKorP1(pSel);		break;
		case MnSYS_LANG_CHN:		DatDp_ListChnP1(pSel);		break;
		case MnSYS_LANG_JPN:		DatDp_ListJpnP1(pSel);		break;
#endif

		case MnSYS_LANG_ENG:
		default:
			DatDp_ListEngP1(pSel);
			break;
	}
#endif
	for(i=(MnDAT_LST_MAX+1); i<=(MNU_LIST_NUM+1); i++)
		MnLST_DispClr(i);

    LDT_UpdBuff(LCD_Y_START,LCD_Y_END);
	DLY_RunMsec(100);
}

void DatChk_Vari(void)
{
	if(lMnDat.fSave    >  TRUE)				lMnDat.fSave    = TRUE;
	if(lMnDat.sav_intv >= MnDAT_INTV_MAX)	lMnDat.sav_intv = MnDAT_INTV_01M;
	if(lMnDat.disp_01m >= MnDAT_DISP_MAX)	lMnDat.disp_01m = MnDAT_DISP_I0;
	if(lMnDat.disp_10m >= MnDAT_DISP_MAX)	lMnDat.disp_10m = MnDAT_DISP_I0;
	if(lMnDat.disp_60m >= MnDAT_DISP_MAX)	lMnDat.disp_60m = MnDAT_DISP_I0;
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Access Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Get
U08 MnDAT_GetFgSave(void)		{	return lMnDat.fSave;		}
U08 MnDAT_GetSavIntv(void)		{	return lMnDat.sav_intv;		}
U08 MnDAT_GetDisp01m(void)		{	return lMnDat.disp_01m;		}
U08 MnDAT_GetDisp10m(void)		{	return lMnDat.disp_10m;		}
U08 MnDAT_GetDisp60m(void)		{	return lMnDat.disp_60m;		}
// Set


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Memory Access
//------------------------------------------------------------------------------------------------------------------------------
void MnDAT_MemLoad(void)
{
	lMnDat.fSave    = FLS_Rd1Byte(F010_DAT_F_SAVE);
	lMnDat.sav_intv = FLS_Rd1Byte(F026_DAT_SAV_INTV);
	lMnDat.disp_01m = FLS_Rd1Byte(F027_DAT_DISP_01M);
	lMnDat.disp_10m = FLS_Rd1Byte(F028_DAT_DISP_10M);
	lMnDat.disp_60m = FLS_Rd1Byte(F029_DAT_DISP_60M);
}

void MnDAT_MemSave(void)
{
	FLS_Wr1Byte(F010_DAT_F_SAVE,   lMnDat.fSave);
	FLS_Wr1Byte(F026_DAT_SAV_INTV, lMnDat.sav_intv);
	FLS_Wr1Byte(F027_DAT_DISP_01M, lMnDat.disp_01m);
	FLS_Wr1Byte(F028_DAT_DISP_10M, lMnDat.disp_10m);
	FLS_Wr1Byte(F029_DAT_DISP_60M, lMnDat.disp_60m);
}

void MnDAT_MemFtry(void)
{
	FLS_Wr1Byte(F010_DAT_F_SAVE,   TRUE);
	FLS_Wr1Byte(F026_DAT_SAV_INTV, MnDAT_INTV_01M);
	FLS_Wr1Byte(F027_DAT_DISP_01M, MnDAT_DISP_I0);
	FLS_Wr1Byte(F028_DAT_DISP_10M, MnDAT_DISP_I0);
	FLS_Wr1Byte(F029_DAT_DISP_60M, MnDAT_DISP_I0);
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MnDAT_InitVari(void)
{
	MnDAT_MemLoad();
	DatChk_Vari();
}

void MnDAT_ProcMain(void)
{
	U08 fDp = FALSE;
	U08 sel = MnDAT_L00_IDLE + 1;

	DatDp_ProcList(sel);

	do
	{
		if(!_SBIT_BTN_DOWN)
		{
			if(sel < (MnDAT_LST_MAX-1))		sel++;
			else 							sel = MnDAT_L00_IDLE + 1;
			fDp = TRUE;
		}

		if(!_SBIT_BTN_UP)
		{
			if(sel > (MnDAT_L00_IDLE+1))	sel--;
			else 							sel = MnDAT_LST_MAX - 1;
			fDp = TRUE;
		}

		if(!_SBIT_BTN_SET)
		{
			switch(sel)
			{	// Page 1
				case MnDAT_L01_F_SAVE:		MnDat_SaveEn();			break;
				case MnDAT_L02_DOWNLOAD:	MnDat_Download();		break;
				case MnDAT_L03_DELETE:		MnDat_Delete(); 		break;
				case MnDAT_L04_SAV_INTV:	MnDat_Interval(); 		break;
				case MnDAT_L05_DP_TERM:		MnDat_DpTerm(); 		break;
				// Page 2
			}
			fDp = TRUE;
		}

		if(fDp)
		{
			DatDp_ProcList(sel);
			fDp = FALSE;
		}
	}
	while(_SBIT_BTN_EXIT);
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

