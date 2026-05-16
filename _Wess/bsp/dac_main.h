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

#ifndef ___DAC_MAIN_H___
#define ___DAC_MAIN_H___

//--------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Constant definitions
//--------------------------------------------------------------------
enum {
	DAC_CURR_IDX_NORMAL = 0,
	DAC_CURR_IDX_04mA,
	DAC_CURR_IDX_12mA,
	DAC_CURR_IDX_20mA,

	DAC_CURR_IDX_MAX
};


//--------------------------------------------------------------------
//  Type definitions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Macros
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Prototypes of the functions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Extern global variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Extern Prototypes of the APIs
//--------------------------------------------------------------------
extern void DAC_InitBoot(void);
extern void DA0_InitOut(U16 dac0_value);

extern void DAC_WriteVal(U16 val);
extern void DAC_OutSpi(U16 dat);
extern void DAC_OutCurr(U16 val, U08 sel);



#endif // ___DAC_MAIN_H___

// EOF

