//--------------------------------------------------------------------------------------------------
//  Copyright (C) 2020, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use,
//  copying, transfer or disclosure of such information is prohibited except by express written
//  agreement with WESS-Global Co.,LTD
//
//  $Date: 2020-07-13 12:00:00 +0900 $
//  $Author: eddie $
//  $Rev: 1.0 $
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------------------------------------
// chipset
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// self
#include "btn_main.h"
// lib
#include "dly_main.h"
// bsp
#include "bsp_main.h"
#include "flash_main.h"
#include "gpio_main.h"
#include "mem_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
// app
#include "menu_main.h"
#include "menu_quick.h"
#include "disp_main.h"
#include "measure_analyze.h"
#include "app_main.h"
#include "data_main.h"
#include "measure_main.h"
// font
#include "font_def.h"


//--------------------------------------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local Funtions
//--------------------------------------------------------------------------------------------------
U08 BTN_GetIdx(void)
{
	U08 rtn = BTN_IDX_MAX;
	U08 save = SFRPAGE;

	SFRPAGE = CONFIG_PAGE;

	if     (_SBIT_BTN_PROG  == BTN_STT_PRESS)	rtn = BTN_IDX_PROG;
	else if(_SBIT_BTN_SET   == BTN_STT_PRESS)	rtn = BTN_IDX_SET;
	else if(_SBIT_BTN_EXIT  == BTN_STT_PRESS)  	rtn = BTN_IDX_EXIT;
	else if(_SBIT_BTN_UP    == BTN_STT_PRESS)	rtn = BTN_IDX_UP;
	else if(_SBIT_BTN_DOWN  == BTN_STT_PRESS)  	rtn = BTN_IDX_DOWN;
	else if(_SBIT_BTN_RIGHT == BTN_STT_PRESS)	rtn = BTN_IDX_RIGHT;
	else if(_SBIT_BTN_LEFT  == BTN_STT_PRESS)	rtn = BTN_IDX_LEFT;

	SFRPAGE = save;

	return rtn;
}


//--------------------------------------------------------------------------------------------------
//  Global APIs
//--------------------------------------------------------------------------------------------------
void BTN_DelayMsec(U32 delay)
{
    U32 cnt_press = 0;

    while(BTN_ChkPush() && cnt_press < delay)
    {
        cnt_press++;
        DLY_RunMsec(1);
    }
}

U08 BTN_ChkLong(U08 idx, U32 delay)
{
    U32 cnt_press = 0;          // OLD: U32 Cnt_Button_press_UL_L = 0;
    U08 flg_delay = FALSE;      // OLD: U08 Flag_Delay_UC_L = 0;

    while(BTN_GetStatus(idx) && cnt_press < delay)
    {
        cnt_press++;
        DLY_RunMsec(1);
    }

    if (cnt_press >= delay)
    	flg_delay = TRUE;

    return flg_delay;
}


S32 BTN_GetValue_UpDn(S32 tmpr, S32 min, S32 max, U16 step, U08 cnt)
{
    if (BTN_GetStatus(BTN_IDX_UP))
    {
        if     (cnt <= 10)      tmpr +=  step;
        else if(cnt <  20)      tmpr += (step * 10);
        else if(cnt <  29)      tmpr += (step * 100);
        else					tmpr += (step * 1000);
    }
    else if (BTN_GetStatus(BTN_IDX_DOWN))
    {
		if     (cnt <= 10)      tmpr -=  step;
		else if(cnt <  20)      tmpr -= (step * 10);
		else if(cnt <  29)      tmpr -= (step * 100);
		else                    tmpr -= (step * 1000);
    }

	if(tmpr > max)       tmpr = max;
	if(tmpr < min)       tmpr = min;

	return tmpr;
}

U08 BTN_GetStatus(U08 idx)
{
	U08 rtn = BTN_DET_IDLE;
	U08 old = SFRPAGE;

	SFRPAGE = CONFIG_PAGE;

	switch (idx)
	{
		case BTN_IDX_PROG:		if(!_SBIT_BTN_PROG)		rtn = BTN_DET_ACTV;		break;
		case BTN_IDX_SET:		if(!_SBIT_BTN_SET)		rtn = BTN_DET_ACTV;		break;
		case BTN_IDX_EXIT:		if(!_SBIT_BTN_EXIT)		rtn = BTN_DET_ACTV;		break;
		case BTN_IDX_UP:		if(!_SBIT_BTN_UP)		rtn = BTN_DET_ACTV;		break;
		case BTN_IDX_DOWN:		if(!_SBIT_BTN_DOWN)		rtn = BTN_DET_ACTV;		break;
		case BTN_IDX_RIGHT:		if(!_SBIT_BTN_RIGHT)	rtn = BTN_DET_ACTV;		break;
		case BTN_IDX_LEFT:		if(!_SBIT_BTN_LEFT)		rtn = BTN_DET_ACTV;		break;
	}

	SFRPAGE = old;
	return rtn;
}

U08 BTN_ChkPush(void)
{
    U08 old = SFRPAGE;
    U08 key, rtn;

    SFRPAGE = CONFIG_PAGE;

    key = _SBIT_BTN_PROG & _SBIT_BTN_UP    & _SBIT_BTN_DOWN & _SBIT_BTN_SET &
		  _SBIT_BTN_EXIT & _SBIT_BTN_RIGHT & _SBIT_BTN_LEFT;

    if(key == BTN_DET_ACTV)		rtn = BTN_STT_PRESS;
    else                        rtn = BTN_STT_RELEASE;

    SFRPAGE = old;
    return rtn;
}



// EOF

