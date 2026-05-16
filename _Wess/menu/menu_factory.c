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
// self
#include "menu_factory.h"
// bsp
#include "font_def.h"
#include "bsp_main.h"
#include "flash_main.h"
#include "gpio_main.h"
#include "dac_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "btn_main.h"
#include "dly_main.h"
// disp
#include "disp_menu.h"
#include "disp_string.h"
// menu
#include "menu_main.h"
#include "menu_system.h"
#include "menu_box.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
LS_MnFTR lMnFtr;


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------
void FtrP1_L01_02_SetDac(U08 lst)
{
	U16 old;
	U16 sel;
	U16 max;
	U16 min;
	U08 cnt = 0;

	switch(lst)
	{
		case MnFTR_L01_DAC_04mA:	old = lMnFtr.dac_04mA;			break;
		case MnFTR_L02_DAC_20mA:	old = lMnFtr.dac_20mA;			break;
		default:					return;
	}

	switch(lst)
	{
		case MnFTR_L01_DAC_04mA:	min = MnFTR_DAC_04mA_MIN;	max = MnFTR_DAC_04mA_MAX;		break;
		case MnFTR_L02_DAC_20mA:	min = MnFTR_DAC_20mA_MIN;	max = MnFTR_DAC_20mA_MAX;		break;
	}

	sel = old;

	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X10, MnBOX_Y01, F_F, "DAC");
	DpSTR_NumbUpd(MnBOX_Y03, sel, max, 0, F_F, "");

	DAC_OutSpi(sel);
	DLY_RunMsec(BTN_WAIT_UD);

	do 
	{
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{
			if(++cnt >= 30)
				cnt = 30;

			sel = BTN_GetValue_UpDn(sel, min, max, 1, cnt);
			DpSTR_NumbUpd(MnBOX_Y03, sel, max, 0, F_F, "");
			DAC_OutSpi(sel);

			BTN_DelayMsec(BTN_WAIT_UD);
		} 
		else
			cnt = 0;
	} 
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

	if((sel != old) && !_SBIT_BTN_SET)
	{
		switch(lst)
		{
			case MnFTR_L01_DAC_04mA:	lMnFtr.dac_04mA = sel; 		break;
			case MnFTR_L02_DAC_20mA:	lMnFtr.dac_20mA = sel; 		break;
		}

		lMnFtr.dac_38mA = lMnFtr.dac_04mA - ((((lMnFtr.dac_20mA - lMnFtr.dac_04mA) + 5) / 8) / 10);
		lMnFtr.dac_21mA = lMnFtr.dac_20mA + (  (lMnFtr.dac_20mA - lMnFtr.dac_04mA) / 16);
	}
}

void FtrP1_L03_DistMax(void)
{
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X06, MnBOX_Y01, F_F, "Distance");

	lMnFtr.dist_max = MnBOX_SelArg4(MnSYS_LANG_ENG, lMnFtr.dist_max, MnBOX_X10, "10m", MnBOX_X10, "20m", 0, "", 0, "", 2);
}

void MnFtrDp_List(U08 sel)
{
	U08 i;
	U08 c  = MnFTR_L00_TITLE + 1;
	U08 ft = FT_EN_08x16;
	U08 pSel[MnFTR_LST_MAX] = {0, };

	pSel[sel] = TRUE;
													 //  0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,	 MnFTR_L00_TITLE+1,		"         FACTORY               ");
	MnLST_DispStr(ft, pSel[c++], MnFTR_L01_DAC_04mA+1,	"DAC 4mA                        ");
	MnLST_DispStr(ft, pSel[c++], MnFTR_L02_DAC_20mA+1,	"DAC 20mA                       ");
	MnLST_DispStr(ft, pSel[c++], MnFTR_L03_DIST_MAX+1,	"Max. Distance                  ");

	for(i=MnFTR_LST_MAX+1; i<=MNU_LIST_NUM+1; i++)
		MnLST_DispClr(i);

	LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
	DLY_RunMsec(250);
}

