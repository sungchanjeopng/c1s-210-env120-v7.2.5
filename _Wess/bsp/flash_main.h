//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------

#ifndef ___FLASH_MAIN_H___
#define ___FLASH_MAIN_H___

//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions
//------------------------------------------------------------------------------------------------------------------------------
// Flash Address
enum {
	// Menu - System
	F000_MSR_UNIT = 0,
	F001_MSR_OPER,
	F002_MSR_EMPTY_L,				F003_MSR_EMPTY_H,
	F004_MSR_DEAD_L,				FLS_ADDR_05_DEAD_H,
	F006_OUT_SET_04mA_L,			FLS_ADDR_07_04mA_H,
	F008_OUT_SET_20mA_L,			FLS_ADDR_09_20mA_H,
	F010_DAT_F_SAVE,
	F011_OUT_PROTOCOL,
	F012_OUT_RF_CH,
	F013_SYS_LANG,          	    // Language
	// Menu - Relay
	F014_OUT_RLY1_ACT_L = 14,		FLS_ADDR_15_RLY1_ACT_H,
	F016_OUT_RLY1_STP_L,			FLS_ADDR_17_RLY1_STP_H,
	F018_OUT_RLY2_ACT_L,			FLS_ADDR_19_RLY2_ACT_H,
	F020_OUT_RLY2_STP_L,			FLS_ADDR_21_RLY2_STP_H,
	F022_OUT_ECHO_DLY_L,			FLS_ADDR_23_EC_DLY_H,
	F024_OUT_ECHO_OUTP,
	// Menu - Logging
	F025_DAT_F_SAVE = 25,
	F026_DAT_SAV_INTV,
	F027_DAT_DISP_01M,
	F028_DAT_DISP_10M,
	F029_DAT_DISP_60M,

	// Menu - Mesure (Complement)
	F034_MSR_PCD_INTV_L = 34,		FLS_ADDR_35_PCD_INTER_H,
	F036_MSR_PCD_TERM,
	F037_MSR_DAMP,
	F038_MSR_TRHD_PERC,
	F039_MSR_FREQ,

	// Menu - Service
	F040_SYS_PASS_L = 40,			FLS_ADDR_41_PASSCODE_H,

	F042_MSR_INTV,
	F043_EGN_TVG_NUM,
	F044_MSR_AMP,
	F045_OUT_TRIM_12mA,
	F046_OUT_TRIM_20mA,
	F047_MSR_ASF_L,					FLS_ADDR_48_ASF_VAL_H,
	F049_MSR_F_DEMO,
	F050_MSR_OFFS_L,				F050_MSR_OFFS_H,

	// Not Define
	F065_FTR_DAC_04mA_L = 65,		FLS_ADDR_66_DFT_04mA_H,
	F067_FTR_DAC_20mA_L,			FLS_ADDR_68_DFT_20mA_H,
	F069_FTR_DAC_38mA_L,			FLS_ADDR_70_DFT_38mA_H,
	F071_FTR_DAC_21mA_L,			FLS_ADDR_72_DFT_21mA_H,

	// Menu - Hidden
	F073_RESERVED = 73,
	F074_RESERVED,
	F075_MSR_TW_DELAY_L,			FLS_ADDR_76_TW_RST_DELAY_H,
	F077_EGN_TH_GAB_L,				F078_EGN_TH_GAB_H,
	F079_EGN_F_DZ_MODE,
	F080_EGN_SMTH_CNT,
	F081_EGN_SMTH_RNG,
	F082_EGN_TW_RST_MODE,
	F083_EGN_F_TH_MODE,
	F084_EGN_TH_ST_RNG,  		    // STOP
	F085_EGN_TRHD_RNG,
	F086_EGN_TH_TW_RNG,				// TW Range
	F087_EGN_PROF_NUM,
	F088_EGN_F_TW_MODE,
	F089_MSR_TW_WIDTH,
	F090_EGN_TRHD_LOW,
	F091_MSR_TRHD_FIX,
	F092_EGN_TRHD_MIN,
	FLS_ADDR_93_SHORT_GAIN_RANGE,
	FLS_ADDR_94_SHORT_GAIN_MULTI,
	FLS_ADDR_95_SHORT_GAIN_Y_ITC,

	// ETC
	F096_RESERVED,
	F097_OUT_RF_BAND,
	F098_OUT_MDB_BAUD,
	F099_OUT_MDB_ADDR,
	F100_MSR_TRHD_MODE,
	F101_FTR_DIST_MAX,

	//TEST
	F102_GLOVAL_CNT = 102,
	//TVG
	F130_EGN_TVG_CST = 130,			// Custom Enabled
	F131_EGN_TVG_CST_START,			// Custom TVG
	F250_EGN_TVG_CST_END   = 250,


	FLS_ADDR_MAX = 255,
};


//------------------------------------------------------------------------------------------------------------------------------
//  Type definitions
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs
//------------------------------------------------------------------------------------------------------------------------------
// Get Memory
extern U08 FLS_Rd1Byte(U08 addr);
extern U16 FLS_Rd2Byte(U08 addr);
// Set Memory
extern void FLS_Wr1Byte(U08 addr, U08 byte);
extern void FLS_Wr2Byte(U08 addr, U16 word);
// Operation
extern void FLS_InitBoot(void);
extern void FLS_InitMain(void);
extern void FLS_WriteAll(void);
extern void FLS_ResetFactory(void);


#endif // ___FLASH_MAIN_H___

// EOF

