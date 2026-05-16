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
#include "dac_main.h"
// lib
#include "dly_main.h"
// bsp
#include "bsp_main.h"
#include "spi_main.h"
#include "flash_main.h"
#include "gpio_main.h"
// app
#include "app_main.h"
// msr
#include "measure_main.h"
#include "measure_analyze.h"
// menu
#include "menu_measure.h"
#include "menu_output.h"
#include "menu_factory.h"


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
void DAC_InitBoot(void)
{
	SFRPAGE = DAC0_PAGE;
	DAC0CN  = 0x80;

	SFRPAGE = DAC1_PAGE;
	DAC1CN  = 0x80;
}

// OLD: void DAC0_InitOut(unsigned int dac0_value)
void DA0_InitOut(U16 dac0_value)
{
	U08 old = SFRPAGE;

	SFRPAGE = DAC0_PAGE;

	DAC0L = dac0_value;
	DAC0H = dac0_value >> 8;

	SFRPAGE = old;
}

// OLD: void DAC0_out(unsigned int dac0_value)
void DAC_WriteVal(U16 val)
{
	U08 old = SFRPAGE;

	SFRPAGE = DAC0_PAGE;

	DAC0L = val;
	DAC0H = val >> 8;

	SFRPAGE = old;
}

// OLD: void Out_DAC_SPI(unsigned int DA_dat)
void DAC_OutSpi(U16 dat)
{
	U08 out;
	U32 val;
	U08 old = SFRPAGE;

	SFRPAGE = SPI0_PAGE;    // Switch to the necessary SFRPAGE
	SPI0CFG = 0x50;         // CKPHA = '0', CKPOL = '1'
	SPI0CKR = 0x95;			// fspi = SYSCLK/(2*(SPI0CKR+1)) = 200kHz;
	DLY_002usec();
	DLY_002usec();

	SFRPAGE = CONFIG_PAGE;
	SBIT_CURR_OUT1 = 0;
	DLY_RunMsec(1);

	val = dat & 0x0000ffff;

	out = val >> 16;	SPI0_WriteByte(out);
	out = val >> 8;		SPI0_WriteByte(out);
	out = val;			SPI0_WriteByte(out);

	SFRPAGE = CONFIG_PAGE;
	SBIT_CURR_OUT1 = 1;

	SFRPAGE = SPI0_PAGE;	// Switch to the necessary SFRPAGE
	SPI0CFG = 0x40;			// CKPHA = '0', CKPOL = '0'
	DLY_002usec();
	DLY_002usec();

	SPI0CKR = 0x01;			// fspi = SYSCLK/(2*(SPI0CKR+1)) = 15MHz;

	SFRPAGE = old;
}

void DAC_OutCurr(U16 val, U08 sel)
{
	U32 outp, max, min, mid;
	U32 cnt1, cnt2, range;
	U16 curr_20ma, curr_04ma;
	U16 set04mA   = MnOUT_GetSet04mA();
	U16 set20mA   = MnOUT_GetSet20mA();
	U08 trim_12mA = MnOUT_GetTrim12mA();
	U08 trim_20mA = MnOUT_GetTrim20mA();
	U08 echo_out  = MnOUT_GetEchoOut();
#if 1
	U16 dac_04mA  = MnFTR_GetDac04mA();
	U16 dac_20mA  = MnFTR_GetDac20mA();
	U16 dac_21mA  = MnFTR_GetDac21mA();
	U16 dac_38mA  = MnFTR_GetDac38mA();
#else
	U16 dac_04mA  = MnOUT_GetDac04mA();
	U16 dac_20mA  = MnOUT_GetDac20mA();
	U16 dac_21mA  = MnOUT_GetDac21mA();
	U16 dac_38mA  = MnOUT_GetDac38mA();
#endif

	switch(MnMSR_GetUnit())
	{
		case MnMSR_UNIT_METER:
			curr_04ma = set04mA;
			curr_20ma = set20mA;
			break;
		case MnMSR_UNIT_FEET:
			curr_04ma = (U16)(set04mA * MSR_CALC_M2F_CONST); //+ 0.5;
			curr_20ma = (U16)(set20mA * MSR_CALC_M2F_CONST); //+ 0.5;
			break;
	}

	range = curr_20ma - curr_04ma;

	max =   dac_20mA + (trim_20mA - 50);
	mid = ((dac_20mA -  dac_04mA) / 2) + dac_04mA;
	min =   dac_04mA - (trim_20mA - 50);
	cnt2 = (max - min) * 1.0;

	switch(sel)
	{
		case DAC_CURR_IDX_NORMAL:
			if(val < curr_04ma)		cnt1 = 0;
			else					cnt1 = val - curr_04ma;
			outp = ((cnt1 * cnt2) / range) + min;

			if(outp >= max)			outp = max;
			if(outp <= min)			outp = min;

			if(MsANL_GetFgErr())
			{
				if(echo_out == MnOUT_ECHO_OUT_38mA)		outp = dac_38mA;
				if(echo_out == MnOUT_ECHO_OUT_HOLD)		goto DAEND;
				if(echo_out == MnOUT_ECHO_OUT_21mA)		outp = dac_21mA;
			}
			break;

		case DAC_CURR_IDX_04mA:		outp = min;		break;
		case DAC_CURR_IDX_12mA:		outp = mid;		break;
		case DAC_CURR_IDX_20mA:		outp = max;		break;
	}
	outp = outp + trim_12mA - 50;
	if (outp >= 65536)
		outp = 65535;

	DAC_OutSpi(outp);

	DAEND: ;
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

