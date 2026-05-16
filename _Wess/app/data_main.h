//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------

#ifndef ___DATA_MAIN_H___
#define ___DATA_MAIN_H___

//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------
// chipset
#include "compiler_defs.h"
#include "C8051F120_defs.h"
#include "sys_def.h"


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Type definitions
//------------------------------------------------------------------------------------------------------------------------------
typedef struct {
	U08 flg_ovf1;       // OLD: unsigned char Log_Flag_OVF_1_UC_G;
	U08 flg_ovf2;		// OLD: unsigned char Log_Flag_OVF_2_UC_G;

} LS_DAT;


//------------------------------------------------------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global variables
//------------------------------------------------------------------------------------------------------------------------------
//extern LS_DAT lDat;

extern BIT gTd_f_en;

extern U08 gLg_page;
extern U32 gLg_addr_main;
extern U32 gLg_addr_start;
extern U32 gLg_addr_end;
extern U32 gLg_addr_tx_start;
extern U32 gLg_ad_n_start;
extern U32 gLg_ad_n_end;
extern U32 gLg_scale_y;
extern U08 gLg_min;
extern U08 gLg_hour;

extern U32 gLg_addr_dat1;
extern U32 gLg_addr_dat2;
extern U32 gLg_addr_dat3;

//extern U08 gLg_flg_ovf2;
extern U08 gLg_flg_ovf3;


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs
//------------------------------------------------------------------------------------------------------------------------------
// Get Local Variables
extern U08 DAT_GetMskOvf1(void);
// Set Local Variables
// Operation
extern void LOG_ResetAddr(void);
extern void LOG_InitAddr(void);
extern void LOG_SetDpAddr(void);
extern void LOG_DpTime(void);
extern void LOG_OutData(void);
extern void LOG_ProcData(void);
extern void LOG_SaveAddr1(void);
extern void LOG_SaveAddr2(void);

extern void DAT_ProcSave(void);

extern void ScTRD_InitVai(void);
extern void ScTRD_ProcMain(void);
extern void ScTRD_ProcEnter(void);


#endif // ___DATA_MAIN_H___
//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

