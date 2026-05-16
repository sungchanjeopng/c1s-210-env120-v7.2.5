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
#include "disp_main.h"
// bsp
#include "flash_main.h"
#include "mem_main.h"
#include "rtc_ds1302.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "adc_main.h"
// app
#include "font_def.h"
#include "app_main.h"
#include "rly_main.h"
#include "prtc_main.h"
// measure
#include "measure_main.h"
#include "measure_analyze.h"
#include "measure_temperature.h"
// menu
#include "menu_box.h"
#include "menu_system.h"
#include "menu_measure.h"
#include "menu_output.h"
#include "menu_quick.h"
#include "menu_test.h"
// disp
#include "disp_menu.h"
#include "disp_string.h"
// screen
#include "screen_main.h"
// font
#include "font_idx_kor.h"
//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------
GV_DP gDp;

U08 gDp_buff_grp[220];      // OLD: unsigned char Buffer_Graph_UC_G[220];
U08 gDp_buff_str[16];       // OLD: unsigned char Array_0_UC_G[16];
U08 gDp_flg_log;            // OLD: unsigned char Log_Flag_disp_UC_G;
U08 gDp_cursor_vert;        // OLD: unsigned char Position_Vert_cursor_UC_G;
S16 gDp_cursor_dist;
U08 gDp_flg_cs_vert;        // OLD: unsigned char Flag_Vert_cursor_UC_G;


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
//  Global APIs - with Local
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local Funtions
//--------------------------------------------------------------------------------------------------
void DspArg_Time(void)
{
    U08 *p_mm;
	U08 p_date[APP_STR_LENGTH];
	U08 p_year[APP_STR_LENGTH];
	U08 p_hour[APP_STR_LENGTH];
	U08 p_min [APP_STR_LENGTH];

    switch (gRt.mm)
    {
        case 1:     p_mm = "JAN";       break;
        case 2:     p_mm = "FEB";       break;
        case 3:     p_mm = "MAR";       break;
        case 4:     p_mm = "APR";       break;
        case 5:     p_mm = "MAY";       break;
        case 6:     p_mm = "JUN";       break;
        case 7:     p_mm = "JUL";       break;
        case 8:     p_mm = "AUG";       break;
        case 9:     p_mm = "SEP";       break;
        case 10:    p_mm = "OCT";       break;
        case 11:    p_mm = "NOV";       break;
        case 12:    p_mm = "DEC";       break;
    }

	APP_Num2Str(gRt.date, p_date, 2, 0, FALSE);
	APP_Num2Str(gRt.yy,   p_year, 2, 0, TRUE);
	APP_Num2Str(gRt.hour, p_hour, 2, 0, TRUE);
	APP_Num2Str(gRt.min,  p_min,  2, 0, TRUE);

    DpSTR_DrawLin(FT_EN_08x16, 0,  43, 0, p_mm);
    DpSTR_DrawLin(FT_EN_08x16, 0,  79, 0, p_date);

    DpSTR_DrawLin(FT_EN_08x16, 0, 106, 0, "20");
    DpSTR_DrawLin(FT_EN_08x16, 0, 124, 0, p_year);

    DpSTR_DrawLin(FT_EN_08x16, 0, 151, 0, p_hour);
    DpSTR_DrawLin(FT_EN_08x16, 0, 169, 0, ":");
    DpSTR_DrawLin(FT_EN_08x16, 0, 178, 0, p_min);
}

