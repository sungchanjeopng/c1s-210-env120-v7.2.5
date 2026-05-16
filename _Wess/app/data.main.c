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
// lib
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// bsp
#include "bsp_main.h"
#include "flash_main.h"
#include "uart_main.h"
#include "gpio_main.h"
#include "mem_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "btn_main.h"
// app
#include "app_main.h"
#include "dly_main.h"
#include "rtc_ds1302.h"
#include "data_main.h"
#include "prtc_main.h"
#include "measure_main.h"
#include "measure_temperature.h"
// disp
#include "disp_main.h"
#include "disp_string.h"
// menu
#include "menu_measure.h"
#include "menu_output.h"
#include "menu_data.h"
// font
#include "font_def.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------
LS_DAT lDat;

BIT gTd_f_en;

U08 gLg_page;               // OLD: unsigned char Log_Trend_page_UC_G;

U32 gLg_addr_main;          // OLD: unsigned long Log_addr_UL_G;
U32 gLg_addr_start;         // OLD: unsigned long Log_Trend_start_addr_0_UL_G;
U32 gLg_addr_end;           // OLD: unsigned long Log_Trend_end_addr_0_UL_G;
U32 gLg_addr_tx_start;		// unsigned long Log_Tx_start_addr_UL_G;
U32 gLg_addr_dat1;         	// OLD: unsigned long Log_addr_dat_1_UL_G;
U32 gLg_addr_dat2;    		// OLD: unsigned long Log_addr_dat_2_UL_G;
U32 gLg_addr_dat3;    		// OLD: unsigned long Log_addr_dat_3_UL_G;
U32 gLg_ad_n_start;         // OLD: unsigned long Log_Trend_start_addr_n_UL_G;
U32 gLg_ad_n_end;           // OLD: unsigned long Log_Trend_end_addr_n_UL_G;

U32 gLg_scale_y;            // OLD: unsigned long Log_Trend_Y_scale_UL_G;
U08 gLg_min;                // OLD: unsigned char Log_Min_UC_G;
U08 gLg_hour;            	// OLD: unsigned char Log_Hr_UC_G;

U08 gLg_flg_ovf3;    		// OLD: unsigned char Log_Flag_OVF_3_UC_G;


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
U08 DAT_GetMskOvf1(void)		{	return lDat.flg_ovf1;		}
// Set


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void LOG_ResetAddr(void)    // OLD: void Log_addr_reset(void)
{
    lDat.flg_ovf1 = 0xa0;
    lDat.flg_ovf2 = 0xa0;
	#if 1
    gLg_addr_main = MEM_ADDR_LOG_DATA_START;
    gLg_addr_dat1 = MEM_ADDR_LOG_DATA_START;
    gLg_addr_dat2 = MEM_ADDR_LOG_DATA_START;
    gLg_addr_dat3 = MEM_ADDR_LOG_DATA_START;
	#else
    gLg_addr_main = 150001;
    gLg_addr_dat1 = 150001;
    gLg_addr_dat2 = 150001;
    gLg_addr_dat3 = 150001;
	#endif
    gLg_flg_ovf3 = 0xa0;
}

void LOG_InitAddr(void)     // OLD: void Log_Init_addr(void)
{
    U32 read[3];
    U32 result;
	
    MEM_ReadSeq_StartAddr(MEM_MRAM, MEM_ADDR_LOG_ADDR_1_START);
    read[0] = MEM_ReadSeq_ByteData(MEM_MRAM);
    read[1] = MEM_ReadSeq_ByteData(MEM_MRAM);
    read[2] = MEM_ReadSeq_ByteData(MEM_MRAM);
    MEM_RwSeq_End(MEM_MRAM);

    result = 0;
    result = result + ((read[0] & 0x000000FF) << 16);
    result = result + ((read[1] & 0x000000FF) << 8);
    result = result +  (read[2] & 0x000000FF);
    gLg_addr_dat1 = result;

    lDat.flg_ovf1 = MEM_ReadByte(MEM_MRAM, MEM_ADDR_LOG_ADDR_1_END);

    MEM_ReadSeq_StartAddr(MEM_MRAM, MEM_ADDR_LOG_ADDR_2_START);
    read[0] = MEM_ReadSeq_ByteData(MEM_MRAM);
    read[1] = MEM_ReadSeq_ByteData(MEM_MRAM);
    read[2] = MEM_ReadSeq_ByteData(MEM_MRAM);
    MEM_RwSeq_End(MEM_MRAM);

    result = 0;
    result = result + ((read[0] & 0x000000FF) << 16);
    result = result + ((read[1] & 0x000000FF) << 8);
    result = result +  (read[2] & 0x000000FF);
    gLg_addr_dat2 = result;

    lDat.flg_ovf2 = MEM_ReadByte(MEM_MRAM, MEM_ADDR_LOG_ADDR_2_END);

	MEM_ReadSeq_StartAddr(MEM_MRAM, MEM_ADDR_LOG_ADDR_3_START);
    read[0] = MEM_ReadSeq_ByteData(MEM_MRAM);
    read[1] = MEM_ReadSeq_ByteData(MEM_MRAM);
    read[2] = MEM_ReadSeq_ByteData(MEM_MRAM);
	MEM_RwSeq_End(MEM_MRAM);

    result = 0;
    result = result + ((read[0] & 0x000000FF) << 16);
    result = result + ((read[1] & 0x000000FF) << 8);
    result = result +  (read[2] & 0x000000FF);
	gLg_addr_dat3 = result;

	gLg_flg_ovf3 = MEM_ReadByte(MEM_MRAM, MEM_ADDR_LOG_ADDR_3_END);

    if((gLg_addr_dat1 == gLg_addr_dat2) && (gLg_addr_dat1 == gLg_addr_dat3))
    {
        if(lDat.flg_ovf1 == lDat.flg_ovf2 && lDat.flg_ovf1 == gLg_flg_ovf3)
        	gLg_addr_main = gLg_addr_dat1;
    }
    else if (gLg_addr_dat1 == gLg_addr_dat2)
    {
        if (lDat.flg_ovf1 == lDat.flg_ovf2)
        {
            gLg_addr_main = gLg_addr_dat1;
            gLg_addr_dat3 = gLg_addr_dat1;
            gLg_flg_ovf3 = lDat.flg_ovf1;
        }
    }
    else if (gLg_addr_dat2 == gLg_addr_dat3)
    {
        if (lDat.flg_ovf2 == gLg_flg_ovf3)
        {
            gLg_addr_main = gLg_addr_dat2;
            gLg_addr_dat1 = gLg_addr_dat2;
            lDat.flg_ovf1 = lDat.flg_ovf2;
        }
    }
    else
        LOG_ResetAddr();

    if (gLg_addr_main < MEM_ADDR_LOG_DATA_START || gLg_addr_main > MEM_ADDR_LOG_DATA_END)
        LOG_ResetAddr();

    gLg_min  = gRt.min;
    gLg_hour = gRt.hour;
}

