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
#include "disp_string.h"
// font
#include "font_def.h"
// bsp
#include "lcd_main.h"
#include "lcd_data.h"
// app
#include "app_main.h"
// menu
#include "menu_box.h"
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
//  Global APIs - Operation
//------------------------------------------------------------------------------------------------------------------------------
void DpSTR_DrawLin(U08 font, U08 fSel, U08 x, U08 y, U08 *pStr)
{
	U08 space = 0;

	switch(font)
	{
		case FT_EN_06x07:		space = 0;		break;
		case FT_EN_08x16:		space = 0;		break;
		case FT_EN_26x47:		space = 3;		break;
		case FT_LC_16x16:		space = 0;		break;
	}
	
	LCD_WrStrBuff(x, y, font, pStr, fSel, space);
}

void DpSTR_LineUpd(U08 font, U08 f_rev, U08 x, U08 y, U08 *p_str)
{
	DpSTR_DrawLin(font, f_rev, x, y, p_str);
	LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
}

void DpSTR_FreqUpd(U08 sel, U08 x, U08 y)
{
	switch(sel)
	{
		case MnMSR_FREQ_130K:	DpSTR_LineUpd(FT_EN_08x16, 0, x, y, "130K");		break;
		case MnMSR_FREQ_160K:	DpSTR_LineUpd(FT_EN_08x16, 0, x, y, "160K");		break;
		case MnMSR_FREQ_270K:	DpSTR_LineUpd(FT_EN_08x16, 0, x, y, "270K");		break;
		case MnMSR_FREQ_380K:	DpSTR_LineUpd(FT_EN_08x16, 0, x, y, "380K");		break;
		// Not Used
		case MnMSR_FREQ_500K: 	DpSTR_LineUpd(FT_EN_08x16, 0, x, y, "500K");		break;
		case MnMSR_FREQ_620K:	DpSTR_LineUpd(FT_EN_08x16, 0, x, y, "620K");		break;
		case MnMSR_FREQ_750K:	DpSTR_LineUpd(FT_EN_08x16, 0, x, y, "750K");		break;
	}
}

void DpSTR_NumbUpd(U08 y0, S32 val, S32 max, U08 place, U08 fSel, U08 *pStr)
{
    U08 *p_addr;
    U16 column;
    U16 numb;
    U16 minus;
    U08 c_unit = 0;
	U08 p_str[APP_STR_LENGTH] = {0, };

    if     (max < 10)       numb = 1;
    else if(max < 100)      numb = 2;
    else if(max < 1000)     numb = 3;
    else if(max < 10000)    numb = 4;
    else                    numb = 5;

    p_addr = pStr;
    while(*pStr++ != NULL)        c_unit++;
    pStr = p_addr;

    column = MnBOX_LEFT + ((MnBOX_WIDTH - ((numb + c_unit) * 9)) / 2);

    if(place != 0)
    	column -= 4;

    if(val < 0)
    {
        minus = val * -1;

		APP_Num2Str(minus, p_str, numb, place, fSel);
        DpSTR_DrawLin(FT_EN_08x16, 0, column-9, y0, "-");
    }
    else
    {
		APP_Num2Str(val, p_str, numb, place, fSel);
        DpSTR_DrawLin(FT_EN_08x16, 0, column-9, y0, " ");
	}

	DpSTR_DrawLin(FT_EN_08x16, 0, column, y0, p_str);

    if(*pStr != NULL)
    {
        column = column + (numb * 9);

        if(place != 0)
        	column += 9;
#if 1
		DpSTR_DrawLin(FT_EN_08x16, 0, column, y0, pStr);
#else
        if(lMnSys.lang == MnSYS_LANG_CHN)
			LCD_WrStrBuff(column, row, FT_LC_16x16, p_unit, 0, 0);
        else
			DpSTR_DrawLin(FT_EN_08x16, 0, column, row, p_unit);
#endif
    }

	LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