void DpFRT_ProcMain(void)
{
	int i;
	S16 tprt;
	U08 p_str0[APP_STR_LENGTH];
	U08 p_str1[APP_STR_LENGTH];
	U08 unit = MnMSR_GetUnit();
	U08 oper = MnMSR_GetOper();
	U08 prtc = MnOUT_GetProtocol();
	U08 mdb_addr = MnOUT_GetMdbAddr();
	S16 level_mt = MsANL_GetLevelMt();
	S16 level_ft = MsANL_GetLevelFt();
	S16 dista_mt = MsANL_GetDistaMt();
	S16 dista_ft = MsANL_GetDistaFt();
	U08 fEco_dtt = MsANL_GetFgEcoDtt();
	U08 fEco_trk = MsANL_GetFgEcoTrk();

	DspArg_Time();

	// Protocol
	switch(prtc)
	{
		case MnOUT_PROT_RF:
			APP_Num2Str(MnOUT_GetRfCh(), p_str0, 2, 0, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, 0, 10, 68, "RF");
			DpSTR_DrawLin(FT_EN_08x16, 0, 35, 68, p_str0);
			break;

		case MnOUT_PROT_MDBS:
			APP_Num2Str(mdb_addr, p_str0, 3, 0, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, 0, 10, 68, "MBUS");
			DpSTR_DrawLin(FT_EN_08x16, 0, 50, 68, p_str0);
			break;

		case MnOUT_PROT_STD:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 10, 68, "");
			break;
	}

	// Operation
	switch(oper)
	{
		case MnMSR_OPER_LEVEL:
			DpSTR_DrawLin(FT_EN_08x16, 0, 10, 86, "D:");		// Distance
			
			switch(unit)
			{
				case MnMSR_UNIT_FEET:	APP_Num2Str(level_ft, p_str0, 3, 1, 0);
										APP_Num2Str(dista_ft, p_str1, 3, 1, 0);		break;
				case MnMSR_UNIT_METER:
				default:				APP_Num2Str(level_mt, p_str0, 4, 2, 0);
										APP_Num2Str(dista_mt, p_str1, 4, 2, 0);		break;
			}

			DpSTR_DrawLin(FT_EN_26x47, 0, 30, 20, p_str0);
			DpSTR_DrawLin(FT_EN_08x16, 0, 23, 86, p_str1);
			break;

		case MnMSR_OPER_DISTA:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 10, 86, "S:");		// Sludge

			switch(unit)
			{
				case MnMSR_UNIT_FEET:	APP_Num2Str(dista_ft, p_str0, 3, 1, 0);
										APP_Num2Str(level_ft, p_str1, 3, 1, 0);		break;
				case MnMSR_UNIT_METER:
				default:				APP_Num2Str(dista_mt, p_str0, 4, 2, 0);
										APP_Num2Str(level_mt, p_str1, 4, 2, 0);		break;
			}

			DpSTR_DrawLin(FT_EN_26x47, 0, 30, 20, p_str0);
			DpSTR_DrawLin(FT_EN_08x16, 0, 23, 86, p_str1);
			break;
	}

	switch(unit)
	{
		case MnMSR_UNIT_FEET: 	DpSTR_DrawLin(FT_EN_26x47, 0, 180, 20, ":<");		// "ft"
								DpSTR_DrawLin(FT_EN_08x16, 0,  60, 86, "ft");		break;
		case MnMSR_UNIT_METER:
		default:				DpSTR_DrawLin(FT_EN_26x47, 0, 180, 20, ";" );		// "m"
								DpSTR_DrawLin(FT_EN_08x16, 0,  67, 86, "m ");		break;
	}

	// Temprature
	tprt = gTp_now;
    
	if((tprt > -250) && (tprt < 1000))
    {
		gDp.f_err_tprt = FALSE;

		if (tprt < 0)
        {
			tprt = -tprt;
			DpSTR_DrawLin(FT_EN_08x16, 0, 149, 68, "-");
		}
        else
			DpSTR_DrawLin(FT_EN_08x16, 0, 149, 68, "+");

		APP_Num2Str(tprt, p_str0, 4, 1, 0);
		DpSTR_DrawLin(FT_EN_08x16, 0, 158, 68, p_str0);
	}
    else
    {
		gDp.f_err_tprt = TRUE;
		DpSTR_DrawLin(FT_EN_08x16, 0, 158, 68, "  X.X");
	}

	
	if(unit == MnMSR_UNIT_FEET)		DpSTR_DrawLin(FT_EN_08x16, 0, 212, 68, "'F");
	else							DpSTR_DrawLin(FT_EN_08x16, 0, 212, 68, "'C");

	if(MnTST_GetGloval_fCnt())
	{
		APP_NumTest2Str(test_global_cnt, p_str0, 10, 0, FALSE);
		DpSTR_DrawLin(FT_EN_08x16, 0, 80, 86,p_str0);
	}
	
	// Current
	for(i=0; i<APP_STR_LENGTH; i++)		p_str0[i] = 0;
	APP_Num2Str(DSP_GetCurrent(), p_str0, 4, 2, FALSE);

	DpSTR_DrawLin(FT_EN_08x16, 0, 167, 86, p_str0);
	DpSTR_DrawLin(FT_EN_08x16, 0, 212, 86, "mA");

	LCD_DataLine_Horz(104, 0, 239);

	switch(MsANL_GetErrStat())
	{
		case MsANL_ERR_ECHO:
			DpSTR_DrawLin(FT_EN_08x16, 0, 20, 110, "ER01 : Loss of Echo");
			break;
		case MsANL_ERR_TPRT:
			DpSTR_DrawLin(FT_EN_08x16, 0, 20, 110, "ER02 : Temp. Error");
			break;

		case MsANL_ERR_NONE:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 21, 110, "R1:");
			if(gRly_state_01)		DpSTR_DrawLin(FT_EN_08x16, 0, 48, 110, " ON");
			else					DpSTR_DrawLin(FT_EN_08x16, 0, 48, 110, "OFF");
			
			DpSTR_DrawLin(FT_EN_08x16, 0, 102, 110, "R2:");
			if(gRly_state_02)		DpSTR_DrawLin(FT_EN_08x16, 0, 129, 110, " ON");
			else					DpSTR_DrawLin(FT_EN_08x16, 0, 129, 110, "OFF");
			
			DpSTR_DrawLin(FT_EN_08x16, 0, 183, 110, "ST");
			if	   (!fEco_dtt && !fEco_trk) 	DpSTR_DrawLin(FT_EN_08x16, 0, 201, 110, "00");
			else if( fEco_dtt && !fEco_trk) 	DpSTR_DrawLin(FT_EN_08x16, 0, 201, 110, "01");
			else if( fEco_dtt &&  fEco_trk) 	DpSTR_DrawLin(FT_EN_08x16, 0, 201, 110, "02");
			break;
	}
}

