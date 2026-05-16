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
#include "menu_pass.h"
// bsp
#include "flash_main.h"
#include "gpio_main.h"
#include "bsp_main.h"
#include "btn_main.h"
// app
#include "app_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "dly_main.h"
// disp
#include "disp_main.h"
#include "disp_string.h"
// menu
#include "menu_main.h"
#include "menu_system.h"
// font
#include "font_def.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------
U08 gPas_fChk;				// OLD: unsigned char Flag_Pass_CHK_UC_G;


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------
U16 MnuPs_UdPass(S16 ud_aux, U08 ud_cnt)
{
	U08 p_str[APP_STR_LENGTH] = {0, };

    if(ud_cnt < 10)
    {
        if(_SBIT_BTN_UP   == BTN_STT_PRESS)    ud_aux++;
        if(_SBIT_BTN_DOWN == BTN_STT_PRESS)    ud_aux--;
    }

    if((ud_cnt >= 10) && (ud_cnt < 20))
    {
        if(_SBIT_BTN_UP   == BTN_STT_PRESS)    ud_aux += 10;
        if(_SBIT_BTN_DOWN == BTN_STT_PRESS)    ud_aux -= 10;
    }

    if((ud_cnt >= 20) && (ud_cnt < 30))
    {

        if(_SBIT_BTN_UP   == BTN_STT_PRESS)    ud_aux += 100;
        if(_SBIT_BTN_DOWN == BTN_STT_PRESS)    ud_aux -= 100;
    }
	if(ud_cnt>= 30)
	{
        ud_cnt = 30;

        if(_SBIT_BTN_UP   == BTN_STT_PRESS)    ud_aux += 1000;
        if(_SBIT_BTN_DOWN == BTN_STT_PRESS)    ud_aux -= 1000;

	}
	
	

    if(ud_aux >= 9999)      ud_aux = 9999;
    if(ud_aux <= 0)         ud_aux = 0;

	APP_Num2Str(ud_aux, p_str, 4, 0, TRUE);
	DpSTR_LineUpd(FT_EN_08x16, 0, 104, 56, p_str);
    BTN_DelayMsec(BTN_WAIT_UD);

    return ud_aux;
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Operation
//------------------------------------------------------------------------------------------------------------------------------
void MnPAS_InitVari(void)
{

}

void MnPAS_ProcMain(void)
{
    U08 cnt_ud = 0;
	U16 sel = 0;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U16 pass = MnSYS_GetPass();

	gPas_fChk = FALSE;

    DISP_ClearScr();



	switch(MnSYS_GetLang())
	{
		case MnSYS_LANG_LOC:
#if defined (_FT_LANG_KR)			
			DpSTR_LineUpd(FT_LC_16x16, FALSE, 80, 40,_F_STR(44, 35, 38, 105, 0,	0, 0,0,0,0,0,0,0,0,0));
			DpSTR_LineUpd(FT_EN_08x16, FALSE, 80+70, 40, "?");
			break;

#elif defined (_FT_LANG_CN)
			DpSTR_LineUpd(FT_LC_16x16, FALSE, 100, 40,_F_STR(29, 71, 0, 0, 0,	0, 0,0,0,0,0,0,0,0,0));
			DpSTR_LineUpd(FT_EN_08x16, FALSE, 100+35, 40, "?");
			break;

#elif defined (_FT_LANG_JP)
			DpSTR_LineUpd(FT_LC_16x16, FALSE, 80, 40,_F_STR(26, 15, 40, 42, 23,	0, 0,0,0,0,0,0,0,0,0));
			DpSTR_LineUpd(FT_EN_08x16, FALSE, 80+85, 40, "?");
			break;
#endif
		case MnSYS_LANG_ENG:
		default:
			DpSTR_LineUpd(FT_EN_08x16, FALSE, 80, 40, "Passcode?");
			break;
	}
	APP_Num2Str(0, p_str, 4, 0, TRUE);
	DpSTR_LineUpd(FT_EN_08x16, FALSE, 104, 56, p_str);
    DLY_RunMsec(300);

    do
    {
        while(!_SBIT_BTN_UP || !_SBIT_BTN_DOWN)
		{
		    sel = MnuPs_UdPass(sel, cnt_ud);
		    cnt_ud++;
		}
        cnt_ud = 0;

    }
	while(_SBIT_BTN_SET && _SBIT_BTN_EXIT);

    if(!_SBIT_BTN_SET)
    {
        if((sel == pass) || (sel == MnSYS_PASS_GOLD_KEY))		gPas_fChk = TRUE;
        else													gPas_fChk = FALSE;

        LDT_ClrBuff(LCD_X_START, LCD_Y_START, LCD_X_END, LCD_Y_END);

        if(gPas_fChk)		DpSTR_LineUpd(FT_EN_08x16, 0, 100, 55, "Okay !");	
		else				DpSTR_LineUpd(FT_EN_08x16, 0, 80,  55, "Invalid !!");
		DLY_RunMsec(1000);
    }
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

