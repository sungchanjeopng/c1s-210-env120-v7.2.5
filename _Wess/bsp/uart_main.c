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
// chipset
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// bsp
#include "flash_main.h"
#include "uart_main.h"
#include "gpio_main.h"
#include "bsp_main.h"
// app
#include "app_main.h"
#include "measure_main.h"
#include "measure_analyze.h"
#include "measure_temperature.h"
#include "dly_main.h"
#include "disp_main.h"
#include "prtc_main.h"
#include "rf_main.h"
#include "isr_main.h"
#include "crc_main.h"
// menu
#include "menu_measure.h"
#include "menu_output.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------
GV_UT gUt;

U08 gUt_level[12];      // OLD: unsigned char UART_data_level_UC_G[12];
U08 gUt_tmprt[12];      // OLD: unsigned char UART_data_temp_UC_G[12];


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
void UAT_InitBoot(void) 
{
	// UART0
	SFRPAGE = UART0_PAGE;
	SCON0   = 0x50;  		// 8bit variable baud rate 
	SSTA0   = 0x0F;
	RI0 = 0;       			// int flag 0 
	TI0 = 0;
	
	// UART1
	SFRPAGE = UART1_PAGE;	// ENV100-M
	SCON1   = 0x50;
	RI1 = 0;
	TI1 = 0;
}

void UAT_Init2nd(void)
{
	SFRPAGE = UART0_PAGE;
    RI0 = 0;  // mdb
}

void UAT_InitVari(void)
{
    S16 i;

    gUt.f_rx = FALSE;

    for(i=0; i<20; i++)
		gUt.rx_pkt[i] = 0;

	gUt.tx_cnt = 0;
}

void UAT_SetLevel(void)
{
	U16 sg_thick = MEAS_GetSgThick();

	gUt_level[3] = (U08)((sg_thick>>8) & 0x00ff);
	gUt_level[4] = (U08)((sg_thick>>0) & 0x00ff);

	gUt_level[5] = 0x00;
	gUt_level[6] = 0x00;

	gUt_level[7] = 0x00;
	gUt_level[8] = 0x00;
}

// OLD: void Comm_Modbus_temp(void)
void UAT_SetTmprt(void)
{
	gUt_tmprt[3] = (S08)((gTp_now>>8) & 0x00ff);
	gUt_tmprt[4] = (S08)((gTp_now>>0) & 0x00ff);
	
	gUt_tmprt[5] = 0x00;
	gUt_tmprt[6] = 0x00;
}

void UAT_SetBaudrate(U08 val)
{
    U08 old = SFRPAGE;

    SFRPAGE = TMR4_PAGE;
    TR4		= ISR_ITR_DISABLE;

    switch(val)
    {
		case MnOUT_MDB_BAUD_04800:	RCAP4H = 0xFC;	RCAP4L = 0xF3;	gUt.baud_cfg = 3600;	break;	// 720*(60/12)
		case MnOUT_MDB_BAUD_09600:	RCAP4H = 0xFE;	RCAP4L = 0x79;	gUt.baud_cfg = 1795;	break;	// 179*(60/12)
		case MnOUT_MDB_BAUD_19200:	RCAP4H = 0xFF;  RCAP4L = 0x3D;	gUt.baud_cfg = 895;		break;
    }

    gUt.baud_cnt = gUt.baud_cfg;

    TR4		= 1;
    SFRPAGE = old;
}

// OLD: void uart_tx_byte(unsigned char dat) 
void UT0_TxByte(U08 dat) 
{
    U08 old = SFRPAGE;

    SFRPAGE = UART0_PAGE;

    SBUF0 = dat;
    while(!TI0);
    TI0 = ISR_ITR_DISABLE;

    SFRPAGE = old;
}

// OLD: void uart_tx_ASCII(unsigned char dat)
void UT0_TxAscii(U08 dat)
{
	U08 i;
	U08 val[2];

	val[0] = (dat / 10) % 10 + '0';
	val[1] =  dat % 10       + '0';

	for(i=0; i<2; i++)
		UT0_TxByte(val[i]);
}

// OLD: void uart1_tx_byte(unsigned char dat) 
void UT1_TxByte(U08 dat)
{
    U08 old = SFRPAGE;

    SFRPAGE = UART1_PAGE;

    SBUF1 = dat;
    while(!TI1);
    TI1 = ISR_ITR_DISABLE;

    SFRPAGE = old;
}

