//--------------------------------------------------------------------------------------------------
//  Copyright (C) 2020, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use,
//  copying, transfer or disclosure of such information is prohibited except by express written
//  agreement with WESS-Global Co.,LTD
//
//  $Date: 2020-07-13 12:00:00 +0900 $
//  $Author: eddie $
//  $Rev: 1.0 $
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------------------------------------
// lib
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
#include "font_def.h"
// Self
#include "lcd_data.h"
// bsp
#include "lcd_main.h"
// app
#include "font_def.h"
#include "disp_main.h"


//--------------------------------------------------------------------------------------------------
//  Global variables
//--------------------------------------------------------------------------------------------------
const U08 code gLd_mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

U08 gLd_buff[LDT_BUFF_SIZE_Y][LDT_BUFF_SIZE_X];		// OLD: unsigned char xdata l_display_array[Y_BYTES][X_BYTES];


//--------------------------------------------------------------------------------------------------
//  Local variables
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global APIs - with Local
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local Funtions
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global APIs - Ext. Call
//--------------------------------------------------------------------------------------------------
// OLD: void GLCD_Vert_Dot_line(unsigned char column_UC_L, unsigned char top_UC_L, unsigned char bottom_UC_L)
void LCD_DataDot_LineVert(U08 column_UC_L, U08 top_UC_L, U08 bottom_UC_L)
{
    U08 bit_pos, byte_offset, mask, row_UC_L;

    bit_pos = column_UC_L & 0x07;		/* get the bit offset into a byte */
    byte_offset = column_UC_L >> 3;		/* get the byte offset into x array */
    mask = gLd_mask[bit_pos]; 	        /* get the mask for this bit */

    for(row_UC_L = top_UC_L; row_UC_L <= bottom_UC_L; row_UC_L++)
    {
        if(row_UC_L%2 == 0)     gLd_buff[row_UC_L][byte_offset] |= mask;
        else                    gLd_buff[row_UC_L][byte_offset] &= ~mask;
    }
}

// OLD: void GLCD_Dot(unsigned char column_UC_L, unsigned char row_UC_L)
void LCD_DataDot_Buff(unsigned char column_UC_L, unsigned char row_UC_L)
{
	unsigned char bit_pos;
	unsigned char byte_offset;
	unsigned char mask;

  	bit_pos = column_UC_L & 0x07;		/* get the bit offset into a byte */
  	byte_offset = column_UC_L >> 3;		/* get the byte offset into x array */
  	mask = gLd_mask[bit_pos]; 	/* get the mask for this bit */
  	gLd_buff[row_UC_L][byte_offset] |= mask;
}

