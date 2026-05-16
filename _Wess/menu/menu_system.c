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
#include "menu_system.h"
// bsp
#include "bsp_main.h"
#include "flash_main.h"
#include "gpio_main.h"
#include "rtc_ds1302.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "btn_main.h"
#include "prtc_main.h"
#include "rf_main.h"
#include "uart_main.h"
// app
#include "dly_main.h"
#include "app_main.h"
#include "measure_main.h"
#include "data_main.h"
// disp
#include "disp_main.h"
#include "disp_menu.h"
#include "disp_string.h"
// menu
#include "menu_main.h"
#include "menu_box.h"
#include "menu_engineer.h"
#include "menu_data.h"
#include "menu_quick.h"
// font
#include "font_def.h"
#include "font_idx_kor.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
LS_MnSYS lMnSys;


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - Item
//------------------------------------------------------------------------------------------------------------------------------
void MnSys_Password(void)
{
	U16 old = lMnSys.pass;
	U16 sel = old;

	MnBOX_DispBox();


	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
		#if defined (_FT_LANG_KR)			
			MnBOX_DispStr(MnSYS_LANG_LOC, MnBOX_X05, MnBOX_Y01, F_F, _F_STR(44, 35, 38, 105, 0,	0, 0,0,0,0,0,0,0,0,0));
			sel = MnBOX_CfgNumb(old, 0, 9999, 1, 0, 1, "");
			break;

		#elif defined (_FT_LANG_CN)
			MnBOX_DispStr(MnSYS_LANG_LOC, MnBOX_X09, MnBOX_Y01, F_F, _F_STR(29, 71, 0, 0, 0,	0, 0,0,0,0,0,0,0,0,0));
			sel = MnBOX_CfgNumb(old, 0, 9999, 1, 0, 1, "");
			break;

		#elif defined (_FT_LANG_JP)
			MnBOX_DispStr(MnSYS_LANG_LOC, MnBOX_X05, MnBOX_Y01, F_F, _F_STR(26, 15, 40, 42, 23,	0, 0,0,0,0,0,0,0,0,0));
			sel = MnBOX_CfgNumb(old, 0, 9999, 1, 0, 1, "");
		#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X07, MnBOX_Y01, F_F, "New Code");
			sel = MnBOX_CfgNumb(old, 0, 9999, 1, 0, 1, "");
			break;
	}

	if(sel != old)
		lMnSys.pass = sel;
}

void MnSys_Time(void)
{
	int i;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U08 p_st2[APP_STR_LENGTH] = {0, };
	U08 p_st3[APP_STR_LENGTH] = {0, };

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "Year");
	DpSTR_LineUpd(FT_EN_08x16, 0, 139, 64, "20  ");
	APP_Num2Str(gRt.yy, p_str, 2, 0, TRUE);
	DpSTR_LineUpd(FT_EN_08x16, FALSE, 157, 64, p_str);
	BTN_DelayMsec(BTN_WAIT_SET);
	gRt.yy = RTC_AdjTime(gRt.yy, 157, 64, RTC_SET_IDX_YY);

    // Date
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "Date");
	DpSTR_LineUpd(FT_EN_08x16, 0, 136, 64, "  /  ");

    // Month
	APP_Num2Str(gRt.mm,   p_st2, 2, 0, TRUE);
	APP_Num2Str(gRt.date, p_st3, 2, 0, TRUE);

	DpSTR_LineUpd(FT_EN_08x16, FALSE, 137, 64, p_st2);
	DpSTR_LineUpd(FT_EN_08x16, FALSE, 163, 64, p_st3);

	for(i=0; i<APP_STR_LENGTH; i++)
	{
		p_str[i] = 0;
		p_st2[i] = 0;
		p_st3[i] = 0;
	}
	BTN_DelayMsec(BTN_WAIT_SET);
	gRt.mm = RTC_AdjTime(gRt.mm, 136, 64, RTC_SET_IDX_MM);
	BTN_DelayMsec(BTN_WAIT_SET);

    // Date
	gRt.date = RTC_AdjTime(gRt.date, 163, 64, 3);

    // Time
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "Time");

    // HH:MM
	DpSTR_LineUpd(FT_EN_08x16, 0, 136, 64, "  :  ");
	APP_Num2Str(gRt.hour, p_st2, 2, 0, TRUE);
	APP_Num2Str(gRt.min,  p_st3, 2, 0, TRUE);

	DpSTR_LineUpd(FT_EN_08x16, FALSE, 136, 64, p_st2);
	DpSTR_LineUpd(FT_EN_08x16, FALSE, 163, 64, p_st3);

	BTN_DelayMsec(BTN_WAIT_SET);
	gRt.hour = RTC_AdjTime(gRt.hour, 136, 64, 4);

	BTN_DelayMsec(BTN_WAIT_SET);
	gRt.min  = RTC_AdjTime(gRt.min, 163, 64, 5);

	RTC_SetTime(gRt.min, gRt.hour, gRt.date, gRt.mm, gRt.dd, gRt.yy);

	if(MnDAT_GetFgSave()) 
	{
        RTC_GetTime();
		gLg_min = gRt.min;
		gLg_hour = gRt.hour;
	}
}

