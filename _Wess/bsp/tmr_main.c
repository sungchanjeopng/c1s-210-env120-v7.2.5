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
// self
#include "tmr_main.h"


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
void TMR_InitBoot(void) 
{
 	// Disable watchdog timer
 	WDTCN = 0xde;
	WDTCN = 0xad;

	SFRPAGE = TIMER01_PAGE;
	TCON  = 0x40;	   		// ENV100-M Enable Timer1 to UART1
	TMOD  = 0x21;    		// tmr1 8bit couneter/timer with auto-reload, tmr0 16bit counter/timer 
	CKCON = 0x02;   		// tmr0,1 prescaled clk devied by 48
	
	TH1	= 0xBF;				// ENV100-M UART 1 BPS 9600
	TH0	= 0x00;				// ENV100-M UART 1 BPS 9600

	// TVG curve
	TH0 = 0xFE;     		// 20Cm = 270.27us - 270.27us / ((1 / SYSCLK_E100) * 48) = 338, ffff - 152(338) = FEAD
	TL0 = 0xAD;				// ex) TH0 = 0xFF; //TL0 = 0x56;  // 10Cm
	
	SFRPAGE = TMR2_PAGE;
	TMR2CF  = 0x08;			// ADC interval

	SFRPAGE = TMR3_PAGE;	// Timer3 : 1sec delay
	RCAP3H = 0x3C;
	RCAP3L = 0xAF;			// (1 / 60 Mhz) * 12 = 200 ns : 10 ms / 200 ns = 50000, ffff - c350(50000) = 3caf
	TMR3CN = 0x04;
	TMR3H  = 0x3C;
	TMR3L  = 0xAF;

	SFRPAGE = TMR4_PAGE;
	TMR4CF = 0x08;			// Baud Rate for UART0, sys clk
}

void TMR_CfgResume(void)
{
	SFRPAGE = TMR3_PAGE;

	TMR3CN = 0x04;

	SFRPAGE = UART0_PAGE;
	RI0 = 0;
	EA  = 1;
	ES0 = 0;

	SFRPAGE = CONFIG_PAGE;
}

void TMR3_Start1sec(void)
{
    U08 save = SFRPAGE;        // preserve SFRPAGE

    SFRPAGE = TMR3_PAGE;
    TR3 = 1;
    SFRPAGE = save;
}

void TMR3_Stop1sec(void)
{
    U08 save = SFRPAGE;        // preserve SFRPAGE

    SFRPAGE = TMR3_PAGE;
    TR3 = 0;
    SFRPAGE = save;
}


// EOF

