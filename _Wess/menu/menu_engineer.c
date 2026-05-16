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
#include "menu_engineer.h"
// bsp
#include "flash_main.h"
#include "gpio_main.h"
#include "tmr_main.h"
#include "dly_main.h"
#include "adc_main.h"
#include "btn_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
// app
#include "isr_main.h"
#include "app_main.h"
#include "font_def.h"
#include "measure_main.h"
#include "measure_analyze.h"
// disp
#include "disp_main.h"
#include "disp_menu.h"
#include "disp_string.h"
// menu
#include "menu_box.h"
#include "menu_main.h"
#include "menu_system.h"
#include "menu_measure.h"
#include "menu_test.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
LS_MnEGN lMnEgn;
U16 lMnEgn_aTvg_idle[60]	= {	
	  1,  12,  19,	24,  27,  30,  33,	35,  37,  38,  40,	41,  43,  44,  45,	46,  47,  48,  49,	50,
	 50,  51,  52,	53,  53,  54,  55,	55,  56,  56,  57,	57,  58,  58,  59,	59,  60,  60,  60,	61,
	 61,  62,  62,	62,  63,  63,  64,	64,  64,  65,  65,	65,  65,  66,  66,	66,  67,  67,  67,	67, 	};


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------
void MnEgn_ProfNum(void) 
{
	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X08, MnBOX_Y01, F_F, "Number");
	lMnEgn.prof_num = MnBOX_CfgNumb(lMnEgn.prof_num, MnEGN_PROF_NUM_MIN, MnEGN_PROF_NUM_MAX, MnEGN_PROF_NUM_STP, 0, F_F, "");
}

void MnEgn_SmthNum(void) 
{
	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X08, MnBOX_Y01, F_F, "Number");
	lMnEgn.smth_cnt = MnBOX_CfgNumb(lMnEgn.smth_cnt, MnEGN_SMTH_CNT_MIN, MnEGN_SMTH_CNT_MAX, MnEGN_SMTH_CNT_STP, 0, F_F, "");
}

void MnEgn_SmthRng(void) 
{
	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Range");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y02, F_F, "(cm)");
	lMnEgn.smth_rng = MnBOX_CfgNumb(lMnEgn.smth_rng, MnEGN_SMTH_RNG_MIN, MnEGN_SMTH_RNG_MAX, MnEGN_SMTH_RNG_STP, 0, F_F, "");
}

void MnEgn_TrhdRng(void) 
{
	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Range");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y02, F_F, "(cm)");
	lMnEgn.trhd_rng = MnBOX_CfgNumb(lMnEgn.trhd_rng, MnEGN_TRHD_RNG_MIN, MnEGN_TRHD_RNG_MAX, MnEGN_TRHD_RNG_STP, 0, F_F, "");
}

void MnEgn_TrhdMin(void) 
{
	F32 calc;
	U08 volt;
	U08 cnt = 0;
	S16 old = lMnEgn.trhd_min;
	S16 sel = old;

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y01, F_F, "MIN");
	DpSTR_NumbUpd(MnBOX_Y02, sel, MnEGN_TRHD_MIN_MAX, 0, F_F, "");

	calc = (sel * MnEGN_TRHD_VOL_MAX) / MnEGN_TRHD_MIN_MAX;
	volt = calc;
	DpSTR_NumbUpd(MnBOX_Y03, volt, MnEGN_TRHD_VOL_MAX, 2, 0, "V");
	BTN_DelayMsec(BTN_WAIT_SET);

	do
	{
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{
			if(++cnt >= 30)
				cnt = 30;

			sel = BTN_GetValue_UpDn(sel, MnEGN_TRHD_MIN_MIN, MnEGN_TRHD_MIN_MAX, MnEGN_TRHD_MIN_STP, cnt);
			DpSTR_NumbUpd(MnBOX_Y02, sel, MnEGN_TRHD_MIN_MAX, 0, 0, "");

			calc = (sel * MnEGN_TRHD_VOL_MAX) / MnEGN_TRHD_MIN_MAX;
			volt = calc;
			DpSTR_NumbUpd(MnBOX_Y03, volt, MnEGN_TRHD_VOL_MAX, 2, 0, "V");
			BTN_DelayMsec(BTN_WAIT_UD);
		}
		else
			cnt = 0;
	}
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if(!_SBIT_BTN_EXIT)
	{
		lMnEgn.trhd_min = old;
		return;
	}

	if(sel != old)
		lMnEgn.trhd_min = sel;
}