void MnSys_Language(void)
{
	S16 old = lMnSys.lang;
	U08 sel = old;

	MnBOX_DispBox();

#if 1
	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
	#if defined (_FT_LANG_KR)			
			MnBOX_DispStr(MnSYS_LANG_LOC, MnBOX_X05, MnBOX_Y01, F_F, _F_STR(62, 61, 49, 95, 0,	0, 0,0,0,0,0,0,0,0,0));
			break;

	#elif defined (_FT_LANG_CN)
			MnBOX_DispStr(MnSYS_LANG_LOC, MnBOX_X05, MnBOX_Y01, F_F, _F_STR(46, 5, 93, 88, 0,	0, 0,0,0,0,0,0,0,0,0));
			break;

	#elif defined (_FT_LANG_JP)
			MnBOX_DispStr(MnSYS_LANG_LOC, MnBOX_X05, MnBOX_Y01, F_F, _F_STR(80, 83, 82, 61, 0,	0, 0,0,0,0,0,0,0,0,0));
	#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, MnBOX_Y01, F_F, "Language");
			break;
	}

	if(lMnSys.lang == MnSYS_LANG_LOC)
	{
		switch(lMnSys.loc)
		{
			case MnSYS_LOC_CHN:		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y03, F_F, "CHN");		break;
			case MnSYS_LOC_JPN:		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y03, F_F, "JPN");		break;
			case MnSYS_LOC_KOR:
			default:				MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y03, F_F, "KOR");		break;
		}
	}
	else
	{
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y03, F_F, "ENG");
	}

#else
	if(lMnSys.lang == MnSYS_LANG_CHN)
    {
		FNT_SetStr(46, 5, 93, 88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		MnBOX_DispStr(MnSYS_LANG_CHN, MnBOX_X06, 32, gFt_str, 0);
	}
    else
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, 32, "Language", 0);

	if(lMnSys.lang == MnSYS_LANG_CHN)
    {
		switch(sel)
        {
    		case MnSYS_LANG_ENG:    FNT_SetStr(84,		93,		0,0,0,0,0,0,0,0,0,0,0,0,0);		break;
    		case MnSYS_LANG_KOR:    FNT_SetStr(116,		93,		0,0,0,0,0,0,0,0,0,0,0,0,0);		break;
    		case MnSYS_LANG_JPN:    FNT_SetStr(50,		93,		0,0,0,0,0,0,0,0,0,0,0,0,0);		break;
    		case MnSYS_LANG_CHN:    FNT_SetStr(62,		93,		0,0,0,0,0,0,0,0,0,0,0,0,0);		break;
		}

		MnBOX_DispStr(MnSYS_LANG_CHN, MnBOX_X10, 64, gFt_str, 0);
	}
    else
	{
		switch(sel)
        {
    		case MnSYS_LANG_ENG:    MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X05, 64, " English ", 0);	break;
    		case MnSYS_LANG_KOR:    MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, 64, " Korean ", 0);	break;
    		case MnSYS_LANG_JPN:    MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, 64, "Japanese", 0);	break;
    		case MnSYS_LANG_CHN:    FNT_SetStr(62,		93,		0,0,0,0,0,0,0,0,0,0,0,0,0);
	                                MnBOX_DispStr(MnSYS_LANG_CHN, MnBOX_X10, 64, gFt_str, 0);	break;
		}
	}
