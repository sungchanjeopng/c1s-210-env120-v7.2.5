//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------

#ifndef ___MEASURE_ANALYZE_H___
#define ___MEASURE_ANALYZE_H___

//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions
//------------------------------------------------------------------------------------------------------------------------------
#define	MsANL_SIG_NUM					200		// OLD: #define	No_of_sig		200		// no of Sig_data_SI_G

enum {
	MsANL_ERR_NONE = 0,
	MsANL_ERR_ECHO,
	MsANL_ERR_TPRT,
};


//------------------------------------------------------------------------------------------------------------------------------
//  Type definitions
//------------------------------------------------------------------------------------------------------------------------------
typedef struct {
	// flag
	U08 fEco_chk;		// Echo Check
	U08 fEco_dly;		// Echo Delay
	U08 fEco_dtt;		// Echo Detected
	U08 fEco_trk;		// Echo Tracking
	U08 fMty_ovr;		// Empty Over
	U08 fTim_win;		// Time Window
	U08 fErr;
	// counter
	U08 cEco_wt;		// Echo Delay Wait(sec)
	U08 cEcho;
	U16 cErr_tw;
	U16 cErr_eco0;
	U16 cErr_eco1;
	// value
	S16 level_mt;
	S16 level_ft;
	S16 dista_mt;		// Distance (Meter)
	S16 dista_ft;		// Distance (Feet)
	S16 dist_1st;
	S16 dist_mod;
	U16 empty;
	U16 damp_num;
	U16 trhd_pct;		// Threshold Percent
	U08 err_stat;		// Error Status
	U16 tm_wd_hi;		// Time Window - High
	U16 tm_wd_lo;		// Time Window - Low

} LS_MsANL;


//------------------------------------------------------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs
//------------------------------------------------------------------------------------------------------------------------------
// Get Flag
extern U08 MsANL_GetFgTimWin(void);
extern U08 MsANL_GetFgEcoChk(void);
extern U08 MsANL_GetFgEcoDly(void);
extern U08 MsANL_GetFgEcoDtt(void);
extern U08 MsANL_GetFgEcoTrk(void);
extern U08 MsANL_GetFgErr(void);
// Get Counter
extern U08 MsANL_GetCtEcho(void);
// Get Value
extern S16 MsANL_GetLevelMt(void);
extern S16 MsANL_GetLevelFt(void);
extern S16 MsANL_GetDistaMt(void);
extern S16 MsANL_GetDistaFt(void);
extern S16 MsANL_GetDist1st(void);
extern S16 MsANL_GetDistMod(void);
extern U16 MsANL_GetEmpty(void);
extern U16 MsANL_GetTrhdPct(void);
extern U08 MsANL_GetErrStat(void);
// Set Local Variables
extern void MsANL_SetFgTimWin(U08 flg);
extern void MsANL_SetCtEcho(U08 cnt);
extern void MsANL_SetDistaMt(S16 val);
extern void MsANL_SetDistMod(S16 val);
// Operation
extern void MsANL_ResetFlag(void);
extern void MsANL_ResetTimer(void);
extern void MsANL_InitVari(void);
extern void MsANL_Proc1Sec(void);
extern void MsANL_CheckErr(void);
extern void MsANL_CalcLevel(void);
extern void MsANL_TrackingSignal(void);
extern void MsANL_ProcSignal(void);

extern void ANZ_ChkEcho_Level(void);
extern void ANZ_SetEmptyPct(U08 pct);
extern void ANZ_SetTw(void);
extern void ANZ_CalcTrshd(void);
extern void ANZ_CalcDamp(void);



#endif // ___MEASURE_ANALYZE_H___
//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

