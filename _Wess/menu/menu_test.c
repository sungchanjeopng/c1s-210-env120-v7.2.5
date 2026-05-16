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
#include "menu_test.h"
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


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
LS_MnTST lMnTst;


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------
void MnTst_Global_Cnt(void)
{
	MnBOX_DispBox();
	MnBOX_DispStr(MnSYS_LANG_ENG, MnBOX_X08, MnBOX_Y01, F_F, "Enable");

	lMnTst.Gloval_fCnt = MnBOX_SelArg4(MnSYS_LANG_ENG, lMnTst.Gloval_fCnt, MnBOX_X11, "NO", MnBOX_X10, "YES", 0, "", 0, "", 2);
	
}

//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions - Disp. List
//------------------------------------------------------------------------------------------------------------------------------
void TstDp_ListP1(U08* pSel)
{
	U08 *pStr = "   ";
	U08 c  = MnTST_L00_IDLE + 1;
	U08 ft = FT_EN_08x16;
														//   0123456789abcdef0123456789abcde
	MnLST_DispStr(ft, FALSE,	 MnTST_L00_IDLE+1,			"           TEST                ");
	MnLST_DispStr(ft, pSel[c++], MnTST_L01_GLOVAL_CNT+1,	"Global Counter                 ");


	if	(lMnTst.Gloval_fCnt)	pStr = "ON";
	else						pStr = "OFF";

	DpSTR_DrawLin(FT_EN_08x16, pSel[MnTST_L01_GLOVAL_CNT], 180, MnSTR_Y_HEIGHT*1,pStr);	


}



void MnTstDp_List(U08 sel)
{
	U08 i;
	U08 page = MNU_PAGE_01;
	U08 font = FT_EN_08x16;
	U08 pSel[MnTST_LST_MAX] = {0, };

	pSel[sel] = TRUE;

	if		(sel > (MNU_LIST_NUM*2))		page = MNU_PAGE_03;
	else if (sel > (MNU_LIST_NUM*1))		page = MNU_PAGE_02;
	else									page = MNU_PAGE_01;

	if		(sel > (MNU_LIST_NUM*2))		pSel[sel-(MNU_LIST_NUM*2)] = TRUE;
	else if	(sel > (MNU_LIST_NUM*1))		pSel[sel-(MNU_LIST_NUM*1)] = TRUE;
	else									pSel[sel-(MNU_LIST_NUM*0)] = TRUE;

	if(page > MNU_PAGE_01)
		return;

	TstDp_ListP1(pSel);

	for(i=(MnTST_LST_MAX+1); i<=(MNU_LIST_NUM+1); i++)
		MnLST_DispClr(i);

    LDT_UpdBuff(LCD_Y_START,LCD_Y_END);
	DLY_RunMsec(100);


}




void MnTSTCk_Vari(void)
{	
	if(lMnTst.Gloval_fCnt > TRUE)						lMnTst.Gloval_fCnt = FALSE;

}

//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Access Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Get
U08 MnTST_GetGloval_fCnt(void)	{	return lMnTst.Gloval_fCnt;	}
// Set


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Memory Access
//------------------------------------------------------------------------------------------------------------------------------
void MnTST_MemLoad(void)
{
	U08 i=0;

	lMnTst.Gloval_fCnt = FLS_Rd1Byte(F102_GLOVAL_CNT);
	


}

void MnTST_MemSave(void)
{
	U08 i = 0;

	FLS_Wr1Byte(F102_GLOVAL_CNT, lMnTst.Gloval_fCnt);

}


void MnTST_MemFtry(void)
{
	U08 i=0;

	FLS_Wr1Byte(F102_GLOVAL_CNT,  FALSE);

}

//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MnTST_InitVari(void)
{
	MnTST_MemLoad();
	MnTSTCk_Vari();

}



void MnTST_ProcMain(void)
{
	U32 cnt = 0;
	U08 fDp = FALSE;
	U08 sel = MnTST_L00_IDLE + 1;

	MnTstDp_List(sel);

	do
	{
        if(!_SBIT_BTN_DOWN)
        {
        	if(sel < MnTST_LST_MAX-1)		sel++;
        	else                            sel = MnTST_L00_IDLE+1;
			fDp = TRUE;
        }

        if(!_SBIT_BTN_UP)
        {
        	if(sel > MnTST_L00_IDLE+1)		sel--;
        	else                            sel = MnTST_LST_MAX-1;
			fDp = TRUE;
        }
		


	    if(!_SBIT_BTN_SET)
	    {
			switch(sel) 
			{	// Page 1
				case MnTST_L01_GLOVAL_CNT:		MnTst_Global_Cnt();       	break;
				// Page 2
			}
			fDp = TRUE;
	    }

		if(fDp)
		{
			MnTstDp_List(sel);
			BTN_DelayMsec(BTN_WAIT_SET);
			fDp = FALSE;			
		}
	}
	while(_SBIT_BTN_EXIT);
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

