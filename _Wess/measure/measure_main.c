//------------------------------------------------------------------------------------------------------------------------------
//  Copyright (C) 2021, WESS-Global Inc., All Rights Reserved.
//
//  The information contained herein is confidential property of WESS-Global Inc. The use, copying, transfer or disclosure of
//  such information is prohibited except by express written agreement with WESS-Global Co.,LTD
//
//  $ Date: 2020-07-13 12:00:00 +0900 $
//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------------------------------------------------
// chipset
#include "compiler_defs.h"
#include "C8051F120_defs.h"
// sys
#include "sys_def.h"
// bsp
#include "bsp_main.h"
#include "flash_main.h"
#include "uart_main.h"
#include "gpio_main.h"
#include "mem_main.h"
#include "btn_main.h"
#include "adc_main.h"
#include "dac_main.h"
#include "dly_main.h"
#include "pcd_main.h"
// app
#include "app_main.h"
#include "data_main.h"
#include "modbus_main.h"
#include "prtc_main.h"
// screen
#include "screen_main.h"
// measure
#include "measure_analyze.h"
#include "measure_temperature.h"
// disp
#include "disp_main.h"
// menu
#include "menu_box.h"
#include "menu_measure.h"
#include "menu_output.h"
#include "menu_quick.h"
#include "menu_engineer.h"
// self
#include "measure_main.h"

#include "modbus_main.h"
//------------------------------------------------------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------------------------------------------------------
//U08 lMsr_cAvrg;					// Raw Data Count for Average // OLD: unsigned char NUM_AD_AVG_UC_G[4];


//------------------------------------------------------------------------------------------------------------------------------
//  Local variables
//------------------------------------------------------------------------------------------------------------------------------
LS_MEAS lMsr;

U16 lMsr_aTvg[MnMSR_TVG_IDX_MAX][60] = {
	{	  1,   4,   8,  12,  16,  20,  24,  28,  32,  36,  40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
		 80,  84,  88,  92,  96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156,    
		160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236,		},
	{	  1,   7,  10,  13,  16,  20,  24,  28,  32,  36,  40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
		 80,  84,  88,  92,  96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156,
		160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236,		},
	{	  1,  14,  16,  18,  20,  22,  24,  28,  32,  36,  40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
		 80,  84,  88,  92,  96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156,    
		160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236,		},
	{	  1,  18,  20,  22,  24,  26,  28,  30,  32,  36,  40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
		 80,  84,  88,  92,  96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156,
		160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236,		},
	{	  1,  22,  24,  26,  28,  30,  32,  34,  36,  38,  40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
		 80,  84,  88,  92,  96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156,
		160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236,		},
	{	  1,  26,  28,  30,  32,  34,  36,  38,  40,  42,  44,  46,  48,  52,  56,  60,  64,  68,  72,  76,
		 80,  84,  88,	92,  96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156,
		160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236,		},
};

#define AUTO_GAIN_MARGIN_LEFT	20		// Left range from echo position
#define AUTO_GAIN_MARGIN_RIGHT	50		// Right range from echo position
#define AUTO_GAIN_SEARCH_RANGE	9		// Search base gain -9 ~ +9
#define AUTO_GAIN_VOLT_THR		106		// Valid signal threshold: 1.0V (255 = 2.4V)
#define AUTO_GAIN_MIN			MnMSR_AMP_MIN
#define AUTO_GAIN_MAX			MnMSR_AMP_MAX

static U08  lAutoGain_MaxSlope;
static U08  lAutoGain_MinVolt;
static U08  lAutoGain_MaxVolt;
static U16  lAutoGain_RangeMin;
static U16  lAutoGain_RangeMax;


