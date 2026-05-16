//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------

#ifndef ___MENU_MEASURE_H___
#define ___MENU_MEASURE_H___

//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions - List
//------------------------------------------------------------------------------------------------------------------------------
enum{
	MnMSR_L00_IDLE = 0,
	// Page 1
	MnMSR_L01_UNIT,			// Display Unit
	MnMSR_L02_OPER,			// Operation
	MnMSR_L03_EMPTY,
	MnMSR_L04_DEADZONE,
	MnMSR_L05_ECHO_AMP,
	MnMSR_L06_FREQ,
	MnMSR_L07_TX_INTV,		// Measure Interval
	// Page 2
	MnMSR_L08_OFFSET,
	MnMSR_L09_DAMPING,
	MnMSR_L10_THRESHOLD,
	MnMSR_L11_ASF_BAND,		// Abnormal Signal Filter
	MnMSR_L12_TW_RST_TM,	// Window RST Time
	MnMSR_L13_TW_RANGE,		// Window Range
	MnMSR_L14_PCD_INTV,		// Clean Interval
	// Page 3
	MnMSR_L15_PCD_TERM,		// Clean Term
	MnMSR_L16_RST_MSR,		// Reset Measure
	MnMSR_L17_RST_FTR,		// Factory Default
	MnMSR_L18_TEST,

	MnMSR_LST_NUMB,
};

#define MnMSR_X0_TITLE_KO		(80)
#define MnMSR_X0_PAGE_KO		(MnMSR_X0_TITLE_KO + 40)


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions - List (Page 1)
//------------------------------------------------------------------------------------------------------------------------------
enum {
    MnMSR_UNIT_METER = 0,
    MnMSR_UNIT_FEET,
};

enum{
	MnMSR_OPER_LEVEL = 0,
	MnMSR_OPER_DISTA,
};

enum{
	MnMSR_EMPTY_MT_MIN = 0,
	MnMSR_EMPTY_MT_DFT = 500,
	MnMSR_EMPTY_MT_10_MAX = 1000,
	MnMSR_EMPTY_MT_20_MAX = 2000,
};

enum{
	MnMSR_EMPTY_FT_MIN = 0,
	MnMSR_EMPTY_FT_DFT = 164,
	MnMSR_EMPTY_FT_10_MAX = 330,
	MnMSR_EMPTY_FT_20_MAX = 660,
};

enum{
	MnMSR_DEAD_MT_MIN = 35,
	MnMSR_DEAD_MT_DFT = 50,
	MnMSR_DEAD_MT_10_MAX = 1000,
	MnMSR_DEAD_MT_20_MAX = 2000,
};

enum{
	MnMSR_DEAD_FT_MIN = 10,
	MnMSR_DEAD_FT_DFT = 15,
	MnMSR_DEAD_FT_10_MAX = 328,
	MnMSR_DEAD_FT_20_MAX = 656,
};

enum {
	MnMSR_TVG_I00_IDLE = 0,
	MnMSR_TVG_I01,
	MnMSR_TVG_I02,
	MnMSR_TVG_I03,
	MnMSR_TVG_I04,
	MnMSR_TVG_I05,
	//MnMSR_TVG_I06_CST,	// Custom
	MnMSR_TVG_IDX_MAX,
};

enum{
	MnMSR_AMP_MIN = 1,
	MnMSR_AMP_DFT = 15,
	MnMSR_AMP_MAX = 50,
};

enum {
	MnMSR_FREQ_130K = 0,
	MnMSR_FREQ_160K,		// Heavy
	MnMSR_FREQ_270K,
	MnMSR_FREQ_380K,		// Light
	// Not Used
	MnMSR_FREQ_500K,
	MnMSR_FREQ_620K,
	MnMSR_FREQ_750K,

	MnMSR_FREQ_MAX,
};


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions - List (Page 2)
//------------------------------------------------------------------------------------------------------------------------------
enum{
	MnMSR_INTV_MIN = 1,
	MnMSR_INTV_DFT = 3,
	MnMSR_INTV_MAX = 60,
};

enum{
	MnMSR_OFFS_MT_MIN = -100,
	MnMSR_OFFS_MT_DFT = 0,
	MnMSR_OFFS_MT_MAX = 100,
};

enum{
	MnMSR_OFFS_FT_MIN = -32,
	MnMSR_OFFS_FT_DFT = 0,
	MnMSR_OFFS_FT_MAX = 32,
};

enum {
	MnMSR_DAMP_I00_IDLE = 0,		// Default: Interval == 3 sec 
	MnMSR_DAMP_I01_T0001,			//  3 sec
	MnMSR_DAMP_I02_T0010,			// 30 sec
	MnMSR_DAMP_I03_T0020,			//  1 min
	MnMSR_DAMP_I04_T0040,			//  2 min
	MnMSR_DAMP_I05_T0100,			//  5 min
	MnMSR_DAMP_I06_T0200,			// 10 min
	MnMSR_DAMP_I07_T0600,			// 30 min
	MnMSR_DAMP_I08_T1200,			// 60 min

	MnMSR_DAMP_IDX_MAX,
};

enum {
	MnMSR_TRHD_MOD_AUTO = 0,
	MnMSR_TRHD_MOD_MANUAL,
};

