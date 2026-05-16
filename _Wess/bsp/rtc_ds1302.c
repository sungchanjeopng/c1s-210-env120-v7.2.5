//--------------------------------------------------------------------
//
//  Copyright (C) 2020, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of
//  WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement
//  with WESS-Global Co.,LTD
//
//  $Id: templet_format.c 1 2020-07-13 12:00:00Z eddie $
//  $Date: 2020-07-13 12:00:00 +0900 
//  $Author: eddie $
//  $Rev: 1.0 $
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------
// lib
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// bsp
#include "dly_main.h"
#include "bsp_main.h"
#include "spi_main.h"
#include "mem_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "btn_main.h"
#include "gpio_main.h"
// app
#include "app_main.h"
#include "rtc_ds1302.h"
// disp
#include "disp_main.h"
#include "disp_string.h"
// font
#include "font_def.h"


//--------------------------------------------------------------------
//  Local Macros
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Local Type definitions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------
GV_RTC gRt;


//--------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Funtion Definitions
//--------------------------------------------------------------------
// OLD: void ds1302_delay(void)
void RtcProc_WaitDly(void)
{
    U16 i;

    for(i=0; i<1; i++){}
}


// OLD: void write_ds1302_byte(unsigned char cmd)
void RtcProc_WriteByte(U08 cmd)
{
    U08 i;
    U08 old = SFRPAGE;

    SFRPAGE = CONFIG_PAGE;

    for(i=0; i<=7; i++)
    {
        if(cmd & 0x01)      SBIT_SPI_MOSI = 1;
        else                SBIT_SPI_MOSI = 0;

        cmd = cmd >> 1;

        SBIT_SPI_CLK = 1;
        RtcProc_WaitDly();
        SBIT_SPI_CLK = 0;
    }

    SFRPAGE = old;
}

// OLD: unsigned char read_ds1302(unsigned char cmd)
U08 RtcProc_ReadData(U08 cmd)
{
    U08 i;
    U08 value = 0;
    U08 old = SFRPAGE;

    SFRPAGE = CONFIG_PAGE;

    SBIT_RTC_CS = 1;
    RtcProc_WaitDly();

    RtcProc_WriteByte(cmd);
    RtcProc_WaitDly();

    P0MDOUT = 0x05;

    for(i=0; i<=7; i++)
    {
		if(SBIT_SPI_MOSI)   value |= 0x80;
		if(i != 7)			value = value >> 1;

		SBIT_SPI_CLK = 1;
		RtcProc_WaitDly();
		SBIT_SPI_CLK = 0;
    }

    SBIT_RTC_CS = 0;
    RtcProc_WaitDly();

    P0MDOUT = 0xF5;

    SFRPAGE = old;

    return value;
}

// OLD: void write_ds1302(unsigned char cmd, unsigned char value)
void RtcProc_WriteData(U08 cmd, U08 value)
{
    U08 old = SFRPAGE;

    SFRPAGE = CONFIG_PAGE;

    SBIT_RTC_CS = 1;
    RtcProc_WaitDly();

    RtcProc_WriteByte(cmd);
    RtcProc_WriteByte(value);

    SBIT_RTC_CS = 0;
    RtcProc_WaitDly();

    SFRPAGE = old;
}

// Old: rtc_get_datetime()
void RTC_GetTime(void)
{
	U08 old = SFRPAGE;

	SPI_ResetFunc();
	SFRPAGE = CONFIG_PAGE;

	gRt.sec  = RtcProc_ReadData(0x81);
	gRt.min  = RtcProc_ReadData(0x83);
	gRt.hour = RtcProc_ReadData(0x85);
	gRt.date = RtcProc_ReadData(0x87);
	gRt.dd   = RtcProc_ReadData(0x8b);
	gRt.mm   = RtcProc_ReadData(0x89);
	gRt.yy   = RtcProc_ReadData(0x8d);

	gRt.sec  = (gRt.sec >>4)*10 + (gRt.sec &0x0f);
	gRt.min  = (gRt.min >>4)*10 + (gRt.min &0x0f);
	gRt.hour = (gRt.hour>>4)*10 + (gRt.hour&0x0f);
	gRt.date = (gRt.date>>4)*10 + (gRt.date&0x0f);
	gRt.dd   = (gRt.dd  >>4)*10 + (gRt.dd  &0x0f);
	gRt.mm   = (gRt.mm  >>4)*10 + (gRt.mm  &0x0f);
	gRt.yy   = (gRt.yy  >>4)*10 + (gRt.yy  &0x0f);

	SBIT_SPI_MOSI = GIO_SET_LO;

	SFRPAGE = old;

	SPI_SetFunc();
}

