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
// bsp
#include "bsp_main.h"
#include "bsp_rest.h"
#include "spi_main.h"
#include "lcd_main.h"
#include "lcd_data.h"
#include "mem_main.h"
#include "tmr_main.h"
#include "rtc_ds1302.h"
#include "prtc_main.h"
#include "uart_main.h"
#include "gpio_main.h"
#include "rly_main.h"
#include "pwm_main.h"
#include "adc_main.h"
#include "dac_main.h"
#include "flash_main.h"
#include "dly_main.h"
// app
#include "isr_main.h"


//--------------------------------------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local Funtions
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global APIs
//--------------------------------------------------------------------------------------------------
void BSP_InitBoot(void)
{
	GIO_InitBoot();
	//BSP_InitPort();
	TMR_InitBoot();
	PWM_InitPca();
	UAT_InitBoot();
	SPI_InitSPI0();
	AD0_InitBoot();
	DAC_InitBoot();
	FLS_InitBoot();
	ETC_InitVoltRef();
	ETC_InitOsc();
	ISR_InitBoot();

	LDT_InitScrn();

	MEM_InitMain(MEM_SRAM);

	UAT_Init2nd();
	DA0_InitOut(0);
}

void BSP_InitPort(void)
{
	SFRPAGE = CONFIG_PAGE;
	P7MDOUT = 0xff;


	SBIT_MRAM_CS   = BIT_HI;
	SBIT_SRAM_CS   = 1;
	SBIT_RTC_CS    = 0;
	SBIT_CURR_OUT1 = 1;



	GIO_PORT_TXCLK = GIO_TXCLK_OFF;

	SBIT_RLY1_OUT = RLY_STT_OFF;
	SBIT_RLY2_OUT = RLY_STT_OFF;
	SBIT_RLY3_OUT = RLY_STT_OFF;
	SBIT_PCD_OUT  = 0;				// State_PCD_UC_G = RLY_OFF;
	SBIT_TEST_PT  = 0;
	
	SBIT_RS485_TR = PRT_485_RX_ON; // mdb

	DLY_RunMsec(500);
}

// EOF