U08 EgnTvg_ResetAll(U16 *tvg_gain)
{
	U16 gain = 0;

	DSP_ConfigBox(MnSYS_LANG_ENG, MnBOX_X02, 32+10, "Reset Gain", 0);
    gain = MnBOX_CfgNumb(gain, 0, 2000, 1, 0, 0, "");

	if(!BTN_GetStatus(BTN_IDX_SET))
		return FALSE;

	*tvg_gain = gain;
	
	return TRUE;
}

void EgnTvg_ProcFunc(void) 
{
	U08 i = 0;
    U08 cnt = 0;
	U08 cursor_x = 0;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U08 p_st2[APP_STR_LENGTH] = {0, };
	U08 p_st3[APP_STR_LENGTH] = {0, };
	U08 p_st4[APP_STR_LENGTH] = {0, };
	U16 empty = MsANL_GetEmpty();
	U08 tvg_idx  = 0;
	U16 tvg_dst  = tvg_idx*20;
	U16 tvg_gain = lMnEgn.aTvg[tvg_idx];
    U16 tvg_dst_min = 0;
    U16 tvg_dst_max = (empty / 20) * 20;
	U16 tvg_buf[60] = {0, };

	for(i=0; i<60; i++)
		tvg_buf[i] = lMnEgn.aTvg[i];
		
    gDp_cursor_vert = 0;	
    gIsr_tm3_cnt = 100;

    DISP_ClearScr();
    DpSTR_LineUpd(FT_EN_08x16, 0,   5,   0, "N:");
    DpSTR_LineUpd(FT_EN_08x16, 0,  92,   0, "D:");
    DpSTR_LineUpd(FT_EN_08x16, 0, 179,   0, "G:");
    //DpSTR_LineUpd(FT_EN_08x16, 0,   5,  18, "A:");
    DpSTR_LineUpd(FT_EN_06x07, 0, 195, 119, "Empty");
    BTN_DelayMsec(BTN_WAIT_SET);
	MEAS_CfgSens(4);

	TMR3_Start1sec();	// 1sec. Timer - Measurement Period
	
    do
    {
		// Reset Gain
		if(BTN_GetStatus(BTN_IDX_RIGHT) && BTN_GetStatus(BTN_IDX_LEFT))
		{
			if(EgnTvg_ResetAll(&tvg_gain))
			{
				for(i=0; i<59; i++)
					lMnEgn.aTvg[i] = tvg_gain;
			}

			BTN_DelayMsec(BTN_WAIT_SET);
		}


		if(BTN_GetStatus(BTN_IDX_PROG))
		{
			//U08 old=lMnEgn.fTvg_load;
			
			DSP_ConfigBox(MnSYS_LANG_ENG,MnBOX_X02, 32+10, "Load Curve", 0);
			lMnEgn.fTvg_load = MnBOX_CfgNumb(lMnEgn.fTvg_load, 0, 5, 1, 0, 0, "");
			
			if(!_SBIT_BTN_SET)
			{
				//tvg_gain = MEAS_GetTvg(lMnEgn.fTvg_load, 0);			
				for(i=0; i<60; i++)
					lMnEgn.aTvg[i] = MEAS_GetTvg(lMnEgn.fTvg_load, i);
				
				tvg_gain = lMnEgn.aTvg[i];
			}
			BTN_DelayMsec(BTN_WAIT_SET);
		}
		
		// Adjust Gain
		if(BTN_GetStatus(BTN_IDX_UP) || BTN_GetStatus(BTN_IDX_DOWN)) 
		{
			if((_SBIT_BTN_UP == BTN_STT_PRESS) || (_SBIT_BTN_DOWN == BTN_STT_PRESS))
			{
                if (++cnt >= 30)
                    cnt = 30;
			}

			tvg_gain = lMnEgn.aTvg[tvg_idx];
			tvg_gain = BTN_GetValue_UpDn(tvg_gain, 0, 2000, 1, cnt);

			lMnEgn.aTvg[tvg_idx] = tvg_gain;

			BTN_DelayMsec(BTN_WAIT_UD);
		}
        else
			cnt = 0;

		// Move index of Tvg Table
		if(BTN_GetStatus(BTN_IDX_RIGHT) || BTN_GetStatus(BTN_IDX_LEFT)) 
		{			
			if(cursor_x > 2) LDT_ClearDot(0+cursor_x-2, 30);
			if(cursor_x > 1) LDT_ClearDot(0+cursor_x-1, 31);
			LDT_ClearDot(0+cursor_x+1, 31);
			LDT_ClearDot(0+cursor_x+2, 30);
			
			if(BTN_GetStatus(BTN_IDX_RIGHT))
			{
				if(tvg_idx < 59)
					tvg_idx += 1;
			}
			else if(BTN_GetStatus(BTN_IDX_LEFT))
			{
				if(tvg_idx > 0)
					tvg_idx -= 1;
			}
			
			tvg_dst = tvg_idx * 20;
			
			if(tvg_dst < tvg_dst_min)
			{
				tvg_dst = tvg_dst_min;
				tvg_idx = tvg_dst_min / 20;
			}
			else if(tvg_dst > tvg_dst_max)
			{
				tvg_dst = tvg_dst_max;
				tvg_idx = tvg_dst_max / 20;
			}

			tvg_gain = lMnEgn.aTvg[tvg_idx];
			cursor_x = (220 * (U32)tvg_dst) / (U32)empty;
			gDp_cursor_vert = cursor_x;

			BTN_DelayMsec(BTN_WAIT_UD);
		}

		// Measure Process
        if(gIsr_tm3_cnt >= 100)
        {
            gIsr_tm3_cnt = 0;
			gDp_flg_cs_vert = TRUE;

			MEAS_PrcEcho();

			DSP_DrawGraph(0, 30 + 2, 220, 85, gAd_data, 0, empty, 256, DP_IDX_GRAPH_4);

			APP_Num2Str(tvg_idx,  p_str, 3, 0, FALSE);
			APP_Num2Str(tvg_dst,  p_st2, 4, 2, FALSE);
			APP_Num2Str(tvg_gain, p_st3, 4, 0, FALSE);
			//APP_Num2Str(gAd_data[tvg_dst], p_st4, 3, 0, FALSE);
			
			DpSTR_DrawLin(FT_EN_08x16, FALSE,  29,  0, p_str);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 116,  0, p_st2);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 156,  0, "m");
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 203,  0, p_st3);
			//DpSTR_DrawLin(FT_EN_08x16, FALSE,  29, 18, p_st4);

			LDT_UpdBuff(LCD_Y_START, LCD_Y_END);

			gDp_flg_cs_vert = FALSE;
        }
    }
	while(_SBIT_BTN_SET != BTN_STT_PRESS && _SBIT_BTN_EXIT != BTN_STT_PRESS);

    if(_SBIT_BTN_EXIT == BTN_STT_PRESS)
    {
		for(i=0; i<60; i++)
			lMnEgn.aTvg[i] = tvg_buf[i];
	}
	if(_SBIT_BTN_SET == BTN_STT_PRESS)	FLS_WriteAll();	
	TMR3_Stop1sec();
}