void Ut0Tx_Standard(void)
{
	U08 i;
	S16 tmp2;
	U08 old = SFRPAGE;
	U08 unit = MnMSR_GetUnit();
	S16 level_mt = MsANL_GetLevelMt();
	S16 level_ft = MsANL_GetLevelFt();
	S16 dista_mt = MsANL_GetDistaMt();
	S16 dista_ft = MsANL_GetDistaFt();

	SFRPAGE = UART0_PAGE;

	UT0_TxByte(0x0d);    //CR
	UT0_TxByte(0x0a);    //LF

	UT0_TxByte(0x44);    //'D'

	if(unit == MnMSR_UNIT_FEET)
	{
		gDp_buff_str[0] = (dista_ft / 1000) % 10 + '0';
		gDp_buff_str[1] = (dista_ft /  100) % 10 + '0';
		gDp_buff_str[2] = (dista_ft /   10) % 10 + '0';
		gDp_buff_str[3] =  dista_ft         % 10 + '0';
	}
	else
	{
		gDp_buff_str[0] = (dista_mt / 1000) % 10 + '0';
		gDp_buff_str[1] = (dista_mt /  100) % 10 + '0';
		gDp_buff_str[2] = (dista_mt /   10) % 10 + '0';
		gDp_buff_str[3] =  dista_mt         % 10 + '0';
	}

	for(i=0; i<4; i++)                     //'Transfer Rag Level, unit 000cm
		UT0_TxByte(gDp_buff_str[i]);

	UT0_TxByte(0x53);    //'S'

	if(unit == MnMSR_UNIT_FEET)
	{
		gDp_buff_str[0] = (level_ft / 1000) % 10 + '0';
		gDp_buff_str[1] = (level_ft /  100) % 10 + '0';
		gDp_buff_str[2] = (level_ft /   10) % 10 + '0';
		gDp_buff_str[3] =  level_ft         % 10 + '0';
	}
	else
	{
		gDp_buff_str[0] = (level_mt / 1000) % 10 + '0';
		gDp_buff_str[1] = (level_mt /  100) % 10 + '0';
		gDp_buff_str[2] = (level_mt /   10) % 10 + '0';
		gDp_buff_str[3] =  level_mt         % 10 + '0';
	}

	for (i = 0; i < 4; i++)                   //Transfer Sludge Level, 000unit cm
		UT0_TxByte(gDp_buff_str[i]);

	UT0_TxByte(0x54);   //'T'
	tmp2 = gTp_now;         //Trasnfer Temperature, 00unit 1'

	if (tmp2 < 0)
	{
		gDp_buff_str[0] = 0x2d;      //'-'
		tmp2 = tmp2 * -1;
	}
	else
		gDp_buff_str[0] = 0x2b;      //'+'

	gDp_buff_str[1] = (tmp2 / 100) % 10 + '0';
	gDp_buff_str[2] = (tmp2 /  10) % 10 + '0';

	for (i=0; i<3; i++)
		UT0_TxByte(gDp_buff_str[i]);

	UT0_TxByte(0x45);   // 'E'

	switch(MsANL_GetErrStat())
	{
		case MsANL_ERR_NONE:		UT0_TxByte(0x30);		UT0_TxByte(0x30);		break;		// '00'
		case MsANL_ERR_ECHO:		UT0_TxByte(0x30);		UT0_TxByte(0x31);		break;		// '01'
		case MsANL_ERR_TPRT:		UT0_TxByte(0x30);		UT0_TxByte(0x32);		break;		// '02'
	}

	UT0_TxByte(0x00);    // NULL

	SFRPAGE = old;
}

// OLD: void data_tx_RF(void) 
void data_tx_RF(void) 
{
	U08 i;
	U08 tx_buff[20];
	U16 aux;
	U08 cnt = 0;
	U08 old = SFRPAGE;
	U08 rf_ch = MnOUT_GetRfCh();
	S16 level_mt = MsANL_GetLevelMt();
	S16 level_ft = MsANL_GetLevelFt();

	SFRPAGE = UART0_PAGE;

	tx_buff[cnt++] = (gUt.tx_cnt>>8)&0xff;	//master id
	tx_buff[cnt++] = (gUt.tx_cnt>>0)&0xff; 	//master id
	tx_buff[cnt++] = '=';
	tx_buff[cnt++] = 0x05; // Header	
	tx_buff[cnt++] = (rf_ch / 10) + 0x30;	//slave id
	tx_buff[cnt++] = (rf_ch % 10) + 0x30; 	//slave id
	tx_buff[cnt++] = 'S';

	if(MnMSR_GetUnit() == MnMSR_UNIT_FEET)
	{
		tx_buff[cnt++] = (level_ft / 1000) % 10 + '0';
		tx_buff[cnt++] = (level_ft / 100)  % 10 + '0';
		tx_buff[cnt++] = (level_ft / 10)   % 10 + '0';
		tx_buff[cnt++] =  level_ft         % 10 + '0';
	}
	else
	{
		tx_buff[cnt++] = (level_mt / 1000) % 10 + '0';
		tx_buff[cnt++] = (level_mt / 100)  % 10 + '0';
		tx_buff[cnt++] = (level_mt / 10)   % 10 + '0';
		tx_buff[cnt++] =  level_mt         % 10 + '0';
	} 		  
	
	aux = DSP_GetCurrent();

	tx_buff[cnt++] = 'A';                     // 'A'
	tx_buff[cnt++] = (aux / 1000) % 10 + '0';
	tx_buff[cnt++] = (aux / 100)  % 10 + '0';
	tx_buff[cnt++] = (aux / 10)   % 10 + '0';
	tx_buff[cnt++] =  aux         % 10 + '0';
	tx_buff[cnt++] = 0x04;	// Tail
	tx_buff[cnt++] = 0x0D;	// CR  

	for(i=0; i<cnt; i++)
		UT0_TxByte(tx_buff[i]);

	SFRPAGE = old;
}