//------------------------------------------------------------------------------------------------------------------------------
//  Local Funtions
//------------------------------------------------------------------------------------------------------------------------------
void MsrCfg_SetTx(U08 set)
{
    U08 old = SFRPAGE;

    SFRPAGE = CONFIG_PAGE;

	// TR
    switch(set)
    {
        case 0:     lMsr.tr_step = 1;    break; //old - TR: 1, ES: 2
        case 1:     lMsr.tr_step = 2;    break; //old - TR: 2, ES: 2
        case 2:     lMsr.tr_step = 3;    break; //old - TR: 2, ES: 2
        case 3:     lMsr.tr_step = 4;    break; //old - TR: 3, ES: 3
        case 4:     lMsr.tr_step = 5;    break; //old - TR: 3, ES: 3
    }

	// ES (Echo Strength)
	switch(set)		// 19.08.22 pter  level 2 -> 5
	{
		case 0:     _GIO_VC_A3 = 1;   _GIO_VC_A2 = 0;   _GIO_VC_A1 = 1;   _GIO_VC_A0 = 1;   break; //old - TR: 1, ES: 2
		case 1:     _GIO_VC_A3 = 1;   _GIO_VC_A2 = 0;   _GIO_VC_A1 = 1;   _GIO_VC_A0 = 1;   break; //old - TR: 2, ES: 2
		case 2:     _GIO_VC_A3 = 1;   _GIO_VC_A2 = 0;   _GIO_VC_A1 = 1;   _GIO_VC_A0 = 1;   break; //old - TR: 2, ES: 2
		case 3:     _GIO_VC_A3 = 1;   _GIO_VC_A2 = 0;   _GIO_VC_A1 = 1;   _GIO_VC_A0 = 1;   break; //old - TR: 3, ES: 3
		case 4:     _GIO_VC_A3 = 0;   _GIO_VC_A2 = 1;   _GIO_VC_A1 = 1;   _GIO_VC_A0 = 1;   break; //old - TR: 3, ES: 3
	}

    SFRPAGE = old;
}

// OLD: void HW_Set(void) 
void MsrCfg_HwAll(void) 
{
	U08 old = SFRPAGE;

	SFRPAGE = CONFIG_PAGE;

	SBIT_MSR_GAIN = GIO_SET_HI;		// 1: 100k, 0: 47K

	MsrCfg_SetTx(MSR_ES_S01);


	if(lMsr.freq < MnMSR_FREQ_500K)		SBIT_MSR_BPF = 1; 
	else                    			SBIT_MSR_BPF = 0; 


	SFRPAGE = old;
}

void MsrCfg_AdcTiming(void) 
{
	F32 dmy_0, dmy_1;
	U32 dummy;

	dmy_1 = lMsr.veloc;
	dmy_0 = (100.0 * 2.0 * 10000000.0) / (SYS_CLK * dmy_1);

	dummy = dmy_0;
	lMsr.dly10cm_h = dummy / 2560;
	lMsr.dly10cm_l = (dummy % 2560) / 10;

	dmy_1 = lMsr.veloc;
	dmy_0 = (10.0 * 120.0 * 10000.0) / dmy_1;	// 1480m/s  1Cm = 13.5135us

	SFRPAGE = TMR2_PAGE;
	TMR2CF = 0x08;

	RCAP2H = (655360 - dmy_0) / 2560;
	RCAP2L = ((655360 - dmy_0) / 10) - (RCAP2H * 256);

	TF2 = 0;
	TMR2H = RCAP2H;
	TMR2L = RCAP2L;

	//////////	ADC0 set : SPS, CH, Start of Conversion Mode(Timer2 overflow)	//////////
	SFRPAGE = ADC0_PAGE;
	ADC0CF = 0x98;		// SAR CLK : 2.5 MHz
	AMX0SL = 7;
	ADC0CN = 0x8D;		//0xCD
}


void MsrCfg_CalcSos(void)      // Sound Of Speed
{
	F32 dmy_0, dmy_1;

	//-------------- temperature correction --------------//
	// c = 1410+ temp_c*4.6 - temp_c*temp_c*0.055 + temp_c*temp_c*0.00029*temp_c;

	if(gTp_deg < -250 || gTp_deg > 1000)
	    gTp_deg = 200;

	dmy_0 = gTp_deg / 10.0;
	dmy_1 = 1410 + dmy_0 * 4.6 - dmy_0 * dmy_0 * 0.055 + dmy_0 * dmy_0 * 0.00029 * dmy_0;

	lMsr.veloc = dmy_1;   // SOS
}

