//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------

#ifndef ___DISP_MAIN_H___
#define ___DISP_MAIN_H___

//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions
//------------------------------------------------------------------------------------------------------------------------------
enum {
    DP_IDX_GRAPH_0 = 0,
    DP_IDX_GRAPH_1,
    DP_IDX_GRAPH_2,
    DP_IDX_GRAPH_3,
    DP_IDX_GRAPH_4,
};


//------------------------------------------------------------------------------------------------------------------------------
//  Type definitions
//------------------------------------------------------------------------------------------------------------------------------
typedef struct {
    //U08 idx_scr;		// OLD: unsigned char Sel_scr_UC_G;
	U08 f_err_tprt; 	// OLD: unsigned char Flag_ER_temp_UC_G;

} GV_DP;


//------------------------------------------------------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global variables
//------------------------------------------------------------------------------------------------------------------------------
extern GV_DP gDp;

extern U08 gDp_buff_grp[];
extern U08 gDp_buff_str[];

extern U08 gDp_flg_log;

extern U08 gDp_cursor_vert;
extern S16 gDp_cursor_dist;
extern U08 gDp_flg_cs_vert;


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs
//------------------------------------------------------------------------------------------------------------------------------
// Get Local Variables
// Set Local Variables
// Operation
extern void DSP_SetBuff(U16 width, U08 *p_val, U16 start, U32 end);
extern U16  DSP_GetCurrent(void);
extern void DSP_ProcDiag_Main(void);
extern void DSP_ConfigBox(U08 lang, U16 column, U08 row, U08 *p_str, U08 sel);
extern void DSP_DrawGraph(U16 x0, U16 y0, U16 width, U16 height, U08* pVal, U16 start, U32 end, U16 max, U08 type);
extern void DSP_DrawEchoQ(unsigned char *ptr_array_UC_L);

extern void DpFRT_ProcMain(void);
extern void DpGRP_ProcMain(U08 idx);
extern void DpINF_ProcMain(void);

extern void DISP_InitVari(void);
extern void DISP_ClearScr(void);
extern void DISP_IntroScr(void);



#endif // ___DISP_MAIN_H___
//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