void MnEgn_TvgCst(void)
{
	MnBOX_DispBox();	
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Enable?");
	
	BTN_DelayMsec(BTN_WAIT_UD);

	lMnEgn.fTvg_cst = MnBOX_SelArg4(MnSYS_LANG_ENG, lMnEgn.fTvg_cst, MnBOX_X11, "NO", MnBOX_X10, "YES", 0, "", 0, "", 2);

	if(lMnEgn.fTvg_cst && BTN_GetStatus(BTN_IDX_SET))
		EgnTvg_ProcFunc();
}



void MnEgn_TvgIdx(void)
{
    S16 old = lMnEgn.tvg_num;
    U08 cnt = 0;
	U08 pStr[APP_STR_LENGTH] = {0, };

    SFRPAGE = CONFIG_PAGE;

    MEAS_CfgSens(4);
    MEAS_PrcEcho();

    LDT_ClrBuff(LCD_X_START, LCD_Y_START, LCD_X_END, LCD_Y_END);

	APP_Num2Str(lMnEgn.tvg_num, pStr, 2, 0, FALSE);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 5, 0, "TVG Curve:");
	DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, "           ");
	
	if	(MnEGN_GetFTvgCst()) 	DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, "Custom");
	else 						DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, pStr);

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
			 #if 1
			 if(MnEGN_GetFTvgCst())
				 DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, "Custom");
			 else 
			 {
				 lMnEgn.tvg_num = BTN_GetValue_UpDn(lMnEgn.tvg_num, MnEGN_TVG_I00_IDLE, MnEGN_TVG_IDX_MAX-1, 1, 0);
				 APP_Num2Str(lMnEgn.tvg_num, pStr, 2, 0, FALSE);
				 
				 DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, "		  ");
				 DpSTR_LineUpd(FT_EN_08x16, FALSE, 120, 0, pStr);
			 } 	 
			 #else
			 MsrDp_StrCfg(lst, cnt, feet);
			 #endif
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
	 	lMnEgn.tvg_num = old; 
	 }

	 TMR3_Stop1sec();

}

