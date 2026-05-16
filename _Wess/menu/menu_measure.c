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
#include "menu_measure.h"
// bsp
#include "bsp_main.h"
#include "flash_main.h"
#include "gpio_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "btn_main.h"
#include "adc_main.h"
#include "dac_main.h"
#include "dly_main.h"
#include "tmr_main.h"
// app
#include "app_main.h"
#include "rly_main.h"
#include "isr_main.h"
// disp
#include "disp_main.h"
#include "disp_menu.h"
#include "disp_string.h"
// msr
#include "measure_main.h"
#include "measure_analyze.h"
// menu
#include "menu_main.h"
#include "menu_box.h"
#include "menu_system.h"
#include "menu_factory.h"
#include "menu_output.h"
#include "menu_quick.h"
#include "menu_pass.h"
#include "menu_engineer.h"
// font
#include "font_def.h"
#include "font_idx_kor.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
LS_MnMSR lMnMsr;


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - Visual Cfg.
//------------------------------------------------------------------------------------------------------------------------------
void MsrDp_StrEmpty(void)
{
    S16 rtn = 0;
	U08 *pUnt = "ft";
	U08 pStr[APP_STR_LENGTH] = {0, };



	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
		#if defined (_FT_LANG_KR)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 75, 0, ":");
			DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 94, 85,  126, 127, 0,   0,	0,	 0, 0,0,0,0,0,0,0));
			  
			break;

		#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 40, 0, ":");
			DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 76, 117,  0, 0, 0,   0,	0,	 0, 0,0,0,0,0,0,0));
			  
			break;

		#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 55, 0, ":");
		    DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 77, 84,  89, 0, 0,   0,	  0,   0, 0,0,0,0,0,0,0));
			
			break;
		#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 0, "Empty:");
			break;
	}



    if(lMnMsr.unit == MnMSR_UNIT_METER)
    {
		pUnt = "m ";
		APP_Num2Str(lMnMsr.empty, pStr, 4, 2, FALSE);
    }
    else if(lMnMsr.unit == MnMSR_UNIT_FEET)
    {
		pUnt = "ft";
		rtn = (S16)MEAS_CalcMeter2Feet(lMnMsr.empty);
		APP_Num2Str(rtn, pStr, 4, 1, FALSE);
    }

	DpSTR_DrawLin(FT_EN_08x16, FALSE, 100,  0, pStr);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 160, 0, pUnt);

}

void MsrDp_StrDeadzone(void)
{
    //F32 calc = 0;
    S16 rtn  = 0;
	U08 *pUnt = "ft";
	U08 pStr[APP_STR_LENGTH] = {0, };
	

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
	#if defined (_FT_LANG_KR)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 55, 0, ":");
			DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 43, 3,  17, 0, 0,   0,	0,	 0, 0,0,0,0,0,0,0));
			  
			break;

	#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 55, 0, ":");
			DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 70, 17,  0, 0, 0,   0,	0,	 0, 0,0,0,0,0,0,0));
			  
			break;

	#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 55, 0, ":");
		    DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 43, 66,  62, 0, 0,   0,	  0,   0, 0,0,0,0,0,0,0));
			
			break;
	#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 0, "Dead Zone:");
			break;
	}



    if(lMnMsr.unit == MnMSR_UNIT_METER)
    {
		pUnt = "m ";
		APP_Num2Str(lMnMsr.dead, pStr, 4, 2, FALSE);
    }
    else if (lMnMsr.unit == MnMSR_UNIT_FEET)
    {
		pUnt = "ft";
		rtn = (S16)MEAS_CalcMeter2Feet(lMnMsr.dead);
		APP_Num2Str(rtn, pStr, 4, 1, FALSE);
    }

	DpSTR_DrawLin(FT_EN_08x16, FALSE, 100, 0, pStr);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 160, 0, pUnt);

}
#if 0
void MsrDp_StrTvgIdx(void)
{
	U08 pStr[APP_STR_LENGTH] = {0, };

	APP_Num2Str(lMnMsr.tvg, pStr, 2, 0, FALSE);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 5, 0, "TVG Curve:");
	DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, "           ");
	
	if	(MnEGN_GetFTvgCst()) 	DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, "Custom");
	else 						DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, pStr);
}
#endif
void MsrDp_StrEchoAmp(void)
{
	U08 p_str[APP_STR_LENGTH] = {0, };

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 70, 0, ":");
			DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 55, 58,  3, 19, 0,   0,	0,	 0, 0,0,0,0,0,0,0));
			  
			break;

#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 85, 0, ":");
			DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 7, 22,  47, 27, 24,   0,	0,	 0, 0,0,0,0,0,0,0));
			  
			break;

#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 101, 0, ":");
		    DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 6, 12,  42, 3, 41,   29,	  0,   0, 0,0,0,0,0,0,0));
			
#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 0, "Echo AMP:");
			break;
	}



	APP_Num2Str(lMnMsr.amp, p_str, 3, 0, FALSE);
	//DpSTR_DrawLin(FT_EN_08x16, FALSE, 5, 0, "Echo AMP:");
	DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, p_str);
}

void MsrDp_StrFreq(void)
{
	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 75, 0, ":");
			DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 88, 136,  55, 0, 0,   0,	0,	 0, 0,0,0,0,0,0,0));
			  
			break;

#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 75, 0, ":");
			DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 156, 145,  0, 0, 0,   0,	0,	 0, 0,0,0,0,0,0,0));
			  
			break;

#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
			DpSTR_DrawLin(FT_EN_08x16, 0, 75, 0, ":");
		    DpSTR_DrawLin(FT_LC_16x16, 0, 5, 0, _F_STR( 57, 71,  67, 0, 0,   0,	  0,   0, 0,0,0,0,0,0,0));
			
			break;
#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 0, "Frequency:");
			break;
	}

	//DpSTR_LineUpd(FT_EN_08x16, 0, 5, 0, "Frequency:");
	DpSTR_FreqUpd(lMnMsr.freq, 100, 0);
}

void MsrDp_StrCfg(U08 idx, U08 cnt)
{
	U08 pStr[APP_STR_LENGTH] = {0, };
	U08 maxRange  = MnFTR_GetDistMax();
	S16 feet; 
	
    switch(idx)
    {
        case MnMSR_L03_EMPTY:
            if(lMnMsr.unit == MnMSR_UNIT_METER) 
            {
				switch(maxRange)
				{
					case MnFTR_DIST_MAX_10m: lMnMsr.empty = BTN_GetValue_UpDn(lMnMsr.empty, MnMSR_EMPTY_MT_MIN, MnMSR_EMPTY_MT_10_MAX, 1, cnt); break;
					case MnFTR_DIST_MAX_20m: lMnMsr.empty = BTN_GetValue_UpDn(lMnMsr.empty, MnMSR_EMPTY_MT_MIN, MnMSR_EMPTY_MT_20_MAX, 1, cnt); break;
				}
                
				APP_Num2Str(lMnMsr.empty, pStr, 4, 2, FALSE);
            }
            else 
            {
            	feet = (S16)MEAS_CalcMeter2Feet(lMnMsr.empty);
				switch(maxRange)
				{
					case MnFTR_DIST_MAX_10m: feet = BTN_GetValue_UpDn(feet, MnMSR_EMPTY_FT_MIN, MnMSR_EMPTY_FT_10_MAX, 1, cnt); break;
					case MnFTR_DIST_MAX_20m: feet = BTN_GetValue_UpDn(feet, MnMSR_EMPTY_FT_MIN, MnMSR_EMPTY_FT_20_MAX, 1, cnt); break;
				}
                
                lMnMsr.empty = (U16)MEAS_CalcFeet2Meter(feet);
				APP_Num2Str(feet, pStr, 4, 1, FALSE);
            }
			DpSTR_LineUpd(FT_EN_08x16, FALSE, 100, 0, pStr);
            break;

        case MnMSR_L04_DEADZONE:
            if(lMnMsr.unit == MnMSR_UNIT_METER) 
            {
				switch(maxRange)
				{
					case MnFTR_DIST_MAX_10m: lMnMsr.dead = BTN_GetValue_UpDn(lMnMsr.dead, MnMSR_DEAD_MT_MIN, MnMSR_DEAD_MT_10_MAX, 1, cnt); break;
					case MnFTR_DIST_MAX_20m: lMnMsr.dead = BTN_GetValue_UpDn(lMnMsr.dead, MnMSR_DEAD_MT_MIN, MnMSR_DEAD_MT_20_MAX, 1, cnt); break;
				}
                
				APP_Num2Str(lMnMsr.dead, pStr, 4, 2, FALSE);
            } 
            else
            {
            	feet = (S16)MEAS_CalcMeter2Feet(lMnMsr.dead);
				switch(maxRange)
				{
					case MnFTR_DIST_MAX_10m: feet = BTN_GetValue_UpDn(feet, MnMSR_DEAD_FT_MIN, MnMSR_DEAD_FT_10_MAX, 1, cnt); break;
					case MnFTR_DIST_MAX_20m: feet = BTN_GetValue_UpDn(feet, MnMSR_DEAD_FT_MIN, MnMSR_DEAD_FT_20_MAX, 1, cnt); break;
				}
                
				lMnMsr.dead = (U16)MEAS_CalcFeet2Meter(feet);
				APP_Num2Str(feet, pStr, 4, 1, FALSE);
            }
			DpSTR_LineUpd(FT_EN_08x16, FALSE, 100, 0, pStr);
            break;

        case MnMSR_L05_ECHO_AMP:
			lMnMsr.amp = BTN_GetValue_UpDn(lMnMsr.amp, MnMSR_AMP_MIN, MnMSR_AMP_MAX, 1, 0);
			APP_Num2Str(lMnMsr.amp, pStr, 3, 0, FALSE);
			DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, pStr);
			break;

        case MnMSR_L06_FREQ:
            lMnMsr.freq = BTN_GetValue_UpDn(lMnMsr.freq, MnMSR_FREQ_130K, MnMSR_FREQ_380K, 1, 0);
			DpSTR_FreqUpd(lMnMsr.freq, 100, 0);
    }
}

