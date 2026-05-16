//-------------------------------------------------------------------------------------------------
//  Copyright (C) 2020, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use,
//  copying, transfer or disclosure of such information is prohibited except by express written
//  agreement with WESS-Global Co.,LTD
//
//  $Date: 2020-07-13 12:00:00 +0900 $
//  $Author: eddie $
//  $Rev: 1.0 $
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------------------------------------
// chip
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// lib
#include "dly_main.h"
// sys
#include "sys_def.h"
#include "font_def.h"
// app
#include "app_main.h"
#include "font_def.h"
#include "disp_main.h"
#include "lcd_data.h"
// bsp
#include "gpio_main.h"
// self
#include "lcd_main.h"


//--------------------------------------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global APIs - with Local
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local Funtions
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global APIs - Ext. Call
//--------------------------------------------------------------------------------------------------
// OLD: void GLCD_240_Out_dat(char dat)
void LCD_CfgData(char dat)
{
    SFRPAGE = CONFIG_PAGE;
    _SBIT_LCD_RD = 1;				/* make sure the RD/ line is high */
    _SBIT_LCD_CD = 0;				/* clr the CD line low to data */
    P3 = dat;						/* output the data to bus */
    _SBIT_LCD_CE = 0;				/* enable chip select */
    _SBIT_LCD_WR = 0;				/* set the WR/ line low */
    _SBIT_LCD_WR = 0;				/* extra delay for pulse width */
    _SBIT_LCD_WR = 1;				/* set the WR/ line back high */
    _SBIT_LCD_CE = 1;				/* force the CE back high */
    P3 = 0xFF;						/* put the I/O lines back as inputs */
}

// OLD: void GLCD_240_Out_ctl(char dat)
void LCD_CfgCtrl(char dat)
{
    SFRPAGE = CONFIG_PAGE;
    _SBIT_LCD_RD = 1;				/* make sure the RD/ line is high */
    _SBIT_LCD_CD = 1;				/* set the CD line high to cmd */
    P3 = dat;						/* output the data to bus */
    _SBIT_LCD_CE = 0;				/* enable chip select */
    _SBIT_LCD_WR = 0;				/* set the WR/ line low */
    _SBIT_LCD_WR = 0;				/* extra delay for pulse width */
    _SBIT_LCD_WR = 1;				/* set the WR/ line back high */
    _SBIT_LCD_CE = 1;				/* force the CE back high */
    P3 = 0xFF;
}

// OLD: char GLCD_240_In_sta(void)
U08 LCD_GetData(void)
{
    U08 dat;

    SFRPAGE = CONFIG_PAGE;
    P3MDOUT = 0x00;

    _SBIT_LCD_WR = 1;				/* make sure the WR/ line is high */
    _SBIT_LCD_CD = 1;				/* set the CD line high for status */
    P3 = 0xFF;						/* make port A input */
    _SBIT_LCD_CE = 0;				/* enable chip select */
    _SBIT_LCD_RD = 0;				/* set the RD/ line low */
    _SBIT_LCD_RD = 0; 				/* extra read delay */
    _SBIT_LCD_RD = 0;				/* more extra read time */
    dat = P3;						/* fetch in the data */
    _SBIT_LCD_RD = 1;				/* set the RD/ line back high */
    _SBIT_LCD_CE = 1;				/* force the CE back high */

    P3MDOUT   = 0xff;

    return(dat);
}

// OLD: void GLCD_240_Busy_wait(void)
void LCD_ChkBusy(void)
{
    char dat;

    do
		dat = LCD_GetData();    
    while((dat & 0x03) != 0x03);	// wait till STA1=1 && STA0=1
}

// OLD: void GLCD_240_Cmd(char cmd)
void LCD_TxCmd0(char cmd)
{
	SFRPAGE = CONFIG_PAGE;
	LCD_ChkBusy();					/* wait for status on command */
	LCD_CfgCtrl(cmd);				/* send out the command itself */
}

// OLD: void GLCD_240_Cmd_1(char cmd,unsigned char parm)
void LCD_TxCmd1(char cmd, unsigned char parm)
{
	SFRPAGE = CONFIG_PAGE;
	LCD_ChkBusy();					/* wait for the status */
	LCD_CfgData(parm);					/* send out the byte of the data */
	LCD_ChkBusy();					/* wait for status on command */
	LCD_CfgCtrl(cmd);					/* send out the command itself */
}

// OLD: void GLCD_240_Cmd_2(char cmd,unsigned int parm)
void LCD_TxCmd2(char cmd,unsigned int parm)
{
	SFRPAGE = CONFIG_PAGE;
	LCD_ChkBusy();					/* wait for the status */
	LCD_CfgData(parm&0xFF);				/* send out the low byte of the data */
	LCD_ChkBusy();					/* wait again for status */
	LCD_CfgData(parm>>8);				/* send out the high byte of the data */
	LCD_ChkBusy();					/* wait for status on command */
	LCD_CfgCtrl(cmd);					/* send out the command itself */
}

// OLD: void GLCD_240_Erase(void)
void LCD_InitErase(void)
{
	int x, y;

	/* loops to write whole LCD memory to a blank pattern */
	LCD_TxCmd2(0x24,0);			/* setup the AP to 0 */

	for(x=0; x<LDT_BUFF_SIZE_X; x++)
		for(y=0; y<LDT_BUFF_SIZE_Y; y++)
			LCD_TxCmd1(0xC0,0);
}

// OLD: void GLCD_240_Init(void)
void LCD_InitCfg(void)
{
	int err=0;
	
	SFRPAGE = CONFIG_PAGE;
	/* initialize the port control lines to the LCD module */
	_SBIT_LCD_CE = 1;		/* set chip select high off output */
	_SBIT_LCD_RS = 1;		/* set RST signal high off output */
	_SBIT_LCD_CD = 0;		/* set the CD line low as output */
	_SBIT_LCD_RD = 1;		/* set RD line high as output */
	_SBIT_LCD_WR = 1;		/* set WR line high as output */
	_SBIT_LCD_FS = 0;		/* force font select line low */

	/* reset the LCD controller chip */
	_SBIT_LCD_RS = 0;		/* set the reset line low */
	DLY_002usec();
    DLY_002usec();
    DLY_002usec();
    DLY_002usec();
    DLY_002usec(); 			//delay_us(10);
	_SBIT_LCD_RS = 1;		/* release reset to back high */

	/* program the controller mode of operation to graphics only mode */
	
	LCD_TxCmd2(0x42,0);				// set graphics start at zero
	LCD_TxCmd2(0x43,30);			// set number of bytes in horz raster
	LCD_TxCmd2(0x40,30*64);			// set text area after the graphics memory
	LCD_TxCmd2(0x41,40);			// set number of text characters on row @6 bits
	LCD_TxCmd0(0x81);				// setup mode set for text / graphics XOR
	LCD_TxCmd0(0x98);				// enable graphics only mode / no text no cursor
	LCD_TxCmd2(0x24,0);				// setup the AP to 0
	
	DLY_020nsec();
	DLY_100nsec();
}


// EOF