void LOG_SetDpAddr(void)
{
	U32 addr_dat0, addr_dat1;
	U32 inter;

	switch(MnDAT_GetSavIntv())
	{
		case MnDAT_INTV_01M:
			switch(MnDAT_GetDisp01m())
			{
				case MnDAT_DISP_I0: 	inter = 7200;		break;		 //((12 * 60) / 1) * 10;
				case MnDAT_DISP_I1: 	inter = 14400;		break;		 //((24 * 60) / 1) * 10;
			}
			break;

		case MnDAT_INTV_10M:
			switch(MnDAT_GetDisp10m())
			{
				case MnDAT_DISP_I0: 	inter = 1440;		break;		 //((24 * 60) / 10) * 10;
				case MnDAT_DISP_I1: 	inter = 10080;		break;		 //((7 * 24 * 60) / 10) * 10;
			}
			break;

		case MnDAT_INTV_60M:
			switch(MnDAT_GetDisp60m())
			{
				case MnDAT_DISP_I0: 	inter = 3360;		break;		 //((14 * 24 * 60) / 60) * 10;
				case MnDAT_DISP_I1: 	inter = 7200;		break;		 //((30 * 24 * 60) / 60) * 10;
			}
			break;
	}

	gDp_flg_log = 0;

	if(lDat.flg_ovf1 == 0xa1 && gLg_addr_end != 250006)
    {
		addr_dat0 = gLg_addr_start - (inter * (U32)gLg_page);

		if(addr_dat0 >= 250006)
        {
			gLg_ad_n_start = addr_dat0;
			addr_dat0 = gLg_addr_start - (inter * ((U32)gLg_page + 1));

			if(addr_dat0 >= 250006)
				gLg_ad_n_end = addr_dat0;
            else
            {
				addr_dat1 = 250006 - addr_dat0;
				addr_dat0 = 349996 - addr_dat1;

				if(addr_dat0 < gLg_addr_end)    gLg_ad_n_end = gLg_addr_end;
                else                            gLg_ad_n_end = addr_dat0;
			}
		}
        else
        {
			addr_dat1 = 250006 - addr_dat0;
			addr_dat0 = 349996 - addr_dat1;

			if (addr_dat0 < gLg_addr_end)
				gDp_flg_log = 1;
            else
            {
				gLg_ad_n_start = addr_dat0;
				addr_dat0 = gLg_ad_n_start - inter;

				if(addr_dat0 >= gLg_addr_end)   gLg_ad_n_end = addr_dat0;
                else                            gLg_ad_n_end = gLg_addr_end;
			}
		}
	}
    else
    {
		addr_dat0 = gLg_addr_start - (inter * (U32)gLg_page);

		if(addr_dat0 < 250006)
			gDp_flg_log = 1;
        else
        {
			gLg_ad_n_start = addr_dat0;
			addr_dat0 = gLg_addr_start - (inter * ((U32) gLg_page + 1));

			if(addr_dat0 < 250006)      gLg_ad_n_end = 250006;
            else                        gLg_ad_n_end = addr_dat0;
		}
	}
}

