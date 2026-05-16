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
#include "bsp_main.h"
#include "gpio_main.h"
// self
#include "dly_main.h"


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
void DLY_020nsec(void)      // 20ns / (1 / 60MHz) = 1.2
{
    _nop_();    _nop_();
}

void DLY_100nsec(void)      // 100ns / (1 / 60MHz) = 6
{
    _nop_();    _nop_();
    _nop_();    _nop_();
    _nop_();    _nop_();
}

void DLY_001usec(void)
{
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();

    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();

    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();

    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     _nop_();
    SBIT_P6_B6 = 0;     SBIT_P6_B6 = 0;     //_nop_ ();// _nop_ ();
}

void DLY_002usec(void) 
{
    DLY_001usec();
    DLY_001usec();
}

void DLY_RunMsec(U16 ms) 
{
	U16 i;

	ms *= 50;

	for(i=0; i<ms; i++) 
	{
		DLY_002usec();
		DLY_002usec();
		DLY_002usec();
		DLY_002usec();
		DLY_002usec();
		DLY_002usec();
		DLY_002usec();
		DLY_002usec();
		DLY_002usec();
		DLY_002usec();
	}
}


// EOF

