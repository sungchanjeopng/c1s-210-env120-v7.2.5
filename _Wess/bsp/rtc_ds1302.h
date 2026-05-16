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

#ifndef ___RTC_DS1302_H___
#define ___RTC_DS1302_H___

//--------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Constant definitions
//--------------------------------------------------------------------
enum {
    RTC_SET_IDX_IDLE = 0,
    RTC_SET_IDX_YY,
    RTC_SET_IDX_MM,
    RTC_SET_IDX_DAY,
    RTC_SET_IDX_H24,
    RTC_SET_IDX_MIN,

    RTC_SET_IDX_MAX,
};


//--------------------------------------------------------------------
//  Type definitions
//--------------------------------------------------------------------
typedef struct {
    U08 yy;     // OLD: unsigned char Time_Year_UC_G;
    U08 mm;     // OLD: unsigned char Time_Month_UC_G;
    U08 dd;     // OLD: unsigned char Time_Day_UC_G;
    U08 date;   // OLD: unsigned char Time_Date_UC_G;
    U08 hour;   // OLD: unsigned char Time_Hour_UC_G;
    U08 min;    // OLD: unsigned char Time_Min_UC_G;
    U08 sec;    // OLD: unsigned char Time_Sec_UC_G;

} GV_RTC;


//--------------------------------------------------------------------
//  Macros
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Prototypes of the functions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Extern global variables
//--------------------------------------------------------------------
extern GV_RTC gRt;


//--------------------------------------------------------------------
//  Extern Prototypes of the APIs
//--------------------------------------------------------------------
//extern void RTC_WriteByte(U08 cmd);
//extern void RTC_WriteData(U08 cmd, U08 value);

extern void RTC_GetTime(void);
extern U08  RTC_AdjTime(U08 para, U08 column, U08 row, U08 sel);
extern void RTC_SetTime(U08 min, U08 hour, U08 date, U08 mm, U08 dd, U08 yy);



#endif // ___RTC_DS1302_H___

// EOF