// OLD: void Disp_Log_time(void)
void LOG_DpTime(void)
{
	U08 *p_mm;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U08 p_st2[APP_STR_LENGTH] = {0, };

    RTC_GetTime();

	switch(gRt.mm)
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

	APP_Num2Str(gRt.date, p_str, 2, 0, FALSE);
	APP_Num2Str(gRt.yy, p_st2, 2, 0, FALSE);

	DpSTR_DrawLin(FT_EN_08x16, FALSE,  4, 16, p_mm);
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 36, 16, p_str);

	DpSTR_DrawLin(FT_EN_08x16, FALSE, 60, 16, "20");
	DpSTR_DrawLin(FT_EN_08x16, FALSE, 76, 16, p_st2);
}

U32 LogDp_Intv01m(void)
{
	U32 point;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U08 p_st2[APP_STR_LENGTH] = {0, };

	switch(MnDAT_GetDisp01m())
	{
		case MnDAT_DISP_I0:
			point = 360;

			APP_Num2Str(12 * (gLg_page+1), p_str, 3, 0, FALSE);
			APP_Num2Str(12 * gLg_page, p_st2, 3, 0, FALSE);

			DpSTR_LineUpd(FT_EN_08x16, 0, 4,   0, "Scale 12hrs");
			DpSTR_LineUpd(FT_EN_08x16, 0, 0, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0, 9, 112, p_str);
			DpSTR_LineUpd(FT_EN_08x16, 0,    36, 112, "hrs");
			DpSTR_LineUpd(FT_EN_08x16, 0, 87+96, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0, 96+96, 112, p_st2);
			DpSTR_LineUpd(FT_EN_08x16, 0, 123 + 96, 112, "hrs");
			break;

		case MnDAT_DISP_I1:
			point = 720;

			APP_Num2Str(24 * (gLg_page + 1), p_str, 3, 0, FALSE);
			APP_Num2Str(24 * gLg_page, p_st2,      3,   0, FALSE);

			DpSTR_LineUpd(FT_EN_08x16, 0, 4, 0, "Scale 24hrs");
			DpSTR_LineUpd(FT_EN_08x16, 0, 0, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,     9, 112, p_str);
			DpSTR_LineUpd(FT_EN_08x16, 0,    36, 112, "hrs");
			DpSTR_LineUpd(FT_EN_08x16, 0, 87+96, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,  96+96, 112, p_st2);
			DpSTR_LineUpd(FT_EN_08x16, 0, 123+96, 112, "hrs");
			break;
	}

	return point;
}

U32 LogDp_Intv10m(void)
{
	U32 point;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U08 p_st2[APP_STR_LENGTH] = {0, };

	switch(MnDAT_GetDisp10m())
	{
		case MnDAT_DISP_I0:
			point = 72;
	
			APP_Num2Str(24*(gLg_page+1), p_str, 3, 0, FALSE);
			APP_Num2Str(24*(gLg_page+0), p_st2, 3, 0, FALSE);

			DpSTR_LineUpd(FT_EN_08x16, 0, 4, 0, "Scale 24hrs");
			DpSTR_LineUpd(FT_EN_08x16, 0, 0, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,      9, 112, p_str);
			DpSTR_LineUpd(FT_EN_08x16, 0,     36, 112, "hrs");
			DpSTR_LineUpd(FT_EN_08x16, 0,  87+96, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,  96+96, 112, p_st2);
			DpSTR_LineUpd(FT_EN_08x16, 0, 123+96, 112, "hrs");
			break;
	
		case MnDAT_DISP_I1:
			point = 504;
	
			APP_Num2Str(7*(gLg_page+1), p_str, 3, 0, FALSE);
			APP_Num2Str(7*(gLg_page+0), p_st2, 3, 0, FALSE);

			DpSTR_LineUpd(FT_EN_08x16, 0, 4, 0, "Scale 7days");
			DpSTR_LineUpd(FT_EN_08x16, 0, 0, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,      9, 112, p_str);
			DpSTR_LineUpd(FT_EN_08x16, 0,     36, 112, "days");
			DpSTR_LineUpd(FT_EN_08x16, 0,  81+96, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,  90+96, 112, p_st2);
			DpSTR_LineUpd(FT_EN_08x16, 0, 117+96, 112, "days");
			break;
	}

	return point;
}