//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - Disp. List
//------------------------------------------------------------------------------------------------------------------------------
void EgnDp_ListP1(U08* pSel)
{
	U08 *pStr = "   ";
	U08 c  = MnEGN_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
													//   0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,	 MnEGN_L00_IDLE+1,		"          ENGINEER             ");
	MnLST_DispStr(ft, pSel[c++], MnEGN_L01_PROF_NUM+1,	"Profile No.                    ");
	MnLST_DispStr(ft, pSel[c++], MnEGN_L02_SMTH_NUM+1,	"Smooth Number                  ");
	MnLST_DispStr(ft, pSel[c++], MnEGN_L03_SMTH_RNG+1,	"Smooth Range                   ");
	MnLST_DispStr(ft, pSel[c++], MnEGN_L04_TRHD_REF+1,	"Threshold Range                ");
	MnLST_DispStr(ft, pSel[c++], MnEGN_L05_TRHD_MIN+1,	"Threshold MIN                  ");
	MnLST_DispStr(ft, pSel[c++], MnEGN_L06_TVG_IDX+1,	"TVG Curve                      ");
	MnLST_DispStr(ft, pSel[c++], MnEGN_L07_TVG_CST+1,	"TVG Custom                     ");

	if	(lMnEgn.fTvg_cst)	pStr = "ON";
	else					pStr = "OFF";

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnEGN_L07_TVG_CST], 180, MnSTR_Y_HEIGHT*8,pStr);
}

void MnEngDp_List(U08 sel)
{
	U08 i;
	U08 page = MNU_PAGE_01;
	U08 font = FT_EN_08x16;
	U08 pSel[MnEGN_LST_MAX] = {0, };

	pSel[sel] = TRUE;

	if		(sel > (MNU_LIST_NUM*2))		page = MNU_PAGE_03;
	else if (sel > (MNU_LIST_NUM*1))		page = MNU_PAGE_02;
	else									page = MNU_PAGE_01;

	if		(sel > (MNU_LIST_NUM*2))		pSel[sel-(MNU_LIST_NUM*2)] = TRUE;
	else if	(sel > (MNU_LIST_NUM*1))		pSel[sel-(MNU_LIST_NUM*1)] = TRUE;
	else									pSel[sel-(MNU_LIST_NUM*0)] = TRUE;

	if(page > MNU_PAGE_01)
		return;

	EgnDp_ListP1(pSel);

	for(i=(MnEGN_LST_MAX+1); i<=(MNU_LIST_NUM+1); i++)
		MnLST_DispClr(i);

    LDT_UpdBuff(LCD_Y_START,LCD_Y_END);
	DLY_RunMsec(100);
}

