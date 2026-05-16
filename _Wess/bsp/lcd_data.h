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

#ifndef ___LCD_DATA_H___
#define ___LCD_DATA_H___

//--------------------------------------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Constant definitions
//--------------------------------------------------------------------------------------------------
#define LDT_BUFF_SIZE_X		30
#define LDT_BUFF_SIZE_Y		128

enum {
	LDT_LINE_OPT_IDLE = 0,
	LDT_LINE_CLEAR,
	LDT_LINE_DATA,
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
extern const U08 code gLd_mask[8];

extern U08 xdata gLd_buff[LDT_BUFF_SIZE_Y][LDT_BUFF_SIZE_X];


//--------------------------------------------------------------------------------------------------
//  Extern global APIs
//--------------------------------------------------------------------------------------------------
extern void LCD_DataDot_LineVert(U08 column, U08 top, U08 bottom);
extern void LCD_DataDot_Buff(U08 column, U08 row);
extern void LCD_WriteGlyph(U08 left, U08 top, U08 width, U08 height, U08* glyph, U08 store_w, U08 f_invert, U08 space_w);
extern void LCD_DataLine_Horz(U08 row, U08 left, U08 right);
extern void LCD_WrStrBuff(U08 left, U08 top, U08 font, char *str, U08 f_invert, U08 space_w);

extern void LDT_DrawBox(U08 left, U08 top, U08 right, U08 bottom);
extern void LDT_UpdBuff(U08 top, U08 bottom);
extern void LDT_LineVert(U08 pos_x, U08 top, U08 bottom, U08 opt);
extern void LDT_ClearDot(U08 column_UC_L, U08 row_UC_L);
extern void LDT_ClrBuff(U08 left, U08 top, U08 right, U08 bottom);

extern void LDT_InitScrn(void);


#endif // ___LCD_DATA_H___

// EOF

