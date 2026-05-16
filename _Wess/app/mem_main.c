//--------------------------------------------------------------------
//
//  Copyright (C) 2020, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of
//  WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement
//  with WESS-Global Co.,LTD
//
//  $Id: templet_format.c 1 2020-07-13 12:00:00Z eddie $
//  $Date: 2020-07-13 12:00:00 +0900 
//  $Author: eddie $
//  $Rev: 1.0 $
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------
// chipset
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// bsp
#include "gpio_main.h"
#include "spi_main.h"
#include "dly_main.h"
// self
#include "mem_main.h"


//--------------------------------------------------------------------
//  Local Macros
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Local Type definitions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------
U32 gMm_addr_raw = MEM_ADDR_RAW_1;      // unsigned long MEM_start_addr_raw_UL_G;


//--------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Funtion Definitions
//--------------------------------------------------------------------
// OLD: void Init_SRAM(unsigned char RAM_sel_UC_L)
void MEM_InitMain(U08 sel)
{
    U08 save_sfrpage = SFRPAGE;

    SFRPAGE = CONFIG_PAGE;

    if     (sel == MEM_MRAM)    SBIT_MRAM_CS = 0;
    else if(sel == MEM_SRAM)    SBIT_SRAM_CS = 0;

    DLY_001usec();

    SPI0_WriteCmmd(sel, SRAM_CMD_WMSR);
    SPI0_WriteByte(SRAM_CMD_Seq);

    SFRPAGE = CONFIG_PAGE;

    DLY_001usec();
    if     (sel == MEM_MRAM)    SBIT_MRAM_CS = 1;
    else if(sel == MEM_SRAM)    SBIT_SRAM_CS = 1;

    SFRPAGE = save_sfrpage;
}

// OLD: void RAM_Read_Seq_Start_addr (unsigned char RAM_sel_UC_L, unsigned long start_addr_UL_L)
void MEM_ReadSeq_StartAddr(U08 ram_idx, U32 addr)
{
    U08 save = SFRPAGE;

    SPI0_WriteCmmd(ram_idx, MRAM_CMD_READ);
    SPI0_WriteAddr(addr);

    SFRPAGE = save;
}

// OLD: unsigned char RAM_Read_Seq_Byte (unsigned char RAM_sel_UC_L)
U08 MEM_ReadSeq_ByteData (U08 ram_idx)
{
    U08 rtn;
    U08 save = SFRPAGE;

    ram_idx = 0;

    rtn = SPI0_ReadByte();

    SFRPAGE = save;

    return rtn;
}

U08 MEM_ReadData_Status(U08 ram_idx)
{
    U08 rtn;
    U08 save = SFRPAGE;

    SPI0_WriteCmmd(ram_idx, MRAM_CMD_RDSR);

    rtn = SPI0_ReadByte();

    SFRPAGE = CONFIG_PAGE;
    DLY_001usec();

    if      (ram_idx == MEM_MRAM)   SBIT_MRAM_CS = 1;
    else if (ram_idx == MEM_SRAM)   SBIT_SRAM_CS = 1;

    SFRPAGE = save;

    return rtn;
}


// OLD: unsigned char RAM_Read_Byte (unsigned char RAM_sel_UC_L, unsigned long addr_UL_L)
U08 MEM_ReadByte(U08 ram_idx, U32 addr)
{
    U08 rtn;
    U08 save = SFRPAGE;

    SPI0_WriteCmmd(ram_idx, MRAM_CMD_READ);
    SPI0_WriteAddr(addr);

    rtn = SPI0_ReadByte();

    SFRPAGE = CONFIG_PAGE;
    DLY_001usec();

    if      (ram_idx == MEM_MRAM)   SBIT_MRAM_CS = 1;
    else if (ram_idx == MEM_SRAM)   SBIT_SRAM_CS = 1;

    SFRPAGE = save;

    return rtn;
}


// OLD: unsigned int RAM_Read_Word (unsigned char RAM_sel_UC_L, unsigned long addr_UL_L)
U16 MEM_ReadWord(U08 sel, U32 addr)
{
    U16 rtn;
    U08 dat_1, dat_2;
    U08 save = SFRPAGE;

    SPI0_WriteCmmd(sel, MRAM_CMD_READ);
    SPI0_WriteAddr(addr);

    dat_1 = SPI0_ReadByte();
    dat_2 = SPI0_ReadByte();

    SFRPAGE = CONFIG_PAGE;
    DLY_001usec();

    if     (sel == MEM_MRAM)    SBIT_MRAM_CS = 1;
    else if(sel == MEM_SRAM)    SBIT_SRAM_CS = 1;

    rtn = dat_1;
    rtn = (rtn << 8) + dat_2;

    SFRPAGE = save;

    return rtn;
}

// OLD: void RAM_Write_Seq_Byte (unsigned char RAM_sel_UC_L, unsigned char dat_UC_L)
void MEM_WriteSeq_Byte(U08 sel, U08 dat)
{
    U08 save = SFRPAGE;

    sel = 0;

    SPI0_WriteByte(dat);

    SFRPAGE = save;
}

// OLD: void RAM_Write_Seq_Start_addr (unsigned char RAM_sel_UC_L, unsigned long start_addr_UL_L)
void MEM_WriteSeq_StartAddr(U08 sel, U32 addr)
{
    U08 save = SFRPAGE;

    if(sel == MEM_MRAM)
        MEM_WriteData_SpiEn(sel, MRAM_CMD_WREN);
    DLY_001usec();

    SPI0_WriteCmmd(sel, MRAM_CMD_WRITE);
    SPI0_WriteAddr(addr);

    SFRPAGE = save;
}