void data_tx_RF_1(void) 
{
	U08 i;
	U08 tx_buff[20];
	U16 aux;
	U08 cnt = 0;
	U08 old = SFRPAGE;
	U08 rf_ch = MnOUT_GetRfCh();
	S16 level_mt = MsANL_GetLevelMt();
	S16 level_ft = MsANL_GetLevelFt();

	SFRPAGE = UART1_PAGE;

	tx_buff[cnt++] = (gUt.tx_cnt>>8)&0xff;	//master id
	tx_buff[cnt++] = (gUt.tx_cnt>>0)&0xff;	//master id
	tx_buff[cnt++] = '=';
	tx_buff[cnt++] = 0x05; // Header	
	tx_buff[cnt++] = (rf_ch / 10) + 0x30;	//slave id
	tx_buff[cnt++] = (rf_ch % 10) + 0x30; 	//slave id
	tx_buff[cnt++] = 'S';

	if(MnMSR_GetUnit() == MnMSR_UNIT_FEET)
	{
		tx_buff[cnt++] = (level_ft / 1000) % 10 + '0';
		tx_buff[cnt++] = (level_ft / 100)  % 10 + '0';
		tx_buff[cnt++] = (level_ft / 10)   % 10 + '0';
		tx_buff[cnt++] =  level_ft         % 10 + '0';
	}
	else
	{
		tx_buff[cnt++] = (level_mt / 1000) % 10 + '0';
		tx_buff[cnt++] = (level_mt / 100)  % 10 + '0';
		tx_buff[cnt++] = (level_mt / 10)   % 10 + '0';
		tx_buff[cnt++] =  level_mt         % 10 + '0';
	} 		  
	
	aux = DSP_GetCurrent();

	tx_buff[cnt++] = 'A';                     // 'A'
	tx_buff[cnt++] = (aux / 1000) % 10 + '0';
	tx_buff[cnt++] = (aux / 100)  % 10 + '0';
	tx_buff[cnt++] = (aux / 10)   % 10 + '0';
	tx_buff[cnt++] =  aux         % 10 + '0';
	tx_buff[cnt++] = 0x04;	// Tail
	tx_buff[cnt++] = 0x0D;	// CR  

	for(i=0; i<cnt; i++)
		UT1_TxByte(tx_buff[i]);
	
	SFRPAGE = old;
}

void UT0_PrcTx(U08 sel)
{
	U08 old = SFRPAGE;

#if 1

	switch(sel)
	{
		case MnOUT_PROT_STD: 	UAT_SetBaudrate(MnOUT_MDB_BAUD_19200);		break;
		case MnOUT_PROT_RF:	 	UAT_SetBaudrate(MnOUT_MDB_BAUD_09600);		break;
		case MnOUT_PROT_MDBS:	
		default:				return;
	}
	
	SFRPAGE = CONFIG_PAGE;
	SBIT_RS485_TR = PRT_485_TX_ON;
	DLY_RunMsec(1);

	switch(sel)
	{
		case MnOUT_PROT_STD: 	
			Ut0Tx_Standard();		
			break;
		case MnOUT_PROT_RF:	 	
			gUt.tx_cnt++;
			data_tx_RF();
			DLY_RunMsec(1);
			data_tx_RF_1();
			break;
		default:			
			break;
	}

	SFRPAGE = CONFIG_PAGE;
	SBIT_RS485_TR = PRT_485_RX_ON;


	SFRPAGE = old;

#else

	if(MnOUT_GetProtocol() == MnOUT_PROT_MDBS)
		return;

	UAT_SetBaudrate(1);

	SFRPAGE = CONFIG_PAGE;
	SBIT_RS485_TR = PRT_485_TX_ON;
	DLY_RunMsec(1);

	if(sel == 0) 
		Ut0Tx_Standard();
	else if (sel == 1) 
	{
		#if 1
		gUt.tx_cnt++;
		#endif
		data_tx_RF();
	}

	SFRPAGE = CONFIG_PAGE;
	SBIT_RS485_TR = PRT_485_RX_ON;

	UAT_SetBaudrate(1);

	SFRPAGE = old;
#endif
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