void MsrDp_VisualCfg(U08 lst)
{
    S16 old;
    U08 cnt = 0;
    S16 feet = 0;

    SFRPAGE = CONFIG_PAGE;

    MEAS_CfgSens(4);
    MEAS_PrcEcho();

    LDT_ClrBuff(LCD_X_START, LCD_Y_START, LCD_X_END, LCD_Y_END);

    switch(lst)
    {
        case MnMSR_L03_EMPTY:		old = lMnMsr.empty;		MsrDp_StrEmpty();				break;
        case MnMSR_L04_DEADZONE:	old = lMnMsr.dead;		MsrDp_StrDeadzone();			break;
        case MnMSR_L05_ECHO_AMP:	old = lMnMsr.amp;		MsrDp_StrEchoAmp();				break;
        case MnMSR_L06_FREQ:		old = lMnMsr.freq;		MsrDp_StrFreq();				break;
	}

    DSP_DrawEchoQ(gAd_data);

    LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
    BTN_DelayMsec(BTN_WAIT_SET);

    TMR3_Start1sec();

    do
    {
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{
            if(++cnt >= 30)
                cnt = 30;


			MsrDp_StrCfg(lst, cnt);

			MEAS_CfgSens(4);
			MEAS_PrcEcho();
			DSP_DrawEchoQ(gAd_data);
			LDT_UpdBuff(LCD_Y_START, LCD_Y_END);

			BTN_DelayMsec(BTN_WAIT_UD);
		}
        else
			cnt = 0;

        if(gIsr_tm3_cnt >= 100)  // 100 = 1sec
        {
        	gIsr_tm3_cnt = 0;

        	MEAS_CfgSens(4);
        	MEAS_PrcEcho();

        	DSP_DrawEchoQ(gAd_data);
        	LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
        }
	}
    while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

    if(!_SBIT_BTN_EXIT)
    {
        switch(lst)
        {
            case MnMSR_L03_EMPTY:		lMnMsr.empty = old;		ANZ_SetEmptyPct(110);	break;
            case MnMSR_L04_DEADZONE:	lMnMsr.dead  = old;		break;
            case MnMSR_L05_ECHO_AMP:	lMnMsr.amp = old;		break;
            case MnMSR_L06_FREQ:		lMnMsr.freq  = old;		break;
        }
    }

    TMR3_Stop1sec();
}


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - List Item (Page 1)
//------------------------------------------------------------------------------------------------------------------------------
void MnMsr_Unit(void)
{
	MnBOX_DispBox();


		switch(MnSYS_GetLang())
		{
			case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
				MnBOX_DispStr(FT_LC_16x16, MnBOX_X07, MnBOX_Y01, F_F, _F_STR( 94, 85,  15, 127, 0,   0,	  0,   0, 0,0,0,0,0,0,0));
				break;
	
	
#elif defined (_FT_LANG_CN)
				MnBOX_DispStr(FT_LC_16x16, MnBOX_X07, MnBOX_Y01, F_F, _F_STR( 66, 107,	18, 4, 0,	 0,   0,   0, 0,0,0,0,0,0,0));
				break;
	
#elif defined (_FT_LANG_JP)
				MnBOX_DispStr(FT_LC_16x16, MnBOX_X10, MnBOX_Y01, F_F, _F_STR( 54, 44,  0, 0, 0,	 0,   0,   0, 0,0,0,0,0,0,0));
				break;
#endif
				break;
			case MnSYS_LANG_ENG:
			default:
				MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "Unit");
				break;
		}




    lMnMsr.unit = MnBOX_SelArg4(MnSYS_LANG_ENG, lMnMsr.unit, MnBOX_X09, "Meter", MnBOX_X10, "Feet", 0, "", 0, "", 2);



	
}

void MnMsr_Operation(void)
{
	S16 sel = lMnMsr.oper;

	MnBOX_DispBox();

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X05, MnBOX_Y01, F_F, "Operation");
			break;

#elif defined (_FT_LANG_CN)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 66, 107,  60, 38, 0,   0,	  0,   0, 0,0,0,0,0,0,0));
			break;

#elif defined (_FT_LANG_JP)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 73, 61,  32, 42, 23,   0,	  0,   0, 0,0,0,0,0,0,0));
			break;
#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X05, MnBOX_Y01, F_F, "Operation");
			break;
	}


	lMnMsr.oper = MnBOX_SelArg4(MnSYS_LANG_ENG, sel, MnBOX_X09, "Level", MnBOX_X06, "Distance", 0, "", 0, "", 2);

	if(sel != lMnMsr.oper)
		MEAS_ResetFlag();
}

void MnMsr_Empty(void)
{
	S16 old = lMnMsr.empty;

	MsrDp_VisualCfg(MnMSR_L03_EMPTY);

	if(old == lMnMsr.empty)
		return;

	MEAS_ResetFlag();

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X03, MnBOX_Y01, F_F, "change mA ?");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, MnBOX_Y02, F_F, "OK - SET");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X05, MnBOX_Y03, F_F, "NO - EXIT");
	BTN_DelayMsec(BTN_WAIT_SET*6);

	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if(!_SBIT_BTN_SET)
		MnOUT_SetSet20mA(lMnMsr.empty);
		//lMnOut.set20mA = lMnMsr.empty;
}

void MnMsr_DeadZone(void)
{
	S16 ex_para = lMnMsr.dead;

	MsrDp_VisualCfg(MnMSR_L04_DEADZONE);

	if(ex_para != lMnMsr.dead)
		MEAS_ResetFlag();
}



void MnMsr_EchoAmp(void)
{
	MsrDp_VisualCfg(MnMSR_L05_ECHO_AMP);
}

void MnMsr_Freq(void)
{
	S16 old = lMnMsr.freq;

	MsrDp_VisualCfg(MnMSR_L06_FREQ);

	if(old != lMnMsr.freq)
		MEAS_ResetFlag();
}


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - List Item (Page 2)
//------------------------------------------------------------------------------------------------------------------------------
void MnMsr_Interval(void) 
{
	U08 old = lMnMsr.intv;
	U08 sel = old;

	MnBOX_DispBox();
	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 94, 85,  88, 10, 0,	 0,   0,   0, 0,0,0,0,0,0,0));
			break;


#elif defined (_FT_LANG_CN)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 66, 107,	110, 113, 0,	 0,   0,   0, 0,0,0,0,0,0,0));
			break;

#elif defined (_FT_LANG_JP)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 73, 61,  57, 69, 0,  0,   0,   0, 0,0,0,0,0,0,0));
			break;
#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, MnBOX_Y01, F_F, "Interval");
			break;
	}


	
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y02, F_F, "(sec)");
	sel = MnBOX_CfgNumb(sel, MnMSR_INTV_MIN, MnMSR_INTV_MAX, 1, 0, FALSE, "");

	if(old != sel)
		lMnMsr.intv = sel;
}

void MnMsr_Offset(void)
{
    S16 feet = 0;
    F32 calc = (lMnMsr.offs * MSR_CALC_M2F_CONST);

    if(calc >= 0)   calc += 0.5;
    else            calc -= 0.5;

    feet = calc;

	MnBOX_DispBox();

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 34, 51,  87, 85, 0,	 0,   0,   0, 0,0,0,0,0,0,0));
			break;


#elif defined (_FT_LANG_CN)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X10, MnBOX_Y01, F_F, _F_STR( 87, 120,	0, 0, 0,	 0,   0,	0, 0,0,0,0,0,0,0));
			break;

#elif defined (_FT_LANG_JP)
			MnBOX_DispStr(FT_LC_16x16, MnBOX_X05, MnBOX_Y01, F_F, _F_STR( 7, 27,  16, 19, 22,  0,	0,	 0, 0,0,0,0,0,0,0));
			break;
#endif
			break;
		case MnSYS_LANG_ENG:
		default:
			MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X08, MnBOX_Y01, F_F, "Offset");
			break;
	}



	switch(lMnMsr.unit)
	{
		case MnMSR_UNIT_FEET:		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y02, F_F, "(ft)");		break;
		case MnMSR_UNIT_METER:		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y02, F_F, "(m)");		break;
		default:					return;
	}

    if(lMnMsr.unit == MnMSR_UNIT_METER)
    {
    	lMnMsr.offs = MnBOX_CfgNumb(lMnMsr.offs, MnMSR_OFFS_MT_MIN, MnMSR_OFFS_MT_MAX, 1, 2, F_F, "");
    }
    else
    {
    	feet = MnBOX_CfgNumb(feet, MnMSR_OFFS_FT_MIN, MnMSR_OFFS_FT_MAX, 1, 1, F_F, "");
    	calc = (feet * MSR_CALC_F2M_CONST);

    	if(calc >= 0)       calc += 0.5;
    	else                calc -= 0.5;

    	lMnMsr.offs = calc;
    }
}