enum {
	MnMSR_TRHD_PCT_STP = 5,
	MnMSR_TRHD_PCT_MIN = 10,
	MnMSR_TRHD_PCT_DFT = 50,
	MnMSR_TRHD_PCT_MAX = 90,
};

enum {
	MnMSR_TRHD_FIX_STP = 20,
	MnMSR_TRHD_FIX_MIN = 20,
	MnMSR_TRHD_FIX_DFT = 120,
	MnMSR_TRHD_FIX_MAX = 220,
};

enum {
	MnMSR_ASF_MIN = 0,
	MnMSR_ASF_DFT = 0,
	MnMSR_ASF_MAX = 9999,
};

enum {
	MnMSR_TW_DELAY_STP = 10,
	MnMSR_TW_DELAY_MIN = 10,
	MnMSR_TW_DELAY_DFT = 60,
	MnMSR_TW_DELAY_MAX = 600,
};

enum {
	MnMSR_TW_RANGE_STP = 10,
	MnMSR_TW_RANGE_MIN = 10,
	MnMSR_TW_RANGE_DFT = 20,
	MnMSR_TW_RANGE_MAX = 100,
};


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions - List (Page 3)
//------------------------------------------------------------------------------------------------------------------------------
enum{
	MnMSR_PCD_INTV_MIN = 0,
	MnMSR_PCD_INTV_DFT = 60,
	MnMSR_PCD_INTV_STP = 60,
	MnMSR_PCD_INTV_MAX = 6000,
};

enum{
	MnMSR_PCD_TERM_MIN = 0,
	MnMSR_PCD_TERM_DFT = 3,
	MnMSR_PCD_TERM_MAX = 100,
};

enum {
	MnMSR_F_DEMO_OFF = 0,
	MnMSR_F_DEMO_ONN,
};


//------------------------------------------------------------------------------------------------------------------------------
//  Type definitions
//------------------------------------------------------------------------------------------------------------------------------
typedef struct {
	// Page 1
	U08 unit;
	U08 oper;			// Operation : sluge level / distance
	U16 empty;
	U16 dead;			// Dead Zone
	U08 tvg;			// Time Variable Gain
	U08 amp;			// Echo AMP
	U08 freq;			// Frequency
	// Page 2
	U08 intv;			// TX Interval
	S16 offs;			// Offset
	U08 damp;			// Damping
	U08 trhd_mod;		// Threshold Mode
	U08 trhd_per;		// Threshold Percent
	U08 trhd_fix;		// Threshold Fixed
	U16 asf;			// Abnormal Signal Filter
	U32 cAsf;
	U16 tw_delay;		// Time-Window Delay Time
	U08 tw_range;		// Time-Window Width
	// Page 3
	U16 pcd_intv;		// Probe Clean Device - Interval
	U08 pcd_term;		// Probe Clean Device - Term
	U08 fTest;

} LS_MnMSR;


//------------------------------------------------------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs - Shared Quick Menu
//------------------------------------------------------------------------------------------------------------------------------
#if 1
extern void MnMsr_EchoAmp(void);
extern void MnMsr_Freq(void);
extern void MnMsr_Damping(void);
extern void MnMsr_Threshold(void);
extern void MnMsr_CfgAsf(void);
extern void MnMsr_RstMsr(void);
#else
extern void MnMsr_Empty(void);
extern void MnMsr_DeadZone(void);
extern void MnMsr_TvgIdx(void);
extern void MnMsr_EchoAmp(void);
extern void MnMsr_Freq(void);
#endif

//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs - Get Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Page 1
extern U08 MnMSR_GetUnit(void);
extern U08 MnMSR_GetOper(void);
extern U16 MnMSR_GetEmpty(void);
extern U16 MnMSR_GetDead(void);
#if 0
extern U08 MnMSR_GetTvg(void);
#endif
extern U08 MnMSR_GetAmp(void);
extern U08 MnMSR_GetFreq(void);
// Page 2
extern U08 MnMSR_GetIntv(void);
extern S16 MnMSR_GetOffs(void);
extern U08 MnMSR_GetDamp(void);
extern U08 MnMSR_GetTrhdMod(void);
extern U08 MnMSR_GetTrhdPer(void);
extern U08 MnMSR_GetTrhdFix(void);
extern U16 MnMSR_GetAsf(void);
extern U16 MnMSR_GetTwDly(void);
extern U08 MnMSR_GetTwRng(void);
// Page 3
extern U16 MnMSR_GetPcdIntv(void);
extern U08 MnMSR_GetPcdTerm(void);
extern U08 MnMSR_GetFgTest(void);


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs - Set Local Variables
//------------------------------------------------------------------------------------------------------------------------------
extern void MnMSR_SetTrhdPer(U08 val);
extern void MnMSR_SetTrhdFix(U08 val);


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs - Memory Access
//------------------------------------------------------------------------------------------------------------------------------
extern void MnMSR_MemLoad(void);
extern void MnMSR_MemSave(void);
extern void MnMSR_MemFtry(void);


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
extern void MnMSR_InitVari(void);
extern void MnMSR_ProcMain(void);



#endif // ___MENU_MEASURE_H___
//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

