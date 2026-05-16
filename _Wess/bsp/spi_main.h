//--------------------------------------------------------------------
//
//  Copyright (C) 2020, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of
//  WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement
//  with WESS-Global Co.,LTD
//
//  $Id: spi_main.h 1 2020-07-13 12:00:00Z eddie $
//  $Date: 2020-07-13 12:00:00 +0900 $
//  $Author: eddie $
//  $Rev: 1.0 $
//
//--------------------------------------------------------------------

#ifndef ___SPI_MAIN_H___
#define ___SPI_MAIN_H___

//--------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Constant definitions
//--------------------------------------------------------------------


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


//--------------------------------------------------------------------
//  Extern Prototypes of the APIs
//--------------------------------------------------------------------
extern void SPI_InitSPI0(void);
extern void SPI_SetFunc(void);
extern void SPI_ResetFunc(void);

extern void SPI0_WriteByte(U08 byte);
extern void SPI0_WriteCmmd(U08 sel, U08 dat);
extern void SPI0_WriteAddr(U32 addr_in);
extern U08  SPI0_ReadByte(void);


#endif // ___SPI_MAIN_H___

// EOF