void MnEgnCk_Vari(void)
{	
	if(lMnEgn.prof_num < MnEGN_PROF_NUM_MIN)		lMnEgn.prof_num = MnEGN_PROF_NUM_DFT;
	if(lMnEgn.prof_num > MnEGN_PROF_NUM_MAX)		lMnEgn.prof_num = MnEGN_PROF_NUM_DFT;
	if(lMnEgn.smth_cnt > MnEGN_SMTH_CNT_MAX)		lMnEgn.smth_cnt = MnEGN_SMTH_CNT_DFT;
	if(lMnEgn.smth_rng < MnEGN_SMTH_RNG_MIN)		lMnEgn.smth_rng = MnEGN_SMTH_RNG_DFT;
	if(lMnEgn.smth_rng > MnEGN_SMTH_RNG_MAX)		lMnEgn.smth_rng = MnEGN_SMTH_RNG_DFT;
	if(lMnEgn.trhd_rng > MnEGN_TRHD_RNG_MAX)		lMnEgn.trhd_rng = MnEGN_TRHD_RNG_DFT;
	if(lMnEgn.trhd_min > MnEGN_TRHD_MIN_MAX)		lMnEgn.trhd_min = MnEGN_TRHD_MIN_DFT;
	if(lMnEgn.tvg_num  >= MnEGN_TVG_IDX_MAX)		lMnEgn.tvg_num	= MnEGN_TVG_I00_IDLE;	
	if(lMnEgn.fTvg_cst > TRUE)						lMnEgn.fTvg_cst = FALSE;	

	// Not Configuration
	if(lMnEgn.fDz_mode > TRUE)		lMnEgn.fDz_mode = FALSE;
	if(lMnEgn.fTh_mode > TRUE)		lMnEgn.fTh_mode = FALSE;
	if(lMnEgn.fTw_mode > TRUE)		lMnEgn.fTw_mode = FALSE;
	if(lMnEgn.trhd_gab > 255)		lMnEgn.trhd_gab = 22;
	if(lMnEgn.trhd_low > 240)		lMnEgn.trhd_low = 0;
	if(lMnEgn.th_st_rg > 90)		lMnEgn.th_st_rg = 0;
	if(lMnEgn.th_tw_rg > 90)		lMnEgn.th_tw_rg = 15;
	if(lMnEgn.fTvg_load > 5)		lMnEgn.fTvg_load = 0;
}