U32 LogDp_Intv60m(void)
{
	U32 point;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U08 p_st2[APP_STR_LENGTH] = {0, };

	switch(MnDAT_GetDisp60m())
	{
		case MnDAT_DISP_I0:
			point = 168;

			APP_Num2Str(14*(gLg_page+1), p_str, 3, 0, FALSE);
			APP_Num2Str(14*(gLg_page+0), p_st2, 3, 0, FALSE);
	
			DpSTR_LineUpd(FT_EN_08x16, 0, 4, 0, "Scale 14days");
			DpSTR_LineUpd(FT_EN_08x16, 0, 0, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,      9, 112, p_str);
			DpSTR_LineUpd(FT_EN_08x16, 0,     36, 112, "days");
			DpSTR_LineUpd(FT_EN_08x16, 0,  81+96, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,  90+96, 112, p_st2);
			DpSTR_LineUpd(FT_EN_08x16, 0, 117+96, 112, "days");
			break;

		case MnDAT_DISP_I1:
			point = 360;	   //(Para_Trend_inter_UI_L / 200) * 10;

			APP_Num2Str(30*(gLg_page+1), p_str, 3, 0, FALSE);
			APP_Num2Str(30*(gLg_page+0), p_st2, 3, 0, FALSE);
	
			DpSTR_LineUpd(FT_EN_08x16, 0, 4,   0, "Scale 30days");
			DpSTR_LineUpd(FT_EN_08x16, 0, 0, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,      9, 112, p_str);
			DpSTR_LineUpd(FT_EN_08x16, 0,     36, 112, "days");
			DpSTR_LineUpd(FT_EN_08x16, 0,  81+96, 112, "-");
			DpSTR_LineUpd(FT_EN_08x16, 0,  90+96, 112, p_st2);
			DpSTR_LineUpd(FT_EN_08x16, 0, 117+96, 112, "days");
			break;
	}

	return point;
}

void ScTRD_InitVai(void)
{
	gTd_f_en = 0;
}

void ScTrdDp_ProcUpd(void)
{
	U08 x;
	U32 addr_dat0, addr_dat1;
	U16 remain, trend, cnt;
	U32 point;
    F32 temp;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U08 p_st2[APP_STR_LENGTH] = {0, };

	LOG_DpTime();

	if (lDat.flg_ovf1 == 0xa0)       remain = (gLg_addr_main - 10 - MEM_ADDR_LOG_DATA_START) / 10;
	else                            remain = 10000;

	DpSTR_LineUpd(FT_EN_08x16, 0, 175, 0, "R:");
	APP_Num2Str(10000-remain, p_str, 5, 0, FALSE);

	DpSTR_DrawLin(FT_EN_08x16, FALSE, 195, 0, p_str);

	switch(MnDAT_GetSavIntv())
	{
		case MnDAT_INTV_01M:		point = LogDp_Intv01m();		break;
		case MnDAT_INTV_10M:		point = LogDp_Intv10m();		break;
		case MnDAT_INTV_60M:		point = LogDp_Intv60m();		break;
	}

	trend = 0;
	gLg_scale_y = 0;
	cnt = 0;

	addr_dat0 = gLg_ad_n_start;
	addr_dat1 = 0;

	for (x = 200; x >= 1; x--)
    {
		if (lDat.flg_ovf1 == 0xa1 && gLg_addr_end != 250006)
        {
			if (gLg_ad_n_end < gLg_ad_n_start)
            {
				if (addr_dat0 >= gLg_ad_n_end)
                {
					trend = MEM_ReadWord(MEM_MRAM, addr_dat0);
					addr_dat1 = addr_dat1 + point;
					addr_dat0 = ((gLg_ad_n_start * 10) - addr_dat1) / 10;
					if (addr_dat0 % 10 != 6)
						addr_dat0 = addr_dat0 + (6 - (addr_dat0 % 10));
					gLg_scale_y = gLg_scale_y + (unsigned long) trend;
					cnt++;

					gDp_buff_grp[x - 1] = trend >> 2;
				}
                else
                {
					gDp_buff_grp[x - 1] = 0;
				}
			}
            else
            {
				if (addr_dat0 <= gLg_ad_n_start && addr_dat0 >= 250006)
                {
					trend = MEM_ReadWord(MEM_MRAM, addr_dat0);
					addr_dat1 = addr_dat1 + point;
					addr_dat0 = ((gLg_ad_n_start * 10) - addr_dat1) / 10;
					if (addr_dat0 % 10 != 6)
						addr_dat0 = addr_dat0 + (6 - (addr_dat0 % 10));
					gLg_scale_y = gLg_scale_y + (U32) trend;
					cnt++;

					gDp_buff_grp[x - 1] = trend >> 2;

					if (addr_dat0 < 250006)
                    {
						addr_dat1 = 250006 - addr_dat0;
						addr_dat0 = 349996 - addr_dat1;
						addr_dat1 *= 10;
					}
				}
                else if (addr_dat0 >= gLg_ad_n_end)
                {
					trend = MEM_ReadWord(MEM_MRAM, addr_dat0);
					addr_dat1 = addr_dat1 + point;
					addr_dat0 = ((349996 * 10) - addr_dat1) / 10;
					if(addr_dat0 % 10 != 6)
						addr_dat0 = addr_dat0 + (6 - (addr_dat0 % 10));
					gLg_scale_y = gLg_scale_y + (U32) trend;
					cnt++;

					gDp_buff_grp[x - 1] = trend >> 2;
				} else {
					gDp_buff_grp[x - 1] = 0;
				}
			}
		}
        else
        {
			if (addr_dat0 >= 250006)
            {
				trend = MEM_ReadWord(MEM_MRAM, addr_dat0);
				addr_dat1 = addr_dat1 + point;
				addr_dat0 = ((gLg_ad_n_start * 10) - addr_dat1) / 10;
				if (addr_dat0 % 10 != 6)
					addr_dat0 = addr_dat0 + (6 - (addr_dat0 % 10));
				gLg_scale_y = gLg_scale_y + (U32) trend;
				cnt++;

				gDp_buff_grp[x - 1] = trend >> 2;
			}
            else
				gDp_buff_grp[x - 1] = 0;
		}
	}

	gLg_scale_y = gLg_scale_y / (U32) cnt;

	if(MnMSR_GetUnit() == MnMSR_UNIT_METER)
    {
		APP_Num2Str(gLg_scale_y*2, p_st2, 4, 2, FALSE);

		DpSTR_LineUpd(FT_EN_08x16, 0, 180, 16, p_st2);
		DpSTR_LineUpd(FT_EN_08x16, 0, 230, 16, "m");
	}
	else
    {
		temp = (gLg_scale_y * MSR_CALC_M2F_CONST) + 0.5;
		APP_Num2Str((U32)temp*2, p_st2, 3, 1, FALSE);

		DpSTR_LineUpd(FT_EN_08x16, 0, 180, 16, p_st2);
		DpSTR_LineUpd(FT_EN_08x16, 0, 220, 16, "ft");
	}

	addr_dat0 = gLg_ad_n_start;
	addr_dat1 = 0;

	DSP_DrawGraph(20, 32, 200, 78, gDp_buff_grp, 0, 200, (gLg_scale_y >> 2) * 2, DP_IDX_GRAPH_3);
	LDT_UpdBuff(LCD_Y_START, LCD_Y_END);
}