#endif
	BTN_DelayMsec(BTN_WAIT_SET);

	do
    {
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN) 
		{
#if 1
			if(sel == MnSYS_LANG_ENG)	sel = MnSYS_LANG_LOC;
			else						sel = MnSYS_LANG_ENG;

			if(sel == MnSYS_LANG_LOC)
			{
				switch(lMnSys.loc)
				{
					case MnSYS_LOC_CHN: 	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y03, F_F, "CHN");		break;
					case MnSYS_LOC_JPN: 	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y03, F_F, "JPN");		break;
					case MnSYS_LOC_KOR:
					default:				MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y03, F_F, "KOR");		break;
				}
			}
			else
			{
				MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y03, F_F, "ENG");
			}
#else
#if defined (_FT_LANG_CN)
			if(sel == MnSYS_LANG_JPN)
                sel = MnSYS_LANG_CHN;

			if(_SBIT_BTN_DOWN == BTN_STT_PRESS)
			{
				if     (sel == MnSYS_LANG_ENG)      sel = MnSYS_LANG_CHN;
				else if(sel == MnSYS_LANG_CHN) 	    sel = MnSYS_LANG_KOR;
				else                        		sel--;
			}
			else if(_SBIT_BTN_UP == BTN_STT_PRESS)
			{
				if     (sel == MnSYS_LANG_CHN)      sel = MnSYS_LANG_ENG;
				else if(sel == MnSYS_LANG_KOR)      sel = MnSYS_LANG_CHN;
				else                          		sel++;
			}
#else
			if(sel == MnSYS_LANG_CHN)
                sel = MnSYS_LANG_JPN;

			if(_SBIT_BTN_DOWN == BTN_STT_PRESS)
			{
				if(sel == MnSYS_LANG_ENG)   sel = MnSYS_LANG_JPN;
				else                	    sel--;
			}
			else if(_SBIT_BTN_UP == BTN_STT_PRESS)
			{
				if(sel == MnSYS_LANG_JPN)   sel = MnSYS_LANG_ENG;
				else                    	sel++;
			}
#endif
			if(lMnSys.lang == MnSYS_LANG_CHN)
            {
				switch(sel)
                {
    				case MnSYS_LANG_ENG:	FNT_SetStr(84, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);		break;
    				case MnSYS_LANG_KOR:	FNT_SetStr(116,93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);		break;
    				case MnSYS_LANG_JPN:	FNT_SetStr(50, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);		break;
    				case MnSYS_LANG_CHN:	FNT_SetStr(62, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);		break;
				}
				MnBOX_DispStr(MnSYS_LANG_CHN, MnBOX_X10, 64, gFt_str, 0);
			}
            else
			{
				switch(sel)
                    {
                    case MnSYS_LANG_ENG:    MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X05, 64, " English ", 0);    break;
    				case MnSYS_LANG_KOR:	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, 64, " Korean ", 0);     break;
    				case MnSYS_LANG_JPN:    MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, 64, "Japanese", 0);     break;
    				case MnSYS_LANG_CHN:    _F_SS(62, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	                                        MnBOX_DispStr(MnSYS_LANG_CHN, MnBOX_X10, 64, gFt_str, 0);   break;
				}
			}