void DspDrw_AxisY(U16 x_start, U16 y_start, U16 x_width, U16 y_height, U08 grp_type)
{
    U08 i;
    U16 x_wdt, y_hei;       // width, height
    U16 marker;             // OLD: U16 marking_position_UI_L;
    F32 y_ratio, interval, idx_aux;
    U16 Y_dot1_UI_L, Y_dot2_UI_L;

    x_wdt = x_start + x_width;
    y_hei = y_start + y_height;

    marker = x_start;
    idx_aux = marker;
    interval = x_width / 11.0;

    LCD_DataLine_Horz(y_hei, x_start, x_wdt);
    LDT_LineVert(x_wdt, y_start, y_hei, LDT_LINE_DATA);

#if 1
	switch(grp_type)
	{
		case DP_IDX_GRAPH_1:
		case DP_IDX_GRAPH_2:
		case DP_IDX_GRAPH_4:
			LCD_DataDot_LineVert(x_wdt - interval, y_start, y_hei - 1);
			LCD_DataDot_LineVert(x_wdt - interval + 1, y_start, y_hei - 1);
			
			for(i = 0; i < 11; i++)
			{
				LDT_ClearDot(marker, y_hei);
				LCD_DataDot_Buff(marker, y_hei + 1);
			
				idx_aux += interval;
				marker = idx_aux;
			}
			
			y_ratio = y_height / 240.0;
			Y_dot1_UI_L = y_hei - (200.0 * y_ratio);
			Y_dot2_UI_L = y_hei - (100.0 * y_ratio);
			
			LCD_DataDot_Buff(x_wdt + 1, Y_dot1_UI_L);
			LCD_DataDot_Buff(x_wdt + 1, Y_dot2_UI_L);
			
			if(gDp_flg_cs_vert == TRUE)                                                                                                                                                          
			{
				LCD_DataDot_LineVert(x_start + gDp_cursor_vert, y_start, y_hei);

				if(x_start + gDp_cursor_vert > 2)	LCD_DataDot_Buff(x_start + gDp_cursor_vert - 2, y_start - 2);
				if(x_start + gDp_cursor_vert > 1)	LCD_DataDot_Buff(x_start + gDp_cursor_vert - 1, y_start - 1);
				LCD_DataDot_Buff(x_start + gDp_cursor_vert + 1, y_start - 1);
				LCD_DataDot_Buff(x_start + gDp_cursor_vert + 2, y_start - 2);
			}
			break;
		case DP_IDX_GRAPH_0:
		case DP_IDX_GRAPH_3:
		default:
			break;
	}
#else
    if(grp_type == 1 || grp_type == 2)
    {
        LCD_DataDot_LineVert(x_wdt - interval, y_start, y_hei - 1);
        LCD_DataDot_LineVert(x_wdt - interval + 1, y_start, y_hei - 1);

        for(i = 0; i < 11; i++)
        {
            LDT_ClearDot(marker, y_hei);
            LCD_DataDot_Buff(marker, y_hei + 1);

            idx_aux += interval;
            marker = idx_aux;
        }

        y_ratio = y_height / 240.0;
        Y_dot1_UI_L = y_hei - (200.0 * y_ratio);
        Y_dot2_UI_L = y_hei - (100.0 * y_ratio);

        LCD_DataDot_Buff(x_wdt + 1, Y_dot1_UI_L);
        LCD_DataDot_Buff(x_wdt + 1, Y_dot2_UI_L);

        if(gDp_flg_cs_vert == TRUE)
        {
            LCD_DataDot_LineVert(x_start + gDp_cursor_vert, y_start, y_hei);
            LCD_DataDot_Buff(x_start + gDp_cursor_vert - 2, y_start - 2);
            LCD_DataDot_Buff(x_start + gDp_cursor_vert - 1, y_start - 1);
            LCD_DataDot_Buff(x_start + gDp_cursor_vert + 1, y_start - 1);
            LCD_DataDot_Buff(x_start + gDp_cursor_vert + 2, y_start - 2);
        }
    }
#endif
}

// OLD: void Disp_Echo_graph(unsigned char *ptr_array_UC_L)
void DspDrw_Echo(U08 *p_array)
{
    U16 aux0, dist_0, dist_1;
	U16 empty = MnMSR_GetEmpty();
	U16 dead  = MnMSR_GetDead();
	U08 fTest = MnMSR_GetFgTest();
	U16 ms_empty = MsANL_GetEmpty();
	S16 dist_1st = MsANL_GetDist1st();
	S16 dist_mod = MsANL_GetDistMod();

    if(!MEAS_GetFgAdc())		dist_0 = dist_1st;
    else						dist_0 = dist_mod;

    if(fTest)   	dist_1 = dist_1st;
    else			dist_1 = dist_0;

    if(dist_1 < dead)			dist_1 = dead;
    if(dist_1 > empty)			dist_1 = empty;

    aux0 = (220 * (U32)dist_1) / (U32)ms_empty;
    gDp_cursor_vert = aux0;

    gDp_flg_cs_vert = 1;

	switch(SCRN_GetScrnIdx())
	{
		case SCR_I02_GRP_LIVE:		aux0 = 220;				break;
		case SCR_I03_GRP_AVRG:		aux0 = ms_empty;		break;
	}

    DSP_DrawGraph(0, 16 + 2, 220, 85, p_array, 0, aux0, 256, DP_IDX_GRAPH_2);
    gDp_flg_cs_vert = 0;

    DpSTR_DrawLin(FT_EN_06x07, 0, 195, 16 + 2 + 85 + 2, "Empty");
}