//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Access Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Get
U08 MnEGN_GetAdcNumb(void)			{	return lMnEgn.prof_num;		}
U08 MnEGN_GetSmthCnt(void)			{	return lMnEgn.smth_cnt;		}
U08 MnEGN_GetSmthRng(void)			{	return lMnEgn.smth_rng;		}
U08 MnEGN_GetTrhdRng(void)			{	return lMnEgn.trhd_rng;		}
U08 MnEGN_GetTrhdMin(void)			{	return lMnEgn.trhd_min;		}
U08 MnEGN_GetTvgNumb(void)		    {	return lMnEgn.tvg_num;		}
U16 MnEGN_GetCstTvg(U08 idx)	    {	return lMnEgn.aTvg[idx];	}
U08 MnEGN_GetFTvgCst(void)		    {	return lMnEgn.fTvg_cst;		}
// Not Configuration
U08 MnEGN_GetFgDzMod(void)			{	return lMnEgn.fDz_mode;		}
U08 MnEGN_GetFgThMod(void)			{	return lMnEgn.fTh_mode;		}
U08 MnEGN_GetFgTwMod(void)			{	return lMnEgn.fTw_mode;		}
U16 MnEGN_GetTrhdGab(void)			{	return lMnEgn.trhd_gab;		}
U08 MnEGN_GetTrhdLow(void)			{	return lMnEgn.trhd_low;		}
U08 MnEGN_GetThStRng(void)			{	return lMnEgn.th_st_rg;		}
U08 MnEGN_GetThTwRng(void)			{	return lMnEgn.th_tw_rg;		}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Memory Access
//------------------------------------------------------------------------------------------------------------------------------
void MnEGN_MemLoad(void)
{
	U08 i=0;

	lMnEgn.prof_num = FLS_Rd1Byte(F087_EGN_PROF_NUM);
	lMnEgn.smth_cnt = FLS_Rd1Byte(F080_EGN_SMTH_CNT);
	lMnEgn.smth_rng = FLS_Rd1Byte(F081_EGN_SMTH_RNG);
	lMnEgn.trhd_rng = FLS_Rd1Byte(F085_EGN_TRHD_RNG);
	lMnEgn.trhd_min = FLS_Rd1Byte(F092_EGN_TRHD_MIN);
	lMnEgn.fTvg_cst = FLS_Rd1Byte(F130_EGN_TVG_CST);
	lMnEgn.tvg_num  = FLS_Rd1Byte(F043_EGN_TVG_NUM);
	
	for(i=0; i<59; i++)
		 lMnEgn.aTvg[i] = FLS_Rd2Byte(F131_EGN_TVG_CST_START+(i*2));

	// Not Configuration
	lMnEgn.fDz_mode = FLS_Rd1Byte(F079_EGN_F_DZ_MODE);
	lMnEgn.fTh_mode = FLS_Rd1Byte(F083_EGN_F_TH_MODE);
	lMnEgn.fTw_mode = FLS_Rd1Byte(F088_EGN_F_TW_MODE);
	lMnEgn.trhd_gab = FLS_Rd2Byte(F077_EGN_TH_GAB_L);
	lMnEgn.trhd_low = FLS_Rd1Byte(F090_EGN_TRHD_LOW);
	lMnEgn.th_st_rg = FLS_Rd1Byte(F084_EGN_TH_ST_RNG);
	lMnEgn.th_tw_rg = FLS_Rd1Byte(F086_EGN_TH_TW_RNG);
}

void MnEGN_MemSave(void)
{
	U08 i = 0;

	FLS_Wr1Byte(F087_EGN_PROF_NUM, lMnEgn.prof_num);
	FLS_Wr1Byte(F080_EGN_SMTH_CNT, lMnEgn.smth_cnt);
	FLS_Wr1Byte(F081_EGN_SMTH_RNG, lMnEgn.smth_rng);
	FLS_Wr1Byte(F085_EGN_TRHD_RNG, lMnEgn.trhd_rng);
	FLS_Wr1Byte(F092_EGN_TRHD_MIN, lMnEgn.trhd_min);
	FLS_Wr1Byte(F130_EGN_TVG_CST,  lMnEgn.fTvg_cst);
	FLS_Wr1Byte(F043_EGN_TVG_NUM,  lMnEgn.tvg_num);
	
	for(i=0; i<59; i++)
		FLS_Wr2Byte(F131_EGN_TVG_CST_START+(i*2), lMnEgn.aTvg[i]);	//	gFl.trhd_mode

	// Not Configuration
	FLS_Wr1Byte(F079_EGN_F_DZ_MODE, lMnEgn.fDz_mode);
	FLS_Wr1Byte(F083_EGN_F_TH_MODE, lMnEgn.fTh_mode);
	FLS_Wr1Byte(F088_EGN_F_TW_MODE, lMnEgn.fTw_mode);
	FLS_Wr2Byte(F077_EGN_TH_GAB_L,  lMnEgn.trhd_gab);
	FLS_Wr1Byte(F084_EGN_TH_ST_RNG, lMnEgn.th_st_rg);
	FLS_Wr1Byte(F086_EGN_TH_TW_RNG, lMnEgn.th_tw_rg);
	FLS_Wr1Byte(F090_EGN_TRHD_LOW,  lMnEgn.trhd_low);
}