void MnMsr_Damping(void)
{
	S16 old = lMnMsr.damp;
	U08 lg_e = MnSYS_LANG_ENG;
	U08 x0   = MnBOX_X08;
	U08 y0   = MnBOX_Y03;

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Number");

	switch(lMnMsr.damp)
	{
		case MnMSR_DAMP_I01_T0001:		MnBOX_DispStr(lg_e, x0, y0, F_F, "   1");		break;		// 03 sec
		case MnMSR_DAMP_I02_T0010:		MnBOX_DispStr(lg_e, x0, y0, F_F, "  10");		break;		// 30 sec
		case MnMSR_DAMP_I03_T0020:		MnBOX_DispStr(lg_e, x0, y0, F_F, "  20");		break;		// 01 min
		case MnMSR_DAMP_I04_T0040:		MnBOX_DispStr(lg_e, x0, y0, F_F, "  40");		break;		// 02 min
		case MnMSR_DAMP_I05_T0100:		MnBOX_DispStr(lg_e, x0, y0, F_F, " 100");		break;		// 05 min
		case MnMSR_DAMP_I06_T0200:		MnBOX_DispStr(lg_e, x0, y0, F_F, " 200");		break;		// 10 min
		case MnMSR_DAMP_I07_T0600:		MnBOX_DispStr(lg_e, x0, y0, F_F, " 600");		break;		// 30 min
		case MnMSR_DAMP_I08_T1200:		MnBOX_DispStr(lg_e, x0, y0, F_F, "1200");		break;		// 60 min
	}
	BTN_DelayMsec(BTN_WAIT_SET);

	do
	{
		if(BTN_GetStatus(BTN_IDX_UP) || BTN_GetStatus(BTN_IDX_DOWN))
		{
			if     (!_SBIT_BTN_UP)			lMnMsr.damp++;
			else if(!_SBIT_BTN_DOWN)		lMnMsr.damp--;

			if     (lMnMsr.damp >= (MnMSR_DAMP_IDX_MAX-1))		lMnMsr.damp = MnMSR_DAMP_IDX_MAX  - 1;
			else if(lMnMsr.damp <= (MnMSR_DAMP_I00_IDLE+1))		lMnMsr.damp = MnMSR_DAMP_I00_IDLE + 1;

			switch (lMnMsr.damp)
			{
				case MnMSR_DAMP_I01_T0001: 	MnBOX_DispStr(lg_e, x0, y0, F_F, "   1");		break;		// 03 sec
				case MnMSR_DAMP_I02_T0010: 	MnBOX_DispStr(lg_e, x0, y0, F_F, "  10");		break;		// 30 sec
				case MnMSR_DAMP_I03_T0020: 	MnBOX_DispStr(lg_e, x0, y0, F_F, "  20");		break;		// 01 min
				case MnMSR_DAMP_I04_T0040: 	MnBOX_DispStr(lg_e, x0, y0, F_F, "  40");		break;		// 02 min
				case MnMSR_DAMP_I05_T0100: 	MnBOX_DispStr(lg_e, x0, y0, F_F, " 100");		break;		// 05 min
				case MnMSR_DAMP_I06_T0200: 	MnBOX_DispStr(lg_e, x0, y0, F_F, " 200");		break;		// 10 min
				case MnMSR_DAMP_I07_T0600: 	MnBOX_DispStr(lg_e, x0, y0, F_F, " 600");		break;		// 30 min
				case MnMSR_DAMP_I08_T1200: 	MnBOX_DispStr(lg_e, x0, y0, F_F, "1200");		break;		// 60 min
			}
			BTN_DelayMsec(BTN_WAIT_UD);
		}
	}
	while (_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if(!_SBIT_BTN_EXIT)
		lMnMsr.damp = old;
}

void MnMsr_Threshold(void)
{
	U16 tmp0;
	F32 tmp1, tmp2;
	U08 cnt = 0;
	S16 per = lMnMsr.trhd_per;
	S16 fix = lMnMsr.trhd_fix;
	U08 mod_old = lMnMsr.trhd_mod;
	U08 mod_sel = mod_old;

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Mode");

	switch(lMnMsr.trhd_mod) 
	{
		case MnMSR_TRHD_MOD_AUTO:		DpSTR_LineUpd(FT_EN_08x16, F_F, MnBOX_X08, MnBOX_Y03, " Auto ");  		break;
		case MnMSR_TRHD_MOD_MANUAL:		DpSTR_LineUpd(FT_EN_08x16, F_F, MnBOX_X08, MnBOX_Y03, "Manual");  		break;
	}
	BTN_DelayMsec(BTN_WAIT_SET);

	do 
	{
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{			
			if	   (mod_sel == MnMSR_TRHD_MOD_MANUAL)	mod_sel = MnMSR_TRHD_MOD_AUTO;
			else if(mod_sel == MnMSR_TRHD_MOD_AUTO)		mod_sel = MnMSR_TRHD_MOD_MANUAL;
			
			BTN_DelayMsec(BTN_WAIT_UD);

			switch(mod_sel) 
			{
				case MnMSR_TRHD_MOD_AUTO:		DpSTR_LineUpd(FT_EN_08x16, F_F, MnBOX_X08, MnBOX_Y03, " Auto ");	break;
				case MnMSR_TRHD_MOD_MANUAL:		DpSTR_LineUpd(FT_EN_08x16, F_F, MnBOX_X08, MnBOX_Y03, "Manual");	break;
			}
		}
	} 
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if(!_SBIT_BTN_EXIT)
		return;

	lMnMsr.trhd_mod = mod_sel;

	if(lMnMsr.trhd_mod == MnMSR_TRHD_MOD_AUTO)
	{
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X04, MnBOX_Y01, F_F, "   Auto   ");  
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y02, F_F, "(%)");
		lMnMsr.trhd_per = MnBOX_CfgNumb(lMnMsr.trhd_per, MnMSR_TRHD_PCT_MIN,
														 MnMSR_TRHD_PCT_MAX, MnMSR_TRHD_PCT_STP, 0, FALSE, "");
		if(per != lMnMsr.trhd_per)
			MsANL_SetFgTimWin(FALSE);
	}
	else
	{
		MnBOX_DispBox();
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X04, MnBOX_Y01, F_F, "  Manual  ");
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y02, F_F, "(V)");

		tmp1 = lMnMsr.trhd_fix;
		tmp2 = (tmp1 * 255.0) / 255.0;
		tmp0 = tmp2;

		DpSTR_NumbUpd(MnBOX_Y03, tmp0, 240, 2, 0, "");
		BTN_DelayMsec(BTN_WAIT_SET);
		
		do 
		{
			if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
			{
				if(++cnt >= 30)
					cnt = 30;
		
				lMnMsr.trhd_fix = BTN_GetValue_UpDn(lMnMsr.trhd_fix, MnMSR_TRHD_FIX_MIN,
																	 MnMSR_TRHD_FIX_MAX, MnMSR_TRHD_FIX_STP, cnt);
				tmp1 = lMnMsr.trhd_fix;
				tmp2 = (tmp1 * 255.0) / 255.0;
				tmp0 = tmp2;

				DpSTR_NumbUpd(MnBOX_Y03, tmp0, 240, 2, 0, "");
				BTN_DelayMsec(BTN_WAIT_UD);
			} 
			else
				cnt = 0;
		}
		while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);
		
		if(!_SBIT_BTN_EXIT)
			lMnMsr.trhd_fix = fix;
	}
}

void MnMsrAsf_CalcVari(void)
{
	U16 i, idx_cursor;
	U32 cnt_asf;
	U16 cnt_thr_up, val_asf_h, val_asf_r;
	U08 flg_thr_up;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U08 p_st2[APP_STR_LENGTH] = {0, };
	U08 p_st3[APP_STR_LENGTH] = {0, };
	F32 veloc    = MEAS_GetVeloc();
	U16 trhd_pct = MsANL_GetTrhdPct();
	U32 aux0 = (U32)gDp_cursor_vert * (U32)MsANL_GetEmpty();

	idx_cursor = aux0 / 220;

	flg_thr_up = 0;
	cnt_thr_up = 0;
	cnt_asf = 0;

	for(i=MnMSR_GetDead(); i<idx_cursor; i++) 
	{
	    if(flg_thr_up == FALSE)
	    {
	        if(gAd_data[i] >= trhd_pct) 
	        {
	            flg_thr_up = TRUE;
	            cnt_thr_up++;
	        }
	    } 
	    else 
	    {
	    	if(gAd_data[i] >= trhd_pct)
				cnt_thr_up++;
	    	else
	    	{
	    		flg_thr_up = FALSE;

	    		if(lMnMsr.cAsf < cnt_thr_up)        lMnMsr.cAsf = cnt_thr_up;
	    		if(cnt_asf     < cnt_thr_up)		cnt_asf     = cnt_thr_up;
	    		
	    		cnt_thr_up = 0;
	    	}
	    }
	}

	if(flg_thr_up == TRUE)
	{
	    if(lMnMsr.cAsf < cnt_thr_up)       lMnMsr.cAsf = cnt_thr_up;
	    if(cnt_asf     < cnt_thr_up)       cnt_asf = cnt_thr_up;
	}

	if(lMnMsr.cAsf == 0)    val_asf_h = 0;
	else                    val_asf_h = ((lMnMsr.cAsf * 2 * 10000) / veloc) + 20;

	if(cnt_asf == 0)        val_asf_r = 0;
	else                    val_asf_r = ((cnt_asf * 2 * 10000) / veloc) + 20;

	APP_Num2Str(lMnMsr.asf, p_str, 4, 0, FALSE);
	APP_Num2Str(val_asf_r,  p_st2, 4, 0, FALSE);
	APP_Num2Str(val_asf_h,  p_st3, 4, 0, FALSE);

	DpSTR_LineUpd(FT_EN_08x16, FALSE,  29, 0, p_str);
	DpSTR_LineUpd(FT_EN_08x16, FALSE, 116, 0, p_st2);
	DpSTR_LineUpd(FT_EN_08x16, FALSE, 203, 0, p_st3);
}

void MnMsrAsf_ProcFunc(void) 
{
	U16 msr_mty = MsANL_GetEmpty();
    U16 aux1 = (220 * (U32)MnMSR_GetDead()) / (U32)msr_mty;

    gDp_flg_cs_vert = 1;

    MnMsrAsf_CalcVari();
	
    while(BTN_GetStatus(BTN_IDX_RIGHT) || BTN_GetStatus(BTN_IDX_LEFT)) 
    {
        LDT_ClearDot(0+gDp_cursor_vert-2, 30+2-2);
        LDT_ClearDot(0+gDp_cursor_vert-1, 30+2-1);
        LDT_ClearDot(0+gDp_cursor_vert+1, 30+2-1);
        LDT_ClearDot(0+gDp_cursor_vert+2, 30+2-2);

        if     (BTN_GetStatus(BTN_IDX_RIGHT))       gDp_cursor_vert += 1;
        else if(BTN_GetStatus(BTN_IDX_LEFT))        gDp_cursor_vert -= 1;

        if(gDp_cursor_vert < aux1)          gDp_cursor_vert = aux1;
        if(gDp_cursor_vert > 220-1)         gDp_cursor_vert = 220 - 1;

        if((gDp_cursor_vert != aux1 && !BTN_GetStatus(BTN_IDX_LEFT)) ||
           (gDp_cursor_vert != 220-1 && !BTN_GetStatus(BTN_IDX_RIGHT))  ) 
        {
        	DSP_DrawGraph(0, 30 + 2, 220, 85, gAd_data, 0, msr_mty, 256, DP_IDX_GRAPH_2);
        	
        	MnMsrAsf_CalcVari();
        }
	}

	DSP_DrawGraph(0, 30 + 2, 220, 85, gAd_data, 0, msr_mty, 256, DP_IDX_GRAPH_2);

	DpSTR_DrawLin(FT_EN_06x07, 0, 195, 30 + 2 + 85 + 2, "Empty");

	gDp_flg_cs_vert = 0;
	LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
}

