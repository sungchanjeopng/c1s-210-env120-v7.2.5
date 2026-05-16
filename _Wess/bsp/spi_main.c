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
#include "dly_main.h"
#include "mem_main.h"
#include "gpio_main.h"
// app
#include "app_main.h"
// self
#include "spi_main.h"


//--------------------------------------------------------------------
//  Local Macros
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Local Type definitions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Funtion Definitions
//--------------------------------------------------------------------
// OLD: void Init_SPI0(void)
void SPI_InitSPI0(void)
{
	U08 SFRPAGE_save = SFRPAGE;     // Save the current SFRPAGE

	SFRPAGE = SPI0_PAGE;            // Switch to the necessary SFRPAGE
	SPI0CFG = 0x40;                 // Enable the SPI as a Master
									// CKPHA = '0', CKPOL = '0'
	SPI0CN  = 0x0D;                 // 4-wire Single Master, SPI enabled

	SPI0CKR = 0x01;	                // fspi = SYSCLK/(2*(SPI0CKR+1)) = 15MHz;
	SFRPAGE = SFRPAGE_save;         // Restore the SFRPAGE
}

// OLD: void SPI_set(void)
void SPI_SetFunc(void)
{
    U08 save_sfrpage = SFRPAGE;

    SPI_InitSPI0();

    SFRPAGE = CONFIG_PAGE;

    XBR0 = 0x06;
    XBR2 = 0x44;

    SFRPAGE = save_sfrpage;
}

// OLD: void SPI_reset(void)
void SPI_ResetFunc(void)
{
    U08 save_sfrpage = SFRPAGE;

    SFRPAGE = SPI0_PAGE;                // Switch to the necessary SFRPAGE

    SPIEN = 0;

    SFRPAGE = CONFIG_PAGE;

    XBR0 = 0x04;
    XBR2 = 0x40;

    SFRPAGE = save_sfrpage;
}

// OLD: void SPI0_WriteByte(unsigned char dat_UC_L)
void SPI0_WriteByte(U08 byte)
{
    U08 save = SFRPAGE;

    SFRPAGE = SPI0_PAGE;
    SPI0DAT = byte;             // Send the command
    while (!SPIF);              // Wait for end of transfer
    SPIF  = 0;                  // Clear the SPI intr. flag

    SFRPAGE  = save;
}

// OLD: unsigned char SPI0_read(void)
U08 SPI0_ReadByte(void)
{
    U08 val, save = SFRPAGE;

    SFRPAGE = SPI0_PAGE;
    SPI0DAT = 0;                // Dummy write to output serial clock
    while (!SPIF);              // Wait for the value to be read
    SPIF = 0;                   // Clear the SPI intr. flag

    val = SPI0DAT;              // Store MRAM value in local var.

    SFRPAGE  = save;

    return val;
}

// OLD: void SPI0_Write_CMD(unsigned char RAM_sel_UC_L, unsigned char dat_UC_L)
void SPI0_WriteCmmd(U08 sel, U08 dat)
{
    U08 save = SFRPAGE;

    SFRPAGE = CONFIG_PAGE;

    if      (sel == MEM_MRAM)      SBIT_MRAM_CS = 0;
    else if (sel == MEM_SRAM)      SBIT_SRAM_CS = 0;

    DLY_001usec();
    SPI0_WriteByte(dat);

    SFRPAGE = save;
}

// OLD: void SPI0_Write_ADDR(unsigned long addr_UL_L)
void SPI0_WriteAddr(U32 addr_in)
{
    U08 save = SFRPAGE;
    U08 addr_0, addr_1, addr_2;

    addr_0 = (U08)((addr_in >> 16) & 0xFF);
    addr_1 = (U08)((addr_in >>  8) & 0xFF);
    addr_2 = (U08) (addr_in        & 0xFF);

    SPI0_WriteByte(addr_0);       // Send the MRAM destination address (MSB first)
    SPI0_WriteByte(addr_1);       // Send the MRAM destination address (MSB first)
    SPI0_WriteByte(addr_2);       // Send the MRAM destination address (MSB first)

    SFRPAGE = save;
}


// EOF

