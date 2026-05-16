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
#include "menu_output.h"
// bsp
#include "bsp_main.h"
#include "flash_main.h"
#include "gpio_main.h"
#include "prtc_main.h"
#include "uart_main.h"
#include "btn_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "dly_main.h"
#include "dac_main.h"
#include "rly_main.h"
#include "rf_main.h"
// msr
#include "measure_main.h"
#include "measure_analyze.h"
// menu
#include "menu_main.h"
#include "menu_system.h"
#include "menu_measure.h"
#include "menu_factory.h"
#include "menu_box.h"
#include "menu_quick.h"
// disp
#include "disp_main.h"
#include "disp_menu.h"
#include "disp_string.h"
// font
#include "font_def.h"
#include "font_idx_kor.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
LS_MnOUT lMnOut;


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - List Item (Page 1)
//------------------------------------------------------------------------------------------------------------------------------
void MnOut_SetCurr(U08 sel)
{
	U16 mt;
	S32 ft;
	U08 *pStr = "    ";
	U08 dist = MnFTR_GetDistMax();
	U08 unit = MnMSR_GetUnit();
	U32 ft_min = MnOUT_SET_CURR_FT_MIN;
	U32 mt_min = MnOUT_SET_CURR_MT_MIN;

	switch(sel)
	{
		case MnOUT_L01_SET_04mA:	mt = lMnOut.set_04mA;		break;
		case MnOUT_L02_SET_20mA:	mt = lMnOut.set_20mA;		break;
		default:					return;
	}

	ft = (S32)MEAS_CalcMeter2Feet(mt);

	switch(unit)
	{
		case MnMSR_UNIT_METER:		pStr = "(m)";		break;
		case MnMSR_UNIT_FEET:		pStr = "(ft)";		break;
		default:					return;
	}

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "Level");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y02, F_F, pStr);

	if(MnMSR_GetUnit() == MnMSR_UNIT_FEET)
	{
		switch(dist)
		{
			case MnFTR_DIST_MAX_10m:	ft = MnBOX_CfgNumb(ft, ft_min, MnOUT_SET_CURR_FT_10_MAX, 1, 1, F_F, "");	break;
			case MnFTR_DIST_MAX_20m:	ft = MnBOX_CfgNumb(ft, ft_min, MnOUT_SET_CURR_FT_20_MAX, 1, 1, F_F, "");	break;
		}

		switch(sel)
		{
			case MnOUT_L01_SET_04mA:	lMnOut.set_04mA = MEAS_CalcFeet2Meter(ft);		break;
			case MnOUT_L02_SET_20mA:	lMnOut.set_20mA = MEAS_CalcFeet2Meter(ft);		break;
		}
	}
	else
	{
		switch(dist)
		{
			case MnFTR_DIST_MAX_10m:	mt = MnBOX_CfgNumb(mt, mt_min, MnOUT_SET_CURR_MT_10_MAX, 1, 2, F_F, "");	break;
			case MnFTR_DIST_MAX_20m:	mt = MnBOX_CfgNumb(mt, mt_min, MnOUT_SET_CURR_MT_20_MAX, 1, 2, F_F, "");	break;
		}

		switch(sel)
		{
			case MnOUT_L01_SET_04mA:	lMnOut.set_04mA = mt;		break;
			case MnOUT_L02_SET_20mA:	lMnOut.set_20mA = mt;		break;
		}
	}
}

void MnOut_TrimCurr(U08 lst)
{
	U08 cnt = 0;
	S16 old, sel;
	U08 dac_opt;

	switch(lst)
	{
		case MnOUT_L03_TRIM_12mA:	old = lMnOut.trim_12mA;		dac_opt = DAC_CURR_IDX_12mA;	break;
		case MnOUT_L04_TRIM_20mA:	old = lMnOut.trim_20mA;		dac_opt = DAC_CURR_IDX_20mA;	break;
		default:					return;
	}

	sel = old;

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01,	F_F, "Trim");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X03, MnBOX_Y02, F_F, "(Center 50)");

	DAC_OutCurr(0, dac_opt);
	DpSTR_NumbUpd(MnBOX_Y03, sel, MnOUT_TRIM_CURR_MAX, 0, F_F, "");
	BTN_DelayMsec(BTN_WAIT_SET);

	do
	{
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{
			if(++cnt >= 30)
				cnt = 30;

			sel = BTN_GetValue_UpDn(sel, MnOUT_TRIM_CURR_MIN, MnOUT_TRIM_CURR_MAX, 1, cnt);

			DAC_OutCurr(0, dac_opt);
			DpSTR_NumbUpd(MnBOX_Y03, sel, MnOUT_TRIM_CURR_MAX, 0, F_F, "");
			BTN_DelayMsec(BTN_WAIT_UD);
		}
		else
			cnt = 0;
	}
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if(!_SBIT_BTN_SET && (sel != old))
	{
		switch(lst)
		{
			case MnOUT_L03_TRIM_12mA:	lMnOut.trim_12mA = sel;		break;
			case MnOUT_L04_TRIM_20mA:	lMnOut.trim_20mA = sel;		break;
		}
	}
	else
	{
		switch(lst)
		{
			case MnOUT_L03_TRIM_12mA:	lMnOut.trim_12mA = old;		break;
			case MnOUT_L04_TRIM_20mA:	lMnOut.trim_20mA = old;		break;
		}
	}

	DAC_OutCurr(MEAS_GetSgThick(), DAC_CURR_IDX_NORMAL);
}

void MnOut_Outp04mA(void)
{
	U32 cnt = 0;

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X08, MnBOX_Y02, F_F, "4mA...");
	BTN_DelayMsec(BTN_WAIT_SET);

	DAC_OutCurr(0, DAC_CURR_IDX_04mA);

	do
	{
		cnt = 0;

		while(!_SBIT_BTN_RIGHT && !_SBIT_BTN_LEFT)
		{
			if(++cnt > 500000)
			{
				MnFTR_ProcMain();
			}
		}
	}
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	DAC_OutCurr(MEAS_GetSgThick(), DAC_CURR_IDX_NORMAL);
}

void MnOut_EchoDelay(void)
{
	U16 sel = lMnOut.echo_dly;
	
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Number");
	lMnOut.echo_dly = MnBOX_CfgNumb(sel, MnOUT_ECHO_DLY_MIN, MnOUT_ECHO_DLY_MAX, MnOUT_ECHO_DLY_STP, 0, F_F, "");
}