void LogTx_Normal(void)
{
	U32 cnt0, cnt1, cnt_tx, mem_end;
	U08 tmp1, tmp2;
	U08 dat, yy, mm, date, hour, min;
	U16 dat0, level, dat1, dat2;
	S16 tmp3;
	U08 p_str[APP_STR_LENGTH] = {0, };
	U08 old = SFRPAGE;
	U08 unit = MnMSR_GetUnit();

	UAT_SetBaudrate(MnOUT_MDB_BAUD_19200);

	DISP_ClearScr();
	DpSTR_LineUpd(FT_EN_08x16, 0, 60, 16, "Data downloding");

	if (lDat.flg_ovf1 == 0xa0)
		cnt0 = (gLg_addr_main - 10 - MEM_ADDR_LOG_DATA_START) / 10;
	else
	{
		cnt0 = 10000;
		mem_end = ((349991 - gLg_addr_tx_start) / 10) + 1;
	}

	if (cnt0 > 0)
	{
		cnt1 = cnt0 * 18;

		SFRPAGE = UART0_PAGE;

		UT0_TxByte(0x5b);				// ' [ '
		UT0_TxByte(0x44);				// ' D '

		dat =  cnt1 	   & 0x0f;		UT0_TxByte(dat+0x30);		//low byte	Transfer data_cnt
		dat = (cnt1 >>	4) & 0x0f;		UT0_TxByte(dat+0x30);		// 4 byte
		dat = (cnt1 >>	8) & 0x0f;		UT0_TxByte(dat+0x30);		// 8 byte
		dat = (cnt1 >> 12) & 0x0f;		UT0_TxByte(dat+0x30);
		dat = (cnt1 >> 16) & 0x0f;		UT0_TxByte(dat+0x30);
		dat = (cnt1 >> 20) & 0x0f;		UT0_TxByte(dat+0x30);
		dat = (cnt1 >> 24) & 0x0f;		UT0_TxByte(dat+0x30);
		dat = (cnt1 >> 28) & 0x0f;		UT0_TxByte(dat+0x30);

		tmp2 =	unit & 0x0f;			UT0_TxByte(tmp2+0x30);		// low byte  Transfer Sel_Unit_UC_G
		tmp2 = (unit >> 4) & 0x0f;		UT0_TxByte(tmp2 + 0x30);	// middle byte
		
		cnt_tx = 0;

		DpSTR_LineUpd(FT_EN_08x16, 0, 141, 48, "%");
		MEM_ReadSeq_StartAddr(MEM_MRAM, gLg_addr_tx_start);

		do 
		{
			cnt_tx++;

			dat0 = (cnt_tx * 1000) / cnt0;

			if(dat0 % 10 == 0)
			{
				APP_Num2Str(dat0/10, p_str, 3, 0, FALSE);
				DpSTR_LineUpd(FT_EN_08x16, 0, 114, 48, p_str);
			}

			// Log data read
			yy	 = MEM_ReadSeq_ByteData(MEM_MRAM);
			mm	 = MEM_ReadSeq_ByteData(MEM_MRAM);
			date = MEM_ReadSeq_ByteData(MEM_MRAM);
			hour = MEM_ReadSeq_ByteData(MEM_MRAM);
			min  = MEM_ReadSeq_ByteData(MEM_MRAM);

			dat1 = MEM_ReadSeq_ByteData(MEM_MRAM);
			dat2 = MEM_ReadSeq_ByteData(MEM_MRAM);

			level = dat1 & 0x00FF;
			level = (level << 8) + (dat2 & 0x00FF);

			dat1 = MEM_ReadSeq_ByteData(MEM_MRAM);
			dat2 = MEM_ReadSeq_ByteData(MEM_MRAM);

			tmp3 = dat1 & 0x00FF;
			tmp3 = (tmp3 << 8) + (dat2 & 0x00FF);

			tmp1 = MEM_ReadSeq_ByteData(MEM_MRAM);

			// Log data tx
			dat =  yy & 0x0f;			UT0_TxByte(dat + 0x30);
			dat = (yy >> 4) & 0x0f; 	UT0_TxByte(dat + 0x30);

			dat =  mm & 0x0f;			UT0_TxByte(dat + 0x30);
			dat = (mm >> 4) & 0x0f; 	UT0_TxByte(dat + 0x30);

			dat =  date & 0x0f; 		UT0_TxByte(dat + 0x30);
			dat = (date >> 4) & 0x0f;	UT0_TxByte(dat + 0x30);

			dat =  hour & 0x0f; 		UT0_TxByte(dat + 0x30);
			dat = (hour >> 4) & 0x0f;	UT0_TxByte(dat + 0x30);

			dat =  min & 0x0f;			UT0_TxByte(dat + 0x30);
			dat = (min >> 4) & 0x0f;	UT0_TxByte(dat + 0x30);

			dat =  level & 0x0f;		UT0_TxByte(dat + 0x30);
			dat = (level >> 4) & 0x0f;	UT0_TxByte(dat + 0x30);
			dat = (level >> 8) & 0x0f;	UT0_TxByte(dat + 0x30);
			dat = (level >> 12) & 0x0f; UT0_TxByte(dat + 0x30);

			dat =  tmp3 & 0x0f; 		UT0_TxByte(dat + 0x30);
			dat = (tmp3 >> 4) & 0x0f;	UT0_TxByte(dat + 0x30);
			dat = (tmp3 >> 8) & 0x0f;	UT0_TxByte(dat + 0x30);
			dat = (tmp3 >> 12) & 0x0f;	UT0_TxByte(dat + 0x30);

			if(lDat.flg_ovf1 == 0xa1)
			{
				if(mem_end == cnt_tx)
				{
					MEM_RwSeq_End(MEM_MRAM);
					MEM_ReadSeq_StartAddr(MEM_MRAM, MEM_ADDR_LOG_DATA_START);
				}
			}

			old = SFRPAGE;
			SFRPAGE = CONFIG_PAGE;

			if(_SBIT_BTN_EXIT == BTN_STT_PRESS)
				cnt_tx = cnt0;

			SFRPAGE = old;
		} while(cnt0 > cnt_tx);

		MEM_RwSeq_End(MEM_MRAM);

		UT0_TxByte(0x5d);
		SFRPAGE = old;
	}

	DISP_ClearScr();

}

