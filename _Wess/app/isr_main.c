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
#include "isr_main.h"
// bsp
#include "flash_main.h"
#include "uart_main.h"
// app
#include "crc_main.h"
#include "prtc_main.h"
// msr
#include "measure_main.h"
// menu
#include "menu_output.h"


//--------------------------------------------------------------------
//  Local Macros
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Local Type definitions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------
U16 gIsr_tm3_cnt;			// OLD: unsigned int  Cnt_1sec_delay_UI_G;


//--------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------
U08 lIs_ut0_rx_cnt; 		// OLD: unsigned char UART_Rx_data_num_UC_G;


//--------------------------------------------------------------------
// Funtion Definitions
//--------------------------------------------------------------------
// OLD: void Init_Interrupts(void)
void ISR_InitBoot(void)
{
	IP = 0x10;				// UART0 interrupt priority high
	IE = 0x82;				// EA = 1; ES0 = 0; ET0 = 1;
	EIE2 = 0x01;			// ET3 = 1;
	//EIE1 = 0x02;			// Enable the SMBus interrupt
}

void ISR_InitVari(void)
{
	gIsr_tm3_cnt = 0;
}

void Interrupt_TIMER0(void)		// Interval: about 270 usec - 20cm
interrupt 1
{
	U08 old = SFRPAGE;

	SFRPAGE = TIMER01_PAGE;

	TF0 = 0;
	TH0 = 0xFE;
	TL0 = 0xAD;
	MEAS_SetDacTvg();

	SFRPAGE = old;
}

void Interrupt_TIMER3(void)		// Interval: 10msec
interrupt 14
{
	U08 old = SFRPAGE;
	SFRPAGE = TMR3_PAGE;

	TF3 = 0;
	gIsr_tm3_cnt++;

	SFRPAGE = old;
}

void Interrupt_UART0(void)
interrupt 4
{
	U08 old = SFRPAGE;
	U08 protocol = MnOUT_GetProtocol();
	U08 mdb_addr = MnOUT_GetMdbAddr();

	SFRPAGE = UART0_PAGE;

	if(RI0)
	{
		EA = 0; ES0 = 0;// ET1 = 0;
		
		SFRPAGE = TMR3_PAGE;
		TMR3CN = 0x00;
		
		SFRPAGE = UART0_PAGE;
	
		if(lIs_ut0_rx_cnt > MDB_RX_PKT_SIZE)
			lIs_ut0_rx_cnt = 0;
		
		if(protocol == MnOUT_PROT_MDBS)
		{
			do
			{
				gUt.baud_cnt = gUt.baud_cfg;
				while(!RI0 && gUt.baud_cnt--);
				
				gUt.rx_pkt[lIs_ut0_rx_cnt] = SBUF0;
				RI0 = 0;
				lIs_ut0_rx_cnt++;
			}
			while((gUt.rx_pkt[MDB_PKT_00_NULL] == mdb_addr) && (lIs_ut0_rx_cnt < 8) && gUt.baud_cnt);
			
			if((gUt.rx_pkt[MDB_PKT_00_NULL] == mdb_addr) && gUt.baud_cnt != 0)
			{
				gCc_b16 = CRC_GetB16(gUt.rx_pkt, (lIs_ut0_rx_cnt-2));

				if(gUt.rx_pkt[lIs_ut0_rx_cnt-2] == (U08)((gCc_b16>>0) & 0x00ff) && 
				   gUt.rx_pkt[lIs_ut0_rx_cnt-1] == (U08)((gCc_b16>>8) & 0x00ff))
				{
					gUt.f_rx = TRUE;
				}
			}
			
			lIs_ut0_rx_cnt = 0;
			gUt.baud_cnt = gUt.baud_cfg;
		}
		
		SFRPAGE = TMR3_PAGE;
		TMR3CN = 0x04;
			
		EA = 1;	
			
		if(protocol == MnOUT_PROT_MDBS)	
			ES0 = 1;	// ET1 = 1;
		
		RI0 = 0;			 
	}

	SFRPAGE = old;
}


// EOF

