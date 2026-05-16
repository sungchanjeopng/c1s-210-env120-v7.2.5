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
#include "menu_box.h"
// bsp
#include "bsp_main.h"
#include "gpio_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "dly_main.h"
#include "btn_main.h"
// font
#include "font_def.h"
// disp
#include "disp_main.h"
#include "disp_string.h"
// menu
#include "menu_system.h"
#include "menu_measure.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Access Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Get
// Set


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MnBOX_DispBox(void)
{
	U08 old = SFRPAGE;

	SFRPAGE = CONFIG_PAGE;

	LDT_ClrBuff(MnBOX_LEFT-5, MnBOX_TOP-3, MnBOX_RIGHT+3, MnBOX_BOTTOM+3);
	LDT_DrawBox(MnBOX_LEFT-3, MnBOX_TOP-1, MnBOX_RIGHT+1, MnBOX_BOTTOM+1);
	LDT_DrawBox(MnBOX_LEFT-2, MnBOX_TOP,   MnBOX_RIGHT,   MnBOX_BOTTOM);
	LDT_UpdBuff(LCD_Y_START, LCD_Y_END);

	SFRPAGE = old;
}

void MnBOX_DispStr(U08 lang, U16 x0, U08 y0, U08 fSel, U08 *pStr)
{
	LDT_ClrBuff(MnBOX_LEFT, MnBOX_TOP+2+32, MnBOX_RIGHT-2, MnBOX_BOTTOM-2);

	if	(lang == MnSYS_LANG_ENG)	DpSTR_DrawLin(FT_EN_08x16, fSel, x0, y0, pStr);
	else							LCD_WrStrBuff(x0, y0, FT_LC_16x16, pStr, fSel, 0);

	LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
}

S32 MnBOX_CfgNumb(S32 num, S32 min, S32 max, U16 step, U08 place, U08 fSel, U08 *pStr)
{
    S16 old = num;
    U08 cnt_ud = 0;

    DpSTR_NumbUpd(MnBOX_Y03, num, max, place, fSel, pStr);
    BTN_DelayMsec(BTN_WAIT_SET);

	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT)
	{
		if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{
			if(++cnt_ud >= 30)
				cnt_ud = 30;

			num = BTN_GetValue_UpDn(num, min, max, step, cnt_ud);

			DpSTR_NumbUpd(MnBOX_Y03, num, max, place, fSel, pStr);
			BTN_DelayMsec(BTN_WAIT_UD);
		}
		else
			cnt_ud = 0;
	}

	if(!_SBIT_BTN_EXIT)
		num = old;

	return num;
}

U16 MnBOX_SelArg4(U08 lang, U16 sel, U16 x1, U08 *pS1, U16 x2, U08 *pS2, U16 x3, U08 *pS3, U16 x4, U08 *pS4, U08 num)
{
	U08 aBuff[4];
	U08 *pData[4];
	U16 old = sel;

	aBuff[0] = x1;     pData[0] = pS1;
	aBuff[1] = x2;     pData[1] = pS2;
	aBuff[2] = x3;     pData[2] = pS3;
	aBuff[3] = x4;     pData[3] = pS4;

	MnBOX_DispStr(lang, aBuff[sel], MnBOX_Y03, F_F, pData[sel]);
	BTN_DelayMsec(BTN_WAIT_SET);

	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT)
	{
	    if(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
	    {
	        if(!_SBIT_BTN_UP)
	        {
	            if(sel >= (num - 1))		sel = 0;
	            else                        sel++;
	        }
	        if(!_SBIT_BTN_DOWN)
	        {
	            if(sel <= 0)				sel = num - 1;
	            else                        sel--;
	        }

	        MnBOX_DispStr(lang, aBuff[sel], MnBOX_Y03, F_F, pData[sel]);
	        DLY_RunMsec(BTN_WAIT_UD);
	    }
	}

	if(!_SBIT_BTN_EXIT)
	    sel = old;

	return sel;
}



//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

