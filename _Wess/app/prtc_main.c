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
// self
#include "prtc_main.h"
// bsp
#include "flash_main.h"
#include "uart_main.h"
#include "gpio_main.h"
// app
#include "rf_main.h"
#include "dly_main.h"
// isr
#include "isr_main.h"
// menu
#include "menu_output.h"


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
void PRT_InitBoot(void)
{
	U08 mdb_baud = MnOUT_GetMdbBaud();

	switch(MnOUT_GetProtocol())
	{
		case MnOUT_PROT_MDBS:	UAT_SetBaudrate(mdb_baud);			break;
		case MnOUT_PROT_RF:		UAT_SetBaudrate(UAT_BAUD_09600);	DLY_RunMsec(1);		break;
		default:            	UAT_SetBaudrate(UAT_BAUD_09600);	break;
	}

}

void PRT_ProcMain(void)
{
	switch(MnOUT_GetProtocol())
	{
		case MnOUT_PROT_MDBS:
			ES0 = ISR_ITR_ENABLE;
			SBIT_RS485_TR = PRT_485_RX_ON;
			
			UAT_SetLevel();
			UAT_SetTmprt();
			break;

		default:
			ES0 = ISR_ITR_DISABLE;
			break;
	}
}


// EOF