void LogTx_Rf(void)
{
	int i;
	U32 cnt0, cnt_tx, mem_end;
	U08 yy, mm, date, hour, min;
	U16 dat0, level, dat1, dat2;
	U16 dmy;
	S16 tmp3, tmp4;
	U08 p_str[APP_STR_LENGTH] = {0, };

	U08 old = SFRPAGE;

	UAT_SetBaudrate(MnOUT_MDB_BAUD_19200);
	
	DISP_ClearScr();
	DpSTR_LineUpd(FT_EN_08x16, 0, 60, 16, "Data downloding");
	
	if(lDat.flg_ovf1 == 0xa0)
		cnt0 = (gLg_addr_main - 10 - MEM_ADDR_LOG_DATA_START) / 10;
	else
	{
		cnt0 = 10000;
		mem_end = ((349991 - gLg_addr_tx_start) / 10) + 1;
	}
	
	cnt_tx = 0;

	DpSTR_LineUpd(FT_EN_08x16, 0, 141, 48, "%");
	MEM_ReadSeq_StartAddr(MEM_MRAM, gLg_addr_tx_start);
	
	do
	{
		cnt_tx++;
		dat0 = (cnt_tx * 1000) / cnt0;
		
		if(dat0 % 10 == 0)
		{
			APP_Num2Str(dat0/10, p_str, 3, 0, FALSE);
			DpSTR_LineUpd(FT_EN_08x16, 0, 114, 48, p_str);
		}

		// Log data read
		yy	 = MEM_ReadSeq_ByteData(MEM_MRAM);
		mm	 = MEM_ReadSeq_ByteData(MEM_MRAM);
		date = MEM_ReadSeq_ByteData(MEM_MRAM);
		hour = MEM_ReadSeq_ByteData(MEM_MRAM);
		min  = MEM_ReadSeq_ByteData(MEM_MRAM);
	
		dat1 = MEM_ReadSeq_ByteData(MEM_MRAM);
		dat2 = MEM_ReadSeq_ByteData(MEM_MRAM);
	
		level = dat1 & 0x00FF;
		level = (level << 8) + (dat2 & 0x00FF);
	
		dat1 = MEM_ReadSeq_ByteData(MEM_MRAM);
		dat2 = MEM_ReadSeq_ByteData(MEM_MRAM);
	
		tmp3 = dat1 & 0x00FF;
		tmp3 = (tmp3 << 8) + (dat2 & 0x00FF);
	
		dmy = MEM_ReadSeq_ByteData(MEM_MRAM);
	
		UT0_TxAscii(yy);
		UT0_TxAscii(mm);
		UT0_TxAscii(date);
		UT0_TxAscii(hour);
		UT0_TxAscii(min);
		
		UT0_TxByte(0x2c);
		
		gDp_buff_str[0] = (level/1000) % 10 + '0';
		gDp_buff_str[1] = (level/100) % 10 + '0';
		gDp_buff_str[2] = (level/10) % 10 + '0';
		gDp_buff_str[3] =  level%10 + '0';
		 
		for(i=0; i<4; i++)
			UT0_TxByte(gDp_buff_str[i]);
	
		UT0_TxByte(0x2c);
	
		tmp4 = tmp3;		 //Trasnfer Temperature, 00unit 1'
		
		if(tmp4 < 0)
		{
			gDp_buff_str[0] = 0x2d; 	 //'-'
			tmp4 = tmp4*-1;
		}
		else
			gDp_buff_str[0] = 0x2b; 	 //'+'
	
		gDp_buff_str[1] = (tmp4/100) % 10 + '0';
		gDp_buff_str[2] = (tmp4/10) % 10 + '0';
		gDp_buff_str[3] =  tmp4%10 + '0';
	
		for(i=0; i<4; i++) 
			UT0_TxByte(gDp_buff_str[i]);
		
		UT0_TxByte(0x0d);
		UT0_TxByte(0x0a);
	
		if (lDat.flg_ovf1 == 0xa1) 
		{
			if (mem_end == cnt_tx) 
			{
				MEM_RwSeq_End(MEM_MRAM);
				MEM_ReadSeq_StartAddr(MEM_MRAM, MEM_ADDR_LOG_DATA_START);
			}
		}
	
		old = SFRPAGE;
		SFRPAGE = CONFIG_PAGE;
		
		if(_SBIT_BTN_EXIT == BTN_STT_PRESS)
			cnt_tx = cnt0;
		
		SFRPAGE = old;
	} 
	while (cnt0 > cnt_tx);
	
	MEM_RwSeq_End(MEM_MRAM);
	
	UT0_TxByte(0x5d);
	SFRPAGE = old;
	
	DISP_ClearScr();

}

