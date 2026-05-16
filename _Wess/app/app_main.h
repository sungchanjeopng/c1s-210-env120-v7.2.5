//--------------------------------------------------------------------
//
//  Copyright (C) 2020, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of
//  WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement
//  with WESS-Global Co.,LTD
//
//  $Id: app_comm.h 1 2020-07-13 12:00:00Z eddie $
//  $Date: 2020-07-13 12:00:00 +0900 $
//  $Author: eddie $
//  $Rev: 1.0 $
//
//--------------------------------------------------------------------

#ifndef ___APP_MAIN_H___
#define ___APP_MAIN_H___

//--------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Constant definitions
//--------------------------------------------------------------------
#define APP_STR_LENGTH		16


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
extern U32 test_global_cnt;


//--------------------------------------------------------------------
//  Extern Prototypes of the APIs
//--------------------------------------------------------------------
extern U08* APP_Num2Str(S32 num, U08 *p_str, U16 digit, U08 decimal, U08 f_fill);
extern U08* APP_NumTest2Str(U32 num, U08 *p_str, U16 digit, U08 decimal, U08 f_fill);
extern void APP_InitBoot(void);

extern void APP_ProcEvery(void);
extern void APP_Proc1Sec(void);


#endif // ___APP_MAIN_H___

// EOF