void MnOut_EchoCurr(void)
{
	U08 sel = lMnOut.echo_out;

	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X07, MnBOX_Y01, F_F, "Current");
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y02, F_F, "(mA)");
	lMnOut.echo_out = MnBOX_SelArg4(MnSYS_LANG_ENG, sel, MnBOX_X05, "  3.8mA  ",
														 MnBOX_X06, "  HOLD  ", MnBOX_X08, "21.0mA", 0, "", 3);
}


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - List Item (Page 2)
//------------------------------------------------------------------------------------------------------------------------------
void MnOut_RlyOper(U08 idx)
{
	S16 feet;
	U16 meter;
	U08 maxRange  = MnFTR_GetDistMax();

	switch(idx)
	{
		case MnOUT_L08_R1_ACT:		meter = lMnOut.rly1_act;		break;
		case MnOUT_L09_R1_STOP:		meter = lMnOut.rly1_stp;		break;
		case MnOUT_L10_R2_ACT:		meter = lMnOut.rly2_act;		break;
		case MnOUT_L11_R2_STOP:		meter = lMnOut.rly2_stp;		break;
		default:					return;
	}

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Level");

	switch(MnMSR_GetUnit())
	{
		case MnMSR_UNIT_METER:		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y02, F_F, "(m)");		break;
		case MnMSR_UNIT_FEET:		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y02, F_F, "(ft)");		break;
	}

	switch(MnMSR_GetUnit())
	{
		case MnMSR_UNIT_METER:
			switch(maxRange)
			{
				case MnFTR_DIST_MAX_10m: meter = MnBOX_CfgNumb(meter, 0, MnMSR_EMPTY_MT_10_MAX, 1, 2, F_F, ""); break;
				case MnFTR_DIST_MAX_20m: meter = MnBOX_CfgNumb(meter, 0, MnMSR_EMPTY_MT_20_MAX, 1, 2, F_F, ""); break;
			}
			break;
		
		case MnMSR_UNIT_FEET:
			switch(maxRange)
			{
				case MnFTR_DIST_MAX_10m: feet  = MnBOX_CfgNumb((S32)MEAS_CalcMeter2Feet(meter), MnOUT_SET_CURR_FT_MIN, MnMSR_EMPTY_FT_10_MAX, 1, 1, F_F, ""); break;
				case MnFTR_DIST_MAX_20m: feet  = MnBOX_CfgNumb((S32)MEAS_CalcMeter2Feet(meter), MnOUT_SET_CURR_FT_MIN, MnMSR_EMPTY_FT_20_MAX, 1, 1, F_F, ""); break;
			}
			meter = MEAS_CalcFeet2Meter(feet);
			break;
	}

	switch(idx)
	{
		case MnOUT_L08_R1_ACT:		lMnOut.rly1_act = meter;		break;
		case MnOUT_L09_R1_STOP:		lMnOut.rly1_stp = meter;		break;
		case MnOUT_L10_R2_ACT:		lMnOut.rly2_act = meter;		break;
		case MnOUT_L11_R2_STOP:		lMnOut.rly2_stp = meter;		break;
	}
}

void MnOut_RelayTest(void) 
{
	U16 temp = 0;
	U08 cnt = 0;
	U08 maxRange = MnFTR_GetDistMax();

	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X09, MnBOX_Y01, F_F, "Level");

	switch(MnMSR_GetUnit())
	{
		case MnMSR_UNIT_METER:		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X11, MnBOX_Y02, F_F, "(m)");	DpSTR_NumbUpd(MnBOX_Y03, temp, 1000, 2, 0, "");		break;
		case MnMSR_UNIT_FEET:		MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y02, F_F, "(ft)");	DpSTR_NumbUpd(MnBOX_Y03, temp, MnMSR_DEAD_FT_10_MAX, 1, 2, "");	break;
	}

	BTN_DelayMsec(BTN_WAIT_SET);

	do 
	{
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{
			if(++cnt >= 30)
				cnt = 30;


			if(MnMSR_GetUnit()==MnMSR_UNIT_METER)
			{
				switch(maxRange)
				{
					case MnFTR_DIST_MAX_10m:
						temp = BTN_GetValue_UpDn(temp, 0, MnMSR_EMPTY_MT_10_MAX, 1, cnt);
						DpSTR_NumbUpd(64, temp, MnMSR_EMPTY_MT_10_MAX, 2, 0, "");
						break;

					case MnFTR_DIST_MAX_20m:
						temp = BTN_GetValue_UpDn(temp, 0, MnMSR_EMPTY_MT_20_MAX, 1, cnt);
						DpSTR_NumbUpd(64, temp, MnMSR_EMPTY_MT_20_MAX, 2, 0, "");
						break;
				}
			}
			else if(MnMSR_GetUnit()==MnMSR_UNIT_FEET)
			{
				switch(maxRange)
				{
					case MnFTR_DIST_MAX_10m:
						temp = BTN_GetValue_UpDn(temp, 0, MnMSR_EMPTY_FT_10_MAX, 1, cnt);
						DpSTR_NumbUpd(64, temp, MnMSR_EMPTY_FT_10_MAX, 1, 2, "");
						break;
				
					case MnFTR_DIST_MAX_20m:
						temp = BTN_GetValue_UpDn(temp, 0, MnMSR_EMPTY_FT_20_MAX, 1, cnt);
						DpSTR_NumbUpd(64, temp, MnMSR_EMPTY_FT_20_MAX, 1, 2, "");
						break;
				}
			}
			
			RLY_ProcOut(temp);

			LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
			BTN_DelayMsec(BTN_WAIT_UD);
		}
		else
			cnt = 0;
		
	}
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	SBIT_RLY1_OUT = RLY_STT_OFF;
	SBIT_RLY2_OUT = RLY_STT_OFF;
}