void MnMsr_CfgAsf(void)
{
	U16 aux0, aux1;
	U08 cnt = 0;
	S16 old = lMnMsr.asf;
	U16 msr_mty = MsANL_GetEmpty();
	F32 veloc   = MEAS_GetVeloc();

	MEAS_CfgSens(0);

	lMnMsr.cAsf = 0;

	aux0 = (220 * (U32)MsANL_GetDistaMt()) / (U32)msr_mty;
	aux1 = (220 * (U32)MnMSR_GetDead())    / (U32)msr_mty;

	if(aux0 < aux1)
		aux0 = aux1;

	gDp_cursor_vert = aux0;

	TMR3_Start1sec();

	DISP_ClearScr();
	DpSTR_LineUpd(FT_EN_08x16, 0,   5, 0, "P:");
	DpSTR_LineUpd(FT_EN_08x16, 0,  92, 0, "R:");
	DpSTR_LineUpd(FT_EN_08x16, 0, 179, 0, "H:");

	MnMsrAsf_ProcFunc();

	gIsr_tm3_cnt = 100;
	BTN_DelayMsec(BTN_WAIT_SET);

	do
	{
	    if(BTN_GetStatus(BTN_IDX_RIGHT) || BTN_GetStatus(BTN_IDX_LEFT))
	        MnMsrAsf_ProcFunc();

	    if(gIsr_tm3_cnt >= 100)      //100 = 1sec
	    {
	        gIsr_tm3_cnt = 0;        // Cnt_sec_interval_UC_G[Num_CH_UC_G]++;

	        MEAS_PrcSub1();
	        MnMsrAsf_ProcFunc();
	    }
	}
	while(_SBIT_BTN_SET != BTN_STT_PRESS && _SBIT_BTN_EXIT != BTN_STT_PRESS);

	if(_SBIT_BTN_SET == BTN_STT_PRESS)
	{
		lMnMsr.asf = ((lMnMsr.cAsf * 2 * 10000) / veloc) + 20;

		MnBOX_DispBox();
		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, MnBOX_Y01, F_F, "SetValue");
		DpSTR_NumbUpd(64, lMnMsr.asf, MnMSR_ASF_MAX, 0, 0, "");
		BTN_DelayMsec(BTN_WAIT_SET);

	    do 
	    {
			if(_SBIT_BTN_UP == BTN_STT_PRESS || _SBIT_BTN_DOWN == BTN_STT_PRESS)	//Button_state_any_press())
			{
				if((_SBIT_BTN_UP == BTN_STT_PRESS) || (_SBIT_BTN_DOWN == BTN_STT_PRESS))
					if(++cnt >= 30)
						cnt = 30;

				lMnMsr.asf = BTN_GetValue_UpDn(lMnMsr.asf, MnMSR_ASF_MIN, MnMSR_ASF_MAX, 1, cnt);
				DpSTR_NumbUpd(64, lMnMsr.asf, MnMSR_ASF_MAX, 0, 0, "");
				BTN_DelayMsec(BTN_WAIT_UD);
			} 
			else
				cnt = 0;
			
		} while(_SBIT_BTN_SET != BTN_STT_PRESS && _SBIT_BTN_EXIT != BTN_STT_PRESS);

		if (_SBIT_BTN_EXIT == BTN_STT_PRESS)
			lMnMsr.asf = old;
	}

	TMR3_Stop1sec();
}

void MnMsr_TwDelay(void) 
{
	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Number");


	lMnMsr.tw_delay = MnBOX_CfgNumb(lMnMsr.tw_delay, MnMSR_TW_DELAY_MIN, MnMSR_TW_DELAY_MAX,
													 MnMSR_TW_DELAY_STP, 0, F_F, "");
}

void MnMsr_TwRange(void) 
{
	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Range");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y02, F_F, "(cm)");

	lMnMsr.tw_range = MnBOX_CfgNumb(lMnMsr.tw_range, MnMSR_TW_RANGE_MIN, MnMSR_TW_RANGE_MAX,
													 MnMSR_TW_RANGE_STP, 0, F_F, "");
}


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - List Item (Page 3)
//------------------------------------------------------------------------------------------------------------------------------
void MnMsr_PcdIntv(void)
{
	S16 old = lMnMsr.pcd_intv;

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "Time");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y02, F_F, "(sec)");
	DpSTR_NumbUpd(MnBOX_Y03, lMnMsr.pcd_intv, MnMSR_PCD_INTV_MAX, 0, 0, "");
	BTN_DelayMsec(BTN_WAIT_SET);

	do
	{
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{
			if(BTN_GetStatus(BTN_IDX_UP))
			{
				if(lMnMsr.pcd_intv <= 50)		lMnMsr.pcd_intv += 10;
				else							lMnMsr.pcd_intv += MnMSR_PCD_INTV_STP;
			}
			if(BTN_GetStatus(BTN_IDX_DOWN))
			{
				if(lMnMsr.pcd_intv <= 60)		lMnMsr.pcd_intv -= 10;
				else							lMnMsr.pcd_intv -= MnMSR_PCD_INTV_STP;
			}

			if(lMnMsr.pcd_intv > MnMSR_PCD_INTV_MAX)		lMnMsr.pcd_intv = MnMSR_PCD_INTV_MAX;
			if(lMnMsr.pcd_intv < MnMSR_PCD_INTV_MIN)		lMnMsr.pcd_intv = MnMSR_PCD_INTV_MIN;

			DpSTR_NumbUpd(MnBOX_Y03, lMnMsr.pcd_intv, MnMSR_PCD_INTV_MAX, 0, 0, "");
			BTN_DelayMsec(BTN_WAIT_UD);
		}
	}
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if(!_SBIT_BTN_EXIT)
		lMnMsr.pcd_intv = old;
}

void MnMsr_PcdTerm(void)
{
	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "Time");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y02, F_F, "(sec)");
	lMnMsr.pcd_term = MnBOX_CfgNumb(lMnMsr.pcd_term, MnMSR_PCD_TERM_MIN, MnMSR_PCD_TERM_MAX, 1, 0, FALSE, "");
}

void MnMsr_RstMsr(void)
{
    U08 flg = FALSE;

	MnBOX_DispBox();

	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X07, MnBOX_Y01, F_F, "Reset?");
	flg = MnBOX_SelArg4(MnSYS_LANG_ENG, flg, MnBOX_X11, "NO", MnBOX_X10, "YES", 0, "", 0, "", 2);

    if(!_SBIT_BTN_SET && flg)
    	MEAS_ResetFlag();
}

void MnMsr_RstFtr(void)
{
	DISP_ClearScr();					//0123456789abcdef0123456789a
	DpSTR_LineUpd(FT_EN_08x16, 0, 0, 16, "   Parameter data          ");
	DpSTR_LineUpd(FT_EN_08x16, 0, 0, 32, "        will be reset!!    ");
	DpSTR_LineUpd(FT_EN_08x16, 0, 0, 64, "  OK - SET     NO - EXIT   ");

	while(!_SBIT_BTN_SET);
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if(!_SBIT_BTN_SET)
	{
		DISP_ClearScr();
		
		FLS_ResetFactory();
		ANZ_SetEmptyPct(110);
	}
}

void MnMsr_TestMode(void)
{
	S16 old = lMnMsr.fTest;

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X07, MnBOX_Y01, F_F, "Enable?");

	lMnMsr.fTest = MnBOX_SelArg4(MnSYS_LANG_ENG, lMnMsr.fTest, MnBOX_X11, "NO", MnBOX_X10, "YES", 0, "", 0, "", 2);

	if(!_SBIT_BTN_EXIT)
		lMnMsr.fTest = old;

	if(old != lMnMsr.fTest)
		MEAS_ResetFlag();
}