void MnEGN_MemFtry(void)
{
	U08 i=0;

	FLS_Wr1Byte(F087_EGN_PROF_NUM, MnEGN_PROF_NUM_DFT);
	FLS_Wr1Byte(F080_EGN_SMTH_CNT, MnEGN_SMTH_CNT_DFT);
	FLS_Wr1Byte(F081_EGN_SMTH_RNG, MnEGN_SMTH_RNG_DFT);
	FLS_Wr1Byte(F085_EGN_TRHD_RNG, MnEGN_TRHD_RNG_DFT);
	FLS_Wr1Byte(F092_EGN_TRHD_MIN, MnEGN_TRHD_MIN_DFT);
	FLS_Wr1Byte(F130_EGN_TVG_CST,  FALSE);
	FLS_Wr1Byte(F043_EGN_TVG_NUM,  MnEGN_TVG_I00_IDLE);
	for(i=0; i<59; i++)
		FLS_Wr2Byte(F131_EGN_TVG_CST_START+(i*2), lMnEgn_aTvg_idle[i]);	//	gFl.trhd_mode
	// Not Configuration
	FLS_Wr1Byte(F079_EGN_F_DZ_MODE, FALSE);
	FLS_Wr1Byte(F083_EGN_F_TH_MODE, FALSE);
	FLS_Wr1Byte(F088_EGN_F_TW_MODE, FALSE);
	FLS_Wr2Byte(F077_EGN_TH_GAB_L,  22);
	FLS_Wr1Byte(F084_EGN_TH_ST_RNG, 0);
	FLS_Wr1Byte(F086_EGN_TH_TW_RNG, 15);
	FLS_Wr1Byte(F090_EGN_TRHD_LOW,  0);
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MnEGN_InitVari(void)
{
	MnEGN_MemLoad();
	MnEgnCk_Vari();

	lMnEgn.fTvg_load = 0;
}

void MnEGN_ProcMain(void)
{
	U32 cnt = 0;
	U08 fDp = FALSE;
	U08 sel = MnEGN_L00_IDLE + 1;

	MnEngDp_List(sel);

	do
	{
        if(!_SBIT_BTN_DOWN)
        {
        	if(sel < MnEGN_LST_MAX-1)		sel++;
        	else                            sel = MnEGN_L00_IDLE+1;
			fDp = TRUE;
        }

        if(!_SBIT_BTN_UP)
        {
        	if(sel > MnEGN_L00_IDLE+1)		sel--;
        	else                            sel = MnEGN_LST_MAX-1;
			fDp = TRUE;
        }
		
		if(!_SBIT_BTN_RIGHT && !_SBIT_BTN_LEFT)
		{
			if(++cnt > 800000)
			{
				MnTST_ProcMain();
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
				case MnEGN_L01_PROF_NUM:	MnEgn_ProfNum();       	break;
				case MnEGN_L02_SMTH_NUM:	MnEgn_SmthNum();    	break;
				case MnEGN_L03_SMTH_RNG:	MnEgn_SmthRng();    	break;
				case MnEGN_L04_TRHD_REF:	MnEgn_TrhdRng();    	break;
				case MnEGN_L05_TRHD_MIN:	MnEgn_TrhdMin();      	break;
				case MnEGN_L06_TVG_IDX:		MnEgn_TvgIdx();			break;
				case MnEGN_L07_TVG_CST:		MnEgn_TvgCst();			break;
				// Page 2
			}
			fDp = TRUE;
	    }

		if(fDp)
		{
			MnEngDp_List(sel);
			fDp = FALSE;
		}
	}
	while(_SBIT_BTN_EXIT);
}

//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