// OLD: void Out_Log_data(void)
void LOG_OutData(void)
{
	U08 old = SFRPAGE;
	U08 protocol = MnOUT_GetProtocol();

	if(lDat.flg_ovf1 == 0xa1)	gLg_addr_tx_start = gLg_addr_main;
	else						gLg_addr_tx_start = MEM_ADDR_LOG_DATA_START;

	SFRPAGE = CONFIG_PAGE;
	SBIT_RS485_TR = PRT_485_TX_ON;

	if((protocol == MnOUT_PROT_STD) || (protocol == MnOUT_PROT_MDBS))			LogTx_Normal();
	else																		LogTx_Rf();

	SFRPAGE = CONFIG_PAGE;
	SBIT_RS485_TR = PRT_485_RX_ON;

	SFRPAGE = old;
}

void LOG_ProcData(void) 
{
	S16 sludge = MEAS_GetSgThick();
	S16 tmpr   = gTp_now;

	MEM_WriteSeq_StartAddr(MEM_MRAM, gLg_addr_main);
	MEM_WriteSeq_Byte(MEM_MRAM, gRt.yy);
	MEM_WriteSeq_Byte(MEM_MRAM, gRt.mm);
	MEM_WriteSeq_Byte(MEM_MRAM, gRt.date);
	MEM_WriteSeq_Byte(MEM_MRAM, gRt.hour);
	MEM_WriteSeq_Byte(MEM_MRAM, gRt.min);

	MEM_WriteSeq_Byte(MEM_MRAM, (U08)((sludge >> 8) & 0xFF));
	MEM_WriteSeq_Byte(MEM_MRAM, (U08)(sludge & 0xFF));

	MEM_WriteSeq_Byte(MEM_MRAM, (U08)((tmpr >> 8) & 0xFF));
	MEM_WriteSeq_Byte(MEM_MRAM, (U08)(tmpr & 0xFF));

	MEM_RwSeq_End(MEM_MRAM);

	switch(MnDAT_GetSavIntv())
	{
		case MnDAT_INTV_01M:
			if(++gLg_min > 59)
			{
				gLg_hour++;
				gLg_min = 0;
			}

			if(gLg_hour > 23)
				gLg_hour = 0;
			break;

		case MnDAT_INTV_10M:
			gLg_min = gLg_min + 10;

			if(gLg_min > 59)
			{
				gLg_hour++;
				gLg_min = gLg_min - 60;
			}

			if(gLg_hour > 23)
				gLg_hour = 0;
			break;

		case MnDAT_INTV_60M:
			if(++gLg_hour > 23)
				gLg_hour = 0;
			break;
	}

	gLg_addr_main += 10;

	if(gLg_addr_main >= MEM_ADDR_LOG_DATA_END)
	{
		gLg_addr_main = MEM_ADDR_LOG_DATA_START;
		lDat.flg_ovf1 = 0xa1;
	}
	gLg_addr_dat1 = gLg_addr_main;

	MEM_WriteSeq_StartAddr(MEM_MRAM, MEM_ADDR_LOG_ADDR_1_START);
	MEM_WriteSeq_Byte(MEM_MRAM, (U08)((gLg_addr_dat1 >> 16) & 0xFF));
	MEM_WriteSeq_Byte(MEM_MRAM, (U08)((gLg_addr_dat1 >> 8) & 0xFF));
	MEM_WriteSeq_Byte(MEM_MRAM, (U08)(gLg_addr_dat1 & 0xFF));
	MEM_RwSeq_End(MEM_MRAM);

	MEM_WriteByte(MEM_MRAM, MEM_ADDR_LOG_ADDR_1_END, lDat.flg_ovf1);
}

