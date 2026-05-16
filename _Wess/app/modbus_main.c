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
#include "modbus_main.h"
// lib
#include "crc_main.h"
#include "dly_main.h"
// bsp
#include "bsp_main.h"
#include "flash_main.h"
#include "uart_main.h"
#include "gpio_main.h"
// app
#include "prtc_main.h"
#include "app_main.h"
#include "measure_temperature.h"
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
void MDB_SetLevel(void)
{
    U08 cnt, i;

    cnt = (gUt.rx_pkt[MDB_PKT_05_SIZE] * 2) + 3;

    gUt.tx_pkt[0] = MnOUT_GetMdbAddr();
    gUt.tx_pkt[1] = 0x03;
    gUt.tx_pkt[2] = gUt.rx_pkt[MDB_PKT_05_SIZE] * 2;

    for(i=3; i<cnt; i++)
    	gUt.tx_pkt[i] = gUt_level[i + (gUt.rx_pkt[3]*2)];

    gCc_b16 = CRC_GetB16(gUt.tx_pkt, cnt);

    gUt.tx_pkt[cnt]   = (U08)((gCc_b16>>0) & 0x00ff);
    gUt.tx_pkt[cnt+1] = (U08)((gCc_b16>>8) & 0x00ff);
}

void MDB_SetTmprt(void)
{
    U08 i;
    U08 dmy_0 = (gUt.rx_pkt[MDB_PKT_05_SIZE] * 2) + 3;

    gUt.tx_pkt[0] = MnOUT_GetMdbAddr();
    gUt.tx_pkt[1] = 0x03;
    gUt.tx_pkt[2] = gUt.rx_pkt[MDB_PKT_05_SIZE] * 2;

    for(i=3; i<dmy_0; i++)
        gUt.tx_pkt[i] = gUt_tmprt[i + ((gUt.rx_pkt[3] - 9) * 2)];

    gCc_b16 = CRC_GetB16(gUt.tx_pkt, dmy_0);

    gUt.tx_pkt[dmy_0]   = (U08)((gCc_b16>>0) & 0x00ff);
    gUt.tx_pkt[dmy_0+1] = (U08)((gCc_b16>>8) & 0x00ff);
}

void MDB_TxMsg(U08 *p_msg, U08 size)
{
    U08 i = 0;

    while(size--)
    {
        UT0_TxByte(p_msg[i]);
        i++;
    }
}

void MDB_PrcMain(void)
{
	U16 dly = 0;
	U08 old = SFRPAGE;

	if(!gUt.f_rx)
		return;

	SFRPAGE = CONFIG_PAGE;
	EA		= ISR_ITR_DISABLE;

	switch(MnOUT_GetMdbBaud())
	{
	    case MnOUT_MDB_BAUD_04800:     dly = 1682;    break;
	    case MnOUT_MDB_BAUD_09600:     dly = 839;     break;
	    case MnOUT_MDB_BAUD_19200:     dly = 418;     break;
	}

	while(dly--);

	SBIT_RS485_TR = PRT_485_TX_ON;

	if(gUt.rx_pkt[MDB_PKT_01_CMD] == MDB_CMD_03_REQ_READ)
	{
	    if((gUt.rx_pkt[MDB_PKT_03_ADDR] < 3) && (gUt.rx_pkt[MDB_PKT_05_SIZE] < 4))
	    {
			MDB_SetLevel();
			MDB_TxMsg(gUt.tx_pkt, (gUt.rx_pkt[MDB_PKT_05_SIZE]*2) + 5);
	    }
	    else if((gUt.rx_pkt[MDB_PKT_03_ADDR] > 8) && (gUt.rx_pkt[MDB_PKT_03_ADDR] < 11) &&
			    (gUt.rx_pkt[MDB_PKT_05_SIZE] < 3))
	    {
	        MDB_SetTmprt();
	        MDB_TxMsg(gUt.tx_pkt, (gUt.rx_pkt[MDB_PKT_05_SIZE]*2) + 5);
	    }
	}

	DLY_RunMsec(5);

	SBIT_RS485_TR = PRT_485_RX_ON;

	gUt.f_rx = FALSE;
	gUt.rx_pkt[MDB_PKT_00_NULL] = 0;

	EA 		= ISR_ITR_ENABLE;		// Interrupt Enable
	SFRPAGE = old;
}


// EOF

