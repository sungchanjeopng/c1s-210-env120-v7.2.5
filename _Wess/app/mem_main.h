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

#ifndef ___MEM_MAIN_H___
#define ___MEM_MAIN_H___

//--------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Constant definitions
//--------------------------------------------------------------------
//***** SRAM Instruction Set *****
#define  SRAM_CMD_WMSR      0x01        // Write Mode Register Command
#define  SRAM_CMD_Seq       0x40        // Sequential mode

//***** Memory *****
#define  MEM_MRAM   1
#define  MEM_SRAM   2

//***** MRAM Instruction Set *****
#define  MRAM_CMD_WRSR      0x01        // Write Status Register Command
#define  MRAM_CMD_WRITE     0x02        // Write Command
#define  MRAM_CMD_READ      0x03        // Read Command
#define  MRAM_CMD_WRDI      0x04        // Reset Write Enable Latch Command
#define  MRAM_CMD_RDSR      0x05        // Read Status Register Command
#define  MRAM_CMD_WREN      0x06        // Set Write Enable Latch Command

//***** Memory address *****			// AD Data (1 ~ 220000)
#define  MEM_ADDR_RAW_1     1           // OLD: #define  MEM_start_addr_raw_1       1
#define  MEM_ADDR_RAW_2     33001       // OLD: #define  MEM_start_addr_raw_2       33001
#define  MEM_ADDR_RAW_3     66001       // OLD: #define  MEM_start_addr_raw_3       66001
#define  MEM_ADDR_RAW_4     99001       // OLD: #define  MEM_start_addr_raw_4       99001



//***** MRAM LOG Data *****     // Log Data (250001 ~ 352005)
#define MEM_ADDR_LOG_DATA_START 		250001
#define MEM_ADDR_LOG_DATA_END			350000

#define MEM_ADDR_LOG_ADDR_1_START		350001
#define MEM_ADDR_LOG_ADDR_1_END			350005

#define MEM_ADDR_LOG_ADDR_2_START		351001
#define MEM_ADDR_LOG_ADDR_2_END			351005

#define MEM_ADDR_LOG_ADDR_3_START		352001
#define MEM_ADDR_LOG_ADDR_3_END			352005

//***** MRAM ECHO Data *****    // Echo Data (360001 ~ 360401)
#define  M_ADDR_ECHO_START				360001
#define  M_ADDR_ECHO_END				360401  // No_of_sig = 200 (200 * 2 = 400)

//***** MRAM DAMP Data *****	// Damp Data (370001 ~ 377199)
#define  M_ADDR_DAMP_START				370001
#define  M_ADDR_DAMP_END				377199

//***** MRAM Auto Gain Best Wave *****	// Best waveform (380001 ~ 382200, ADC_DATA_MAX=2200)
#define  M_ADDR_AUTOGAIN_BEST_START		380001
#define  M_ADDR_AUTOGAIN_BEST_END		382200


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
extern U32 gMm_addr_raw;


//--------------------------------------------------------------------
//  Extern Prototypes of the APIs
//--------------------------------------------------------------------
extern void MEM_InitMain(U08 RAM_sel_UC_L);

extern void MEM_ReadSeq_StartAddr(U08 sel, U32 addr);
extern U08  MEM_ReadSeq_ByteData (U08 ram_idx);
extern U08  MEM_ReadData_Status(U08 ram_idx);

extern U08  MEM_ReadByte(U08 ram_idx, U32 addr);
extern U16  MEM_ReadWord(U08 sel, U32 addr);

extern void MEM_WriteData_SpiEn(U08 sel, U08 dat);
extern void MEM_WriteSeq_Byte(U08 sel, U08 dat);
extern void MEM_WriteSeq_StartAddr(U08 sel, U32 addr);
extern void MEM_WriteData_Status(U08 sel, U08 dat);
extern void MEM_WriteByte(U08 sel, U32 addr, U08 dat);
extern void MEM_WriteWord(U08 sel, U32 addr, U16 dat16);

extern void MEM_EraseMram(void);
extern void MEM_Reset_Status(void);
extern void MEM_RwSeq_End(U08 ram_idx);

#if 0
extern U08  MEM_WriteCmd(U08 ram, U08 cmd, U08 dat);
#endif

#endif // ___MEM_MAIN_H___

// EOF

