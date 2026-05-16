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
#include "pwm_main.h"


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
// OLD: void Init_PCA(void)
void PWM_InitPca(void)		// PCA: Programmable Counter Array
{
	SFRPAGE = PCA0_PAGE;
	PCA0MD = 0x88;
}


// EOF

