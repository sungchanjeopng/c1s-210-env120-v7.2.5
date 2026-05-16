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
// self
#include "crc_main.h"


//--------------------------------------------------------------------
//  Local Macros
//--------------------------------------------------------------------
#define POLYNORMIAL     0xA001


//--------------------------------------------------------------------
//  Local Type definitions
//--------------------------------------------------------------------


//--------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------
U16 gCc_b16;    // OLD: unsigned int crc16_UI_G;


//--------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Funtion Definitions
//--------------------------------------------------------------------
// OLD: unsigned int CRC16(unsigned char *puchMsg, unsigned char usDataLen)
U16 CRC_GetB16(U08 *p_msg, U08 size)
{
	U08 i;
	U16 flag;
	U16 crc = 0xffff;

	while(size--)
	{
	    crc ^= *p_msg++;
	    for(i=0; i<8; i++)
	    {
	    	flag = crc & 0x0001;
	    	crc >>= 1;
	    	if(flag) crc ^= POLYNORMIAL;
	    }
	}

	return crc;
}


// EOF

