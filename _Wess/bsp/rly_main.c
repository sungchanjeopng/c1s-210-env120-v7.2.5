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
#include "gpio_main.h"
// app
#include "app_main.h"
#include "rly_main.h"
// msr
#include "measure_analyze.h"
#include "measure_main.h"
// menu
#include "menu_output.h"
#include "menu_measure.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------
U08 gRly_state_01;      // OLD: unsigned char State_RLY_1_UC_G;
U08 gRly_state_02;      // OLD: unsigned char State_RLY_2_UC_G;
U08 gRly_state_03;      // OLD: unsigned char State_RLY_3_UC_G;


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
void RLY_InitVari(void)
{ 
	gRly_state_01 = RLY_STT_OFF;
	gRly_state_02 = RLY_STT_OFF;
	gRly_state_03 = RLY_STT_OFF;
}

void RLY_ProcOut(U16 dat)
{
	U08 old = SFRPAGE;

	SFRPAGE = CONFIG_PAGE;

	if(MnMSR_GetUnit()==MnMSR_UNIT_FEET)
		dat = MEAS_CalcFeet2Meter(dat);

	//High alarm
	if	   (dat >= MnOUT_GetRly1Act())		gRly_state_01 = RLY_STT_ON;
	else if(dat <  MnOUT_GetRly1Stp())		gRly_state_01 = RLY_STT_OFF;

	//low alarm
	if	   (dat <= MnOUT_GetRly2Act())		gRly_state_02 = RLY_STT_ON;
	else if(dat >  MnOUT_GetRly2Stp())		gRly_state_02 = RLY_STT_OFF;

	if(MsANL_GetFgErr())		gRly_state_03 = RLY_STT_ON;
	else						gRly_state_03 = RLY_STT_OFF;

	SBIT_RLY1_OUT = gRly_state_01;
	SBIT_RLY2_OUT = gRly_state_02;
	SBIT_RLY3_OUT = gRly_state_03;

	SFRPAGE = old;
}



//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

