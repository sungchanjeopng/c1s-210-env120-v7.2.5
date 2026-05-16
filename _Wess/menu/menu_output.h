//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------

#ifndef ___MENU_OUTPUT_H___
#define ___MENU_OUTPUT_H___

//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions - List
//------------------------------------------------------------------------------------------------------------------------------
enum{
	MnOUT_L00_IDLE = 0,
	// Page 1
	MnOUT_L01_SET_04mA,
	MnOUT_L02_SET_20mA,
	MnOUT_L03_TRIM_12mA,
	MnOUT_L04_TRIM_20mA,
	MnOUT_L05_OUTP_04mA,
	MnOUT_L06_ECHO_DLY,
	MnOUT_L07_ECHO_CURR,    // Current: mA
	// Page 2
	MnOUT_L08_R1_ACT,
	MnOUT_L09_R1_STOP,
	MnOUT_L10_R2_ACT,
	MnOUT_L11_R2_STOP,
	MnOUT_L12_RLY_TEST,
	MnOUT_L13_PROTOCOL,

	MnOUT_LST_MAX,
};

#define MnOUT_X0_TITLE_KO		(80)
#define MnOUT_X0_PAGE_KO		(MnOUT_X0_TITLE_KO + 40)


//------------------------------------------------------------------------------------------------------------------------------
//  Constant definitions - List (Page 1)
//------------------------------------------------------------------------------------------------------------------------------
enum {
	MnOUT_DAC_CFG_IDLE = 0,
	MnOUT_DAC_CFG_04mA,
	MnOUT_DAC_CFG_20mA,

	MnOUT_DAC_CFG_MAX,
};

enum {
	MnOUT_ECHO_OUT_38mA = 0,
	MnOUT_ECHO_OUT_HOLD,
	MnOUT_ECHO_OUT_21mA,

	MnOUT_ECHO_OUT_MAX,
};

enum {
	MnOUT_PROT_STD = 0,		// Protocol - Standard
	MnOUT_PROT_RF,
	MnOUT_PROT_MDBS,		// ModBus
	MnOUT_PROT_MAX,
};

enum {
	MnOUT_MDB_BAUD_04800 = 0,
	MnOUT_MDB_BAUD_09600,
	MnOUT_MDB_BAUD_19200,
	MnOUT_MDB_BAUD_MAX,
};

enum {
	MnOUT_SET_CURR_MT_MIN = 0,
	MnOUT_SET_04mA_MT_DFT = 0,
	MnOUT_SET_20mA_MT_DFT = 500,
	MnOUT_SET_CURR_MT_10_MAX = 1000,
	MnOUT_SET_CURR_MT_20_MAX = 2000,
};

enum {
	MnOUT_SET_CURR_FT_MIN = 0,
	MnOUT_SET_CURR_FT_DFT = 0,
	MnOUT_SET_CURR_FT_10_MAX = 328,
	MnOUT_SET_CURR_FT_20_MAX = 656,
};

enum {
	MnOUT_TRIM_CURR_MIN = 0,
	MnOUT_TRIM_CURR_DFT = 50,
	MnOUT_TRIM_CURR_MAX = 99,
};

enum {
	MnOUT_ECHO_DLY_STP = 10,
	MnOUT_ECHO_DLY_MIN = 20,
	MnOUT_ECHO_DLY_DFT = 120,
	MnOUT_ECHO_DLY_MAX = 990,
};

enum {
	MnOUT_RLY_OPER_MT_MIN = 0,
	MnOUT_RLY1_ACT_DFT = 900,
	MnOUT_RLY1_STP_DFT = 800,
	MnOUT_RLY2_ACT_DFT = 0,
	MnOUT_RLY2_STP_DFT = 10,
	MnOUT_RLY_OPER_MT_10_MAX = 1000,
	MnOUT_RLY_OPER_MT_20_MAX = 2000,
};

enum {
	MnOUT_RLY_OPER_FT_MIN = 0,
	MnOUT_RLY_OPER_FT_DFT = 0,
	MnOUT_RLY_OPER_FT_10_MAX = 328,
	MnOUT_RLY_OPER_FT_20_MAX = 656,
};

enum {
	MnOUT_RF_CH_MIN = 1,
	MnOUT_RF_CH_DFT = 1,
	MnOUT_RF_CH_MAX = 4,
};

enum {
	MnOUT_MDB_ADDR_MIN = 1,
	MnOUT_MDB_ADDR_DFT = 1,
	MnOUT_MDB_ADDR_MAX = 247,
};


//------------------------------------------------------------------------------------------------------------------------------
//  Type definitions
//------------------------------------------------------------------------------------------------------------------------------
typedef struct {
	// Page 1
	U16 set_04mA;
	U16 set_20mA;
	U08 trim_12mA;
	U08 trim_20mA;
	U16 echo_dly;
	U08 echo_out;
	// Page 2
	U16 rly1_act;
	U16 rly1_stp;
	U16 rly2_act;
	U16 rly2_stp;
	U08 protocol;
	U08 mdb_addr;
	U08 mdb_baud;
	U08 rf_ch;

} LS_MnOUT;


//------------------------------------------------------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global variables
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs - Get Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Page 1
extern U16 MnOUT_GetSet04mA(void);
extern U16 MnOUT_GetSet20mA(void);
extern U08 MnOUT_GetTrim12mA(void);
extern U08 MnOUT_GetTrim20mA(void);
extern U16 MnOUT_GetEchoDly(void);
extern U08 MnOUT_GetEchoOut(void);
// Page 2
extern U16 MnOUT_GetRly1Act(void);
extern U16 MnOUT_GetRly1Stp(void);
extern U16 MnOUT_GetRly2Act(void);
extern U16 MnOUT_GetRly2Stp(void);
extern U08 MnOUT_GetProtocol(void);
extern U08 MnOUT_GetMdbAddr(void);
extern U08 MnOUT_GetMdbBaud(void);
extern U08 MnOUT_GetRfCh(void);


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs - Get Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Page 1
extern void MnOUT_SetSet20mA(U16 val);


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs - Memory Access
//------------------------------------------------------------------------------------------------------------------------------
extern void MnOUT_MemLoad(void);
extern void MnOUT_MemSave(void);
extern void MnOUT_MemFtry(void);


//------------------------------------------------------------------------------------------------------------------------------
//  Extern global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
extern void MnOUT_InitVari(void);
extern void MnOUT_ProcMain(void);



#endif // ___MENU_OUTPUT_H___
//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