// OLD: void GLCD_Glyph(unsigned char left, unsigned char top, unsigned char width, unsigned char height, unsigned char *glyph, unsigned char store_width, unsigned char Flag_invert_UC_L, unsigned char space_width_UC_L)
void LCD_WriteGlyph(U08 left, U08 top, U08 width, U08 height, U08 *glyph, U08 store_width, U08 Flag_invert_UC_L, U08 space_width_UC_L)
{
	U08 bit_pos;
	U08 byte_offset;
	U08 x_bits;
	U08 remaining_bits;
	U08 mask;
	U08 char_mask;
	U08 y;
	U08 *glyph_scan;
	U08 glyph_offset;

    bit_pos = left & 0x07;		/* get the bit offset into a byte */

    glyph_offset = 0;			/* start at top of the glyph rasters */

    for (y = top; y < (top + height); y++)
    {
        byte_offset = left >> 3;        /* get the byte offset into x direction,  left >> 3 = left / 8 */
        x_bits = width + space_width_UC_L + space_width_UC_L;					/* get length in y direction to write */
        remaining_bits = 8 - bit_pos;	/* number of bits left in byte */
        mask = gLd_mask[bit_pos];	/* get mask for this bit */

        glyph_scan = glyph + glyph_offset;	 /* point to start of glyph raster */

        char_mask = 0x80;			/* initial character glyph mask */

        /* boundary checking here to account for the possibility of  */
        /* write past the right of the screen.                        */
        while((x_bits) && (byte_offset < LDT_BUFF_SIZE_X)) /* while there are bits still to write */
        {
            if(x_bits > (width + space_width_UC_L) || x_bits <= space_width_UC_L)
            {
                /* check if the character pixel is set or not */
                if(Flag_invert_UC_L == 0)
                {
                  gLd_buff[y][byte_offset] &= ~mask;	/* clear the image pixel */
                }
                else
                {
                  gLd_buff[y][byte_offset] |= mask;	/* set image pixel */
                }
            }
            else
            {
                /* check if the character pixel is set or not */
                if(Flag_invert_UC_L == 0)
                {
                    if(*glyph_scan & char_mask)
                    {
                      gLd_buff[y][byte_offset] |= mask;	/* set image pixel */
                    }
                    else
                    {
                      gLd_buff[y][byte_offset] &= ~mask;	/* clear the image pixel */
                    }
                }
                else
                {
                    if(*glyph_scan & char_mask)
                    {
                      gLd_buff[y][byte_offset] &= ~mask;	/* clear the image pixel */
                    }
                    else
                    {
                      gLd_buff[y][byte_offset] |= mask;	/* set image pixel */
                    }
                }

                /* shift over to next glyph bit */
                char_mask >>= 1;
                if(char_mask == 0)				/* reset for next byte in raster */
                {
                    char_mask = 0x80;
                    glyph_scan++;
                }
            }

            if(gLd_mask[0] & 0x80)
            {
              mask >>= 1;
            }
            else
            {
              mask <<= 1;
            }

            x_bits--;
            remaining_bits--;
            if(remaining_bits == 0)
            {
                /* just crossed over a byte boundry, reset byte counts */
                remaining_bits = 8;
                byte_offset++;
                mask = gLd_mask[0];
            }
        }
    /* bump the offset to next raster */
    glyph_offset += store_width;
    }
}

// OLD: void GLCD_Horz_line(unsigned char row_UC_L, unsigned char left_UC_L, unsigned char right_UC_L)
void LCD_DataLine_Horz(unsigned char row_UC_L, unsigned char left_UC_L, unsigned char right_UC_L)
{
    unsigned char bit_pos;
    unsigned char byte_offset;
    unsigned char x_bits;
    unsigned char remaining_bits;
    unsigned char mask;

    bit_pos = left_UC_L & 0x07;		   /* get starting bit offset into byte */

    byte_offset = left_UC_L >> 3;		   /* get byte offset into y direction */
    x_bits = (right_UC_L - left_UC_L) + 1;   /* get length in the x direction to write */
    remaining_bits = 8 - bit_pos;  /* number of bits left in byte */
    mask = gLd_mask[bit_pos];  /* get mask for this bit */

    while(x_bits)				   /* while there are still bits to write */
    {
        if((remaining_bits == 8) && (x_bits > 7))
        {
            /* here if we are byte aligned and have at least 1 byte to write */
            /* do the entire byte at once instead of bit by bit */
            while(x_bits > 7)			/* while there are at least 8 more bits to do */
            {
                gLd_buff[row_UC_L][byte_offset] = 0xFF;
                byte_offset++;
                x_bits -= 8;
            }
        }
        else
        {
            /* we are not byte aligned or an entire byte does not need written */
            /* do each individual bit                                          */
            gLd_buff[row_UC_L][byte_offset] |= mask;
            if(gLd_mask[0] & 0x80)
            {
            	mask >>= 1;
            }
            else
            {
            	mask <<= 1;
            }

            x_bits--;
            remaining_bits--;

            if(remaining_bits == 0)
            {
                /* might have bust gotton byte aligned */
                /* so reset for beginning of a byte */
                remaining_bits = 8;
                byte_offset++;
                mask = gLd_mask[0];
            }
        }
    }
}