void MsrCfg_TxClk(U08 tx_cnt) 
{
	SFRPAGE = CONFIG_PAGE;

	switch(MnMSR_GetFreq())
	{
		case MnMSR_FREQ_130K: // 130 kHz
			while(tx_cnt)
			{
				
				//GIO_PORT_TXCLK = GIO_TXCLK_OFF;
				//_nop_();		_nop_();        _nop_();
				//_nop_();		_nop_();        _nop_();
				
				GIO_PORT_TXCLK = GIO_TXCLK_ON1;
				DLY_001usec();
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;

				//GIO_PORT_TXCLK = GIO_TXCLK_OFF;
				//_nop_();		_nop_();        _nop_();
				//_nop_();		_nop_();        _nop_();

				GIO_PORT_TXCLK = GIO_TXCLK_ON2;
				DLY_001usec();
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;

				tx_cnt--;
			}
			GIO_PORT_TXCLK = GIO_TXCLK_OFF;
			break;

		case MnMSR_FREQ_160K: // 160 kHz
			while(tx_cnt) 
			{

				//GIO_PORT_TXCLK = GIO_TXCLK_OFF;
				//_nop_();		_nop_();        _nop_();
				//_nop_();		_nop_();        _nop_();

				GIO_PORT_TXCLK = GIO_TXCLK_ON1;
				DLY_001usec();
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;

				//GIO_PORT_TXCLK = GIO_TXCLK_OFF;
				//_nop_();		_nop_();        _nop_();
				//_nop_();		_nop_();        _nop_();

				GIO_PORT_TXCLK = GIO_TXCLK_ON2;
				DLY_001usec();
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;

				tx_cnt--;
			}
			GIO_PORT_TXCLK = GIO_TXCLK_OFF;
			break;

		case MnMSR_FREQ_270K: // 270 kHz
			while(tx_cnt) 
			{

				//GIO_PORT_TXCLK = GIO_TXCLK_OFF;
				//_nop_();		_nop_();        _nop_();	
				//_nop_();		_nop_();        _nop_();

		        GIO_PORT_TXCLK = GIO_TXCLK_ON1;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        _nop_();        _nop_();        _nop_();

				//GIO_PORT_TXCLK = GIO_TXCLK_OFF;
				//_nop_();		_nop_();        _nop_();
				//_nop_();		_nop_();        _nop_();

		        GIO_PORT_TXCLK = GIO_TXCLK_ON2;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        _nop_();        _nop_();        _nop_();

		        tx_cnt--;
		    }
		    GIO_PORT_TXCLK = GIO_TXCLK_OFF;
		    break;

		case MnMSR_FREQ_380K: // 380 kHz
		    while(tx_cnt) 
		    {
		        GIO_PORT_TXCLK = GIO_TXCLK_ON1;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        _nop_();        _nop_();        _nop_();

		        GIO_PORT_TXCLK = GIO_TXCLK_ON2;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        SBIT_P6_B6 = 0;
		        _nop_();        _nop_();        _nop_();

		        tx_cnt--;
		    }
		    GIO_PORT_TXCLK = GIO_TXCLK_OFF;
		    break;

			
		case MnMSR_FREQ_500K: // 500 kHz
		    while (tx_cnt) 
		    {
		        GIO_PORT_TXCLK = GIO_TXCLK_ON1;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        _nop_();        _nop_();        _nop_();

		        GIO_PORT_TXCLK = GIO_TXCLK_ON2;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        _nop_();        _nop_();        _nop_();

		        tx_cnt--;
		    }
		    GIO_PORT_TXCLK = GIO_TXCLK_OFF;
		    break;

		case MnMSR_FREQ_620K: // 620 kHz
		    while(tx_cnt)
		    {
		        GIO_PORT_TXCLK = GIO_TXCLK_ON1;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        _nop_();        _nop_();        _nop_();

		        GIO_PORT_TXCLK = GIO_TXCLK_ON2;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        _nop_();        _nop_();        _nop_();

		        tx_cnt--;
		    }
		    GIO_PORT_TXCLK = GIO_TXCLK_OFF;
		    break;

		case MnMSR_FREQ_750K: // 750 kHz
		    while (tx_cnt) 
		    {
		        GIO_PORT_TXCLK = GIO_TXCLK_ON1;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        _nop_();        _nop_();    _nop_();

		        GIO_PORT_TXCLK = GIO_TXCLK_ON2;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
				SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;	SBIT_P6_B6 = 0;
		        SBIT_P6_B6 = 0;
		        _nop_();        _nop_();        _nop_();

		        tx_cnt--;
		    }
		    GIO_PORT_TXCLK = GIO_TXCLK_OFF;
		    break;
	}
}

