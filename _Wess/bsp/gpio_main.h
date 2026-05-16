//--------------------------------------------------------------------
//
//  Copyright (C) 2020, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of
//  WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement
//  with WESS-Global Co.,LTD
//
//  $Id: templet_format.h 1 2020-07-13 12:00:00Z eddie $
//  $Date: 2020-07-13 12:00:00 +0900 $
//  $Author: eddie $
//  $Rev: 1.0 $
//
//--------------------------------------------------------------------

#ifndef ___GPIO_MAIN_H___
#define ___GPIO_MAIN_H___

//--------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Constant definitions
//--------------------------------------------------------------------
#define GIO_TXCLK_ON1			0x79	// ENV100-S
#define GIO_TXCLK_ON2			0x7A	// ENV100-S
#define GIO_TXCLK_OFF		0x78	// ENV100-S

#define GIO_SET_HI		1
#define GIO_SET_LO		0


//--------------------------------------------------------------------
//  Type definitions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Macros
//--------------------------------------------------------------------
#define GIO_PORT_TXCLK		P7

// Port0
sbit SBIT_SPI_CLK	= P0^2;
sbit SBIT_SPI_MISO 	= P0^3;
sbit SBIT_SPI_MOSI 	= P0^4;
// Port1
sbit SBIT_MSR_GAIN  = P1^0;     // OLD: sbit AMP_2nd_gain = P1^0;
// Port2
sbit SBIT_SRAM_CS	= P2^0;
sbit SBIT_RLY1_OUT	= P2^2;		// OLD: sbit RLY_1_out = P2^2;
sbit SBIT_RLY2_OUT	= P2^3;		// OLD: sbit RLY_2_out = P2^3;
sbit SBIT_RLY3_OUT	= P2^4;		// OLD: sbit RLY_3_out = P2^4;
sbit SBIT_PCD_OUT	= P2^5;		// OLD: sbit PCD_out = P2^5;
// Port4
sbit SBIT_MRAM_CS  	= P4^0;
sbit SBIT_RTC_CS	= P4^1;

sbit _SBIT_BTN_LEFT  = P4^2;
sbit _SBIT_BTN_DOWN  = P4^3;
sbit _SBIT_BTN_UP    = P4^4;
sbit _SBIT_BTN_RIGHT = P4^5;
sbit _SBIT_BTN_PROG  = P4^6;
sbit _SBIT_BTN_SET   = P4^7;
// Port5
sbit _SBIT_BTN_EXIT  = P5^0;
sbit _GIO_VC_A0 = P5^5;
sbit _GIO_VC_A1 = P5^4;
sbit _GIO_VC_A2 = P5^3;
sbit _GIO_VC_A3 = P5^2;
// Port6
sbit SBIT_MSR_BPF   = P6^0;		// OLD: sbit Sel_BPF_freq = P6^0;
sbit SBIT_TEST_PT   = P6^2;		// OLD: sbit Test_port = P6^2;
sbit SBIT_RS485_TR  = P6^3;     // OLD: sbit RS_485_TR_sw	= P6^3;  	// mdb
sbit SBIT_CURR_OUT1 = P6^4;		// OLD: sbit CS_Current_out_1 = P6^4;	// ENV100-S
sbit SBIT_P6_B6	    = P6^6;		// OLD: sbit P6_EXC6   = P6^6;			// ENV100-S
// Port6
// equate the LCD Controller control signals to their port assignments //
sbit _SBIT_LCD_CE  = P7^5;		// port bit for Chip Select //
sbit _SBIT_LCD_CD  = P7^2;		// port bit for Control/Data Select //
sbit _SBIT_LCD_WR  = P7^4;		// port bit for the Write Strobe //
sbit _SBIT_LCD_RD  = P7^3;					// port bit for the Read Strobe //
sbit _SBIT_LCD_RS  = P7^6;				// port bit for Display Reset //
sbit _SBIT_LCD_FS  = P7^7;					// port bit for Font Select //

//--------------------------------------------------------------------
//  Prototypes of the functions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Extern global variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Extern Prototypes of the APIs
//--------------------------------------------------------------------
extern void GIO_InitBoot(void);



#endif // ___GPIO_MAIN_H___

// EOF