void MnOut_Protocol(void)
{
	U08 ft = MnSYS_LANG_ENG;
	S16 old = lMnOut.protocol;
	S16 sel = old;

	MnBOX_DispBox();
	MnBOX_DispStr(ft, MnBOX_X10, MnBOX_Y01, F_F, "Type");
	sel = MnBOX_SelArg4(ft, sel, MnBOX_X06, "Standard", MnBOX_X06, "   RF   ", MnBOX_X06, " Modbus ", 0, "", 3);

	if(!_SBIT_BTN_SET && (sel != old))
		lMnOut.protocol = sel;

	if(lMnOut.protocol == MnOUT_PROT_RF)
	{
		MnBOX_DispBox();
		MnBOX_DispStr(ft, MnBOX_X09, MnBOX_Y01, F_F, "RF CH.");
		lMnOut.rf_ch = MnBOX_CfgNumb(lMnOut.rf_ch, MnOUT_RF_CH_MIN, MnOUT_RF_CH_MAX, 1, 0, F_F, "");

		UAT_SetBaudrate(1);
		DLY_RunMsec(1);
	}
	else if(lMnOut.protocol == MnOUT_PROT_MDBS)
	{
		MnBOX_DispStr(ft, MnBOX_X07, MnBOX_Y01, F_F, "Address");
		lMnOut.mdb_addr = MnBOX_CfgNumb(lMnOut.mdb_addr, MnOUT_MDB_ADDR_MIN, MnOUT_MDB_ADDR_MAX, 1, 0, F_F, "");

		MnBOX_DispStr(ft, MnBOX_X06, MnBOX_Y01, F_F, "  Baud  ");
		lMnOut.mdb_baud = MnBOX_SelArg4(ft, lMnOut.mdb_baud, MnBOX_X08, " 4800 ",
															 MnBOX_X08, " 9600 ",
															 MnBOX_X09, "19200", 0, "", 3);
		UAT_SetBaudrate(lMnOut.mdb_baud);
	}
}


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - Disp. List
//------------------------------------------------------------------------------------------------------------------------------
void OutDp_ListCmmn(U08 *pSel, U08 c)
{
	DpSTR_DrawLin(FT_EN_08x16, pSel[c],  8, MnSTR_Y_HEIGHT*c++, "R1");
	DpSTR_DrawLin(FT_EN_08x16, pSel[c],  8, MnSTR_Y_HEIGHT*c++, "R1");
	DpSTR_DrawLin(FT_EN_08x16, pSel[c],  8, MnSTR_Y_HEIGHT*c++, "R2");
	DpSTR_DrawLin(FT_EN_08x16, pSel[c],  8, MnSTR_Y_HEIGHT*c++, "R2");
}

