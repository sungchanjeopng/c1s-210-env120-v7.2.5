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

#ifndef ___UART_MAIN_H___
#define ___UART_MAIN_H___

//--------------------------------------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------------------------------------
#include "sys_def.h"
#include "modbus_main.h"


//--------------------------------------------------------------------------------------------------
//  Constant definitions
//--------------------------------------------------------------------------------------------------
enum {
    UT_COM_IDLE = 0,
    UT_COM_RF,
    UT_COM_MODEBUS,

    UT_COM_MAX,
};

enum {
	UAT_BAUD_04800 = 0,
	UAT_BAUD_09600,
	UAT_BAUD_19200,
};


//--------------------------------------------------------------------------------------------------
//  Type definitions
//--------------------------------------------------------------------------------------------------
typedef struct {
	U08 f_rx; 							// OLD: bit Flag_485_Rx_B_G;
	U08 rx_pkt[MDB_RX_PKT_SIZE];		// OLD: U08 UART_Rx_data_UC_G[20];
	U08 tx_pkt[MDB_TX_PKT_SIZE]; 		// OLD: unsigned char Modbus_data_UC_G[12];

	U16 baud_cfg;		// OLD: unsigned int Cnt_UART_dly_default_UI_G;
	U16 baud_cnt;		// OLD: unsigned int Cnt_UART_dly_UI_G;

	#if 1
	U16 tx_cnt;
	#endif
} GV_UT;


//--------------------------------------------------------------------------------------------------
//  Macros
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Extern global variables
//--------------------------------------------------------------------------------------------------
extern GV_UT gUt;

extern U08 gUt_level[];
extern U08 gUt_tmprt[];


//--------------------------------------------------------------------------------------------------
//  Extern global APIs
//--------------------------------------------------------------------------------------------------
extern void UAT_InitBoot(void);
extern void UAT_Init2nd(void);
extern void UAT_InitVari(void);

extern void UAT_SetLevel(void);
extern void UAT_SetTmprt(void);

extern void UAT_SetBaudrate(U08 BAUD_rate_UC_L);

extern void UT0_TxByte(U08 dat);
extern void UT0_TxAscii(U08 dat);

extern void UT1_TxByte(U08 dat);

extern void data_tx_RF(void);
extern void UT0_PrcTx(U08 sel);


#endif // ___UART_MAIN_H___

// EOF

