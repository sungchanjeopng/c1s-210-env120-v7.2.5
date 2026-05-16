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
#include "gpio_main.h"


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
// OLD: void Init_Port_IO_ENV100_S(void)
void GIO_InitBoot(void)
{
	SFRPAGE = CONFIG_PAGE;
	P0MDOUT = 0xf5;
	P1MDOUT = 0x01;
	P2MDOUT = 0xff;
	P3MDOUT = 0xff;
	P4MDOUT = 0x03;
	P5MDOUT = 0xfe;
	P6MDOUT = 0xff;
	P7MDOUT = 0xfc;

	XBR0 = 0x06;	//0x04;
	XBR2 = 0x44;	//uart1 
}


// EOF