// OLD: void rtc_set_datetime(unsigned char Time_Min_UC_L,unsigned char Time_Hour_UC_L,unsigned char Time_Date_UC_L, unsigned char Time_Month_UC_L,unsigned char Time_Day_UC_L,unsigned char Time_Year_UC_L)
void RTC_SetTime(U08 min, U08 hour, U08 date, U08 mm, U08 dd, U08 yy)
{
    U08 dat;

    SPI_ResetFunc();

    // sec
    RtcProc_WriteData(0x80, 0x00);

    // min
    dat = min;
    dat /= 10;
    min %= 10;
    min = (dat<<4) | min;
    RtcProc_WriteData(0x82, min);

    // hour
    dat = hour;
    dat /= 10;
    hour %= 10;
    hour = (dat<<4) | hour;
    RtcProc_WriteData(0x84, hour);

    // date
    dat = date;
    dat /=10;
    date %=10;
    date = (dat<<4) | date;
    RtcProc_WriteData(0x86, date);

    // month
    dat = mm;
    dat /=10;
    mm %=10;
    mm = (dat<<4) | mm;
    RtcProc_WriteData(0x88, mm);

    // day
    dat = dd;
    dat /= 10;
    dd %= 10;
    dd = (dat<<4) | dd;
    RtcProc_WriteData(0x8a, dd);

    // yy
    dat = yy;
    dat /= 10;
    yy %= 10;
    yy = (dat<<4) | yy;
    RtcProc_WriteData(0x8c, yy);

    SPI_SetFunc();
}

// OLD: unsigned char Time_adj(unsigned char Para_UC_L, unsigned char Y_column_UC_L, unsigned char row_UC_L, unsigned char Sel_UC_L)
U08 RTC_AdjTime(U08 para, U08 column, U08 row, U08 sel)
{
    U16 cnt;
    S08 aux = para;
	U08 p_str[APP_STR_LENGTH] = {0, };

	do
    {
		DpSTR_LineUpd(FT_EN_08x16, 0, column, row, "  ");

		if(_SBIT_BTN_SET != BTN_STT_PRESS)
        {
			cnt = 0;

			while(!BTN_ChkPush() && (cnt < 200))
            {
				cnt++;
				DLY_RunMsec(1);
			}
		}

		do
        {
			if(_SBIT_BTN_UP   == BTN_STT_PRESS)    aux++;
			if(_SBIT_BTN_DOWN == BTN_STT_PRESS)    aux--;

			switch(sel)
            {
                case RTC_SET_IDX_YY:
                    if     (aux > 50)       aux = 0;
                    else if(aux < 0)        aux = 50;

					APP_Num2Str(aux, p_str, 2, 0, TRUE);
					DpSTR_LineUpd(FT_EN_08x16, FALSE,  column, row, p_str);
                    break;

                case RTC_SET_IDX_MM:
                    if     (aux > 12)       aux = 1;
                    else if(aux < 1)        aux = 12;

					APP_Num2Str(aux, p_str, 2, 0, TRUE);
					DpSTR_LineUpd(FT_EN_08x16, FALSE,  column, row, p_str);
                    break;

                case RTC_SET_IDX_DAY:
                    if     (aux > 31)       aux = 1;
                    else if(aux < 1)        aux = 31;
					APP_Num2Str(aux, p_str, 2, 0, TRUE);
					DpSTR_LineUpd(FT_EN_08x16, FALSE,  column, row, p_str);
                    break;

                case RTC_SET_IDX_H24:
                    if     (aux > 23)       aux = 0;
                    else if(aux < 0)        aux = 23;

					APP_Num2Str(aux, p_str, 2, 0, TRUE);
					DpSTR_LineUpd(FT_EN_08x16, FALSE,  column, row, p_str);
                    break;

                case RTC_SET_IDX_MIN:
                    if     (aux > 59)       aux = 00;
                    else if(aux < 0)        aux = 59;

					APP_Num2Str(aux, p_str, 2, 0, TRUE);
					DpSTR_LineUpd(FT_EN_08x16, FALSE,  column, row, p_str);
                    break;
			}

			if(_SBIT_BTN_SET != BTN_STT_PRESS)
				BTN_DelayMsec(BTN_WAIT_UD);
		} while (_SBIT_BTN_UP == BTN_STT_PRESS || _SBIT_BTN_DOWN == BTN_STT_PRESS);

		if(_SBIT_BTN_SET != BTN_STT_PRESS)
        {
			cnt = 0;
			while(!BTN_ChkPush() && cnt < 200)
            {
				cnt++;
				DLY_RunMsec(1);
			}
		}
	} while(_SBIT_BTN_SET);

	para = aux;

	return para;
}

// EOF