#endif
			BTN_DelayMsec(BTN_WAIT_UD);
		}
	}
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if     (!_SBIT_BTN_EXIT)	lMnSys.lang = old;
    else if(!_SBIT_BTN_SET)		lMnSys.lang = sel;
}

void MnSys_Version(void)
{
	return;
}


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - Disp. List
//------------------------------------------------------------------------------------------------------------------------------
#if 1
void SysDp_ListP1(U08* pSel)
{
	U08 c = MnDAT_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			#if defined (_FT_LANG_KR)			
			MnLST_DispClr(1);
			DpSTR_DrawLin(ft_loc, FALSE, MnSYS_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(FKR_SI_, FKR_SEU, FKR_TEM, 1,1,1,1,1,1,1,1,1,1,1,1));
			
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L01_PASSCODE+1, _F_STR(FKR_BI_,	 FKR_MIL,  FKR_BEON, FKR_HO_, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L02_LANG+1,	   _F_STR(FKR_EON,	 FKR_EO_,  FKR_SEON, FKR_TAEG,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L03_TIME+1,	   _F_STR(FKR_HYEON, FKR_JAE,  FKR_SI_,	 FKR_GAN, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L04_VERSION+1,  _F_STR(FKR_BEO,	 FKR_JEON, 1,		 1,		  1,1,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
																//	0123456789abcdef0123456789abcde
			DpSTR_DrawLin(ft_loc, FALSE, MnSYS_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(79, 80, 91, 82,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L01_PASSCODE+1, _F_STR(29,  71,	1,	1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L02_LANG+1,	   _F_STR(46,   5, 93, 88,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L03_TIME+1,	   _F_STR(173, 110, 1, 1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L04_VERSION+1,  _F_STR(98,	3, 68, 57,1,1,1,1,1,1,1,1,1,1,1));			
			#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
																//	0123456789abcdef0123456789abcde
			DpSTR_DrawLin(ft_loc, FALSE, MnSYS_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(13, 15, 20, 31,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L01_PASSCODE+1, _F_STR(26, 15, 40, 42, 23,	1, 1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L02_LANG+1,	   _F_STR(80, 83, 82, 61, 1,	1, 1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L03_TIME+1,	   _F_STR(75, 59, 68, 50, 1,	1, 1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnSYS_L04_VERSION+1,  _F_STR(25, 42, 14,  34,  41, 1, 1,1,1,1,1,1,1,1,1));
			#endif
			break;
		case MnSYS_LANG_ENG:
		default:
																//	0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE, 	 MnSYS_L00_IDLE+1,	   "           SYSTEM              ");
			MnLST_DispStr(ft_eng, pSel[c++], MnSYS_L01_PASSCODE+1, "Passcode                       ");
			MnLST_DispStr(ft_eng, pSel[c++], MnSYS_L02_LANG+1,	   "Language                       ");
			MnLST_DispStr(ft_eng, pSel[c++], MnSYS_L03_TIME+1,	   "Time                           ");
			MnLST_DispStr(ft_eng, pSel[c++], MnSYS_L04_VERSION+1,  "Version                        ");
			break;
	}

	DpSTR_DrawLin(ft_eng, pSel[MnSYS_L04_VERSION], 160, MnSTR_Y_HEIGHT*MnSYS_L04_VERSION, _FW_VERSION);
}
#else
void SysDp_ListEngP1(U08* pSel)
{
	U08 c  = MnSYS_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
													//   0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,     MnSYS_L00_IDLE+1,		"          SYSTEM               ");
	MnLST_DispStr(ft, pSel[c++], MnSYS_L01_PASSCODE+1,	"Passcode                       ");
	MnLST_DispStr(ft, pSel[c++], MnSYS_L02_TIME+1,		"Time                           ");
	MnLST_DispStr(ft, pSel[c++], MnSYS_L03_LANG+1,		"Language                       ");
	MnLST_DispStr(ft, pSel[c++], MnSYS_L04_VERSION+1,	"Version                        ");

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnSYS_L04_VERSION], 160, MnSTR_Y_HEIGHT*MnSYS_L04_VERSION, _FW_VERSION);
}

#if defined (_FT_LANG_KR)
void SysDp_ListKorP1(U08* pSel)
{
	U08 i;

	MnLST_DispClr(1);
	_F_SS(FKR_SI_,	FKR_SEU,	FKR_TEM,	1,1,1,1,1,1,1,1,1,1,1,1);
	DpSTR_DrawLin(FT_LC_16x16, FALSE, MnSYS_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, gFt_str);

	for(i=(MnSYS_L00_IDLE+1); i<=MNU_LIST_NUM; i++)
	{
		switch(i)
		{
			case MnSYS_L01_PASSCODE:	_F_SS(FKR_BI_,	FKR_MIL,	FKR_BEON,	FKR_HO_, 	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnSYS_L02_TIME:		_F_SS(FKR_HYEON,FKR_JAE, 	FKR_SI_,	FKR_GAN,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnSYS_L03_LANG:		_F_SS(FKR_EON,	FKR_EO_, 	FKR_SEON,	FKR_TAEG,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnSYS_L04_VERSION:		_F_SS(FKR_BEO,	FKR_JEON,	1,			1,			1,1,1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnSYS_L04_VERSION], 160, MnSTR_Y_HEIGHT*MnSYS_L04_VERSION, _FW_VERSION);
}

#elif defined (_FT_LANG_CN)
void SysDp_ListChnP1(U08* pSel)
{
	U08 i;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "           SYSTEM              ");

	for(i=(MnSYS_L00_IDLE+1); i<=MNU_LIST_NUM; i++)
	{
		switch(i)
		{																		// 0123456789abcdef0123456789abcde
			case MnSYS_L01_PASSCODE: 	_F_SS(29,		71,		1, 		1,		1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnSYS_L02_TIME:		_F_SS(51,		110,	91,		82,		1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnSYS_L03_LANG:		_F_SS(46,		5,		93,		88,		1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnSYS_L04_VERSION:		_F_SS(98,		3,		68,		57,		1,1,1,1,1,1,1,1,1,1,1);		break;
		}

		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnSYS_L04_VERSION], 160, MnSTR_Y_HEIGHT*MnSYS_L04_VERSION, _FW_VERSION);
}

#elif defined (_FT_LANG_JP)
void SysDp_ListJpnP1(U08* pSel)
{
	U08 i;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "           SYSTEM              ");

	for(i=(MnSYS_L00_IDLE+1); i<=MNU_LIST_NUM; i++)
	{
		switch(i)
		{
			case MnSYS_L01_PASSCODE: 	_F_SS(132, 108, 162, 170, 124,	0,	1,	1,1,1,1,1,1,1,1);		break;
			case MnSYS_L02_TIME:		_F_SS(107, 94,	166, 0,	  1,	1,	1,	1,1,1,1,1,1,1,1);		break;
			case MnSYS_L03_LANG:		_F_SS(156, 166, 101, 170, 107,	0,	1,	1,1,1,1,1,1,1,1);		break;
			case MnSYS_L04_VERSION:		_F_SS(112, 136, 123, 89,  90,	85,	0,	1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnSYS_L04_VERSION], 160, MnSTR_Y_HEIGHT*MnSYS_L04_VERSION, _FW_VERSION);
}

#endif
#endif
void SysDp_ProcList(U08 sel)
{
	U08 i;
	U08 page = MNU_PAGE_01;
	U08 font = FT_EN_08x16;
	U08 pSel[MnSYS_LST_MAX] = {0, };

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
		case MNU_PAGE_01:	SysDp_ListP1(pSel);	break;
		default:
			return;
	}

#else
	if(page > MNU_PAGE_01)
		return;

	switch(lMnSys.lang)
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			SysDp_ListKorP1(pSel);
#elif defined (_FT_LANG_CN)
			SysDp_ListChnP1(pSel);
#elif defined (_FT_LANG_JP)
			SysDp_ListJpnP1(pSel);		
#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			SysDp_ListEngP1(pSel);
			break;
	}
#endif
	for(i=(MnSYS_LST_MAX+1); i<=(MNU_LIST_NUM+1); i++)
		MnLST_DispClr(i);

    LDT_UpdBuff(LCD_Y_START,LCD_Y_END);
	DLY_RunMsec(100);
}

void SysChk_Vari(void)
{
	if(lMnSys.pass >= MnSYS_PASS_MAX)			lMnSys.pass = MnSYS_PASS_DFT;
	if(lMnSys.lang >= MnSYS_LANG_MAX)			lMnSys.lang = MnSYS_LANG_ENG;

#if defined (_FT_LANG_KR)
	lMnSys.loc = MnSYS_LOC_KOR;

#elif defined (_FT_LANG_CN)
	lMnSys.loc = MnSYS_LOC_CHN;

#elif defined (_FT_LANG_JP)
	lMnSys.loc = MnSYS_LOC_JPN;

#else
	lMnSys.loc = MnSYS_LOC_KOR;

#endif
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Access Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Get
U16 MnSYS_GetPass(void)			{	return lMnSys.pass;		}
U08 MnSYS_GetLang(void)			{	return lMnSys.lang;		}
U08 MnSYS_GetLocFt(void)		{	return lMnSys.loc;		}
// Set


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Memory Access
//------------------------------------------------------------------------------------------------------------------------------
void MnSYS_MemLoad(void)
{
	lMnSys.pass = FLS_Rd2Byte(F040_SYS_PASS_L);
	lMnSys.lang = FLS_Rd1Byte(F013_SYS_LANG);
}

void MnSYS_MemSave(void)
{
	FLS_Wr2Byte(F040_SYS_PASS_L, lMnSys.pass);
	FLS_Wr1Byte(F013_SYS_LANG,   lMnSys.lang);
}

void MnSYS_MemFtry(void)
{
	FLS_Wr2Byte(F040_SYS_PASS_L, MnSYS_PASS_DFT);
	FLS_Wr1Byte(F013_SYS_LANG,   MnSYS_LANG_ENG);
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MnSYS_InitVari(void)
{
	MnSYS_MemLoad();
	SysChk_Vari();
}

void MnSYS_ProcMain(void)
{
	U32 cnt = 0;
	U08 fDp = FALSE;
	U08 sel = MnSYS_L00_IDLE + 1;

	SysDp_ProcList(sel);

	do
	{
	    if(!_SBIT_BTN_DOWN)
	    {
	    	if(sel < (MnSYS_LST_MAX-1))		sel++;
	    	else                            sel = (MnSYS_L00_IDLE+1);
			fDp = TRUE;
	    }

	    if(!_SBIT_BTN_UP)
	    {
	        if(sel > (MnSYS_L00_IDLE+1))	sel--;
	        else                	        sel = (MnSYS_LST_MAX-1);
			fDp = TRUE;
	    }

		if(!_SBIT_BTN_RIGHT && !_SBIT_BTN_LEFT)
		{
			if(++cnt > 800000)
			{
				MnEGN_ProcMain();
				cnt = 0;
				fDp = TRUE;
			}			
		}
		else
			cnt = 0;

	    if(!_SBIT_BTN_SET)
	    {
			switch(sel)
			{	// Page 1
				case MnSYS_L01_PASSCODE:	MnSys_Password();		break;
				case MnSYS_L02_LANG:		MnSys_Language();		break;
				case MnSYS_L03_TIME:		MnSys_Time();			break;
				case MnSYS_L04_VERSION:		MnSys_Version();		break;
				// Page 2
			}
			fDp = TRUE;
	    }

		if(fDp)
		{
			SysDp_ProcList(sel);
			fDp = FALSE;
		}
	}
	while(_SBIT_BTN_EXIT);
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