// OLD: void Transmission(unsigned char Sel_Tx_UC_L) 
void MsrCfg_ProcTx(U08 tx_sel) 
{
	U08 tx_num;
	U08 old = SFRPAGE;

	MsrCfg_SetTx(MSR_ES_S01);	// Echo Stregth

	if(MnMSR_GetFreq() < MnMSR_FREQ_380K)	tx_num = (lMsr.tr_step * 5) + 5;
	else									tx_num = (lMsr.tr_step + 2) * 5; 

	lMsr.tvg_idx = 0;
	MEAS_SetDacTvg();

	SFRPAGE = TIMER01_PAGE;
	TH0 = 0xFE;
	TL0 = 0xAD;
	TR0 = 1;                      // TVG curve

	SFRPAGE = PCA0_PAGE;
	PCA0CN = 0x40;	//CR = 1;     // Tx interval

	ES0 = 0;

	if(tx_sel == MSR_TX_RUN)
		MsrCfg_TxClk(tx_num);

	ES0 = 1;

	SFRPAGE = old;
}

void MsrCfg_Dly10cm(void) 
{
    U08 pca0l = 0;
    U08 pca0h = 0;

    SFRPAGE = PCA0_PAGE;

    while(pca0l < lMsr.dly10cm_l || pca0h < lMsr.dly10cm_h)	// 10Cm hold
    {
    	pca0l = PCA0L;
    	pca0h = PCA0H;
    }
}

// OLD: void End_of_Capture(void) 
void MsrCfg_EndCapture(void) 
{
	SFRPAGE = PCA0_PAGE;
	PCA0CN = 0x00;				//CR = 0;
	SFRPAGE = TIMER01_PAGE;
	TR0 = 0;

	DAC_WriteVal(0);
}


//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Access Local Variables
//------------------------------------------------------------------------------------------------------------------------------
// Get
S16 MEAS_GetSgThick(void)		{	return lMsr.sg_thick;		}
U08 MEAS_GetFgAdc(void)			{	return lMsr.fAdc_full;		}
U08 MEAS_GetCtAvrg(void)		{	return lMsr.cAvrg;			}
F32 MEAS_GetVeloc(void)			{	return lMsr.veloc;			}
U08 MEAS_AutoGain_GetValue(void)	{	return lMsr.auto_gain;	}
U08 MEAS_AutoGain_GetMaxSlope(void)	{	return lAutoGain_MaxSlope;	}

U16 MEAS_GetTvg(U08 num, U08 idx)
{
	if( (idx < 0) || (idx > 59))
		return 0;

	return lMsr_aTvg[num][idx];
}

//------------------------------------------------------------------------------------------------------------------------------
//  Global APIs - Public
//------------------------------------------------------------------------------------------------------------------------------
void MEAS_CfgSens(U08 idx) 
{
	if((idx >= 0) && (idx <= 3))		lMsr.intv = MnMSR_GetIntv();
	if((idx >= 0) && (idx <= 4))		lMsr.freq = MnMSR_GetFreq();
}

void MEAS_InitVari(void)
{
	lMsr.fAdc_full = FALSE;
	lMsr.cIntv = 0;
	lMsr.sg_thick  = 0;
	lMsr.fauto_gain = 0;
	lMsr.auto_calib = MnMSR_GetAmp();
	lMsr.auto_gain  = MnMSR_GetAmp();
}