// OLD: void Log_addr_save_1(void)
void LOG_SaveAddr1(void)
{
	gLg_addr_dat2 = gLg_addr_dat1;
	lDat.flg_ovf2  = lDat.flg_ovf1;

	MEM_WriteSeq_StartAddr(MEM_MRAM, MEM_ADDR_LOG_ADDR_2_START);
	MEM_WriteSeq_Byte(MEM_MRAM, (U08) ((gLg_addr_dat2 >> 16) & 0xFF));
	MEM_WriteSeq_Byte(MEM_MRAM, (U08) ((gLg_addr_dat2 >> 8) & 0xFF));
	MEM_WriteSeq_Byte(MEM_MRAM, (U08) (gLg_addr_dat2 & 0xFF));
	MEM_RwSeq_End(MEM_MRAM);

	MEM_WriteByte(MEM_MRAM, MEM_ADDR_LOG_ADDR_2_END, lDat.flg_ovf2);
}

// OLD: void Log_addr_save_2(void)
void LOG_SaveAddr2(void)
{
	gLg_addr_dat3 = gLg_addr_dat2;
	gLg_flg_ovf3  = lDat.flg_ovf2;

	MEM_WriteSeq_StartAddr(MEM_MRAM, MEM_ADDR_LOG_ADDR_3_START);
	MEM_WriteSeq_Byte(MEM_MRAM, (U08) ((gLg_addr_dat3 >> 16) & 0xFF));
	MEM_WriteSeq_Byte(MEM_MRAM, (U08) ((gLg_addr_dat3 >> 8) & 0xFF));
	MEM_WriteSeq_Byte(MEM_MRAM, (U08) (gLg_addr_dat3 & 0xFF));
	MEM_RwSeq_End(MEM_MRAM);

	MEM_WriteByte(MEM_MRAM, MEM_ADDR_LOG_ADDR_3_END, gLg_flg_ovf3);
}

void DAT_ProcSave(void)
{
	if(!MnDAT_GetFgSave())
		return;

	if(MnDAT_GetSavIntv() == MnDAT_INTV_60M)
	{
		if((gLg_hour == gRt.hour) && (gLg_min == gRt.min))
		{
			LOG_ProcData();
			LOG_SaveAddr1();
			LOG_SaveAddr2();
		}
	}
	else if((gLg_hour == gRt.hour) && (gLg_min == gRt.min))
	{
		LOG_ProcData();
		LOG_SaveAddr1();
		LOG_SaveAddr2();
	}
}


void ScTRD_ProcMain(void)
{
	if(!gTd_f_en)
		return;

	switch(BTN_GetIdx())
	{
		case BTN_IDX_LEFT:
			gLg_page++;
			break;

		case BTN_IDX_RIGHT:
			if(gLg_page > 0)
				gLg_page--;
			break;

		case BTN_IDX_EXIT:
			gLg_page = 0;
			DISP_ClearScr();
			gTd_f_en = 0;
			break;

		default:
			break;
	}

	switch(BTN_GetIdx())
	{
		case BTN_IDX_LEFT:
		case BTN_IDX_RIGHT:
			LOG_SetDpAddr();
			
			if(gDp_flg_log == FALSE)
			{
				ScTrdDp_ProcUpd();
				DLY_RunMsec(300);
			} 
			else
				gLg_page--;
			break;

		default:
			break;
	}
}

void ScTRD_ProcEnter(void)
{
	if(!BTN_ChkLong(BTN_IDX_SET, 1000))
		return;

	gTd_f_en = TRUE;
	gLg_page = 0;

	if(DAT_GetMskOvf1() == 0xa1) 
	{
		if(gLg_addr_main == MEM_ADDR_LOG_DATA_START) 	gLg_addr_start = 349996;
		else							gLg_addr_start = gLg_addr_main - 5;
		
		gLg_addr_end = gLg_addr_main + 5;

		gLg_ad_n_start = gLg_addr_start;
		LOG_SetDpAddr();
	}
	else
	{
		gLg_addr_start = gLg_addr_main - 5;
		gLg_addr_end = MEM_ADDR_LOG_DATA_START + 5;

		gLg_ad_n_start = gLg_addr_start;
	}

	DISP_ClearScr();
	ScTrdDp_ProcUpd();

	while(_SBIT_BTN_SET == BTN_STT_PRESS);
}



//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

