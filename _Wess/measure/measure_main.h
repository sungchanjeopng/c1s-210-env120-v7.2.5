//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------

#ifndef ___MEASURE_MAIN_H___
#define ___MEASURE_MAIN_H___

//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions
//------------------------------------------------------------------------------------------------------------------------------
#define	MSR_NUM_ECHO_CHK_START      70      // Start of Echo CHK
#define MSR_ES_S01					1		// Echo Strength Step-1

#define MSR_CALC_M2F_CONST			(0.3280839895)
#define MSR_CALC_F2M_CONST			(3.04800000001219)

enum {
	MSR_TX_IDLE = 0,
	MSR_TX_RUN,
};

// Sensor Index
enum {
	MSR_SS_IDX_0 = 0,
	MSR_SS_IDX_1,
	MSR_SS_IDX_2,
	MSR_SS_IDX_3,

	MSR_SS_IDX_MAX,
};


//------------------------------------------------------------------------------------------------------------------------------
//  Type definitions
//------------------------------------------------------------------------------------------------------------------------------
typedef struct {
	// Flag
	U08 fAdc_full;
	// Counter
	U16 cIntv;
	U08 cAvrg;
	// Value
	U08 intv;
	F32 veloc;			// Velocity (Speed of Sound)
	U08 freq;
	S16 sg_thick;		// Sludge Thickness (Height)
	U08 tr_step;
	U08 tvg_idx;
	U08 dly10cm_h;		// OLD: unsigned char delay_10Cm_H_UC_G;
	U08 dly10cm_l;		// OLD: unsigned char delay_10Cm_L_UC_G;
	// Auto Gain
	U08 fauto_gain;		// Auto gain flag (0: off, 1: searching)
	U08 auto_calib;		// Current candidate gain while searching
	U08 auto_gain;		// Selected best gain

} LS_MEAS;


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
extern U08 MEAS_GetFgAdc(void);
extern U08 MEAS_GetCtAvrg(void);
extern U16 MEAS_GetTvg(U08 num, U08 idx);
extern S16 MEAS_GetSgThick(void);
extern F32 MEAS_GetVeloc(void);
extern U08 MEAS_AutoGain_GetValue(void);
extern U08 MEAS_AutoGain_GetMaxSlope(void);
// Set Local Variables
// Operation
extern F32  MEAS_CalcFeet2Meter(U16 feet);
extern F32  MEAS_CalcMeter2Feet(U16 meter);
extern void MEAS_CfgThreshold(void);
extern void MEAS_CfgSens(U08 sel);
extern void MEAS_SetDacTvg(void);
extern void MEAS_ResetFlag(void);
extern void MEAS_SetOpr(void);
// Public
extern void MEAS_InitVari(void);
extern void MEAS_PrcEcho(void);
extern void MEAS_PrcSub1(void);
extern void MEAS_ProcMain(void);




#endif // ___MEASURE_MAIN_H___
//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