void MEAS_SetDacTvg(void)		// TVG: Time Variable Gain
{
	U08 tvg = MnEGN_GetTvgNumb();
	U08 amp;
	U32 gain = 0;

	if(lMsr.fauto_gain)	amp = lMsr.auto_calib;
	else				amp = MnMSR_GetAmp();

    lMsr.tvg_idx++;

    if(lMsr.tvg_idx > 59)
    	lMsr.tvg_idx = 59;

#if 1
	if(tvg >= MnMSR_TVG_IDX_MAX)
		tvg = MnMSR_TVG_I00_IDLE;

	if	(MnEGN_GetFTvgCst())	gain = MnEGN_GetCstTvg(lMsr.tvg_idx) * amp;
	else						gain = lMsr_aTvg[tvg][lMsr.tvg_idx] * amp;
#else
	if(!MnEGN_GetFgTvgSel())	tvg= MnMSR_TVG_I06_CST;

	if(tvg >= MnMSR_TVG_IDX_MAX)
		tvg = MnMSR_TVG_I00_IDLE;

	switch(tvg)
	{
		case MnMSR_TVG_I06_CST:
		case MnMSR_TVG_I00_IDLE:
		default:
			if(MnEGN_GetFTvgCst())		gain = MnEGN_GetCstTvg(lMsr.tvg_idx) * amp;
			else						gain = lMsr_aTvg[tvg][lMsr.tvg_idx] * amp;
			break;
	}
#endif
	if(gain > 2000)
		gain = 2000;
		
	DAC_WriteVal(gain);
}

void MEAS_CfgThreshold(void)
{
	U08 trhd_mod = MnMSR_GetTrhdMod();
	U08 trhd_per = MnMSR_GetTrhdPer();
	U08 trhd_fix = MnMSR_GetTrhdFix();
	U08 save_per = trhd_per;

	if(trhd_mod == MnMSR_TRHD_MOD_AUTO)
	{
		if(!_SBIT_BTN_UP) 
		{
			MnMSR_SetTrhdPer(trhd_per + MnMSR_TRHD_PCT_STP);
			
			if(MnMSR_GetTrhdPer() > MnMSR_TRHD_PCT_MAX)
				MnMSR_SetTrhdPer(MnMSR_TRHD_PCT_MAX);
		}
		
		if(!_SBIT_BTN_DOWN) 
		{
			MnMSR_SetTrhdPer(trhd_per - MnMSR_TRHD_PCT_STP);
			
			if(MnMSR_GetTrhdPer() < MnMSR_TRHD_PCT_MIN)
				MnMSR_SetTrhdPer(MnMSR_TRHD_PCT_MIN);
		}
	}
	else
	{
		if(!_SBIT_BTN_UP) 
		{
			if(trhd_fix > 200)		MnMSR_SetTrhdFix(220);
			else					MnMSR_SetTrhdFix(trhd_fix + 20);
		}
		
		if(!_SBIT_BTN_DOWN)
		{
			if(trhd_fix < 40)		MnMSR_SetTrhdPer(20);
			else					MnMSR_SetTrhdFix(trhd_fix - 20);
		}
	}

	ANZ_CalcTrshd();
	SCRN_ProcDisp();
	
	FLS_WriteAll();
 	BTN_DelayMsec(300);
	
	if(save_per != MnMSR_GetTrhdPer())
		MsANL_SetFgTimWin(FALSE);
}

F32 MEAS_CalcFeet2Meter(U16 feet)
{
	return (feet * MSR_CALC_F2M_CONST) + 0.5;
}

F32 MEAS_CalcMeter2Feet(U16 meter)
{
	return (meter * MSR_CALC_M2F_CONST) + 0.5;
}

void MEAS_ResetFlag(void) 
{
	lMsr.fAdc_full  = 0;
	lMsr.cIntv = 0;

	MsANL_ResetFlag();
	
	ANZ_SetEmptyPct(110);
}

void MEAS_SetOpr(void)
{
	U08 unit = MnMSR_GetUnit();
	U08 oper = MnMSR_GetOper();

	switch(oper)
	{
		case MnMSR_OPER_LEVEL:
			if(unit == MnMSR_UNIT_FEET)		lMsr.sg_thick = MsANL_GetLevelFt();
			else							lMsr.sg_thick = MsANL_GetLevelMt();
			break;

		case MnMSR_OPER_DISTA:
		default:
			if(unit == MnMSR_UNIT_FEET)		lMsr.sg_thick = MsANL_GetDistaFt();
			else							lMsr.sg_thick = MsANL_GetDistaMt();
			break;
	}
}

