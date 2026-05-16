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
#include "menu_quick.h"
// lib
#include "dly_main.h"
// bsp
#include "bsp_main.h"
#include "flash_main.h"
#include "gpio_main.h"
#include "mem_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "adc_main.h"
#include "tmr_main.h"
#include "rtc_ds1302.h"
// app
#include "app_main.h"
#include "btn_main.h"
#include "isr_main.h"
#include "data_main.h"
// measure
#include "measure_main.h"
#include "measure_analyze.h"
// disp
#include "disp_main.h"
#include "disp_menu.h"
#include "disp_string.h"
// menu
#include "menu_main.h"
#include "menu_pass.h"
#include "menu_measure.h"
#include "menu_system.h"
#include "menu_data.h"
// font
#include "font_def.h"
#include "font_idx_kor.h"
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
void QuiDp_ListP1(U08* pSel)
{
	U08 c  = MnQUI_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			#if defined (_FT_LANG_KR)				
			MnLST_DispClr(1);
																//  0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE,	 MnQUI_L00_IDLE+1,	   "           QUICK               ");
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L01_ECHO_AMP+1,    			_F_STR(FKR_SU_,	 FKR_SIN,	FKR_GAM,  FKR_DO_,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L02_FREQ+1, 		  			_F_STR(FKR_JU_,   FKR_PA,    FKR_SU_,1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L03_DAMPING+1,  	  			_F_STR(FKR_DAEM,   FKR_PING,  1,        1, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L04_THRESHOLD+1,   			_F_STR(FKR_MUN,	   FKR_TEOG,  FKR_JEON, FKR_AB_,   1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L05_ASF+1,	   				_F_STR(1,		   1,		  1,		1,		   FKR_SEOL, FKR_JEONG,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L06_RST_MSR+1,				_F_STR(FKR_CHEUG, FKR_JEONG, FKR_CHO, FKR_GI_, FKR_HWA,1,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
			DpSTR_DrawLin(ft_loc, FALSE,	 MNU_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(178,  179,   180,     18,		1,		1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L01_ECHO_AMP+1,   		 	_F_STR( 7,  22,  47,  27, 24,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L02_FREQ+1, 					_F_STR(156,  145,  1,  1,  1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L03_DAMPING+1,  				_F_STR(157, 159, 161,  35,   1,   1,1,1,1,1,1,1,1,1,1));		
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L04_THRESHOLD+1,   			_F_STR(109,  59,  69,  20,   1,   1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L05_ASF+1,	   				_F_STR(  1,   1,  15,  83,   1,   1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L06_RST_MSR+1,				_F_STR(66, 107,  187,  188, 106, 82,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_JP) 						// TBD
			MnLST_DispClr(1);
			#if 1
																//	0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE,	 MnQUI_L00_IDLE+1,	   "           QUICK               ");
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L01_ECHO_AMP+1,    _F_STR( 6, 12,  42, 3, 41,	 29,   1,   1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L02_FREQ+1, _F_STR(57,	71, 67, 1,  1,	 1,   1,   1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L03_DAMPING+1,  _F_STR(56, 65, 85, 63, 1, 1, 1, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L04_THRESHOLD+1,   _F_STR(15, 38, 19, 13, 33,	30, 42, 37, 23,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L05_ASF+1,	   _F_STR(1, 1, 82, 61, 1, 1,  1, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L06_RST_MSR+1,		_F_STR(73, 61, 36, 16, 19,22,1,1,1,1,1,1,1,1,1));
			#endif
			#endif
			DpSTR_DrawLin(ft_eng, pSel[MnQUI_L05_ASF], 10, MnSTR_Y_HEIGHT*5, "ASF");		
			break;
		case MnSYS_LANG_ENG:
		default:
																//	 0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE,	 MnQUI_L00_IDLE+1,		"           QUICK               ");
			MnLST_DispStr(ft_eng, pSel[c++], MnQUI_L01_ECHO_AMP+1,	"Echo AMP                       ");
			MnLST_DispStr(ft_eng, pSel[c++], MnQUI_L02_FREQ+1,		"Frequency                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnQUI_L03_DAMPING+1,	"Damping                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnQUI_L04_THRESHOLD+1,	"Threshold                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnQUI_L05_ASF+1,		"ASF Value                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnQUI_L06_RST_MSR+1,	"Measure Reset                  ");
			break;		
	}
}

#if 0
void QuiDp_ListEngP1(U08 *pSel)
{
	U08 c  = MnQUI_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
													//   0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,	 MnQUI_L00_IDLE+1,		"           QUICK               ");
	MnLST_DispStr(ft, pSel[c++], MnQUI_L01_EMPTY+1,		"Empty                          ");
	MnLST_DispStr(ft, pSel[c++], MnQUI_L02_DEADZONE+1,	"Dead Zone                      ");
	MnLST_DispStr(ft, pSel[c++], MnQUI_L03_TVG_STEP+1,	"TVG Curve                      ");
	MnLST_DispStr(ft, pSel[c++], MnQUI_L04_RX_AMP+1,	"Echo AMP                       ");
	MnLST_DispStr(ft, pSel[c++], MnQUI_L05_FREQ+1,		"Frequency                      ");
}

#if defined (_FT_LANG_KR)
void QuiDp_ListKorP1(U08* pSel)
{
	U08 c  = MnQUI_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;
	
	MnLST_DispStr(ft_eng, FALSE,	 MnQUI_L00_IDLE+1,	   "           QUICK               ");
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L01_EMPTY+1,    _F_STR(94,	85,	1,	39,		72,		1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L02_DEADZONE+1, _F_STR(43,	3,		17,	1,	1,		1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L03_TVG_STEP+1, _F_STR(58,	105,1,	118,	119,	1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L04_RX_AMP+1,   _F_STR(55,	58,	1,	3,		19,		1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L05_FREQ+1,	   _F_STR(94,	85,	1,	11,		19,		1,1,1,1,1,1,1,1,1,1));


	#if 0
	U08 i;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "             QUICK             ");

	for(i=(MnQUI_L00_IDLE+1); i<=MNU_LIST_NUM; i++)
	{
		switch(i)
		{
			case MnQUI_L01_EMPTY:		_F_SS(94,	85,	1,	39,		72,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnQUI_L02_DEADZONE:	_F_SS(43,	3,		17,	1,	1,		1,1,1,1,1,1,1,1,1,1); 		break;
			case MnQUI_L03_TVG_STEP:	_F_SS(58,	105,1,	118,	119,	1,1,1,1,1,1,1,1,1,1);		break;
			case MnQUI_L04_RX_AMP:		_F_SS(55,	58,	1,	3,		19,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnQUI_L05_FREQ:		_F_SS(94,	85,	1,	11,		19,		1,1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
	#endif
}

#elif defined (_FT_LANG_CN)
void QuiDp_ListChnP1(U08* pSel)
{
	U08 c  = MnQUI_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;
													     //	0123456789abcdef0123456789abcde
	MnLST_DispStr(ft_eng, FALSE,	 MnQUI_L00_IDLE+1,	   "           QUICK               ");
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L01_EMPTY+1,    _F_STR(76,	117,	1,		1,		1,		1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L02_DEADZONE+1, _F_STR(70,	17,		1,		1,		1,		1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L03_TVG_STEP+1, _F_STR(7,	22,		165,	166,	1,		1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L04_RX_AMP+1,   _F_STR(7,	22,		47,		27,		24,		1,1,1,1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L05_FREQ+1,	   _F_STR(63,	64,		74,		78,		1,		1,1,1,1,1,1,1,1,1,1));

	
	#if 0
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "           QUICK               ");

	for(i=(MnQUI_L00_IDLE+1); i<=MNU_LIST_NUM; i++)
	{
		switch(i)
		{
			case MnQUI_L01_EMPTY:		_F_SS(76,	117,	1,		1,		1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnQUI_L02_DEADZONE:	_F_SS(70,	17,		1,		1,		1,		1,1,1,1,1,1,1,1,1,1); 		break;
			case MnQUI_L03_TVG_STEP:	_F_SS(7,	22,		165,	166,	1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnQUI_L04_RX_AMP:		_F_SS(7,	22,		47,		27,		24,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnQUI_L05_FREQ:		_F_SS(63,	64,		74,		78,		1,		1,1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
	#endif
}

#elif defined (_FT_LANG_JP)
void QuiDp_ListJpnP1(U08* pSel)
{
	U08 c  = MnQUI_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;

												      //	0123456789abcdef0123456789abcde
	MnLST_DispStr(ft_eng, FALSE,	 MnQUI_L00_IDLE+1,	   "           QUICK               ");
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L01_EMPTY+1,    _F_STR( 94, 156,  96, 154, 157,	 0,   1,   1, 1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L02_DEADZONE+1, _F_STR(136,	94, 166, 114,  87,	 0,   1,   1, 1,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L03_TVG_STEP+1, _F_STR(130, 118, 106, 166, 171, 132, 162, 170, 0,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L04_RX_AMP+1,   _F_STR(107, 152, 106, 166, 171,	85, 166, 138, 0,1,1,1,1,1,1));
	MnLST_DispStr(ft_loc, pSel[c++], MnQUI_L05_FREQ+1,	   _F_STR(108, 156, 118, 107, 171, 129,  89, 124, 0,1,1,1,1,1,1));



	#if 0
	U08 i;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "           QUICK               ");

	for(i=(MnQUI_L00_IDLE+1); i<=MNU_LIST_NUM; i++)
	{
		switch(i)
		{
			case MnQUI_L01_EMPTY:		_F_STR(94, 156, 96,154,157,		  0,1,1,1,1,1,1,1,1,1);		break;
			case MnQUI_L02_DEADZONE:	_F_STR(136, 94,166,114, 87,		  0,1,1,1,1,1,1,1,1,1); 	break;
			case MnQUI_L03_TVG_STEP:	_F_STR(130,118,106,166,171,132,162,170,	0,1,1,1,1,1,1);		break;
			case MnQUI_L04_RX_AMP:		_F_STR(107,152,106,166,171, 85,166,138,	0,1,1,1,1,1,1);		break;
			case MnQUI_L05_FREQ:		_F_STR(108,156,118,107,171,129, 89,124,	0,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
	#endif
}

#endif

#endif

void QuiDp_ProcList(U08 sel)
{
	U08 i;
	U08 page = MNU_PAGE_01;
	U08 pSel[MnQUI_LST_MAX] = {0, };

	pSel[sel] = TRUE;

	if		(sel > (MNU_LIST_NUM*2))		page = MNU_PAGE_03;
	else if (sel > (MNU_LIST_NUM*1))		page = MNU_PAGE_02;
	else									page = MNU_PAGE_01;

	if		(sel > (MNU_LIST_NUM*2))		pSel[sel-(MNU_LIST_NUM*2)] = TRUE;
	else if	(sel > (MNU_LIST_NUM*1))		pSel[sel-(MNU_LIST_NUM*1)] = TRUE;
	else									pSel[sel-(MNU_LIST_NUM*0)] = TRUE;

	if(page > MNU_PAGE_01)
		return;


#if 1
	switch(page)
	{		
		case MNU_PAGE_01:	QuiDp_ListP1(pSel);	break;
		default:
			return;
	}
#else
#if 1
	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			QuiDp_ListKorP1(pSel);

#elif defined (_FT_LANG_CN)
			QuiDp_ListChnP1(pSel);

#elif defined (_FT_LANG_JP)
			QuiDp_ListJpnP1(pSel);

#endif
			break;
		
#endif
#endif


	for(i=(MnQUI_LST_MAX+1); i<=(MNU_LIST_NUM+1); i++)
		MnLST_DispClr(i);
	
    LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
	BTN_DelayMsec(100);
}

void MnQui_ProcExit(void)
{
    DISP_ClearScr();
    DpSTR_LineUpd(FT_EN_08x16, 0, 60, 55, "Storing data...");
    FLS_WriteAll();

    SFRPAGE = CONFIG_PAGE;
    DLY_RunMsec(1000);
	
	if(MnDAT_GetFgSave())
	{
		RTC_GetTime();
		gLg_min=gRt.min;
		gLg_hour=gRt.hour;
	}

    DISP_ClearScr();
    SCRN_ProcDisp();

    SFRPAGE = TMR3_PAGE;

    TMR3CN = 0x04;

    SFRPAGE = UART0_PAGE;
    RI0 = 0;
    EA  = 1;
    ES0 = 0;

    SFRPAGE = CONFIG_PAGE;

    while(_SBIT_BTN_EXIT == BTN_STT_PRESS);
}
//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MnQUI_ProcMain(void)
{
	U08 sel = MnQUI_L00_IDLE + 1;
	U08 fDp = FALSE;

	MnPAS_ProcMain();

	if(!gPas_fChk)
	{
		DISP_ClearScr();
    	SCRN_ProcDisp();
		return;
	}
	gPas_fChk = FALSE;

	QuiDp_ProcList(sel);

	do
	{
		if(!_SBIT_BTN_DOWN)
		{
			if(sel < (MnQUI_LST_MAX-1))		sel++;
			else							sel = MnQUI_L00_IDLE + 1;
			fDp = TRUE;
		}
		
		if(!_SBIT_BTN_UP)
		{
			if(sel > (MnQUI_L00_IDLE+1))	sel--;
			else							sel = MnQUI_LST_MAX - 1;
			fDp = TRUE;
		}

	    if(!_SBIT_BTN_SET)
	    {
	        switch(sel)
	        { 
	        	#if 1
				case MnQUI_L01_ECHO_AMP:	MnMsr_EchoAmp();		break;
				case MnQUI_L02_FREQ:		MnMsr_Freq();			break;
				case MnQUI_L03_DAMPING:		MnMsr_Damping();		break;
				case MnQUI_L04_THRESHOLD:	MnMsr_Threshold();		break;
				case MnQUI_L05_ASF:			MnMsr_CfgAsf();			break;
				case MnQUI_L06_RST_MSR:		MnMsr_RstMsr();			break;
				#else
				case MnQUI_L01_EMPTY:		MnMsr_Empty();			break;
				case MnQUI_L02_DEADZONE:	MnMsr_DeadZone();      	break;
				case MnQUI_L03_TVG_IDX:		MnMsr_TvgIdx(); 	    break;
				case MnQUI_L04_RX_AMP:		MnMsr_EchoAmp();		break;
				case MnQUI_L05_FREQ:        MnMsr_Freq();			break;
				//case MnQUI_L06_ASF:			ANZ_CalcAsf();              break;
				//case MnQUI_L07_OFFSET:		MnMsr_Offset();  	  		break;
				//case MnQUI_L08_MSR_RESET:		MnMsr_RstMsr();      		break;
				#endif
	        }
			fDp = TRUE;
	    }

		if(fDp)
		{
	        QuiDp_ProcList(sel);
			fDp = FALSE;
		}

	}
	while(_SBIT_BTN_EXIT);

	MnQui_ProcExit();
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