//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - Disp. List
//------------------------------------------------------------------------------------------------------------------------------
#if 1
void MsrDp_ListP1(U08* pSel)
{
	U08 c  = MnMSR_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			#if defined (_FT_LANG_KR)
			MnLST_DispClr(1);
			
			DpSTR_DrawLin(ft_loc, FALSE, MnMSR_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(FKR_CHEUG, FKR_JEONG,1,1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnMSR_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#1/3");

			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L01_UNIT+1,	   _F_STR(FKR_CHEUG, FKR_JEONG,	FKR_DAN,  FKR_WI_, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L02_OPER+1,	   _F_STR(FKR_SEUL,  FKR_LEO,	FKR_JI_,  1,	   1,	     FKR_SANG, FKR_DEUNG, FKR_SU_,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L03_EMPTY+1,    _F_STR(FKR_CHEUG, FKR_JEONG,	FKR_BEOM, FKR_WI_, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L04_DEADZONE+1, _F_STR(FKR_BUL,	 FKR_GAM,	FKR_DAE,  1,1,1,1,1,1,1,1,1,1,1,1));
			#if 0
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L05_TVG_IDX+1,  _F_STR(1,		 1,			1,		  1,	   FKR_SEON, FKR_TAEG,1,1,1,1,1,1,1,1,1));
			#endif
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L05_ECHO_AMP+1, _F_STR(FKR_SU_,	 FKR_SIN,	FKR_GAM,  FKR_DO_,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L06_FREQ+1,	   _F_STR(FKR_JU_,   FKR_PA,    FKR_SU_,1,1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L07_TX_INTV+1,  _F_STR(FKR_CHEUG,  FKR_JEONG, FKR_JU_,  FKR_GI_,   1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, pSel[MnMSR_L02_OPER],   60, MnSTR_Y_HEIGHT*2, "/");
			#if 0
			DpSTR_DrawLin(ft_eng, pSel[MnMSR_L05_TVG_IDX], 9, MnSTR_Y_HEIGHT*5, "TVG");
			#endif
			#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
			
			DpSTR_DrawLin(ft_loc, FALSE, MnMSR_X0_TITLE_KO-10, MnSTR_Y_HEIGHT*0, _F_STR(66, 107, 187, 82,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnMSR_X0_PAGE_KO+20,  MnSTR_Y_HEIGHT*0, "#1/3");
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L01_UNIT+1,	   _F_STR(66, 107,  18,   4,  1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L02_OPER+1,	   _F_STR(66, 107,  60,  38,  1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L03_EMPTY+1,	   _F_STR(76, 117,   1,	  1,  1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L04_DEADZONE+1, _F_STR(70,  17,   1,	  1,  1,1,1,1,1,1,1,1,1,1,1));
			#if 0
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L05_TVG_IDX+1,  _F_STR( 7,  22, 165, 166,  1,1,1,1,1,1,1,1,1,1,1));
			#endif
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L05_ECHO_AMP+1, _F_STR( 7,  22,  47,  27, 24,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L06_FREQ+1,	   _F_STR(156,  145,  1,  1,  1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L07_TX_INTV+1,  _F_STR( 66, 107, 110, 113,   1,   1,1,1,1,1,1,1,1,1,1));

			#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
			DpSTR_DrawLin(ft_loc, FALSE, MnMSR_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(81, 73, 1, 1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnMSR_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#1/3");															  	
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L01_UNIT+1,	   _F_STR(54, 44,  1,   1,	1,	 1,   1,   1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L02_OPER+1,	   _F_STR( 73,  61, 32, 42, 23, 1, 1,   1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L03_EMPTY+1,    _F_STR( 77, 84,  89, 1, 1,   1,	  1,   1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L04_DEADZONE+1, _F_STR(43,  66, 62, 1,  1,   1,	  1,   1, 1,1,1,1,1,1,1));
			#if 0
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L05_TVG_IDX+1,  _F_STR(130, 118, 106, 166, 171, 132, 162, 170, 0,1,1,1,1,1,1));
			#endif
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L05_ECHO_AMP+1, _F_STR(6, 12, 42, 3, 41,  29, 1, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L06_FREQ+1,	   _F_STR(57, 71, 67, 1, 1, 1,  1, 1, 0,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L07_TX_INTV+1,  _F_STR( 73, 61, 57, 69,   1,   1,1,1,1,1,1,1,1,1,1));
			#endif
			break;
		case MnSYS_LANG_ENG:
		default:
																 // 0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE,	 MnMSR_L00_IDLE+1,	   "      MEASUREMENT #1/3         ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L01_UNIT+1,	   "Display Unit                   ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L02_OPER+1,	   "Operation                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L03_EMPTY+1,    "Empty                          ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L04_DEADZONE+1, "Dead Zone                      ");
			#if 0
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L05_TVG_IDX+1,  "TVG Curve                      ");
			#endif
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L05_ECHO_AMP+1, "Echo AMP                       ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L06_FREQ+1,	   "Frequency                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L07_TX_INTV+1,  "Measure Cycle                  ");

			break;
	}	
}

void MsrDp_ListP2(U08* pSel)
{
	U08 c  = MnMSR_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			#if defined (_FT_LANG_KR)
			MnLST_DispClr(1);
			
			DpSTR_DrawLin(ft_loc, FALSE, MnMSR_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(FKR_CHEUG, FKR_JEONG,1,1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnMSR_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#2/3");
		
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L08_OFFSET-6,	_F_STR(FKR_MI_,	   FKR_SE_,   FKR_JO_,  FKR_JEONG, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L09_DAMPING-6,	_F_STR(FKR_DAEM,   FKR_PING,  1,        1, 1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L10_THRESHOLD-6, _F_STR(FKR_MUN,	   FKR_TEOG,  FKR_JEON, FKR_AB_,   1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L11_ASF_BAND-6,	_F_STR(1,		   1,		  1,		1,		   FKR_SEOL, FKR_JEONG,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L12_TW_RST_TM-6, _F_STR(FKR_WIN,	   FKR_DO_,   FKR_U__,  1,		   FKR_RI_,  FKR_SES,   FKR_SI_, FKR_GAN,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L13_TW_RANGE-6,  _F_STR(FKR_WIN,	   FKR_DO_,   FKR_U__,  1,		   FKR_BEOM, FKR_WI_,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L14_PCD_INTV-6,  _F_STR(FKR_SE_,	  FKR_CHEOG, FKR_JU_, FKR_GI_, 1,      1,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_CN)
            MnLST_DispClr(1);
			
			DpSTR_DrawLin(ft_loc, FALSE, MnMSR_X0_TITLE_KO-10, MnSTR_Y_HEIGHT*0, _F_STR(66, 107, 187, 82,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnMSR_X0_PAGE_KO+20,  MnSTR_Y_HEIGHT*0, "#2/3");
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L08_OFFSET-6,	_F_STR( 87,  120,   1,	1,   1,   1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L09_DAMPING-6,	_F_STR(157, 159, 161,  35,   1,   1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L10_THRESHOLD-6, _F_STR(109,  59,  69,  20,   1,   1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L11_ASF_BAND-6,	_F_STR(  1,   1,  15,  83,   1,   1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L12_TW_RST_TM-6, _F_STR(7,  22,   167, 67, 155,   51,110,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L13_TW_RANGE-6,  _F_STR(7, 22, 167,   85,	 25,   1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L14_PCD_INTV-6, _F_STR(67,  65, 110, 113,  1, 1,1,1,1,1,1,1,1,1,1));

			#elif defined (_FT_LANG_JP)	
			MnLST_DispClr(1);
			DpSTR_DrawLin(ft_loc, FALSE, MnMSR_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(81, 73, 1, 1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnMSR_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#2/3");															  	
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L08_OFFSET-6,	_F_STR( 7, 27, 16, 19, 22,   0,   1,   1,	 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L09_DAMPING-6,	_F_STR(56,  65,  85, 63, 1, 1,  1, 1,   1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L10_THRESHOLD-6, _F_STR(15, 38, 19, 13, 33, 30, 42, 37, 23,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L11_ASF_BAND-6,	_F_STR(1, 1, 82, 61, 1, 1,  1,	1,   1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L12_TW_RST_TM-6, _F_STR(5, 4, 41, 23, 5, 36,  16,	19,   22,68,87,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L13_TW_RANGE-6,	_F_STR(5, 4, 41, 23, 5, 78,  58,	1,   1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L14_PCD_INTV-6, _F_STR( 10,	36, 42, 24, 41,  11, 57, 69, 1,	1, 1,1,1,1,1));
			#endif

			DpSTR_DrawLin(ft_eng, pSel[MnMSR_L11_ASF_BAND-7], 10, MnSTR_Y_HEIGHT*4, "ASF");			
			break;
		case MnSYS_LANG_ENG:
		default:
																  // 0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE,	 MnMSR_L00_IDLE+1,		"      MEASUREMENT #2/3         ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L08_OFFSET-6,	"Offset                         ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L09_DAMPING-6,	"Damping                        ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L10_THRESHOLD-6, "Threshold                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L11_ASF_BAND-6,	"ASF Value                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L12_TW_RST_TM-6, "Window Reset Time              ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L13_TW_RANGE-6,	"Window Range                   ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L14_PCD_INTV-6,  "Clean Interval                 ");

			break;
	}	
}

void MsrDp_ListP3(U08* pSel)
{
	U08 c  = MnMSR_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;
	U08 *pStr = "";

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			#if defined (_FT_LANG_KR)
			MnLST_DispClr(1);

			DpSTR_DrawLin(ft_loc, FALSE, MnMSR_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(FKR_CHEUG, FKR_JEONG,1,1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnMSR_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#3/3");
			
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L15_PCD_TERM-13, _F_STR(FKR_SE_,	  FKR_CHEOG, FKR_SI_, FKR_GAN, 1,      1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L16_RST_MSR-13,	_F_STR(FKR_CHEUG, FKR_JEONG, FKR_CHO, FKR_GI_, FKR_HWA,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L17_RST_FTR-13,	_F_STR(FKR_GONG,  FKR_JANG,	 FKR_CHO, FKR_GI_, FKR_HWA,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L18_TEST-13, 	_F_STR(FKR_TE_,	  FKR_SEU,   FKR_TEU,1,FKR_MO_, FKR_DEU,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
															  	  // 0123456789abcdef0123456789abcde
			DpSTR_DrawLin(ft_loc, FALSE, MnMSR_X0_TITLE_KO-10, MnSTR_Y_HEIGHT*0, _F_STR(66, 107, 187, 82,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnMSR_X0_PAGE_KO+20,  MnSTR_Y_HEIGHT*0, "#3/3");			
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L15_PCD_TERM-13, _F_STR(67,  65,  51, 110,  1, 1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L16_RST_MSR-13,	_F_STR(66, 107,  187,  188, 106, 82,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L17_RST_FTR-13,	_F_STR(181,  26,  12,  19,  91, 82,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L18_TEST-13, 	_F_STR(66,  92,  60,  38,  1, 1,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
		    DpSTR_DrawLin(ft_loc, FALSE, MnMSR_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(81, 73, 1, 1,1,1,1,1,1,1,1,1,1,1,1));
		 	DpSTR_DrawLin(ft_eng, FALSE, MnMSR_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#3/3");											  

			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L15_PCD_TERM-13, _F_STR(10,  36, 42,  24, 41, 11, 68, 87,  1,	 1, 1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L16_RST_MSR-13,	_F_STR(73,  61, 36,  16, 19, 22, 1, 1,  1,	 1, 1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L17_RST_FTR-13,	_F_STR(49,	69,  53,  1,   1,   1,   1,   1,	1,	 1, 1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnMSR_L18_TEST-13, 	_F_STR(20,	15, 22, 32, 42, 23,   1,	1,	1,	 1, 1,1,1,1,1));
			#endif
			break;
		case MnSYS_LANG_ENG:
		default:
																  // 0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE,	 MnMSR_L00_IDLE+1,		"      MEASUREMENT #3/3         ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L15_PCD_TERM-13, "Clean Term                     ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L16_RST_MSR-13,	"Measure Reset                  ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L17_RST_FTR-13,	"Factory Reset                  ");
			MnLST_DispStr(ft_eng, pSel[c++], MnMSR_L18_TEST-13, 	"Test Mode                      ");	
			break;
	}	

	if	(lMnMsr.fTest)	pStr = "ON";
	else				pStr = "OFF";
	
	DpSTR_DrawLin(ft_eng, pSel[MnMSR_L18_TEST-14], 180, MnSTR_Y_HEIGHT*4, pStr);
}

#else
void MsrDp_ListEngP1(U08* pSel)
{
	U08 c  = MnMSR_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
													//   0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,     MnMSR_L00_IDLE+1,		"      MEASUREMENT #1/3         ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L01_UNIT+1,		"Display Unit                   ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L02_OPER+1,		"Operation                      ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L03_EMPTY+1,		"Empty                          ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L04_DEADZONE+1,	"Dead Zone                      ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L05_TVG_IDX+1,	"TVG Curve                      ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L06_ECHO_AMP+1,	"Echo AMP                       ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L07_FREQ+1,		"Frequency                      ");
}

void MsrDp_ListEngP2(U08* pSel)
{
	U08 c  = MnMSR_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
													//	 0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,     MnMSR_L00_IDLE+1,		"      MEASUREMENT #2/3         ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L08_TX_INTV-6,	"Measure Cycle                  ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L09_OFFSET-6,	"Offset                         ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L10_DAMPING-6,	"Damping                        ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L11_THRESHOLD-6,	"Threshold                      ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L12_ASF_BAND-6,	"ASF Band                       ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L13_TW_RST_TM-6,	"Window Reset Time              ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L14_TW_RANGE-6,	"Window Range                   ");
}

void MsrDp_ListEngP3(U08* pSel)
{
	U08 *pStr = "   ";
	U08 c  = MnMSR_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
													//	 0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,     MnMSR_L00_IDLE+1,		"      MEASUREMENT #3/3         ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L15_PCD_INTV-13,	"Clean Interval                 ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L16_PCD_TERM-13,	"Clean Term                     ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L17_RST_MSR-13,	"Measure Reset                  ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L18_RST_FTR-13,	"Factory Reset                  ");
	MnLST_DispStr(ft, pSel[c++], MnMSR_L19_TEST-13,		"Demo Mode                      ");

	if(lMnMsr.fDemo)	pStr = "ON";
	else				pStr = "OFF";

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnMSR_L19_TEST-14], 180, MnSTR_Y_HEIGHT*5, pStr);
}

#if defined (_FT_LANG_KR)
void MsrDp_ListKorP1(U08* pSel)
{
	U08 i;

	MnLST_DispClr(1);
	_F_SS(FKR_CHEUG,FKR_JEONG,	1,1,1,1,1,1,1,1,1,1,1,1,1);
	DpSTR_DrawLin(FT_LC_16x16, FALSE, MnMSR_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, gFt_str);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, MnMSR_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#1/3");

	for(i=(MnMSR_L00_IDLE+1); i<=MNU_LIST_NUM; i++)
	{
		switch(i)
		{
			case MnMSR_L01_UNIT:		_F_SS(FKR_CHEUG,FKR_JEONG,	FKR_DAN,	FKR_WI_, 	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L02_OPER:		_F_SS(FKR_SEUL,	FKR_LEO,	FKR_JI_,1,1,FKR_SANG,	FKR_DEUNG,	FKR_SU_,	1,1,1,1,1,1,1);	break;
			case MnMSR_L03_EMPTY:		_F_SS(FKR_CHEUG,FKR_JEONG,	FKR_BEOM,	FKR_WI_, 	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L04_DEADZONE: 	_F_SS(FKR_BUL,	FKR_GAM,	FKR_DAE,	1,			1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L05_TVG_IDX: 	_F_SS(1,1,1,1,	FKR_SEON,	FKR_TAEG,	1,			1,		1,1,1,1,1,1,1);		break;
			case MnMSR_L06_ECHO_AMP:	_F_SS(FKR_SU_,	FKR_SIN,	FKR_GAM,	FKR_DO_, 	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L07_FREQ:		_F_SS(1,		1,			1,			1,			1,1,1,1,1,1,1,1,1,1,1);		break;
			//case MnMSR_L07_FREQ:		_F_SS(FKR_JU_,	FKR_PA_,	FKR_SU_,	1,			1,1,1,1,1,1,1,1,1,1,1);		break;
		}

		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnMSR_L02_OPER],   60, MnSTR_Y_HEIGHT*2, "/");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnMSR_L05_TVG_IDX], 9, MnSTR_Y_HEIGHT*5, "TVG");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnMSR_L07_FREQ],    9, MnSTR_Y_HEIGHT*7, "Frequency");
}

void MsrDp_ListKorP2(U08* pSel)
{
	U08 i;

	MnLST_DispClr(1);
	_F_SS(FKR_CHEUG,FKR_JEONG,	1,1,1,1,1,1,1,1,1,1,1,1,1);
	DpSTR_DrawLin(FT_LC_16x16, FALSE, MnMSR_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, gFt_str);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, MnMSR_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#2/3");

	for(i=(MNU_LIST_NUM+1); i<=(MNU_LIST_NUM*2); i++)
	{
		switch(i)
		{
			case MnMSR_L08_TX_INTV:		_F_SS(FKR_CHEUG,FKR_JEONG,	FKR_JU_,	FKR_GI_,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L09_OFFSET:		_F_SS(FKR_MI_,	FKR_SE_,	FKR_JO_,	FKR_JEONG,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L10_DAMPING:		_F_SS(FKR_PYEONG,FKR_GYUN,	FKR_BO_,	FKR_JEONG,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L11_THRESHOLD:	_F_SS(FKR_MUN,	FKR_TEOG,	FKR_JEON,	FKR_AB_,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L12_ASF_BAND:	_F_SS(1,1,1,1,	FKR_SEOL,	FKR_JEONG,	1,			1,1,1,1,1,1,1,1);			break;
			case MnMSR_L13_TW_RST_TM: 	_F_SS(FKR_WIN,	FKR_DO_,	FKR_U__,1,	FKR_RI_,	FKR_SES,FKR_SI_,FKR_GAN,1,1,1,1,1,1,1);	break;
			case MnMSR_L14_TW_RANGE: 	_F_SS(FKR_WIN,	FKR_DO_,	FKR_U__,1,	FKR_BEOM,	FKR_WI_,1,1,1,1,1,1,1,1,1);	break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i-MNU_LIST_NUM], i-MNU_LIST_NUM+1, "");
	}

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnMSR_L12_ASF_BAND-7], 10, MnSTR_Y_HEIGHT*5, "ASF");
}

void MsrDp_ListKorP3(U08* pSel)
{
	U08 i;
	U08 *pStr = "   ";

	MnLST_DispClr(1);
	_F_SS(FKR_CHEUG,FKR_JEONG,	1,1,1,1,1,1,1,1,1,1,1,1,1);
	DpSTR_DrawLin(FT_LC_16x16, FALSE, MnMSR_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, gFt_str);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, MnMSR_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#3/3");

	for(i=((MNU_LIST_NUM*2)+1); i<MnMSR_LST_NUMB; i++)
	{
		switch(i)
		{
			case MnMSR_L15_PCD_INTV:	_F_SS(FKR_SE_,	FKR_CHEOG,	FKR_JU_,	FKR_GI_,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L16_PCD_TERM:	_F_SS(FKR_SE_,	FKR_CHEOG, 	FKR_SI_,	FKR_GAN,	1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L17_RST_MSR:		_F_SS(FKR_CHEUG,FKR_JEONG,	FKR_CHO,	FKR_GI_,	FKR_HWA,1,1,1,1,1,1,1,1,1,1);	break;
			case MnMSR_L18_RST_FTR:		_F_SS(FKR_GONG,	FKR_JANG,	FKR_CHO,	FKR_GI_,	FKR_HWA,1,1,1,1,1,1,1,1,1,1);	break;
			case MnMSR_L19_TEST:		_F_SS(FKR_DE_,	FKR_MO_,1,	FKR_MO_,	FKR_DEU,	  1,1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i-(MNU_LIST_NUM*2)], i-(MNU_LIST_NUM*2)+1, "");
	}

	if(lMnMsr.fDemo)	pStr = "ON";
	else				pStr = "OFF";

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnMSR_L19_TEST-14], 180, MnSTR_Y_HEIGHT*5, pStr);
}

#elif defined (_FT_LANG_CN)
void MsrDp_ListChnP1(U08* pSel)
{
	U08 i;
	U08 height = MnSTR_Y_HEIGHT;
	U08 ft_en = FT_EN_08x16;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "      MEASUREMENT #1/3         ");

	for(i=(MnMSR_L00_IDLE+1); i<=MNU_LIST_NUM; i++)
	{
		switch(i)
		{
			case MnMSR_L01_UNIT: 		_F_SS(66,		107,	18,		4,		1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L02_OPER: 		_F_SS(66,		107,	60, 	38,		1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L03_EMPTY: 		_F_SS(76,		117,	1,		1,		1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L04_DEADZONE: 	_F_SS(70,		17,		1,		1,		1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L05_TVG_IDX: 	_F_SS(7,		22,		165,	166,	1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L06_ECHO_AMP: 	_F_SS(7,		22,		47,		27,		24,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L07_FREQ:		_F_SS(63,		64,		74,		78,		1,		1,1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
}

void MsrDp_ListChnP2(U08* pSel)
{
	U08 i;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "      MEASUREMENT #2/3         ");

	for(i=(MNU_LIST_NUM+1); i<=(MNU_LIST_NUM*2); i++)
	{
		switch(i)
		{
			case MnMSR_L08_TX_INTV: 	_F_SS(66,		107,	110,	113,1,	1,	1,	1,	1,1,1,1,1,1,1);		break;
			case MnMSR_L09_OFFSET:		_F_SS(42,		94,		1,	1,	1,	1,	1,	1,		1,1,1,1,1,1,1);		break;
			case MnMSR_L10_DAMPING:		_F_SS(111,		33,		1,	1,	1,	1,	1,	1,		1,1,1,1,1,1,1);		break;
			case MnMSR_L11_THRESHOLD:	_F_SS(109,		59,		69,		20, 1,	1,	1,	1,	1,1,1,1,1,1,1);		break;
			case MnMSR_L12_ASF_BAND:	_F_SS(1,	1,	15,		83,		1,		1,	1,	1,	1,1,1,1,1,1,1);		break;
			case MnMSR_L13_TW_RST_TM: 	_F_SS(167,		171,	172,1,	173,	174,	1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L14_TW_RANGE: 	_F_SS(167,		168,	169,1,	1,	1,	1,	1,		1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i-MNU_LIST_NUM], i-MNU_LIST_NUM+1, "");
	}
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnMSR_L12_ASF_BAND-7], 10, MnSTR_Y_HEIGHT*5, "ASF");
}

void MsrDp_ListChnP3(U08* pSel)
{
	U08 i;
	U08 *pStr = "	";
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "      MEASUREMENT #3/3         ");

	for(i=((MNU_LIST_NUM*2)+1); i<MnMSR_LST_NUMB; i++)
	{
		switch(i)
		{
			case MnMSR_L15_PCD_INTV:	_F_SS(67,		65,		110,	113,	1,		1,		1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L16_PCD_TERM:	_F_SS(67,		65,		51,		110,	1,		1,		1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L17_RST_MSR:		_F_SS(66,		107,	91,		82,		26,		4,		1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L18_RST_FTR:		_F_SS(12,		19,		91,		82,		1,		1,		1,1,1,1,1,1,1,1,1);		break;
			case MnMSR_L19_TEST:		_F_SS(66,		92,		60,		38,		1,		1,		1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i-(MNU_LIST_NUM*2)], i-(MNU_LIST_NUM*2)+1, "");
	}

	if(lMnMsr.fDemo)	pStr = "ON";
	else				pStr = "OFF";

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnMSR_L19_TEST-14], 180, MnSTR_Y_HEIGHT*5, pStr);
}

#elif defined (_FT_LANG_JP)
void MsrDp_ListJpnP1(U08* pSel)
{
	U08 i;
	U08 height = MnSTR_Y_HEIGHT;
	U08 ft_en = FT_EN_08x16;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "      MEASUREMENT #1/3         ");

	for(i=(MnMSR_L00_IDLE+1); i<=MNU_LIST_NUM; i++)
	{
		switch(i)
		{
			case MnMSR_L01_UNIT: 		_F_SS(114,	166,87,	0,	1,  1,	1,	1,	1,	1,1,1,1,1,1);		break;
			case MnMSR_L02_OPER: 		_F_SS(93,	57,	159,170,106,154,166,0,	1,	1,1,1,1,1,1);		break;
			case MnMSR_L03_EMPTY: 		_F_SS(94,	156,96, 154,157,0,	1,	1,	1,	1,1,1,1,1,1);		break;
			case MnMSR_L04_DEADZONE: 	_F_SS(136,	94,	166,114,87, 0,	1,	1,	1,	1,1,1,1,1,1);		break;
			case MnMSR_L05_TVG_IDX: 	_F_SS(130,	118,106,166,171,132,162,170,0,	1,1,1,1,1,1);		break;
			case MnMSR_L06_ECHO_AMP: 	_F_SS(107,	152,106,166,171,85,	166,138,0,	1,1,1,1,1,1);		break;
			case MnMSR_L07_FREQ:		_F_SS(108,	156,118,107,171,129,89, 124,0,	1,1,1,1,1,1);		break;
		}

		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
}

void MsrDp_ListJpnP2(U08* pSel)
{
	U08 i;
	U08 height = MnSTR_Y_HEIGHT;
	U08 ft_en = FT_EN_08x16;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "      MEASUREMENT #2/3         ");

	for(i=(MNU_LIST_NUM+1); i<=(MNU_LIST_NUM*2); i++)
	{
		switch(i)
		{																		// 0123456789abcdef0123456789abcde
			case MnMSR_L08_TX_INTV: 	_F_SS(112,	98,	121,87, 106,152,89,	96,	0,	1,1,1,1,1,1);		break;
			case MnMSR_L09_OFFSET:		_F_SS( 93,	136,110,118,123,0,  1, 	1,	1,	1,1,1,1,1,1);		break;
			case MnMSR_L10_DAMPING:		_F_SS(139,	87,	96,	166,171,107,94, 166,1,	1,1,1,1,1,1);		break;
			case MnMSR_L11_THRESHOLD:	_F_SS(108,	159,118,106,152,142,170,158,124,1,1,1,1,1,1);		break;
			case MnMSR_L12_ASF_BAND:	_F_SS(171,	171,171,110,118,121,87,	0,	1,	1,1,1,1,1,1);		break;
			case MnMSR_L13_TW_RST_TM: 	DpSTR_DrawLin(ft_en, pSel[i], 8, height*i,"Window Reset Time              ");	break;
			case MnMSR_L14_TW_RANGE: 	DpSTR_DrawLin(ft_en, pSel[i], 8, height*i,"Window Range                   ");	break;
		}

		switch(i)
		{
			case MnMSR_L13_TW_RST_TM:
			case MnMSR_L14_TW_RANGE:
				if(pSel[i])		DpSTR_DrawLin(ft_en, pSel[i], 0, height*i,">");
				else			DpSTR_DrawLin(ft_en, pSel[i], 0, height*i," ");
				break;

			default:	MnLST_DispStr(FT_LC_16x16, pSel[i-MNU_LIST_NUM], i-MNU_LIST_NUM+1, "");		break;
		}
	}

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnMSR_L12_ASF_BAND-7], 10, MnSTR_Y_HEIGHT*5, "ASF");
}

void MsrDp_ListJpnP3(U08* pSel)
{
	U08 i;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "      MEASUREMENT #3/3         ");

	for(i=(MNU_LIST_NUM+1); i<MnMSR_LST_NUMB; i++)
	{
		switch(i)
		{
			case MnMSR_L15_PCD_INTV:	_F_SS(98,	157,170,126,166,99, 106,152,89,	96,	0,1,1,1,1);		break;
			case MnMSR_L16_PCD_TERM:	_F_SS(98,	157,170,126,166,99, 171,107,94, 166,0,1,1,1,1);		break;
			case MnMSR_L17_RST_MSR:		_F_SS(112,  98, 121,87, 157,110,118,123,0,	1,	1,1,1,1,1);		break;
			case MnMSR_L18_RST_FTR:		_F_SS(106,	154, 96,94,	0,  1,  1,  1,	1,	1,	1,1,1,1,1);		break;
			case MnMSR_L19_TEST:		_F_SS(121,	108,123,149,170,124,0,	1,	1,	1,	1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i-(MNU_LIST_NUM*2)], i-(MNU_LIST_NUM*2)+1, "");
	}
}

#endif
#endif

void MsrDp_ProcList(U08 sel)
{
	U08 i;
	U08 page = MNU_PAGE_01;
	U08 pSel[MnMSR_LST_NUMB] = {0, };

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
		case MNU_PAGE_01:		MsrDp_ListP1(pSel);		break;
		case MNU_PAGE_02:		MsrDp_ListP2(pSel);		break;
		case MNU_PAGE_03:		MsrDp_ListP3(pSel);		break;
		default:
			return;
	}

#else
	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			switch(page)
			{
#if defined (_FT_LANG_KR)
				case MNU_PAGE_01:		MsrDp_ListKorP1(pSel);		break;
				case MNU_PAGE_02:		MsrDp_ListKorP2(pSel);		break;
				case MNU_PAGE_03:		MsrDp_ListKorP3(pSel);		break;

#elif defined (_FT_LANG_CN)
				case MNU_PAGE_01:		MsrDp_ListChnP1(pSel);		break;
				case MNU_PAGE_02:		MsrDp_ListChnP2(pSel);		break;
				case MNU_PAGE_03:		MsrDp_ListChnP3(pSel);		break;

#elif defined (_FT_LANG_JP)
				case MNU_PAGE_01:		MsrDp_ListJpnP1(pSel);		break;
				case MNU_PAGE_02:		MsrDp_ListJpnP2(pSel);		break;
				case MNU_PAGE_03:		MsrDp_ListJpnP3(pSel);		break;
#endif
			}
			break;
		case MnSYS_LANG_ENG:
		default:
			switch(page)
			{
				case MNU_PAGE_01:		MsrDp_ListEngP1(pSel);		break;
				case MNU_PAGE_02:		MsrDp_ListEngP2(pSel);		break;
				case MNU_PAGE_03:		MsrDp_ListEngP3(pSel);		break;
			}
			break;
	}
#endif
	if(page == MNU_PAGE_03)
	{
		for(i=(MnMSR_LST_NUMB-(MNU_LIST_NUM*2)+1); i<=(MNU_LIST_NUM+1); i++)
			MnLST_DispClr(i);
	}

    LDT_UpdBuff(LCD_Y_START,LCD_Y_END);
	DLY_RunMsec(100);
}

void MsrChk_Vari(void)
{
	U08 maxRange  = MnFTR_GetDistMax();
	
	// Page 1
	if(lMnMsr.unit  > MnMSR_UNIT_FEET)		lMnMsr.unit  = MnMSR_UNIT_METER;
	if(lMnMsr.oper  > MnMSR_OPER_DISTA)		lMnMsr.oper  = MnMSR_OPER_LEVEL;
	
	switch(maxRange)
	{
		case MnFTR_DIST_MAX_10m:
			if(lMnMsr.empty > MnMSR_EMPTY_MT_10_MAX)	lMnMsr.empty = MnMSR_EMPTY_MT_DFT;
			if(lMnMsr.dead  > MnMSR_DEAD_MT_10_MAX)		lMnMsr.dead  = MnMSR_DEAD_MT_DFT;
			break;
		case MnFTR_DIST_MAX_20m:
			if(lMnMsr.empty > MnMSR_EMPTY_MT_20_MAX)	lMnMsr.empty = MnMSR_EMPTY_MT_DFT;
			if(lMnMsr.dead  > MnMSR_DEAD_MT_20_MAX)		lMnMsr.dead  = MnMSR_DEAD_MT_DFT;
			break;
	}

	if(lMnMsr.amp   > MnMSR_AMP_MAX)		lMnMsr.amp   = MnMSR_AMP_DFT;
	if(lMnMsr.freq  > MnMSR_FREQ_MAX)		lMnMsr.amp   = MnMSR_FREQ_380K;
	// Page 2
	if(lMnMsr.intv < MnMSR_INTV_MIN)			lMnMsr.intv = MnMSR_INTV_DFT;
	if(lMnMsr.intv > MnMSR_INTV_MAX)			lMnMsr.intv = MnMSR_INTV_DFT;
	if(lMnMsr.offs < MnMSR_OFFS_MT_MIN)			lMnMsr.offs = MnMSR_OFFS_MT_DFT;
	if(lMnMsr.offs > MnMSR_OFFS_MT_MAX)			lMnMsr.offs = MnMSR_OFFS_MT_DFT;
	if(lMnMsr.damp > MnMSR_DAMP_IDX_MAX)		lMnMsr.damp = MnMSR_DAMP_I01_T0001;
	if(lMnMsr.trhd_mod > MnMSR_TRHD_MOD_MANUAL)	lMnMsr.trhd_mod = MnMSR_TRHD_MOD_AUTO;
	if(lMnMsr.trhd_per > MnMSR_TRHD_PCT_MAX)	lMnMsr.trhd_per = MnMSR_TRHD_PCT_DFT;
	if(lMnMsr.trhd_fix > MnMSR_TRHD_FIX_MAX)	lMnMsr.trhd_fix = MnMSR_TRHD_FIX_DFT;
	if(lMnMsr.asf	   > MnMSR_ASF_MAX)			lMnMsr.asf		= MnMSR_ASF_DFT;
	if(lMnMsr.tw_delay < MnMSR_TW_DELAY_MIN)	lMnMsr.tw_delay = MnMSR_TW_DELAY_DFT;
	if(lMnMsr.tw_delay > MnMSR_TW_DELAY_MAX)	lMnMsr.tw_delay = MnMSR_TW_DELAY_DFT;
	if(lMnMsr.tw_range < MnMSR_TW_RANGE_MIN)	lMnMsr.tw_range = MnMSR_TW_RANGE_DFT;
	if(lMnMsr.tw_range > MnMSR_TW_RANGE_MAX) 	lMnMsr.tw_range = MnMSR_TW_RANGE_DFT;
	// Page 3
	if(lMnMsr.pcd_intv > MnMSR_PCD_INTV_MAX) 	lMnMsr.pcd_intv = MnMSR_PCD_INTV_DFT;
	if(lMnMsr.pcd_term < MnMSR_PCD_TERM_MIN) 	lMnMsr.pcd_term = MnMSR_PCD_TERM_DFT;
	if(lMnMsr.pcd_term > MnMSR_PCD_TERM_MAX) 	lMnMsr.pcd_term = MnMSR_PCD_TERM_DFT;
	if(lMnMsr.fTest > TRUE)						lMnMsr.fTest = FALSE;

}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Get Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Page 1
U08 MnMSR_GetUnit(void)			{	return lMnMsr.unit;		}
U08 MnMSR_GetOper(void)			{	return lMnMsr.oper;		}
U16 MnMSR_GetEmpty(void)		{	return lMnMsr.empty;	}
U16 MnMSR_GetDead(void)			{	return lMnMsr.dead;		}
U08 MnMSR_GetAmp(void)			{	return lMnMsr.amp;		}
U08 MnMSR_GetFreq(void)			{	return lMnMsr.freq;		}
U08 MnMSR_GetIntv(void)			{	return lMnMsr.intv;			}
// Page 2
S16 MnMSR_GetOffs(void)			{	return lMnMsr.offs;			}
U08 MnMSR_GetDamp(void)			{	return lMnMsr.damp;			}
U08 MnMSR_GetTrhdMod(void)		{	return lMnMsr.trhd_mod;		}
U08 MnMSR_GetTrhdPer(void)		{	return lMnMsr.trhd_per;		}
U08 MnMSR_GetTrhdFix(void)		{	return lMnMsr.trhd_fix;		}
U16 MnMSR_GetAsf(void)			{	return lMnMsr.asf;			}
U16 MnMSR_GetTwDly(void)		{	return lMnMsr.tw_delay;		}
U08 MnMSR_GetTwRng(void)		{	return lMnMsr.tw_range;		}
U16 MnMSR_GetPcdIntv(void)		{	return lMnMsr.pcd_intv;		}
// Page 3
U08 MnMSR_GetPcdTerm(void)		{	return lMnMsr.pcd_term;		}
U08 MnMSR_GetFgTest(void)		{	return lMnMsr.fTest;		}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Set Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Page 2
void MnMSR_SetTrhdPer(U08 val)		{	lMnMsr.trhd_per = val;		}
void MnMSR_SetTrhdFix(U08 val)		{	lMnMsr.trhd_fix = val;		}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Memory Access
//------------------------------------------------------------------------------------------------------------------------------
void MnMSR_MemLoad(void)
{	// Page 1
	lMnMsr.unit  = FLS_Rd1Byte(F000_MSR_UNIT);
	lMnMsr.oper  = FLS_Rd1Byte(F001_MSR_OPER);
	lMnMsr.empty = FLS_Rd2Byte(F002_MSR_EMPTY_L);
	lMnMsr.dead  = FLS_Rd2Byte(F004_MSR_DEAD_L);
	lMnMsr.amp   = FLS_Rd1Byte(F044_MSR_AMP);
	lMnMsr.freq  = FLS_Rd1Byte(F039_MSR_FREQ);
	// Page 2
	lMnMsr.intv		= FLS_Rd1Byte(F042_MSR_INTV);
	lMnMsr.offs		= FLS_Rd2Byte(F050_MSR_OFFS_L);
	lMnMsr.damp		= FLS_Rd1Byte(F037_MSR_DAMP);
	lMnMsr.trhd_mod = FLS_Rd1Byte(F100_MSR_TRHD_MODE);
	lMnMsr.trhd_per = FLS_Rd1Byte(F038_MSR_TRHD_PERC);
	lMnMsr.trhd_fix = FLS_Rd1Byte(F091_MSR_TRHD_FIX);
	lMnMsr.asf		= FLS_Rd2Byte(F047_MSR_ASF_L);
	lMnMsr.tw_delay = FLS_Rd2Byte(F075_MSR_TW_DELAY_L);
	lMnMsr.tw_range = FLS_Rd1Byte(F089_MSR_TW_WIDTH);
	// Page 3
	lMnMsr.pcd_intv = FLS_Rd2Byte(F034_MSR_PCD_INTV_L);
	lMnMsr.pcd_term = FLS_Rd1Byte(F036_MSR_PCD_TERM);
	lMnMsr.fTest    = FLS_Rd1Byte(F049_MSR_F_DEMO);

}

void MnMSR_MemSave(void)
{	// Page 1
    FLS_Wr1Byte(F000_MSR_UNIT,	  lMnMsr.unit);
    FLS_Wr1Byte(F001_MSR_OPER,	  lMnMsr.oper);
    FLS_Wr2Byte(F002_MSR_EMPTY_L, lMnMsr.empty);
    FLS_Wr2Byte(F004_MSR_DEAD_L,  lMnMsr.dead);
    FLS_Wr1Byte(F044_MSR_AMP,	  lMnMsr.amp);
    FLS_Wr1Byte(F039_MSR_FREQ,	  lMnMsr.freq);
	// Page 2
    FLS_Wr1Byte(F042_MSR_INTV,		 lMnMsr.intv);
    FLS_Wr2Byte(F050_MSR_OFFS_L,	 lMnMsr.offs);
    FLS_Wr1Byte(F037_MSR_DAMP,		 lMnMsr.damp);
	FLS_Wr1Byte(F100_MSR_TRHD_MODE,  lMnMsr.trhd_mod);       
    FLS_Wr1Byte(F038_MSR_TRHD_PERC,  lMnMsr.trhd_per);
	FLS_Wr1Byte(F091_MSR_TRHD_FIX,   lMnMsr.trhd_fix);
    FLS_Wr2Byte(F047_MSR_ASF_L,		 lMnMsr.asf);
    FLS_Wr2Byte(F075_MSR_TW_DELAY_L, lMnMsr.tw_delay);
	FLS_Wr1Byte(F089_MSR_TW_WIDTH,   lMnMsr.tw_range);
	// Page 3
	FLS_Wr2Byte(F034_MSR_PCD_INTV_L, lMnMsr.pcd_intv);
	FLS_Wr1Byte(F036_MSR_PCD_TERM,   lMnMsr.pcd_term);
	FLS_Wr1Byte(F049_MSR_F_DEMO,     lMnMsr.fTest);

}

void MnMSR_MemFtry(void)
{	// Page 1
	FLS_Wr1Byte(F000_MSR_UNIT,	  MnMSR_UNIT_METER);
	FLS_Wr1Byte(F001_MSR_OPER,	  MnMSR_OPER_LEVEL);
	FLS_Wr2Byte(F002_MSR_EMPTY_L, MnMSR_EMPTY_MT_DFT);
	FLS_Wr2Byte(F004_MSR_DEAD_L,  MnMSR_DEAD_MT_DFT);
	FLS_Wr1Byte(F044_MSR_AMP,	  MnMSR_AMP_DFT);
	FLS_Wr1Byte(F039_MSR_FREQ,	  MnMSR_FREQ_380K);
	// Page 2
	FLS_Wr1Byte(F042_MSR_INTV,		 MnMSR_INTV_DFT);
	FLS_Wr2Byte(F050_MSR_OFFS_L,	 MnMSR_OFFS_MT_DFT);
	FLS_Wr1Byte(F037_MSR_DAMP,		 MnMSR_DAMP_I03_T0020);
	FLS_Wr1Byte(F100_MSR_TRHD_MODE,  MnMSR_TRHD_MOD_AUTO);
	FLS_Wr1Byte(F038_MSR_TRHD_PERC,  MnMSR_TRHD_PCT_DFT);
	FLS_Wr1Byte(F091_MSR_TRHD_FIX,   MnMSR_TRHD_FIX_DFT);
	FLS_Wr2Byte(F047_MSR_ASF_L,		 MnMSR_ASF_DFT);
	FLS_Wr2Byte(F075_MSR_TW_DELAY_L, MnMSR_TW_DELAY_DFT);
	FLS_Wr1Byte(F089_MSR_TW_WIDTH,   MnMSR_TW_RANGE_DFT);
	// Page 3
	FLS_Wr2Byte(F034_MSR_PCD_INTV_L, MnMSR_PCD_INTV_DFT);
	FLS_Wr1Byte(F036_MSR_PCD_TERM,   MnMSR_PCD_TERM_DFT);
	FLS_Wr1Byte(F049_MSR_F_DEMO,	 FALSE);

}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MnMSR_InitVari(void)
{
	MnMSR_MemLoad();
	MsrChk_Vari();
}

void MnMSR_ProcMain(void)
{
	U08 fDp = FALSE;
	U08 sel = MnMSR_L00_IDLE + 1;

	MsrDp_ProcList(sel);

	do
	{
		if(!_SBIT_BTN_DOWN)
		{
			if(sel < (MnMSR_LST_NUMB-1))		sel++;
			else								sel = MnMSR_L00_IDLE + 1;
			fDp = TRUE;
		}
		
		if(!_SBIT_BTN_UP)
		{
			if(sel > (MnMSR_L00_IDLE+1))		sel--;
			else								sel = MnMSR_LST_NUMB - 1;
			fDp = TRUE;
		}

		if(!_SBIT_BTN_SET)
		{
			switch(sel)
			{	// Page 1
				case MnMSR_L01_UNIT:			MnMsr_Unit();			break;
				case MnMSR_L02_OPER:			MnMsr_Operation();		break;
				case MnMSR_L03_EMPTY:			MnMsr_Empty();			break;
				case MnMSR_L04_DEADZONE:		MnMsr_DeadZone();		break;
				case MnMSR_L05_ECHO_AMP:		MnMsr_EchoAmp();		break;
				case MnMSR_L06_FREQ:			MnMsr_Freq();			break;
				case MnMSR_L07_TX_INTV:			MnMsr_Interval();		break;
				// Page 2
				case MnMSR_L08_OFFSET:			MnMsr_Offset();			break;
				case MnMSR_L09_DAMPING:			MnMsr_Damping();		break;
				case MnMSR_L10_THRESHOLD:		MnMsr_Threshold();		break;
				case MnMSR_L11_ASF_BAND:		MnMsr_CfgAsf();			break;
				case MnMSR_L12_TW_RST_TM:		MnMsr_TwDelay();		break;
				case MnMSR_L13_TW_RANGE:		MnMsr_TwRange();		break;
				case MnMSR_L14_PCD_INTV:		MnMsr_PcdIntv();		break;
				// Page 3
				case MnMSR_L15_PCD_TERM:		MnMsr_PcdTerm();		break;
				case MnMSR_L16_RST_MSR:			MnMsr_RstMsr();			break;
				case MnMSR_L17_RST_FTR:			MnMsr_RstFtr();			break;
				case MnMSR_L18_TEST:			MnMsr_TestMode();		break;
			}
			fDp = TRUE;
		}

		if(fDp)
		{
			MsrDp_ProcList(sel);
			fDp = FALSE;
		}
	}
	while(_SBIT_BTN_EXIT);
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