void MEAS_PrcEcho(void) 
{
	MsTPR_CalcVal();

	MsrCfg_HwAll();
	MsANL_ResetTimer();

	MsrCfg_CalcSos();
	MDB_PrcMain();
	MsrCfg_AdcTiming();
	MsrCfg_ProcTx(MSR_TX_RUN);
	MsrCfg_Dly10cm();

	ADC_PrcEcho();
	MDB_PrcMain();
	MsrCfg_EndCapture();
}


static void MEAS_AutoGain_SaveBestWave(void)
{
	U16 i;
	for(i=0; i<ADC_DATA_MAX; i++)
		MEM_WriteByte(MEM_MRAM, M_ADDR_AUTOGAIN_BEST_START + i, gAd_data[i]);
}

static void MEAS_AutoGain_LoadBestWave(void)
{
	U16 i;
	for(i=0; i<ADC_DATA_MAX; i++)
		gAd_data[i] = MEM_ReadByte(MEM_MRAM, M_ADDR_AUTOGAIN_BEST_START + i);
}

static void MEAS_AutoGain_SetRange(U16 echo_pos)
{
	U16 deadzone = MnMSR_GetDead();
	U16 empty = MnMSR_GetEmpty();

	if(echo_pos < deadzone)
		echo_pos = deadzone;
	if(echo_pos > empty)
		echo_pos = empty;

	if(echo_pos <= (deadzone + AUTO_GAIN_MARGIN_LEFT))
		lAutoGain_RangeMin = deadzone;
	else
		lAutoGain_RangeMin = echo_pos - AUTO_GAIN_MARGIN_LEFT;

	if((empty - echo_pos) < AUTO_GAIN_MARGIN_RIGHT)
		lAutoGain_RangeMax = empty;
	else
		lAutoGain_RangeMax = echo_pos + AUTO_GAIN_MARGIN_RIGHT;

	if(lAutoGain_RangeMax > ADC_DATA_MAX)
		lAutoGain_RangeMax = ADC_DATA_MAX;
	if(lAutoGain_RangeMin >= lAutoGain_RangeMax)
		lAutoGain_RangeMin = deadzone;
}

static U08 MEAS_AutoGain_CalcSlope(void)
{
	U16 i;
	U08 min_volt = 255;
	U08 max_volt = 0;

	for(i=lAutoGain_RangeMin; i<lAutoGain_RangeMax; i++)
	{
		if(gAd_data[i] < min_volt) min_volt = gAd_data[i];
		if(gAd_data[i] > max_volt) max_volt = gAd_data[i];
	}

	lAutoGain_MinVolt = min_volt;
	lAutoGain_MaxVolt = max_volt;

	if(max_volt <= AUTO_GAIN_VOLT_THR)
		return 0;

	if(max_volt > min_volt)
		return (max_volt - min_volt);

	return 0;
}

static void MEAS_AutoGain_DoMeasure(U08 gain)
{
	lMsr.fauto_gain = 1;
	lMsr.auto_calib = gain;
	MEAS_PrcEcho();
	lMsr.fauto_gain = 0;
}