// OLD: void Disp_Echo_Common(void)
void DspScr_EchoComm(void)
{
	int i;
	U08 pos_x1, pos_x2;
	U08 p_str[APP_STR_LENGTH] = {0,};
	U08 p_st2[APP_STR_LENGTH] = {0,};
	U08 lang = MnSYS_GetLang();
	U08 loca = MnSYS_GetLocFt();
	U08 unit = MnMSR_GetUnit();
	F32 calc = (MnMSR_GetEmpty() * MSR_CALC_M2F_CONST) + 0.5;
	S16 feet = calc;

	switch(lang)
	{
#if 1
		case MnSYS_LANG_LOC:
			switch(loca)
			{
				case MnSYS_LOC_CHN:
					FNT_SetStr(76, 117, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					DpSTR_DrawLin(FT_EN_08x16, FALSE, 40, 0, ":");
					break;

				case MnSYS_LOC_JPN:
					FNT_SetStr(77, 84, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					DpSTR_DrawLin(FT_EN_08x16, FALSE, 60, 0, ":");
					break;

				case MnSYS_LOC_KOR:
				default:
					FNT_SetStr(94, 85, 1, 39, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					break;
			}
			LCD_WrStrBuff(5, 0, FT_LC_16x16, gFt_str, 0, 0);

			break;

#else
		case MnSYS_LANG_KOR:
			FNT_SetStr(94, 85, 1, 39, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			LCD_WrStrBuff(5, 0, FT_LC_16x16, gFt_str, 0, 0);
			break;

		case MnSYS_LANG_CHN:
			FNT_SetStr(76, 117, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			LCD_WrStrBuff(5, 0, FT_LC_16x16, gFt_str, 0, 0);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 40, 0, ":");
			break;

		case MnSYS_LANG_JPN:
			FNT_SetStr(94, 156, 96, 154, 157, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			LCD_WrStrBuff(5, 0, FT_LC_16x16, gFt_str, 0, 0);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 69, 0, ":");
			break;

#endif
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 5, 0, "Empty:");
			break;
	}

	switch(unit)
	{
		case MnMSR_UNIT_FEET:
			APP_Num2Str(feet, p_str, 3, 1, FALSE);

			switch(lang)
			{
#if 1
				case MnSYS_LANG_LOC:
					switch(loca)
					{
						case MnSYS_LOC_JPN:		pos_x1 = 78;	pos_x2 = 110;		break;
						case MnSYS_LOC_CHN:
						case MnSYS_LOC_KOR:
						default:				pos_x1 = 80;	pos_x2 = 119;		break;
					}
					break;

#else
				case MnSYS_LANG_KOR:	pos_x1 = 80;	pos_x2 = 119;	break;
				case MnSYS_LANG_JPN:	pos_x1 = 78;	pos_x2 = 110;	break;

#endif
				case MnSYS_LANG_ENG:
				default:
					pos_x1 = 59;	pos_x2 = 95;
					break;
			}

			DpSTR_DrawLin(FT_EN_08x16, FALSE, pos_x1, 0, p_str);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, pos_x2, 0, "ft");
			break;

		case MnMSR_UNIT_METER:
		default:
			APP_Num2Str(MnMSR_GetEmpty(), p_str, 4, 2, FALSE);

			switch(lang)
			{
#if 1
				case MnSYS_LANG_LOC:
					switch(loca)
					{
						case MnSYS_LOC_JPN:		pos_x1 = 78;	pos_x2 = 120;		break;
						case MnSYS_LOC_CHN:
						case MnSYS_LOC_KOR:
						default:				pos_x1 = 80;	pos_x2 = 127;		break;
					}
					break;

#else
				case MnSYS_LANG_KOR:	pos_x1 = 80;	pos_x2 = 127;	break;
				case MnSYS_LANG_JPN:	pos_x1 = 78;	pos_x2 = 120;	break;
#endif
				case MnSYS_LANG_ENG:
				default:				pos_x1 = 59;	pos_x2 = 104;	break;
			}
			DpSTR_DrawLin(FT_EN_08x16, FALSE, pos_x1, 0, p_str);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, pos_x2, 0, "m");
			break;
	}

	for(i=0; i<APP_STR_LENGTH; i++)
		p_str[i] = 0;

	if(MnMSR_GetTrhdMod() == MnMSR_TRHD_MOD_AUTO)
	{
		APP_Num2Str(MnMSR_GetTrhdPer(), p_str, 2, 0, FALSE);

		pos_x1 = 171;
		pos_x2 = 207;

		DpSTR_DrawLin(FT_EN_08x16, FALSE, pos_x1, 0, "Thr.");
		DpSTR_DrawLin(FT_EN_08x16, FALSE, pos_x2, 0, p_str);
		DpSTR_DrawLin(FT_EN_08x16, FALSE, 225, 0, "%");
	}
	else
	{
		APP_Num2Str(MnMSR_GetTrhdFix(), p_str, 4, 2, FALSE);

		pos_x1 = 162;
		pos_x2 = 190;

		DpSTR_DrawLin(FT_EN_08x16, FALSE, pos_x1, 0, "Thr.");
		DpSTR_DrawLin(FT_EN_08x16, FALSE, pos_x2, 0, p_str);
	}

	for(i=0; i<APP_STR_LENGTH; i++)
	{
		p_str[i] = 0;
		p_st2[i] = 0;
	}

	DpSTR_DrawLin(FT_EN_08x16, FALSE, 5, 112, "D");

	switch(unit)
	{
		case MnMSR_UNIT_FEET:
			APP_Num2Str(MsANL_GetDistaFt(), p_str, 3, 1, FALSE);
			APP_Num2Str(MsANL_GetLevelFt(), p_st2, 3, 1, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, FALSE,  16, 112, p_str);
			DpSTR_DrawLin(FT_EN_08x16, FALSE,  54, 112, "ft");
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 163, 112, "S");
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 174, 112, p_st2);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 212, 112, "ft");
			break;

		case MnMSR_UNIT_METER:
		default:
			APP_Num2Str(MsANL_GetDistaMt(), p_str, 4, 2, FALSE);
			APP_Num2Str(MsANL_GetLevelMt(), p_st2, 4, 2, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, FALSE,  16, 112, p_str);
			DpSTR_DrawLin(FT_EN_08x16, FALSE,  62, 112, "m");
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 163, 112, "S");
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 174, 112, p_st2);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 220, 112, "m");
			break;
	}
}

