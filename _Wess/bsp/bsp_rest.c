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
// chipset
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// bsp
// app
// self
#include "bsp_rest.h"


//--------------------------------------------------------------------
//  Local Macros
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Local Type definitions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Funtion Definitions
//--------------------------------------------------------------------
// OLD: void Init_Voltage_Reference(void)
void ETC_InitVoltRef(void)
{
	SFRPAGE = LEGACY_PAGE;
	REF0CN = 0x07;
}

// OLD: void Init_Oscillator(void)
void ETC_InitOsc(void)
{
	int i = 0;

	SFRPAGE = CONFIG_PAGE;

	OSCXCN = 0x67;               	// external osc control, crystal osc mode, 10~30mhz
	
	for(i=0; i<3000; i++);  		// Wait 1ms for initialization
	while((OSCXCN & 0x80) == 0);
	
	PLL0CN = 0x04;
	CCH0CN &= ~0x20;
	SFRPAGE = LEGACY_PAGE;
	FLSCL = 0xA0;                // sys clock 75mhz 이하

	SFRPAGE = CONFIG_PAGE;
	CCH0CN |= 0x20;
	PLL0CN |= 0x01;   // pll on
	PLL0DIV = 0x01;   // 나누기 1 
	PLL0FLT = 0x17;   // 45~90mhz, 7.8~12.5mhz
	PLL0MUL = 0x05;   // 곱하기 5
	
	for (i=0; i<15; i++);  // Wait 5us for initialization
	
	PLL0CN |= 0x02;   // pll enable
	
	while((PLL0CN & 0x10) == 0);  // pll frequency is locked
	
	CLKSEL = 0x02;    // sys clk from pll
	OSCICN &= ~0x80;  // programmed internal osc is devided by 1
}


// EOF