static U08 MEAS_AutoGain_Search(U16 echo_pos)
{
	S16 gain;
	S16 stt;
	S16 end;
	U08 test_gain;
	U08 slope;
	U08 base_gain = MnMSR_GetAmp();

	if(echo_pos == 0)
		echo_pos = MsANL_GetDist1st();

	MEAS_AutoGain_SetRange(echo_pos);

	lAutoGain_MaxSlope = 0;
	lMsr.auto_gain = base_gain;
	MEAS_AutoGain_SaveBestWave();

	stt = (S16)base_gain - AUTO_GAIN_SEARCH_RANGE;
	end = (S16)base_gain + AUTO_GAIN_SEARCH_RANGE;

	if(stt < AUTO_GAIN_MIN) stt = AUTO_GAIN_MIN;
	if(end > AUTO_GAIN_MAX) end = AUTO_GAIN_MAX;

	for(gain=stt; gain<=end; gain++)
	{
		test_gain = (U08)gain;
		MEAS_AutoGain_DoMeasure(test_gain);

		slope = MEAS_AutoGain_CalcSlope();

		if(slope > lAutoGain_MaxSlope)
		{
			lAutoGain_MaxSlope = slope;
			lMsr.auto_gain = test_gain;
			MEAS_AutoGain_SaveBestWave();
		}

		MDB_PrcMain();
	}

	lMsr.auto_calib = lMsr.auto_gain;
	MEAS_AutoGain_LoadBestWave();
	return TRUE;
}

void MEAS_PrcSub1(void) 
{
	if(MsANL_GetCtEcho() > lMsr.cAvrg)
	{
		MsANL_SetCtEcho(1);
		lMsr.fAdc_full = TRUE;
	}

	MEAS_PrcEcho();

	ES0 = 0;

	ANZ_ChkEcho_Level();

	if(MnOUT_GetProtocol() == MnOUT_PROT_MDBS)
	    ES0 = 1;

	SFRPAGE = UART0_PAGE;
	RI0 = 0;

	if(PCD_GetFgRun()==TRUE || MsANL_GetFgEcoDly()==TRUE || MsANL_GetFgEcoChk()==FALSE)
	{
		if(gTd_f_en == FALSE)
			DSP_SetBuff(220, gAd_data, 0, MsANL_GetEmpty());
		ADC_AvrgRaw();
		return;
	}

	if(MnMSR_GetFgAutoGain())
	{
		MEAS_AutoGain_Search(MsANL_GetDistMod());

		ES0 = 0;
		ANZ_ChkEcho_Level();
		if(MnOUT_GetProtocol() == MnOUT_PROT_MDBS)
			ES0 = 1;
		SFRPAGE = UART0_PAGE;
		RI0 = 0;
	}

	if(gTd_f_en == FALSE)
		DSP_SetBuff(220, gAd_data, 0, MsANL_GetEmpty());

	ADC_WriteRaw();
	//MDB_PrcMain();
	ADC_AvrgRaw();
	ADC_ProcSmooth(MnEGN_GetSmthCnt(), MnEGN_GetSmthRng());

	ANZ_CalcTrshd();
}

void MEAS_ProcMain(void) 
{
	U08 fTest = MnMSR_GetFgTest();
	U08 fEco_dtt = MsANL_GetFgEcoDtt();
	S16 dist_1st = MsANL_GetDist1st();
	S16 dist_mod = MsANL_GetDistMod();


	
	if(fTest)	lMsr.cAvrg = 1;
	else		lMsr.cAvrg = MnEGN_GetAdcNumb();

	lMsr.intv = MnMSR_GetIntv();
	lMsr.freq = MnMSR_GetFreq();

	if(++lMsr.cIntv < lMsr.intv)
		return;

	if(PCD_GetFgRun()==TRUE)
		return;


	lMsr.cIntv = 0;


	MEAS_PrcSub1();

	MDB_PrcMain();





	if(MsANL_GetFgEcoDly()==TRUE)
		return;

	if(MsANL_GetFgEcoChk()==TRUE)
	{

		MsANL_ProcSignal();

		if(lMsr.fAdc_full)
		{
			if(fEco_dtt)
			{
				if(MsANL_GetFgTimWin())		MsANL_TrackingSignal();
				else						ANZ_SetTw();
			}
		}
		else
		{
			if(MsANL_GetCtEcho() <= 3)
			{
				MsANL_CheckErr();
				return;
			}

			MsANL_SetDistMod(dist_1st);
		}

		if(fTest)
		{
			MsANL_SetDistaMt(dist_1st);
		}
		else
		{
			if(dist_mod != 0)
				ANZ_CalcDamp();
		}
		
		MsANL_CalcLevel();
	}
	MsANL_CheckErr();
}


//------------------------------------------------------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------------------------------------------------------