// OLD: void GLCD_Box(unsigned char left, unsigned char top, unsigned char right, unsigned char bottom)
void LDT_DrawBox(U08 left, U08 top, U08 right, U08 bottom)
{
  	/* to draw a box requires two vertical lines */
  	LDT_LineVert(left,  top, bottom, LDT_LINE_DATA);
  	LDT_LineVert(right, top, bottom, LDT_LINE_DATA);
  	/* and two horizonal lines */
  	LCD_DataLine_Horz(top, left, right);
  	LCD_DataLine_Horz(bottom, left, right);
}

// OLD: void GLCD_Text(unsigned char left, unsigned char top, unsigned char font_UC_L, char *str, unsigned char Flag_invert_UC_L, unsigned char space_width_UC_L)
void LCD_WrStrBuff(U08 left, U08 top, U08 font, char *str, U08 f_invert, U08 space)
{
	U08 code *p_glyph;
	U08 x = left;
	U08 glyph, i;
	U08 width;
	U08 height;
	U08 w_store;		// width store
	U16 tmp0, tmp1;

	while(*str != 0x00)
	{
		glyph = (U08)*str;

		/* check to make sure the symbol is a legal one */
		/* if not then just replace it with the default character */
		if((glyph < gFnt[font].glyph_beg) || (glyph > gFnt[font].glyph_end))
			glyph = gFnt[font].glyph_def;

		/* make zero based index into the font data arrays */
		glyph -= gFnt[font].glyph_beg;
		width  = gFnt[font].fixed_width;	/* check if it is a fixed width */

		if(width == 0)
			width = gFnt[font].pixel_width_table[glyph];	/* get the variable width instead */

		height = gFnt[font].glyph_height;

		//if(font == F3_EN_24x26 || font == F4_EN_26x47)
		if(font == FT_EN_26x47)
		{
			w_store = gFnt[font].char_width_table[glyph];

			tmp0 = 0;
			for(i = 0; i < glyph; i++)
			{
				tmp1 = gFnt[font].char_width_table[i];
				tmp0 = tmp0 + (tmp1 * (U16)height);
			}

			p_glyph = gFnt[font].glyph_table + tmp0;
		}
		else
		{
			switch(font)
			{
				case FT_EN_06x07:	w_store = 1;	break;
				//case F1_EN_05x08:	w_store = 1;	break;
				case FT_EN_08x16:	w_store = 1;	break;
				//case F5_KO_16x16:	w_store = 2;	break;
				case FT_LC_16x16:	w_store = 2;	break;
			}

			p_glyph = gFnt[font].glyph_table + ((U16)glyph * (U16)w_store * (U16)height);
		}

		/* range check / limit things here */
		if(x > LCD_X_END)								x = LCD_X_END;
		if((x + width + (space*2)) > (LCD_X_END + 1))	width = (LCD_X_END + 1) - x - (space*2);

		if(top > LCD_Y_END)								top = LCD_Y_END;
		if((top + height) > (LCD_Y_END + 1))			height = (LCD_Y_END + 1) - top;

		LCD_WriteGlyph(x, top, width, height, p_glyph, w_store, f_invert, space);  /* plug symbol into buffer */

		x = x + width + (space*2);			/* move right for next character */
		str++;								/* point to next character in string */
	}
}

void LDT_LineVert(U08 pos_x, U08 top, U08 btm, U08 opt)
{
	U08 i;
	U08 b_pos;
	U08 offset;
	U08 mask;

  	b_pos  = pos_x & 0x07;			/* get the bit offset into a byte */
  	offset = pos_x >> 3;			/* get the byte offset into x array */
  	mask   = gLd_mask[b_pos]; 		/* get the mask for this bit */

	switch(opt)
	{
		case LDT_LINE_CLEAR:	for(i=top; i<=btm; i++)		gLd_buff[i][offset] &= ~mask;	break;
		case LDT_LINE_DATA:		for(i=top; i<=btm; i++)		gLd_buff[i][offset] |=  mask;	break;
	}
}