void MnFtrCk_Vari(void)
{
	if		(lMnFtr.dac_04mA >  MnFTR_DAC_04mA_MAX)		lMnFtr.dac_04mA = MnFTR_DAC_04mA_DFT;
	else if	(lMnFtr.dac_04mA <  MnFTR_DAC_04mA_MIN)		lMnFtr.dac_04mA = MnFTR_DAC_04mA_DFT;
	if		(lMnFtr.dac_20mA >  MnFTR_DAC_20mA_MAX)		lMnFtr.dac_20mA = MnFTR_DAC_20mA_DFT;
	else if	(lMnFtr.dac_20mA <  MnFTR_DAC_20mA_MIN)		lMnFtr.dac_20mA = MnFTR_DAC_20mA_DFT;
	if		(lMnFtr.dist_max >= MnFTR_DIST_MAX_MAX)		lMnFtr.dist_max = MnFTR_DIST_MAX_10m;
	
	lMnFtr.dac_21mA = lMnFtr.dac_20mA +   ((lMnFtr.dac_20mA - lMnFtr.dac_04mA) / 16);
	lMnFtr.dac_38mA = lMnFtr.dac_04mA - ((((lMnFtr.dac_20mA - lMnFtr.dac_04mA) + 5) / 8) / 10);
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Access Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Get
U16 MnFTR_GetDac04mA(void)		{	return lMnFtr.dac_04mA;		}
U16 MnFTR_GetDac20mA(void)		{	return lMnFtr.dac_20mA;		}
U16 MnFTR_GetDac38mA(void)		{	return lMnFtr.dac_38mA;		}
U16 MnFTR_GetDac21mA(void)		{	return lMnFtr.dac_21mA;		}
U08 MnFTR_GetDistMax(void)		{	return lMnFtr.dist_max;		}
// Set


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MnFTR_MemLoad(void)
{
	lMnFtr.dac_04mA = FLS_Rd2Byte(F065_FTR_DAC_04mA_L);
	lMnFtr.dac_20mA = FLS_Rd2Byte(F067_FTR_DAC_20mA_L);
	lMnFtr.dac_38mA = FLS_Rd2Byte(F069_FTR_DAC_38mA_L);
	lMnFtr.dac_21mA = FLS_Rd2Byte(F071_FTR_DAC_21mA_L);
	
	lMnFtr.dist_max = FLS_Rd1Byte(F101_FTR_DIST_MAX);
}

void MnFTR_MemSave(void)
{
	FLS_Wr2Byte(F065_FTR_DAC_04mA_L, lMnFtr.dac_04mA);
	FLS_Wr2Byte(F067_FTR_DAC_20mA_L, lMnFtr.dac_20mA);
	FLS_Wr2Byte(F069_FTR_DAC_38mA_L, lMnFtr.dac_38mA);
	FLS_Wr2Byte(F071_FTR_DAC_21mA_L, lMnFtr.dac_21mA);
	
	FLS_Wr1Byte(F101_FTR_DIST_MAX, lMnFtr.dist_max);
}

void MnFTR_MemFtry(void)
{
	U16 dac_38mA = MnFTR_DAC_04mA_DFT - ((((MnFTR_DAC_20mA_DFT - MnFTR_DAC_04mA_DFT) + 5) / 8) / 10);
	U16 dac_21mA = MnFTR_DAC_20mA_DFT + (  (MnFTR_DAC_20mA_DFT - MnFTR_DAC_04mA_DFT) / 16);

	FLS_Wr2Byte(F065_FTR_DAC_04mA_L, MnFTR_DAC_04mA_DFT);
	FLS_Wr2Byte(F067_FTR_DAC_20mA_L, MnFTR_DAC_20mA_DFT);
	FLS_Wr2Byte(F069_FTR_DAC_38mA_L, dac_38mA);
	FLS_Wr2Byte(F071_FTR_DAC_21mA_L, dac_21mA);
	FLS_Wr1Byte(F101_FTR_DIST_MAX,   MnFTR_DIST_MAX_10m);
}

void MnFTR_InitVari(void)
{
	MnFTR_MemLoad();
	MnFtrCk_Vari();
}

void MnFTR_ProcMain(void)
{
	U08 sel = MnFTR_L00_TITLE+1;
	U08 fDp = FALSE;

	MnFtrDp_List(sel);

	do
	{
		if(!_SBIT_BTN_DOWN)
		{
			if(sel < (MnFTR_LST_MAX-1)) 		sel++;
			else								sel = MnFTR_L00_TITLE + 1;
			fDp = TRUE;
		}

		if(!_SBIT_BTN_UP)
		{
			if(sel > (MnFTR_L00_TITLE+1))		sel--;
			else								sel = MnFTR_LST_MAX - 1;
			fDp = TRUE;
		}

		if(!_SBIT_BTN_SET)
		{
			MnBOX_DispBox();
			
			switch(sel)
			{	// Page 1
				case MnFTR_L01_DAC_04mA:		FtrP1_L01_02_SetDac(sel);		break;
				case MnFTR_L02_DAC_20mA:		FtrP1_L01_02_SetDac(sel);		break;
				case MnFTR_L03_DIST_MAX:		FtrP1_L03_DistMax();			break;
			}
			
			fDp = TRUE;
		}

		if(fDp)
		{
			fDp = FALSE;
			MnFtrDp_List(sel);
		}
	} 
	while(_SBIT_BTN_EXIT);

}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

