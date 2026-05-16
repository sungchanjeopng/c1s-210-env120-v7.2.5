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

#ifndef ___LCD_MAIN_H___
#define ___LCD_MAIN_H___

//--------------------------------------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------------------------------------
// chipset
#include "compiler_defs.h"
#include "C8051F120_defs.h"


//--------------------------------------------------------------------------------------------------
//  Constant definitions
//--------------------------------------------------------------------------------------------------
// LCD screen and bitmap image array consants //
enum {
	LCD_X_START = 0,	LCD_X_END = 239,
	LCD_Y_START = 0,	LCD_Y_END = 127,
};


//--------------------------------------------------------------------------------------------------
//  Type definitions
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Macros
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Extern global variables
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Extern global APIs
//--------------------------------------------------------------------------------------------------
extern void LCD_CfgData(U08 dat);
extern void LCD_CfgCtrl(U08 dat);
extern U08  LCD_GetData(void);
extern void LCD_ChkBusy(void);

extern void LCD_TxCmd0(char cmd);
extern void LCD_TxCmd1(char cmd, U08 parm);
extern void LCD_TxCmd2(char cmd, U16 parm);

extern void LCD_InitErase(void);
extern void LCD_InitCfg(void);


#endif // ___LCD_MAIN_H___

// EOF