void DpGRP_ProcMain(U08 idx)
{
	U08 lang = MnSYS_GetLang();
	U08 loca = MnSYS_GetLocFt();

	switch(idx)
	{
		case SCR_I02_GRP_LIVE:
#if 1
			switch(lang)
			{
				case MnSYS_LANG_LOC:
					#if defined (_FT_LANG_KR)
					FNT_SetStr(114, 57, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					LCD_WrStrBuff(95, 111, FT_LC_16x16, gFt_str, 0, 0);
					break;
					#elif defined (_FT_LANG_CN)
					FNT_SetStr(132, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					LCD_WrStrBuff(104, 111, FT_LC_16x16, gFt_str, 0, 0);
					break;
					#elif defined (_FT_LANG_JP)
					#if 0
					FNT_SetStr(36, 3, 37, 17, 92, 31, 1, 1, 1, 1, 1, 1, 1, 1, 1);
					LCD_WrStrBuff(60, 111, FT_LC_16x16, gFt_str, 0, 0);	
					#endif
					DpSTR_DrawLin(FT_EN_08x16, 0, 102, 112, "Real");
					break;
					#endif
				case MnSYS_LANG_ENG:
				default:
					DpSTR_DrawLin(FT_EN_08x16, 0, 102, 112, "Real");
					break;
			}
#else
			if(lang == MnSYS_LANG_CHN)
			{
				FNT_SetStr(132, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				LCD_WrStrBuff(104, 111, FT_LC_16x16, gFt_str, 0, 0);
			}
			else
				DpSTR_DrawLin(FT_EN_08x16, 0, 102, 112, "Real");
#endif
			DspDrw_Echo(gDp_buff_grp);
			break;

		case SCR_I03_GRP_AVRG:
#if 1
			switch(lang)
			{
				case MnSYS_LANG_LOC:
					#if defined (_FT_LANG_KR)
					FNT_SetStr(117, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					LCD_WrStrBuff(102, 111, FT_LC_16x16, gFt_str, 0, 0);
					break;
					#elif defined (_FT_LANG_CN)
					FNT_SetStr(163, 164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					LCD_WrStrBuff(104, 111, FT_LC_16x16, gFt_str, 0, 0);
					break;
					#elif defined (_FT_LANG_JP)
					#if 0
					FNT_SetStr(93, 94, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
					LCD_WrStrBuff(104, 111, FT_LC_16x16, gFt_str, 0, 0);
					#endif
					DpSTR_DrawLin(FT_EN_08x16, 0, 102, 112, "Avg.");
					break;					
					#endif
				case MnSYS_LANG_ENG:
				default:
					DpSTR_DrawLin(FT_EN_08x16, 0, 102, 112, "Avg.");
					break;
			}
#else
			if(lang == MnSYS_LANG_CHN)
			{
				FNT_SetStr(163, 164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				LCD_WrStrBuff(104, 111, FT_LC_16x16, gFt_str, 0, 0);
			}
			else
				DpSTR_DrawLin(FT_EN_08x16, 0, 102, 112, "Avg.");
#endif

			DspDrw_Echo(gAd_data);
			break;

		default:
			return;
	}

	DspScr_EchoComm();
}

void DspInfo_Init(void)
{
    DpSTR_DrawLin(FT_EN_08x16, 0, 103,   0, ":");
    DpSTR_DrawLin(FT_EN_08x16, 0, 103,  16, ":");
    DpSTR_DrawLin(FT_EN_08x16, 0, 103,  32, ":");
    DpSTR_DrawLin(FT_EN_08x16, 0, 103,  48, ":");
    DpSTR_DrawLin(FT_EN_08x16, 0, 103,  64, ":");
    DpSTR_DrawLin(FT_EN_08x16, 0, 103,  80, ":");
    DpSTR_DrawLin(FT_EN_08x16, 0, 103,  96, ":");
    DpSTR_DrawLin(FT_EN_08x16, 0, 103, 112, ":");
}

void DspInfo_Empty(void)
{
	S16 temp = 0;
	U16 empty = MnMSR_GetEmpty();
	U08 p_str[APP_STR_LENGTH] = {0,};

#if 0
	DpSTR_DrawLin(FT_EN_08x16, 0, 5, 0, "Empty");

#else
	U08 lang = MnSYS_GetLang();
	switch(lang)
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			FNT_SetStr(94, 85, 1, 39, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 측정 범위
			LCD_WrStrBuff(5, 0, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_CN)
			FNT_SetStr(76, 117, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// 측정 범위
			LCD_WrStrBuff(5, 0, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_JP)	
			FNT_SetStr(77, 84, 89, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); //
			LCD_WrStrBuff(5, 0, FT_LC_16x16, gFt_str, 0, 0);
			break;
#endif
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 0, "Empty");
			break;
	}
#endif

	switch(MnMSR_GetUnit())
	{
		case MnMSR_UNIT_FEET:
			temp = (empty * MSR_CALC_M2F_CONST) + 0.5;

			APP_Num2Str(temp, p_str, 3, 1, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, 0, 119, 0, p_str);
			DpSTR_DrawLin(FT_EN_08x16, 0, 164, 0, "ft");
			break;

		case MnMSR_UNIT_METER:
		default:
			APP_Num2Str(empty, p_str, 4, 2, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, 0, 119, 0, p_str);
			DpSTR_DrawLin(FT_EN_08x16, 0, 164, 0, "m");
			break;
	}
}

void DspInfo_DeadZone(void)
{
	S16 temp = 0;
	U16 dead = MnMSR_GetDead();
	U08 p_str[APP_STR_LENGTH] = {0,};

#if 0
	DpSTR_DrawLin(FT_EN_08x16, 0, 5, 16, "Dead Zone");
#else
	U08 lang = MnSYS_GetLang();

	switch(lang)
	{	
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			FNT_SetStr(FKR_BUL,	 FKR_GAM,	FKR_DAE,  0,0,0,0,0,0,0,0,0,0,0,0);	// 불감지 거리
			LCD_WrStrBuff(5, 16, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_CN)
			FNT_SetStr(70, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// 불감지 거리
			LCD_WrStrBuff(5, 16, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_JP)
			FNT_SetStr(43, 66, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 불감지 거리
			LCD_WrStrBuff(5, 16, FT_LC_16x16, gFt_str, 0, 0);
			break;
#endif
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 16, "Dead Zone");
			break;
	}
#endif

	switch(MnMSR_GetUnit())
	{
		case MnMSR_UNIT_FEET:
			temp = (dead * MSR_CALC_M2F_CONST) + 0.5;
			APP_Num2Str(temp, p_str, 3, 1, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, FALSE, 119, 16, p_str);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 164, 16, "ft");
			break;

		case MnMSR_UNIT_METER:
		default:
			APP_Num2Str(dead, p_str, 4, 2, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, FALSE, 119, 16, p_str);
			DpSTR_DrawLin(FT_EN_08x16, FALSE, 164, 16, "m");
			break;
	}
}

void DspInfo_CurrOut(void)
{
	S16 temp = 0;
	U08 p_str[APP_STR_LENGTH] = {0,};
	U16 set04mA = MnOUT_GetSet04mA();
	U16 set20mA = MnOUT_GetSet20mA();

	// Title
	DpSTR_DrawLin(FT_EN_08x16, 0,  5, 32, "4mA");
	DpSTR_DrawLin(FT_EN_08x16, 0, 41, 32, "/");
	DpSTR_DrawLin(FT_EN_08x16, 0, 59, 32, "20mA");

#if 1
	switch(MnMSR_GetUnit())
	{
		case MnMSR_UNIT_FEET:
			temp = (set04mA * MSR_CALC_M2F_CONST) + 0.5;
			APP_Num2Str(temp, p_str, 3, 1, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, 0, 118, 32, p_str);
			DpSTR_DrawLin(FT_EN_08x16, 0, 156, 32, "ft");
			DpSTR_DrawLin(FT_EN_08x16, 0, 173, 32, "/");

			temp = (set20mA * MSR_CALC_M2F_CONST) + 0.5;
			APP_Num2Str(temp, p_str, 3, 1, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, 0, 185, 32, p_str);
			DpSTR_DrawLin(FT_EN_08x16, 0, 221, 32, "ft");
			break;

		case MnMSR_UNIT_METER:
		default:
			APP_Num2Str(set04mA, p_str, 4, 2, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, 0, 119, 32, p_str);
			DpSTR_DrawLin(FT_EN_08x16, 0, 164, 32, "m");
			DpSTR_DrawLin(FT_EN_08x16, 0, 175, 32, "/");

			APP_Num2Str(set20mA, p_str, 4, 2, FALSE);

			DpSTR_DrawLin(FT_EN_08x16, 0, 185, 32, p_str);
			DpSTR_DrawLin(FT_EN_08x16, 0, 229, 32, "m");
			break;
	}
#else
    F32 temp;
    S16 feet = 0;

	// Value - Current Output
	if (lMnMsr.unit == MnMSR_UNIT_METER)
    {
		DpSTR_DrawLin(FT_EN_08x16, 0, 119, 32, LCD_WriteNum2Str(lMnOut.set_04mA, 4, 2, 0));
		DpSTR_DrawLin(FT_EN_08x16, 0, 164, 32, "m");
		DpSTR_DrawLin(FT_EN_08x16, 0, 175, 32, "/");
		DpSTR_DrawLin(FT_EN_08x16, 0, 185, 32, LCD_WriteNum2Str(lMnOut.set_20mA, 4, 2, 0));
		DpSTR_DrawLin(FT_EN_08x16, 0, 229, 32, "m");
	}
    else
    {
		temp = (lMnOut.set_04mA * 0.3280839895) + 0.5;
		feet = temp;
		DpSTR_DrawLin(FT_EN_08x16, 0, 118, 32, LCD_WriteNum2Str(feet, 3, 1, 0));
		DpSTR_DrawLin(FT_EN_08x16, 0, 156, 32, "ft");
		DpSTR_DrawLin(FT_EN_08x16, 0, 173, 32, "/");
		temp = (lMnOut.set_20mA * 0.3280839895) + 0.5;
		feet = temp;
		DpSTR_DrawLin(FT_EN_08x16, 0, 185, 32, LCD_WriteNum2Str(feet, 3, 1, 0));
		DpSTR_DrawLin(FT_EN_08x16, 0, 221, 32, "ft");
	}
#endif
}

void DspInfo_EchoAmp(void)
{
#if 0
	U08 pStr[APP_STR_LENGTH] = {0,};

	APP_Num2Str(MnMSR_GetAmp(), pStr, 3, 0, FALSE);


#else
	U08 pStr[APP_STR_LENGTH] = {0,};
	// Title
	U08 lang = MnSYS_GetLang();
	switch(lang)
	{	
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			FNT_SetStr(55, 58, 1, 3, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // ECHO AMP 19.08.26 PETER
			LCD_WrStrBuff(5, 48, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_CN)
			FNT_SetStr(7, 22, 47, 27, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// 수신  // 19.08.26 PETER
			LCD_WrStrBuff(5, 48, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_JP)
			FNT_SetStr(6, 12, 42, 3, 41, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// 수신  // 19.08.26 PETER
			LCD_WrStrBuff(5, 48, FT_LC_16x16, gFt_str, 0, 0);

			break;
#endif
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 48, "Echo Amp");
			break;
	}
	APP_Num2Str(MnMSR_GetAmp(), pStr, 3, 0, FALSE);
	DpSTR_DrawLin(FT_EN_08x16, 0, 127, 48, pStr);
#endif
}

void DspInfo_Asf(void)		// Abnormal Signal Filter
{
	U08 p_str[APP_STR_LENGTH] = {0,};

#if 0
	DpSTR_DrawLin(FT_EN_08x16, 0, 5, 64, "ASF Value");

	APP_Num2Str(MnMSR_GetAsf(), p_str, 4, 0, FALSE);
	DpSTR_DrawLin(FT_EN_08x16, 0, 120, 64, p_str);
#else


	U08 lang = MnSYS_GetLang();
	switch(lang)
	{	
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			FNT_SetStr(1, 1, 1, 1, 50, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// ASF 설정
			LCD_WrStrBuff(5, 64, FT_LC_16x16, gFt_str, 0, 0);
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 64, "ASF");
			break;
#elif defined (_FT_LANG_CN)
			FNT_SetStr(1, 1, 15, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// ASF 설정
			LCD_WrStrBuff(5, 64, FT_LC_16x16, gFt_str, 0, 0);
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 64, "ASF");
			break;
#elif defined (_FT_LANG_JP)
			FNT_SetStr(1, 1, 82, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// ASF 설정
			LCD_WrStrBuff(5, 64, FT_LC_16x16, gFt_str, 0, 0);
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 64, "ASF");
			break;
#endif
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 64, "ASF Value");
			break;
	}
	
	APP_Num2Str(MnMSR_GetAsf(), p_str, 4, 0, FALSE);
	DpSTR_DrawLin(FT_EN_08x16, 0, 120, 64, p_str);

#endif
}

void DspInfo_Offset(void)
{
    F32 temp;
    U16 aux0;
    S16 feet = 0;
	U08 p_str[APP_STR_LENGTH] = {0,};
	S16 offset = MnMSR_GetOffs();
	U08 lang = MnSYS_GetLang();

	switch(lang)
	{	
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			FNT_SetStr(FKR_MI_,	   FKR_SE_,   FKR_JO_,  FKR_JEONG, 0,0,0,0,0,0,0,0,0,0,0);	// ASF 설정
			LCD_WrStrBuff(5, 80, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_CN)
			FNT_SetStr( 87,  120,   0,	0,   0,   0,0,0,0,0,0,0,0,0,0);	// ASF 설정
			LCD_WrStrBuff(5, 80, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_JP)
			FNT_SetStr( 7, 27, 16, 19, 22,   0,   0,   0, 0,0, 0,0, 0,0, 0);	// ASF 설정
			LCD_WrStrBuff(5, 80, FT_LC_16x16, gFt_str, 0, 0);
			break;
#endif
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 80, "Offset");
			break;
	}

	if(offset < 0)
    {
		aux0 = offset*-1;
		DpSTR_DrawLin(FT_EN_08x16, 0, 114, 80, "-");
	}
    else if(offset > 0)
    {
		aux0 = offset;
		DpSTR_DrawLin(FT_EN_08x16, 0, 114, 80, "+");
	}
    else
		aux0 = offset;

	if(MnMSR_GetUnit() == MnMSR_UNIT_METER)
    {
		APP_Num2Str(aux0, p_str, 4, 2, FALSE);
		DpSTR_DrawLin(FT_EN_08x16, 0, 120, 80, p_str);
		DpSTR_DrawLin(FT_EN_08x16, 0, 165, 80, "m");
	}
    else
    {
		temp = (aux0 * MSR_CALC_M2F_CONST);

		if(temp >= 0)		temp += 0.5;
		else				temp -= 0.5;

		feet = temp;

		APP_Num2Str(feet, p_str, 3, 1, FALSE);
		DpSTR_DrawLin(FT_EN_08x16, 0, 120, 80, p_str);
		DpSTR_DrawLin(FT_EN_08x16, 0, 165, 80, "ft");
	}
}

void DspInfo_TestMode(void)
{
#if 0
	DpSTR_DrawLin(FT_EN_08x16, 0, 5, 96, "Test mode");

	switch(MnMSR_GetFgTest())
	{
		case MnMSR_F_DEMO_OFF:		DpSTR_DrawLin(FT_EN_08x16, 0, 127, 96, "OFF");		break;
		case MnMSR_F_DEMO_ONN:		DpSTR_DrawLin(FT_EN_08x16, 0, 127, 96, " ON");		break;
	}
#else

	U08 lang = MnSYS_GetLang();

	switch(lang)
	{	
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			FNT_SetStr(FKR_TE_,	  FKR_SEU,   FKR_TEU,1,FKR_MO_, FKR_DEU,0,0,0,0,0,0,0,0,0);	// 테스트 모드
			LCD_WrStrBuff(5, 96, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_CN)
			FNT_SetStr(66, 92,  60,  38,  0, 0,0,0,0,0,0,0,0,0,0); // 테스트 모드
			LCD_WrStrBuff(5, 96, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_JP)
			FNT_SetStr(20, 15, 22, 32, 42, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 테스트 모드
			LCD_WrStrBuff(5, 96, FT_LC_16x16, gFt_str, 0, 0);
			break;
#endif
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 96, "Test Mode");
			break;
	}

	switch(MnMSR_GetFgTest())
	{
		case MnMSR_F_DEMO_OFF:		DpSTR_DrawLin(FT_EN_08x16, 0, 127, 96, "OFF");		break;
		case MnMSR_F_DEMO_ONN:		DpSTR_DrawLin(FT_EN_08x16, 0, 127, 96, " ON");		break;
	}

#endif
}

void DspInfo_Density(void)
{
#if 0
	DpSTR_DrawLin(FT_EN_08x16, 0, 5, 112, "Frequency");
	DpSTR_FreqUpd(MnMSR_GetFreq(), 127, 112);

#else
	U08 lang = MnSYS_GetLang();

	switch(lang)
	{	
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)
			FNT_SetStr(FKR_JU_,   FKR_PA,    FKR_SU_,0,0,0,0,0,0,0,0,0,0,0,0);
			LCD_WrStrBuff(5, 112, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_CN)
			FNT_SetStr(156, 145, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			LCD_WrStrBuff(5, 112, FT_LC_16x16, gFt_str, 0, 0);
			break;
#elif defined (_FT_LANG_JP)
			FNT_SetStr(57, 71, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			LCD_WrStrBuff(5, 112, FT_LC_16x16, gFt_str, 0, 0);
			break;
#endif
		case MnSYS_LANG_ENG:
		default:
			DpSTR_DrawLin(FT_EN_08x16, 0, 5, 112, "Frequency");
			break;
	}
	DpSTR_FreqUpd(MnMSR_GetFreq(), 127, 112);
#endif
}

void DpINF_ProcMain(void)
{
	DspInfo_Init();
	DspInfo_Empty();
	DspInfo_DeadZone();
	DspInfo_CurrOut();
	DspInfo_EchoAmp();
	DspInfo_Asf();
	DspInfo_Offset();
	DspInfo_TestMode();
	DspInfo_Density();
}


//--------------------------------------------------------------------------------------------------
//  Global APIs - Ext. Only
//--------------------------------------------------------------------------------------------------
void DSP_SetBuff(U16 width, U08 *p_val, U16 start, U32 end)
{
    U16 i;
    U32 aux;
    F32 interval;
    F32 idx = 0;
    U08 old = SFRPAGE;

    SFRPAGE = CONFIG_PAGE;

    interval = (end - start) / (width * 1.0);

    for(i=0; i<width; i++)
    {
    	aux = idx;
    	gDp_buff_grp[i] = *(p_val + aux);
    	idx += interval;
    }

    SFRPAGE = old;
}

U16 DSP_GetCurrent(void)
{
    U16 current;
	U16 curr_04ma, curr_20ma;
    F32 dg_c;
    F32 dg_data  = MEAS_GetSgThick();
	U16 set04mA  = MnOUT_GetSet04mA();
	U16 set20mA  = MnOUT_GetSet20mA();
	U08 echo_out = MnOUT_GetEchoOut();

	switch(MnMSR_GetUnit())
	{
		case MnMSR_UNIT_METER:
			curr_04ma = set04mA;
			curr_20ma = set20mA;
			break;
		case MnMSR_UNIT_FEET:
			curr_04ma = (U16)((F32)set04mA * MSR_CALC_M2F_CONST); //+ 0.5;
			curr_20ma = (U16)((F32)set20mA * MSR_CALC_M2F_CONST); //+ 0.5;
			break;
	}

	dg_c = (16.0 / (curr_20ma - curr_04ma)) * (dg_data - curr_04ma) + 4.0;
	
    if (dg_c < 4)       dg_c = 4;
    if (dg_c > 20)      dg_c = 20;

    current = dg_c * 100;

    if(MsANL_GetFgErr())
    {
        if(echo_out == MnOUT_ECHO_OUT_38mA)     current = 380;
        if(echo_out == MnOUT_ECHO_OUT_21mA)     current = 2100;
    }

    return current;
}

void DSP_ConfigBox(U08 lang, U16 column, U08 row, U08 *p_str, U08 sel)
{
    U08 old = SFRPAGE;
	U16 top = MnBOX_TOP+10;
	U16 bot = MnBOX_BOTTOM+10;
	
    SFRPAGE = CONFIG_PAGE;
	
    // clear window
    LDT_ClrBuff((MnBOX_LEFT-2)-3, top-3, MnBOX_RIGHT+3, bot+3);

    // window frame
    LDT_DrawBox((MnBOX_LEFT-2)-1, top-1, MnBOX_RIGHT+1, bot+1);
    LDT_DrawBox((MnBOX_LEFT-2), top, MnBOX_RIGHT, bot);

#if 1
	switch(lang)
	{
		case MnSYS_LANG_ENG:	DpSTR_DrawLin(FT_EN_08x16, sel, column, row, p_str);		break;
		default:				LCD_WrStrBuff(column, row, FT_LC_16x16, p_str, sel, 0);		break;
	}

#else
	if	   (lang == MnSYS_LANG_ENG) 	DpSTR_DrawLin(FT_EN_08x16, sel, column, row, p_str);
	else if(lang == MnSYS_LANG_CHN) 	LCD_WrStrBuff(column, row, FT_LC_16x16, p_str, sel, 0);
	else if(lang == MnSYS_LANG_KOR) 	LCD_WrStrBuff(column, row, FT_LC_16x16, p_str, sel, 0);
#endif

    LDT_UpdBuff(LCD_Y_START,LCD_Y_END);

    SFRPAGE = old;
}

void DSP_DrawGraph(U16 x_start, U16 y_start, U16 x_width, U16 y_height, U08* p_val,
 				   U16 idx_start, U32 idx_end, U16 max, U08 type)
{
    U32 aux;
    U16 i, dat1, height, deadzone;
	F32 dat2;
    F32 ratio_y, interval, idx_aux;
    U08 old = SFRPAGE;

    SFRPAGE = CONFIG_PAGE;
	
    height   = y_start + y_height;
    ratio_y  = y_height / (max * 1.0);
    interval = (idx_end - idx_start) / (x_width * 1.0);
    idx_aux  = 0;

    dat2     = MsANL_GetTrhdPct() * (y_height / 240.0);
    deadzone = (220 * (U32)MnMSR_GetDead()) / (U32)MsANL_GetEmpty();

    for(i=x_start; i<(x_start+x_width); i++)
    {
        aux = idx_aux;
        dat1 = *(p_val + aux) * ratio_y;
        idx_aux += interval;

		LDT_LineVert(i, y_start, height, LDT_LINE_CLEAR);

        switch(type)
        {
            case DP_IDX_GRAPH_1:
                if(i > deadzone)
					LDT_LineVert(i, height-dat1, height, LDT_LINE_DATA);
                break;

            case DP_IDX_GRAPH_2: 
                if(i > deadzone)
                {
					LDT_LineVert(i, height-dat1, height, LDT_LINE_DATA);

                	if((i % 2) == 0)
                    {
                        if(dat1 > dat2)         LDT_ClearDot(i, height - dat2);
                        else                    LCD_DataDot_Buff(i, height - dat2);
                    }
                }
            	break;

            case DP_IDX_GRAPH_3:
                LCD_DataDot_Buff(i, height - dat1);
                break;
            case DP_IDX_GRAPH_4:
				LDT_LineVert(i, height-dat1, height, LDT_LINE_DATA);

				#if 1
				if((i % 2) == 0)
				{
					if(dat1 > dat2) 		LDT_ClearDot(i, height - dat2);
					else					LCD_DataDot_Buff(i, height - dat2);
				}
				#endif
				break;
        }
    }

    DspDrw_AxisY(x_start, y_start, x_width, y_height, type);

    SFRPAGE = old;
}

void DSP_DrawEchoQ(U08 *pArr) 
{
	ANZ_SetEmptyPct(110);

	gDp_flg_cs_vert = 0;

	DSP_DrawGraph(0, 30 + 2, 220, 85, pArr, 0, MsANL_GetEmpty(), 256, DP_IDX_GRAPH_1);
	DpSTR_DrawLin(FT_EN_06x07, 0, 195, 30 + 2 + 85 + 2, "Empty");
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void DISP_InitVari(void)
{
	S16 i;

	gDp.f_err_tprt = FALSE;
	gDp_flg_log = 0;

	for(i=0; i<220; i++)
		gDp_buff_grp[i] = 0;
}

void DISP_ClearScr(void)
{
    LDT_ClrBuff (LCD_X_START, LCD_Y_START, LCD_X_END, LCD_Y_END);
    LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
}

void DISP_IntroScr(void)
{
	LDT_ClrBuff(LCD_X_START, LCD_Y_START, LCD_X_END, LCD_Y_END);
	DpSTR_LineUpd(FT_EN_08x16, 0, 84, 56, "Start...");		
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