#if 1
void OutDp_ListP1(U08 *pSel)
{
	U08 c  = MnOUT_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			#if defined (_FT_LANG_KR)			
			MnLST_DispClr(1);

			DpSTR_DrawLin(ft_loc, FALSE, MnOUT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(FKR_CHUL, FKR_LYEOG, 1,1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnOUT_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#1/2");

			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L01_SET_04mA+1,  _F_STR(1,        1,         1,        1,         1,       FKR_SEOL, FKR_JEONG,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L02_SET_20mA+1,  _F_STR(1,        1,         1,        1,         1,       FKR_SEOL, FKR_JEONG,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L03_TRIM_12mA+1, _F_STR(FKR_TEU,  FKR_LIM,	1, 		  1, 		 1,       1,        1,        1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L04_TRIM_20mA+1, _F_STR(FKR_TEU,  FKR_LIM,	1, 		  1, 		 1,       1,        1,        1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L05_OUTP_04mA+1, _F_STR(FKR_CHUL, FKR_LYEOG, 1, 		  1, 		 1,       1,        1,        1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L06_ECHO_DLY+1,  _F_STR(FKR_E__,  FKR_LEO,	FKR_JI_,  FKR_YEON,  FKR_SI_, FKR_GAN,  1,        1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L07_ECHO_CURR+1, _F_STR(FKR_E__,  FKR_LEO,	FKR_CHUL, FKR_LYEOG, 1,       1,        1,        1,1,1,1,1,1,1,1));

			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L01_SET_04mA],   8, MnSTR_Y_HEIGHT*1, " 4mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L02_SET_20mA],   8, MnSTR_Y_HEIGHT*2, "20mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L03_TRIM_12mA], 50, MnSTR_Y_HEIGHT*3, "12mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L04_TRIM_20mA], 50, MnSTR_Y_HEIGHT*4, "20mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L05_OUTP_04mA], 50, MnSTR_Y_HEIGHT*5, "4mA");
			#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
			DpSTR_DrawLin(ft_loc, FALSE, MnOUT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(101, 12, 1,1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnOUT_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#1/2");
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L01_SET_04mA+1, _F_STR(1,  1,1,91,  131, 1, 1,	  1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L02_SET_20mA+1, _F_STR(1,  1,1,91,  131, 1, 1,	  1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L03_TRIM_12mA+1, _F_STR(1,  1,1,101,  12, 42, 94,	  1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L04_TRIM_20mA+1, _F_STR(1,  1,1,101,  12, 42, 94,	  1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L05_OUTP_04mA+1, _F_STR(101,  12, 1, 1,	  1,  1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L06_ECHO_DLY+1,  _F_STR( 48, 114, 43, 89, 36,  182,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L07_ECHO_CURR+1, _F_STR( 48, 114, 43, 89, 101, 12,1,1,1,1,1,1,1,1,1));

			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L01_SET_04mA], 10, MnSTR_Y_HEIGHT*1, "4mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L02_SET_20mA], 10, MnSTR_Y_HEIGHT*2, "20mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L03_TRIM_12mA], 10, MnSTR_Y_HEIGHT*3, "12mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L04_TRIM_20mA], 10, MnSTR_Y_HEIGHT*4, "20mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L05_OUTP_04mA], 58, MnSTR_Y_HEIGHT*5, "4mA");
			#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
			DpSTR_DrawLin(ft_loc, FALSE, MnOUT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(48, 51, 1,1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnOUT_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#1/2");
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L01_SET_04mA+1,  _F_STR(1, 1, 1,  82, 61,1, 1,  1,   1,	  1, 1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L02_SET_20mA+1,  _F_STR(1, 1, 1, 82, 61, 1,1,  1,   1,	  1, 1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L03_TRIM_12mA+1, _F_STR(1, 1, 1, 22, 36, 31, 1, 1,   1,	  1, 1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L04_TRIM_20mA+1, _F_STR(1, 1, 1, 22, 36, 31, 1, 1,   1,	  1, 1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L05_OUTP_04mA+1, _F_STR(48, 51, 1,1, 1,1, 1,  1,   1,	  1, 1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L06_ECHO_DLY+1,  _F_STR( 6, 35, 42, 48,  51,  86, 64, 68,  87, 1, 1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L07_ECHO_CURR+1, _F_STR( 6, 35, 42, 68, 48, 51, 1, 1, 1,  1, 1,1,1,1,1));

			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L01_SET_04mA],  10, MnSTR_Y_HEIGHT*1, " 4mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L02_SET_20mA],  10, MnSTR_Y_HEIGHT*2, "20mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L03_TRIM_12mA], 10, MnSTR_Y_HEIGHT*3, "12mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L04_TRIM_20mA], 10, MnSTR_Y_HEIGHT*4, "20mA");
			DpSTR_DrawLin(ft_eng, pSel[MnOUT_L05_OUTP_04mA], 58, MnSTR_Y_HEIGHT*5, "4mA");			
			#endif
			break;
		case MnSYS_LANG_ENG:
		default:
                                                                  // 0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE,     MnOUT_L00_IDLE+1, 	    "         OUTPUT #1/2           ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L01_SET_04mA+1,  "Set 4mA                        ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L02_SET_20mA+1,  "Set 20mA                       ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L03_TRIM_12mA+1, "Trim 12mA                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L04_TRIM_20mA+1, "Trim 20mA                      ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L05_OUTP_04mA+1, "Output 4mA                     ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L06_ECHO_DLY+1,  "Error Delay                     ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L07_ECHO_CURR+1, "Error Output                    ");
			break;
	}
}

void OutDp_ListP2(U08 *pSel)
{
	U08 c  = MnOUT_L00_IDLE + 1;
	U08 ft_eng = FT_EN_08x16;
	U08 ft_loc = FT_LC_16x16;

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			#if defined (_FT_LANG_KR)			
			MnLST_DispClr(1);

			DpSTR_DrawLin(ft_loc, FALSE, MnOUT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(FKR_CHUL, FKR_LYEOG, 1,1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnOUT_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#2/2");

			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L08_R1_ACT-6,   _F_STR(1,		1,	     1,		  FKR_JEOB, FKR_JEOM, FKR_DAD,  FKR_HIM,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L09_R1_STOP-6,  _F_STR(1,		1,	     1,		  FKR_JEOB, FKR_JEOM, FKR_YEOL, FKR_LIM,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L10_R2_ACT-6,   _F_STR(1,		1,	     1,		  FKR_JEOB, FKR_JEOM, FKR_DAD,  FKR_HIM,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L11_R2_STOP-6,  _F_STR(1,	    1,	     1,		  FKR_JEOB, FKR_JEOM, FKR_YEOL, FKR_LIM,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L12_RLY_TEST-6, _F_STR(FKR_LIL,  FKR_LE_, FKR_I__, 1,		FKR_JEOM, FKR_GEOM, 1,	    1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L13_PROTOCOL-6, _F_STR(FKR_TONG, FKR_SIN, 1,		  1,		1,		  1,		1,	    1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_CN)
			MnLST_DispClr(1);
																 // 0123456789abcdef0123456789abcde
			DpSTR_DrawLin(ft_loc, FALSE, MnOUT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(101, 12, 1,1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnOUT_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#2/2");
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L08_R1_ACT-6,   _F_STR(  1, 102, 86,  1,  1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L09_R1_STOP-6,  _F_STR(  1,   8, 61,  1,  1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L10_R2_ACT-6,   _F_STR(  1, 102, 86,  1,  1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L11_R2_STOP-6,  _F_STR(  1,   8, 61,  1,  1,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L12_RLY_TEST-6, _F_STR( 81,  69, 24, 66, 92,1,1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L13_PROTOCOL-6, _F_STR(103,   184,  185,  186,  1,1,1,1,1,1,1,1,1,1,1));
			#elif defined (_FT_LANG_JP)
			MnLST_DispClr(1);
																 // 0123456789abcdef0123456789abcde
			DpSTR_DrawLin(ft_loc, FALSE, MnOUT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, _F_STR(48, 51, 1,1,1,1,1,1,1,1,1,1,1,1,1));
			DpSTR_DrawLin(ft_eng, FALSE, MnOUT_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#2/2");
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L08_R1_ACT-6,   _F_STR(1,36, 38, 42,   52, 45,74, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L09_R1_STOP-6, _F_STR(1, 36, 38, 42,   47, 70,74, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L10_R2_ACT-6,   _F_STR(1,  36, 38, 42,   52, 45,74, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L11_R2_STOP-6,  _F_STR(1, 36, 38, 42,   47, 70,74, 1, 1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L12_RLY_TEST-6, _F_STR(36, 38, 42, 20, 15, 22, 1,1,1,1,1,1,1,1,1));
			MnLST_DispStr(ft_loc, pSel[c++], MnOUT_L13_PROTOCOL-6, _F_STR(29, 39, 22, 12, 37,   1, 1,1,1,1,1,1,1,1,1));
			#endif

			OutDp_ListCmmn(pSel, 1);
			break;
		case MnSYS_LANG_ENG:
		default:
                                                                 // 0123456789abcdef0123456789abcde
			MnLST_DispStr(ft_eng, FALSE,     MnOUT_L00_IDLE+1, 	   "         OUTPUT #2/2           ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L08_R1_ACT-6,   "R1 Act                         ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L09_R1_STOP-6,  "R1 Stop                        ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L10_R2_ACT-6,   "R2 Act                         ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L11_R2_STOP-6,  "R2 Stop                        ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L12_RLY_TEST-6, "Relay Test                     ");
			MnLST_DispStr(ft_eng, pSel[c++], MnOUT_L13_PROTOCOL-6, "Protocol                       ");
			break;
	}	
}

#else
void OutDp_ListEngP1(U08 *pSel)
{
	U08 c  = MnOUT_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
													//   0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,	 MnOUT_L00_IDLE+1,		"         OUTPUT #1/2           ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L01_SET_04mA+1,	"Set 4mA                       ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L02_SET_20mA+1,	"Set 20mA                      ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L03_TRIM_12mA+1,	"Trim 12mA                      ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L04_TRIM_20mA+1,	"Trim 20mA                      ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L05_OUTP_04mA+1,	"Output 4mA                     ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L06_ECHO_DLY+1,	"Echo Delay                     ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L07_ECHO_CURR+1,	"Echo Output                    ");
}

void OutDp_ListEngP2(U08 *pSel)
{
	U08 c  = MnOUT_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
													//   0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,	 MnOUT_L00_IDLE+1,		"         OUTPUT #2/2           ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L08_R1_ACT-6,	"R1 Act                         ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L09_R1_STOP-6,	"R1 Stop                        ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L10_R2_ACT-6,	"R2 Act                         ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L11_R2_STOP-6,	"R2 Stop                        ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L12_RLY_TEST-6,	"Relay Test                     ");
	MnLST_DispStr(ft, pSel[c++], MnOUT_L13_PROTOCOL-6,	"Protocol                       ");
}

#if defined (_FT_LANG_KR)
void OutDp_ListKorP1(U08 *pSel)
{
	U08 i;
	U08 c = 1;
	U08 height = MnSTR_Y_HEIGHT;
	U08 ft_en = FT_EN_08x16;

	MnLST_DispClr(1);
	_F_SS(FKR_CHUL,	FKR_LYEOG, 1,1,1,1,1,1,1,1,1,1,1,1,1);
	DpSTR_DrawLin(FT_LC_16x16, FALSE, MnOUT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, gFt_str);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, MnOUT_X0_PAGE_KO,  MnSTR_Y_HEIGHT*0, "#1/2");

	for(i=(MnOUT_L00_IDLE+1); i<MnOUT_LST_MAX; i++)
	{
		switch(i)
		{
			case MnOUT_L01_SET_04mA: 	_F_SS(1,1,1,1,1,FKR_SEOL,	FKR_JEONG,	1,					1,1,1,1,1,1,1);		break;
			case MnOUT_L02_SET_20mA: 	_F_SS(1,1,1,1,1,FKR_SEOL,	FKR_JEONG,	1,					1,1,1,1,1,1,1);		break;
			case MnOUT_L03_TRIM_12mA:	_F_SS(FKR_TEU,	FKR_LIM,	1,			1,			1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L04_TRIM_20mA: 	_F_SS(FKR_TEU,	FKR_LIM,	1,			1,			1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L05_OUTP_04mA:	_F_SS(FKR_CHUL,	FKR_LYEOG,	1,			1,			1,1,1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L06_ECHO_DLY:	_F_SS(FKR_E__,	FKR_LEO, 	FKR_JI_,	FKR_YEON,	FKR_SI_,	FKR_GAN,	1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L07_ECHO_CURR:	_F_SS(FKR_E__,	FKR_LEO, 	FKR_CHUL,	FKR_LYEOG, 	1,1,1,1,1,1,1,1,1,1,1);		break;
		}

		switch(i)
		{
			case MnOUT_L01_SET_04mA:
			case MnOUT_L02_SET_20mA:
				if(pSel[i])		DpSTR_DrawLin(ft_en, pSel[i], 0, height*i,">");
				else			DpSTR_DrawLin(ft_en, pSel[i], 0, height*i," ");
				break;
		}

		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L01_SET_04mA],   8, MnSTR_Y_HEIGHT*c++, " 4mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L02_SET_20mA],   8, MnSTR_Y_HEIGHT*c++, "20mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L03_TRIM_12mA], 50, MnSTR_Y_HEIGHT*c++, "12mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L04_TRIM_20mA], 50, MnSTR_Y_HEIGHT*c++, "20mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L05_OUTP_04mA], 50, MnSTR_Y_HEIGHT*c++, "4mA");
}

void OutDp_ListKorP2(U08 *pSel)
{
	U08 i;

	MnLST_DispClr(1);
	_F_SS(FKR_CHUL, FKR_LYEOG, 1,1,1,1,1,1,1,1,1,1,1,1,1);
	DpSTR_DrawLin(FT_LC_16x16, FALSE, MnOUT_X0_TITLE_KO, MnSTR_Y_HEIGHT*0, gFt_str);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, MnOUT_X0_PAGE_KO, MnSTR_Y_HEIGHT*0, "#2/2");

	for(i=(MNU_LIST_NUM+1); i<=(MNU_LIST_NUM*2); i++)
	{
		switch(i)
		{
			case MnOUT_L08_R1_ACT:		_F_SS(1,1,1,	FKR_JEOB,	FKR_JEOM,	FKR_DAD,	FKR_HIM,	1,1,1,1,1,1,1,1);	break;
			case MnOUT_L09_R1_STOP:		_F_SS(1,1,1,	FKR_JEOB,	FKR_JEOM, 	FKR_YEOL,	FKR_LIM,	1,1,1,1,1,1,1,1);	break;
			case MnOUT_L10_R2_ACT:		_F_SS(1,1,1,	FKR_JEOB,	FKR_JEOM, 	FKR_DAD,	FKR_HIM,	1,1,1,1,1,1,1,1);	break;
			case MnOUT_L11_R2_STOP:		_F_SS(1,1,1,	FKR_JEOB,	FKR_JEOM, 	FKR_YEOL,	FKR_LIM,	1,1,1,1,1,1,1,1);	break;
			case MnOUT_L12_RLY_TEST: 	_F_SS(FKR_LIL,	FKR_LE_,	FKR_I__,1,	FKR_JEOM,	FKR_GEOM,	1,1,1,1,1,1,1,1,1);	break;
			case MnOUT_L13_PROTOCOL: 	_F_SS(FKR_TONG,	FKR_SIN, 	1, 		1,	1,			1,			1,1,1,1,1,1,1,1,1);	break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i-MNU_LIST_NUM], i-MNU_LIST_NUM+1, "");
	}
	OutDp_ListCmmn(pSel, 1);
}

#elif defined (_FT_LANG_CN)
void OutDp_ListChnP1(U08 *pSel)
{
	U08 i;
	U08 c = 1;
	U08 height = MnSTR_Y_HEIGHT;
	U08 ft_en = FT_EN_08x16;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "         OUTPUT #1/2           ");

	for(i=(MnOUT_L00_IDLE+1); i<MnOUT_LST_MAX; i++)
	{
		switch(i)
		{
			case MnOUT_L01_SET_04mA: 	DpSTR_DrawLin(ft_en, pSel[i], 8, height*i," 4 mA                          ");	break;
			case MnOUT_L02_SET_20mA: 	DpSTR_DrawLin(ft_en, pSel[i], 8, height*i,"20 mA                          ");	break;
			case MnOUT_L03_TRIM_12mA:	_F_SS(101,		12,		66,		92,		1,		1,	1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L04_TRIM_20mA: 	_F_SS(101,		12,		66,		92,		1,		1,	1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L05_OUTP_04mA:	_F_SS(101,		12,		66,		92,		1,		1,	1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L06_ECHO_DLY:	_F_SS(48,		114,	36,		51,		110,	1,	1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L07_ECHO_CURR:	_F_SS(48,		114,	43,		89,		101,	12,	1,1,1,1,1,1,1,1,1);		break;
		}

		switch(i)
		{
			case MnOUT_L01_SET_04mA:
			case MnOUT_L02_SET_20mA:
				if(pSel[i])		DpSTR_DrawLin(ft_en, pSel[i], 0, height*i,">");
				else			DpSTR_DrawLin(ft_en, pSel[i], 0, height*i," ");
				break;

			default:	MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");		break;
		}
	}

	#if 1
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L03_TRIM_12mA], 80, MnSTR_Y_HEIGHT*3, "12mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L04_TRIM_20mA], 80, MnSTR_Y_HEIGHT*4, "20mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L05_OUTP_04mA], 80, MnSTR_Y_HEIGHT*5, " 4mA");
	#else
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L03_TRIM_12mA], 80, MnSTR_Y_HEIGHT*c++, "12mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L04_TRIM_20mA], 80, MnSTR_Y_HEIGHT*c++, "20mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L05_OUTP_04mA], 80, MnSTR_Y_HEIGHT*c++, " 4mA");
	#endif
}

void OutDp_ListChnP2(U08 *pSel)
{
	U08 i;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "         OUTPUT #2/2           ");

	for(i=(MNU_LIST_NUM+1); i<=(MNU_LIST_NUM*2); i++)
	{
		switch(i)
		{
			case MnOUT_L08_R1_ACT:		_F_SS(1,		102,	86,		1,		1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L09_R1_STOP:		_F_SS(1,		8,		61,		1,		1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L10_R2_ACT:		_F_SS(1,		102,	86,		1,		1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L11_R2_STOP:		_F_SS(1,		8,		61,		1,		1,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L12_RLY_TEST: 	_F_SS(81,		69,		24,		66,		92,		1,1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L13_PROTOCOL: 	_F_SS(103,		7,		1, 		1, 		1,		1,1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i-MNU_LIST_NUM], i-MNU_LIST_NUM+1, "");
	}
	OutDp_ListCmmn(pSel, 1);
}

#elif defined (_FT_LANG_JP)
void OutDp_ListJpnP1(U08 *pSel)
{
	U08 i;
	U08 c = 1;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "         OUTPUT #1/2           ");

	for(i=(MnOUT_L00_IDLE+1); i<MnOUT_LST_MAX; i++)
	{
		switch(i)
		{
			case MnOUT_L01_SET_04mA: 	_F_SS(171,	171,171,171,110,118,121,87,	0,	1,	1,1,1,1,1);		break;
			case MnOUT_L02_SET_20mA: 	_F_SS(171,	171,171,171,110,118,121,87,	0,	1,	1,1,1,1,1);		break;
			case MnOUT_L03_TRIM_12mA:	_F_SS(171,	171,171,171,121,108,123,0,	1,	1,	1,1,1,1,1);		break;
			case MnOUT_L04_TRIM_20mA: 	_F_SS(171,	171,171,171,121,108,123,0,	1,	1,	1,1,1,1,1);		break;
			case MnOUT_L05_OUTP_04mA:	_F_SS(171,	171,171,171,121,108,123,0,	1,	1,	1,1,1,1,1);		break;
			case MnOUT_L06_ECHO_DLY:	_F_SS(91,	156,170,171,93, 98, 159,107,94,	166,0,1,1,1,1);		break;
			case MnOUT_L07_ECHO_CURR:	_F_SS(91,	156,170,171,106,152,119,157,154,98,	0,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i], i+1, "");
	}
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L01_SET_04mA],  10, MnSTR_Y_HEIGHT*c++, " 4mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L02_SET_20mA],  10, MnSTR_Y_HEIGHT*c++, "20mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L03_TRIM_12mA], 10, MnSTR_Y_HEIGHT*c++, "12mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L04_TRIM_20mA], 10, MnSTR_Y_HEIGHT*c++, "20mA");
	DpSTR_DrawLin(FT_EN_08x16, pSel[MnOUT_L05_OUTP_04mA], 10, MnSTR_Y_HEIGHT*c++, "4mA");
}

void OutDp_ListJpnP2(U08 *pSel)
{
	U08 i;
	U08 c = 1;
										//	 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, 0, "         OUTPUT #2/2           ");

	for(i=(MNU_LIST_NUM+1); i<=(MNU_LIST_NUM*2); i++)
	{
		switch(i)
		{
			case MnOUT_L08_R1_ACT:		_F_SS(171,	171,110,118,123,0,	1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L09_R1_STOP:		_F_SS(171,	171,157,110,118,123,0,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L10_R2_ACT:		_F_SS(171,	171,110,118,123,0,	1,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L11_R2_STOP:		_F_SS(171,	171,157,110,118,123,0,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L12_RLY_TEST: 	_F_SS(157,	159,170,121,108,123,0,1,1,1,1,1,1,1,1);		break;
			case MnOUT_L13_PROTOCOL: 	_F_SS(138,	160,123,102,158,0,  1,1,1,1,1,1,1,1,1);		break;
		}
		MnLST_DispStr(FT_LC_16x16, pSel[i-MNU_LIST_NUM], i-MNU_LIST_NUM+1, "");
	}
	OutDp_ListCmmn(pSel, c);
}

#endif
#endif

void OutDp_ProcList(U08 sel)
{
	U08 i;
	U08 page = MNU_PAGE_01;
	U08 pSel[MnOUT_LST_MAX] = {0, };

	pSel[sel] = TRUE;

	if		(sel > (MNU_LIST_NUM*2))		page = MNU_PAGE_03;
	else if (sel > (MNU_LIST_NUM*1))		page = MNU_PAGE_02;
	else									page = MNU_PAGE_01;

	if		(sel > (MNU_LIST_NUM*2))		pSel[sel-(MNU_LIST_NUM*2)] = TRUE;
	else if	(sel > (MNU_LIST_NUM*1))		pSel[sel-(MNU_LIST_NUM*1)] = TRUE;
	else									pSel[sel-(MNU_LIST_NUM*0)] = TRUE;

	if		(sel > (MNU_LIST_NUM*2))		page = MNU_PAGE_03;
	else if (sel > (MNU_LIST_NUM*1))		page = MNU_PAGE_02;
	else									page = MNU_PAGE_01;

#if 1
	switch(page)
	{
		case MNU_PAGE_01:		OutDp_ListP1(pSel);		break;
		case MNU_PAGE_02:		OutDp_ListP2(pSel);		break;
		default:
			return;
	}
#else
	if(page > MNU_PAGE_02)
		return;

	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
			switch(page)
			{
#if defined (_FT_LANG_KR)
				case MNU_PAGE_01:		OutDp_ListKorP1(pSel);		break;
				case MNU_PAGE_02:		OutDp_ListKorP2(pSel);		break;

#elif defined (_FT_LANG_CN)
				case MNU_PAGE_01:		OutDp_ListChnP1(pSel);		break;
				case MNU_PAGE_02:		OutDp_ListChnP2(pSel);		break;

#elif defined (_FT_LANG_JP)
				case MNU_PAGE_01:		OutDp_ListJpnP1(pSel);		break;
				case MNU_PAGE_02:		OutDp_ListJpnP2(pSel);		break;

#endif
			}

			break;
		case MnSYS_LANG_ENG:
		default:
			switch(page)
			{
				case MNU_PAGE_01:		OutDp_ListEngP1(pSel);		break;
				case MNU_PAGE_02:		OutDp_ListEngP2(pSel);		break;
			}
			break;
	}
#endif
	if(page == MNU_PAGE_02)
	{
		for(i=(MnOUT_LST_MAX+1-7); i<=(MNU_LIST_NUM+1); i++)
			MnLST_DispClr(i);
	}

    LDT_UpdBuff(LCD_Y_START,LCD_Y_END);
	DLY_RunMsec(100);
}

void OutChk_Vari(void)
{
	U08 maxRange  = MnFTR_GetDistMax();
	
	// Page 1
	switch(maxRange)
	{
		case MnFTR_DIST_MAX_10m:
			if(lMnOut.set_04mA  > MnOUT_SET_CURR_MT_10_MAX)	lMnOut.set_04mA  = MnOUT_SET_04mA_MT_DFT;
			if(lMnOut.set_20mA  > MnOUT_SET_CURR_MT_20_MAX)	lMnOut.set_20mA  = MnOUT_SET_20mA_MT_DFT;
			break;
		case MnFTR_DIST_MAX_20m:
			if(lMnOut.set_04mA  > MnOUT_SET_CURR_MT_10_MAX)	lMnOut.set_04mA  = MnOUT_SET_04mA_MT_DFT;
			if(lMnOut.set_20mA  > MnOUT_SET_CURR_MT_20_MAX)	lMnOut.set_20mA  = MnOUT_SET_20mA_MT_DFT;
			break;
	}
	
	if(lMnOut.trim_12mA > MnOUT_TRIM_CURR_MAX)		lMnOut.trim_12mA = MnOUT_TRIM_CURR_DFT;
	if(lMnOut.trim_20mA > MnOUT_TRIM_CURR_MAX)		lMnOut.trim_20mA = MnOUT_TRIM_CURR_DFT;
	if(lMnOut.echo_dly  < MnOUT_ECHO_DLY_MIN)		lMnOut.echo_dly  = MnOUT_ECHO_DLY_DFT;
	if(lMnOut.echo_dly  > MnOUT_ECHO_DLY_MAX)		lMnOut.echo_dly  = MnOUT_ECHO_DLY_DFT;
	if(lMnOut.echo_out >= MnOUT_ECHO_OUT_MAX)		lMnOut.echo_out  = MnOUT_ECHO_OUT_HOLD;

	// Page 2
	switch(maxRange)
	{
		case MnFTR_DIST_MAX_10m:
			if(lMnOut.rly1_act > MnOUT_RLY_OPER_MT_10_MAX)	lMnOut.rly1_act = MnOUT_RLY1_ACT_DFT;
			if(lMnOut.rly1_stp > MnOUT_RLY_OPER_MT_10_MAX)	lMnOut.rly1_stp = MnOUT_RLY1_STP_DFT;
			if(lMnOut.rly2_act > MnOUT_RLY_OPER_MT_10_MAX)	lMnOut.rly2_act = MnOUT_RLY2_ACT_DFT;
			if(lMnOut.rly2_stp > MnOUT_RLY_OPER_MT_10_MAX)	lMnOut.rly2_stp = MnOUT_RLY2_STP_DFT;
			break;
		case MnFTR_DIST_MAX_20m:
			if(lMnOut.rly1_act > MnOUT_RLY_OPER_MT_20_MAX)	lMnOut.rly1_act = MnOUT_RLY1_ACT_DFT;
			if(lMnOut.rly1_stp > MnOUT_RLY_OPER_MT_20_MAX)	lMnOut.rly1_stp = MnOUT_RLY1_STP_DFT;
			if(lMnOut.rly2_act > MnOUT_RLY_OPER_MT_20_MAX)	lMnOut.rly2_act = MnOUT_RLY2_ACT_DFT;
			if(lMnOut.rly2_stp > MnOUT_RLY_OPER_MT_20_MAX)	lMnOut.rly2_stp = MnOUT_RLY2_STP_DFT;
			break;
	}
	
	if(lMnOut.protocol >= MnOUT_PROT_MAX)			lMnOut.protocol = MnOUT_PROT_STD;
	if(lMnOut.mdb_addr >  MnOUT_MDB_ADDR_MAX)		lMnOut.mdb_addr = MnOUT_MDB_ADDR_DFT;
	if(lMnOut.mdb_baud >= MnOUT_MDB_BAUD_MAX)		lMnOut.mdb_baud = MnOUT_MDB_BAUD_09600;
	if(lMnOut.rf_ch    >  MnOUT_RF_CH_MAX)			lMnOut.rf_ch    = MnOUT_RF_CH_DFT;
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Get Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Page 1
U16 MnOUT_GetSet04mA(void)		{	return lMnOut.set_04mA;		}
U16 MnOUT_GetSet20mA(void)		{	return lMnOut.set_20mA;		}
U08 MnOUT_GetTrim12mA(void)		{	return lMnOut.trim_12mA;	}
U08 MnOUT_GetTrim20mA(void)		{	return lMnOut.trim_20mA;	}
U16 MnOUT_GetEchoDly(void)		{	return lMnOut.echo_dly;		}
U08 MnOUT_GetEchoOut(void)		{	return lMnOut.echo_out;		}
// Page 2
U16 MnOUT_GetRly1Act(void)		{	return lMnOut.rly1_act;		}
U16 MnOUT_GetRly1Stp(void)		{	return lMnOut.rly1_stp;		}
U16 MnOUT_GetRly2Act(void)		{	return lMnOut.rly2_act;		}
U16 MnOUT_GetRly2Stp(void)		{	return lMnOut.rly2_stp;		}
U08 MnOUT_GetProtocol(void)		{	return lMnOut.protocol;		}
U08 MnOUT_GetMdbAddr(void)		{	return lMnOut.mdb_addr;		}
U08 MnOUT_GetMdbBaud(void)		{	return lMnOut.mdb_baud;		}
U08 MnOUT_GetRfCh(void)			{	return lMnOut.rf_ch;		}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Get Local Variables
//------------------------------------------------------------------------------------------------------------------------------
void MnOUT_SetSet20mA(U16 val)		{	lMnOut.set_20mA = val;		}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Memory Access
//------------------------------------------------------------------------------------------------------------------------------
void MnOUT_MemLoad(void)
{
	// Page 1
	lMnOut.set_04mA  = FLS_Rd2Byte(F006_OUT_SET_04mA_L);
	lMnOut.set_20mA  = FLS_Rd2Byte(F008_OUT_SET_20mA_L);
	lMnOut.trim_12mA = FLS_Rd1Byte(F045_OUT_TRIM_12mA);
	lMnOut.trim_20mA = FLS_Rd1Byte(F046_OUT_TRIM_20mA);
	lMnOut.echo_dly  = FLS_Rd2Byte(F022_OUT_ECHO_DLY_L);
	lMnOut.echo_out  = FLS_Rd1Byte(F024_OUT_ECHO_OUTP);
	// Page 2
	lMnOut.rly1_act = FLS_Rd2Byte(F014_OUT_RLY1_ACT_L);
	lMnOut.rly1_stp = FLS_Rd2Byte(F016_OUT_RLY1_STP_L);
	lMnOut.rly2_act = FLS_Rd2Byte(F018_OUT_RLY2_ACT_L);
	lMnOut.rly2_stp = FLS_Rd2Byte(F020_OUT_RLY2_STP_L);
	lMnOut.protocol = FLS_Rd1Byte(F011_OUT_PROTOCOL);
	lMnOut.mdb_addr = FLS_Rd1Byte(F099_OUT_MDB_ADDR);
	lMnOut.mdb_baud = FLS_Rd1Byte(F098_OUT_MDB_BAUD);
	lMnOut.rf_ch	= FLS_Rd1Byte(F012_OUT_RF_CH);
}

void MnOUT_MemSave(void)
{
	// Page 1
	FLS_Wr2Byte(F006_OUT_SET_04mA_L, lMnOut.set_04mA);
	FLS_Wr2Byte(F008_OUT_SET_20mA_L, lMnOut.set_20mA);
	FLS_Wr1Byte(F045_OUT_TRIM_12mA,  lMnOut.trim_12mA);
	FLS_Wr1Byte(F046_OUT_TRIM_20mA,  lMnOut.trim_20mA);
	FLS_Wr2Byte(F022_OUT_ECHO_DLY_L, lMnOut.echo_dly);
	FLS_Wr1Byte(F024_OUT_ECHO_OUTP,  lMnOut.echo_out);
	// Page 2
	FLS_Wr2Byte(F014_OUT_RLY1_ACT_L, lMnOut.rly1_act);
	FLS_Wr2Byte(F016_OUT_RLY1_STP_L, lMnOut.rly1_stp);
	FLS_Wr2Byte(F018_OUT_RLY2_ACT_L, lMnOut.rly2_act);
	FLS_Wr2Byte(F020_OUT_RLY2_STP_L, lMnOut.rly2_stp);
	FLS_Wr1Byte(F011_OUT_PROTOCOL,   lMnOut.protocol);
	FLS_Wr1Byte(F099_OUT_MDB_ADDR,   lMnOut.mdb_addr);
	FLS_Wr1Byte(F098_OUT_MDB_BAUD,   lMnOut.mdb_baud);
	FLS_Wr1Byte(F012_OUT_RF_CH, 	 lMnOut.rf_ch);
}

void MnOUT_MemFtry(void)
{
	// Page 1
	FLS_Wr2Byte(F006_OUT_SET_04mA_L, MnOUT_SET_04mA_MT_DFT);
	FLS_Wr2Byte(F008_OUT_SET_20mA_L, MnOUT_SET_20mA_MT_DFT);
	FLS_Wr1Byte(F045_OUT_TRIM_12mA,  MnOUT_TRIM_CURR_DFT);
	FLS_Wr1Byte(F046_OUT_TRIM_20mA,  MnOUT_TRIM_CURR_DFT);
	FLS_Wr2Byte(F022_OUT_ECHO_DLY_L, MnOUT_ECHO_DLY_DFT);
	FLS_Wr1Byte(F024_OUT_ECHO_OUTP,  MnOUT_ECHO_OUT_HOLD);
	// Page 2
	FLS_Wr2Byte(F014_OUT_RLY1_ACT_L, MnOUT_RLY1_ACT_DFT);
	FLS_Wr2Byte(F016_OUT_RLY1_STP_L, MnOUT_RLY1_STP_DFT);
	FLS_Wr2Byte(F018_OUT_RLY2_ACT_L, MnOUT_RLY2_ACT_DFT);
	FLS_Wr2Byte(F020_OUT_RLY2_STP_L, MnOUT_RLY2_STP_DFT);
	FLS_Wr1Byte(F011_OUT_PROTOCOL,   MnOUT_PROT_STD);
	FLS_Wr1Byte(F099_OUT_MDB_ADDR,   MnOUT_MDB_ADDR_DFT);
	FLS_Wr1Byte(F098_OUT_MDB_BAUD,   MnOUT_MDB_BAUD_09600);
	FLS_Wr1Byte(F012_OUT_RF_CH,		 MnOUT_RF_CH_DFT);
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MnOUT_InitVari(void)
{
	MnOUT_MemLoad();
	OutChk_Vari();
}

void MnOUT_ProcMain(void)
{
	U08 fDp = FALSE;
	U08 sel = MnOUT_L00_IDLE + 1;

	OutDp_ProcList(sel);

	do
	{
		if(!_SBIT_BTN_DOWN)
		{
			if(sel < (MnOUT_LST_MAX-1))		sel++;
			else							sel = MnOUT_L00_IDLE + 1;
			fDp = TRUE;
		}

		if(!_SBIT_BTN_UP)
		{
			if(sel > (MnOUT_L00_IDLE+1))	sel--;
			else							sel = (MnOUT_LST_MAX-1);
			fDp = TRUE;
		}

		if(!_SBIT_BTN_SET)
	    {
			MnBOX_DispBox();

			switch(sel)
			{	// Page 1
				case MnOUT_L01_SET_04mA:
				case MnOUT_L02_SET_20mA:	MnOut_SetCurr(sel);		break;
				case MnOUT_L03_TRIM_12mA:
				case MnOUT_L04_TRIM_20mA:	MnOut_TrimCurr(sel);	break;
				case MnOUT_L05_OUTP_04mA:	MnOut_Outp04mA();		break;
				case MnOUT_L06_ECHO_DLY:	MnOut_EchoDelay();		break;
				case MnOUT_L07_ECHO_CURR:	MnOut_EchoCurr();		break;
				// Page 2
				case MnOUT_L08_R1_ACT:		MnOut_RlyOper(sel);		break;		
				case MnOUT_L09_R1_STOP:		MnOut_RlyOper(sel);		break;
				case MnOUT_L10_R2_ACT:		MnOut_RlyOper(sel);		break;
				case MnOUT_L11_R2_STOP:		MnOut_RlyOper(sel);		break;
				case MnOUT_L12_RLY_TEST:	MnOut_RelayTest();		break;
				case MnOUT_L13_PROTOCOL:	MnOut_Protocol();		break;
			}
			fDp = TRUE;
		}

		if(fDp)
		{
			OutDp_ProcList(sel);
			fDp = FALSE;
		}

	}
	while(_SBIT_BTN_EXIT);
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

