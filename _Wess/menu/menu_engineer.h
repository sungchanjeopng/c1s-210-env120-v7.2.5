//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------

#ifndef ___MENU_EGNDEN_H___
#define ___MENU_EGNDEN_H___

//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions
//------------------------------------------------------------------------------------------------------------------------------
enum{
	MnEGN_L00_IDLE = 0,
	// Page 1
	MnEGN_L01_PROF_NUM,
	MnEGN_L02_SMTH_NUM,     // Smooth Num.
	MnEGN_L03_SMTH_RNG,     // Smooth Range (Smooth Str.)
	MnEGN_L04_TRHD_REF,     // Threshold Ref. - Thr_Ref_Interface_Range
	MnEGN_L05_TRHD_MIN,	
	MnEGN_L06_TVG_IDX,   	
	MnEGN_L07_TVG_CST,	    // Set Echo amplifier Gain Table
	// Page 2
	MnEGN_LST_MAX,
};

enum {
	MnEGN_PROF_NUM_MIN = 10,
	MnEGN_PROF_NUM_STP = 10,
	MnEGN_PROF_NUM_DFT = 30,
	MnEGN_PROF_NUM_MAX = 100,
};

enum {
	MnEGN_SMTH_CNT_MIN = 0,
	MnEGN_SMTH_CNT_STP = 1,
	MnEGN_SMTH_CNT_DFT = 2,
	MnEGN_SMTH_CNT_MAX = 10,
};

enum {
	MnEGN_SMTH_RNG_MIN = 1,
	MnEGN_SMTH_RNG_STP = 1,
	MnEGN_SMTH_RNG_DFT = 5,
	MnEGN_SMTH_RNG_MAX = 100,
};

enum {
	MnEGN_TRHD_RNG_MIN = 10,
	MnEGN_TRHD_RNG_STP = 10,
	MnEGN_TRHD_RNG_DFT = 100,
	MnEGN_TRHD_RNG_MAX = 300,
};

enum {
	MnEGN_TRHD_MIN_MIN = 0,
	MnEGN_TRHD_MIN_STP = 1,
	MnEGN_TRHD_MIN_DFT = 128,
	MnEGN_TRHD_VOL_MAX = 240,
	MnEGN_TRHD_MIN_MAX = 255,
};

enum {
	MnEGN_TVG_I00_IDLE = 0,
	MnEGN_TVG_I01,
	MnEGN_TVG_I02,
	MnEGN_TVG_I03,
	MnEGN_TVG_I04,
	MnEGN_TVG_I05,
	//MnMSR_TVG_I06_CST,	// Custom
	MnEGN_TVG_IDX_MAX,
};


//------------------------------------------------------------------------------------------------------------------------------
//  Type definitions
//------------------------------------------------------------------------------------------------------------------------------
typedef struct {
	// Page 1
	U08 prof_num;
	U08 smth_cnt;
	U08 smth_rng;
	U08 trhd_rng;
	U08 trhd_min;
	U08 tvg_num;
	U08 fTvg_cst;
	U16 aTvg[60];

	// Not Configuration
	U08 fTvg_load;
	U08 fDz_mode;
	U08 fTh_mode;		// Flag: Treshold Mode
	U08 fTw_mode;		// Flag: TW Mode
	U16 trhd_gab;
	U08 trhd_low;
	U08 th_st_rg;		// Threshold Stop Range
	U08 th_tw_rg;		// Threshold Ref. TW Range

} LS_MnEGN;


//------------------------------------------------------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs
//------------------------------------------------------------------------------------------------------------------------------
// Get Local Variables
extern U08 MnEGN_GetAdcNumb(void);
extern U08 MnEGN_GetSmthCnt(void);
extern U08 MnEGN_GetSmthRng(void);
extern U08 MnEGN_GetTrhdRng(void);
extern U08 MnEGN_GetTrhdMin(void);
extern U08 MnEGN_GetFTvgCst(void);
extern U16 MnEGN_GetCstTvg(U08 idx);
extern U08 MnEGN_GetTvgNumb(void);	
// Not Configuration
extern U08 MnEGN_GetFgDzMod(void);
extern U08 MnEGN_GetFgThMod(void);
extern U08 MnEGN_GetFgTwMod(void);
extern U16 MnEGN_GetTrhdGab(void);
extern U08 MnEGN_GetTrhdLow(void);
extern U08 MnEGN_GetThStRng(void);
extern U08 MnEGN_GetThTwRng(void);


// Set Local Variables
// Memory Access
extern void MnEGN_MemLoad(void);
extern void MnEGN_MemSave(void);
extern void MnEGN_MemFtry(void);
// Public
extern void MnEGN_InitVari(void);
extern void MnEGN_ProcMain(void);


#endif // ___MENU_EGNDEN_H___
//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