// OLD: void GLCD_Clr_Dot(unsigned char column_UC_L, unsigned char row_UC_L)
void LDT_ClearDot(U08 pos_x, U08 pos_y)
{
	U08 bit_pos;
	U08 byte_offset;
	U08 mask;

  	bit_pos     = pos_x & 0x07;			/* get the bit offset into a byte */
  	byte_offset = pos_x >> 3;			/* get the byte offset into x array */
  	mask        = gLd_mask[bit_pos]; 	/* get the mask for this bit */

  	gLd_buff[pos_y][byte_offset] &= ~mask;
}

// OLD: void Init_GLCD_240(void)
void LDT_InitScrn(void)
{
	LCD_InitCfg();		// initialize LCD display
	LCD_InitErase();	// erase the display 

	DISP_ClearScr();
}

// OLD: void GLCD_Clear_area(unsigned char left,  unsigned char top, unsigned char right, unsigned char bottom)
void LDT_ClrBuff(U08 left, U08 top, U08 right, U08 bottom)
{
    U08 bit_pos;
    U08 y;
    U08 byte_offset;
    U08 x_bits;
    U08 remaining_bits;
    U08 mask;

    bit_pos = left & 0x07;					/* get starting bit offset into byte */

    for(y = top; y <= bottom; y++)
    {
        byte_offset = left >> 3;			/* get byte offset into x direction */
        x_bits = (right - left) + 1;		/* get length in the x direction to write */
        remaining_bits = 8 - bit_pos;		/* number of bits left in byte */
        mask = gLd_mask[bit_pos];		/* get mask for this bit */

        while(x_bits)						/* while there are still bits to write */
        {
            if((remaining_bits == 8) && (x_bits > 7))
            {
                /* here if we are byte aligned and have at least 1 byte to write */
                /* do the entire byte at once instead of bit by bit */
                while(x_bits > 7)			/* while there are at least 8 more bits to do */
                {
                    gLd_buff[y][byte_offset] = 0x00;
                    byte_offset++;
                    x_bits -= 8;
                }
            }
            else
            {
                /* here if not byte aligned or an entire byte does not need written */
                /* thus do bit by bit */
                gLd_buff[y][byte_offset] &= ~mask;

                if(gLd_mask[0] & 0x80)         mask >>= 1;
                else                            mask <<= 1;

                x_bits--;
                remaining_bits--;

                if(remaining_bits == 0)
                {
                    /* might have bust gotton byte aligned */
                    /* so reset for beginning of a byte */
                    remaining_bits = 8;
                    byte_offset++;

                    mask = gLd_mask[0];
                }
            }
        }
    }
}

// OLD: void GLCD_Update(unsigned char top, unsigned char bottom)
void LDT_UpdBuff(U08 top, U08 bottom)
{
	U08 x;
	U08 y;
	U08 dat;
	U08 *p_col;

	for(y=top; y<=bottom; y++)
	{
		/* set addr ptr to start of line */
		LCD_TxCmd2(0x24, y*30);				/* scan the display up top to bottom */
		LCD_TxCmd0(0xB0);					/* send the auto write command */

		p_col = &gLd_buff[y][0];			/* point to row of data in buffer */

		for(x=0; x<LDT_BUFF_SIZE_X; x++)
		{
			do
				dat = LCD_GetData();
			while((dat & 0x08) != 0x08);	/* wait till STA3=1 */

			LCD_CfgData(*p_col++);
		}

		do									/* poll loop for ready for auto reset command */
			dat = LCD_GetData();	
		while((dat & 0x08) != 0x08);		/* wait till STA2=1 */

		LCD_CfgCtrl(0xB2);					/* send the auto reset command */
	}
}


// EOF

