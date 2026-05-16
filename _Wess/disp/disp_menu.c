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
#include "disp_menu.h"
// bsp
#include "flash_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
// app
#include "dly_main.h"
#include "menu_main.h"
#include "menu_system.h"
#include "menu_data.h"
#include "menu_output.h"
#include "menu_quick.h"
// disp
#include "disp_main.h"
#include "disp_string.h"
// font
#include "font_def.h"


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
void MnLST_DispClr(U08 line)
{																// 0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 0, (line-1)*MnSTR_Y_HEIGHT, "                               ");
}

#if 1
void MnLST_DispStr(U08 font, U08 fSel, U08 pos_y, U08 *pStr)
{
	U08 *pSymb = " ";
	U08 y0 = (pos_y - 1) * MnSTR_Y_HEIGHT;

	if(fSel)		pSymb = ">";
	else			pSymb = " ";

	#if 1
									       //0123456789abcdef0123456789abcde
	DpSTR_DrawLin(FT_EN_08x16, fSel, 0, y0, "                               ");
	DpSTR_DrawLin(FT_EN_08x16, fSel, 0, y0, pSymb);
	DpSTR_DrawLin(font, fSel, 8, y0, pStr);
	#else
	if(MnSYS_GetLang() == MnSYS_LANG_ENG)
	{
		DpSTR_DrawLin(font, fSel, 0, y0, pSymb);
		DpSTR_DrawLin(font, fSel, 8, y0, pStr);
	}
	else
	{										//	 0123456789abcdef0123456789abcde
		DpSTR_DrawLin(FT_EN_08x16, fSel, 0, y0, "                               ");
		DpSTR_DrawLin(FT_EN_08x16, fSel, 0, y0, pSymb);

		if(font == FT_LC_16x16)		DpSTR_DrawLin(font, fSel, 8, y0, gFt_str);
		else						DpSTR_DrawLin(font, fSel, 8, y0, pStr);
	}
	#endif
}

#else
void MnLST_DispStr(U08 font, U08 fSel, U08 pos_y, U08 *pStr)
{
	U08 *pSymb = " ";
	U08 y0 = (pos_y - 1) * MnSTR_Y_HEIGHT;

	if(fSel)		pSymb = ">";
	else			pSymb = " ";

	if(MnSYS_GetLang() == MnSYS_LANG_ENG)
	{
		DpSTR_DrawLin(font, fSel, 0, y0, pSymb);
		DpSTR_DrawLin(font, fSel, 8, y0, pStr);
	}
	else
	{										//	 0123456789abcdef0123456789abcde
		DpSTR_DrawLin(FT_EN_08x16, fSel, 0, y0, "                               ");
		DpSTR_DrawLin(FT_EN_08x16, fSel, 0, y0, pSymb);

		if(font == FT_LC_16x16)		DpSTR_DrawLin(font, fSel, 8, y0, gFt_str);
		else						DpSTR_DrawLin(font, fSel, 8, y0, pStr);
	}
}
#endif

//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