// OLD: void SPI0_Write_WREN (unsigned char RAM_sel_UC_L, unsigned char dat_UC_L)
void MEM_WriteData_SpiEn(U08 sel, U08 dat)
{
	U08 save = SFRPAGE;

	SFRPAGE = CONFIG_PAGE;
	if     (sel == MEM_MRAM)    SBIT_MRAM_CS = 0;
	else if(sel == MEM_SRAM)    SBIT_SRAM_CS = 0;
	DLY_001usec();

	SPI0_WriteByte(dat);		// Send the WREN command

	SFRPAGE = CONFIG_PAGE;
	DLY_001usec();

	if     (sel == MEM_MRAM)    SBIT_MRAM_CS = 1;
	else if(sel == MEM_SRAM)    SBIT_SRAM_CS = 1;

	SFRPAGE = save;
}

// OLD: void SPI0_Write_WRSR (unsigned char dat_UC_L)
void MEM_WriteData_Status(U08 sel, U08 dat)
{
	U08 save = SFRPAGE;

	SPI0_WriteCmmd(sel, MRAM_CMD_WRSR);
	SPI0_WriteByte(dat);
	
	SFRPAGE = CONFIG_PAGE;
	DLY_001usec();

	if     (sel == MEM_MRAM)    SBIT_MRAM_CS = 1;
	else if(sel == MEM_SRAM)    SBIT_SRAM_CS = 1;

	SFRPAGE = save;
}

// OLD: void RAM_Write_Byte (unsigned char RAM_sel_UC_L, unsigned long addr_UL_L, unsigned char dat_UC_L)
void MEM_WriteByte(U08 sel, U32 addr, U08 dat)
{
    U08 save = SFRPAGE;

    if(sel == MEM_MRAM)
        MEM_WriteData_SpiEn(sel, MRAM_CMD_WREN);

    DLY_001usec();

    SPI0_WriteCmmd(sel, MRAM_CMD_WRITE);
    SPI0_WriteAddr(addr);
    SPI0_WriteByte(dat);

    SFRPAGE = CONFIG_PAGE;
    DLY_001usec();

    if     (sel == MEM_MRAM)    SBIT_MRAM_CS = 1;
    else if(sel == MEM_SRAM)    SBIT_SRAM_CS = 1;

    SFRPAGE = save;
}

// OLD: void RAM_Write_Word(unsigned char RAM_sel_UC_L, unsigned long addr_UL_L, unsigned int dat_UI_L)
void MEM_WriteWord(U08 sel, U32 addr, U16 dat16)
{
	U08 dat_1, dat_2;
	U08 save_sfrpage = SFRPAGE;

	dat_1 = (dat16 >> 8) & 0xff;
	dat_2 = dat16 & 0xff;

	if(sel == MEM_MRAM)
		MEM_WriteData_SpiEn(sel, MRAM_CMD_WREN);
	DLY_001usec();

	SPI0_WriteCmmd(sel, MRAM_CMD_WRITE);
	SPI0_WriteAddr(addr);

	SPI0_WriteByte(dat_1);
	SPI0_WriteByte(dat_2);

	SFRPAGE = CONFIG_PAGE;
	DLY_001usec();

	if     (sel == MEM_MRAM)    SBIT_MRAM_CS = 1;
	else if(sel == MEM_SRAM)    SBIT_SRAM_CS = 1;

	SFRPAGE = save_sfrpage;
}

// OLD: void RAM_RW_Seq_end (unsigned char RAM_sel_UC_L)
void MEM_RwSeq_End(U08 ram_idx)
{
    U08 save = SFRPAGE;

    SFRPAGE = CONFIG_PAGE;
    DLY_001usec();

    if     (ram_idx == MEM_MRAM)    SBIT_MRAM_CS = 1;
    else if(ram_idx == MEM_SRAM)    SBIT_SRAM_CS = 1;

    SFRPAGE = save;
}

// OLD: void Erase_MRAM(void)	// 0 ~ 141900
void MEM_EraseMram(void)	
{
    U32 i;

    MEM_WriteSeq_StartAddr(MEM_MRAM, 0);

    for(i = 0; i < 141901; i++)
        MEM_WriteSeq_Byte (MEM_MRAM, 0);

    MEM_RwSeq_End(MEM_MRAM);
}

// Stsatus Register Reset (Memory Protection and Block Write Protection)
void MEM_Reset_Status(void)
{
	switch(MEM_ReadData_Status(MEM_MRAM))
	{
		case 0x00:
		case 0x02:	break;
		default:	MEM_WriteData_Status(MEM_MRAM,0x00);	break;
	}
}


#if 0
U08 MEM_WriteCmd(U08 ram, U08 cmd, U08 dat)
{
	U08 rtn = RTN_OK;
	U08 save = SFRPAGE;

	SFRPAGE = CONFIG_PAGE;

	switch(ram)
	{
		case MEM_MRAM:		SBIT_MRAM_CS = BIT_LO;		break;
		case MEM_SRAM: 		SBIT_SRAM_CS = BIT_LO;		break;
		default:			SFRPAGE = save;
							return RTN_ERR;				break;
	}

	DLY_001usec();
	SPI0_WriteByte(cmd);
	SPI0_WriteByte(dat);
	DLY_001usec();

	switch(ram)
	{
		case MEM_MRAM:		SBIT_MRAM_CS = BIT_HI;		break;
		case MEM_SRAM: 		SBIT_SRAM_CS = BIT_HI;		break;
		default:			SFRPAGE = save;
							return RTN_ERR;				break;
	}

	SFRPAGE = save;

	return rtn;
}
#endif

// EOF

