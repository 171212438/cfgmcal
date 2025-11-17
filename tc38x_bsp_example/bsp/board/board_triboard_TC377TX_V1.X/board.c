/**************************************************************************************************
 * FILE: board.c
 * BOARD FAMILY:
 *    TriBoard TC377TX V1.X
 * UC SUPPORT:
 *    TC377TX  (LFBGA-292 ADAS Package)
 * DESCRIPTION:
 *    Implementation of Board support routines.
 * HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

/* board implementation guard */
#if defined (TRIBOARD_TC377TX_V1_X)

/* include common BSP header file */
#include "bsp.h"


/* ------------------------------------------------------------------------------------------------
 * Board LED port-pin setup
 *    The EVB has only 4 on-board LEDs, the rest is GPIOs
 * ----------------------------------------------------------------------------------------------*/

const BOARD_LED_S board_led[BOARD_NB_LEDS] =
{
	{33, 4},  /* LED_0 */
	{33, 5},  /* LED_1 */
	{33, 6},  /* LED_2 */
	{33, 7},  /* LED_3 */
	{01, 4},  /* LED_4 */
	{01, 5},  /* LED_5 */
	{01, 6},  /* LED_6 */
	{01, 7},  /* LED_7 */
};


/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_board_wdg_Disable()
 *    It disables external WATCHDOG present on evaluation board.
 * ----------------------------------------------------------------------------------------------*/

void bsp_board_wdg_Disable(void)
{
	/* commands sequence for external WDG to disable it */
    const uint16_t wdtdiscmd[] = 	{
    		0xBE09,   							/* Clear Duration error event */
			0x8756, 0x87de, 0x86ad, 0x8625,		/* unprotect register (PROTCFG) */
			0x8d27,								/* disable window watchdog */
			/*0x8801,*/							/* disable err pin monitor (not A-step) */
			0x87be, 0x8668, 0x877d, 0x8795		/* protect register (PROTCFG) */
	};

    /* QSPI module selection for communication with TLF30682*/
	Ifx_QSPI * const  module_qspi =  ((Ifx_QSPI*) ((uint32_t) &MODULE_QSPI0 + (0x100 * BOARD_TLF30682_QSPI_MODULE)));

	/* Reset QSPI module, if already enabled before, to get clear state */
    if (module_qspi->GLOBALCON.B.EN)
	{
		unlock_wdtcon();
		module_qspi->KRST1.B.RST = 1;
		module_qspi->KRST0.B.RST = 1;
		while(module_qspi->KRST0.B.RSTSTAT == 0);
		module_qspi->KRSTCLR.B.CLR = 1;
		lock_wdtcon();
	}

    /* QSPI module pins setting */
    bsp_uc_port_EnableOutput(BOARD_TLF30682_SLS_PORT,  BOARD_TLF30682_SLS_INDEX,  BOARD_TLF30682_SLS_PC,  UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_1);
    bsp_uc_port_EnableOutput(BOARD_TLF30682_SCLK_PORT, BOARD_TLF30682_SCLK_INDEX, BOARD_TLF30682_SCLK_PC, UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_1);
    bsp_uc_port_EnableOutput(BOARD_TLF30682_MTSR_PORT, BOARD_TLF30682_MTSR_INDEX, BOARD_TLF30682_MTSR_PC, UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_1);
    bsp_uc_port_EnableInput (BOARD_TLF30682_MRST_PORT, BOARD_TLF30682_MRST_INDEX, UC_PORT_INPUT_MODE_PULL_UP);

	/* Enable QSPI module */
	unlock_wdtcon();
	module_qspi->CLC.U = 8;   /* Enable the SPI module */
	(void)module_qspi->CLC.U;  /* read back to get effective */
	module_qspi->PISEL.U = 1;
	lock_wdtcon();

	/* program QSPI parameters */
	module_qspi->GLOBALCON.U = 0x60003C00;  /* RESETS=1,CLKSEL=fPER,EXPECT=15,SI=0, TQ=4 */
	module_qspi->GLOBALCON1.U = 0x00000600;  /* RXFM=1,TXFM=1 (Single Move Mode for RX/TX) */
	module_qspi->SSOC.U = 0x00020000;		/* enable SLSO21, low active */
	module_qspi->ECON[1].U = 0x00002947;			/* Q=1,A=0,B=1,C=1 */

	/* Make sure that flags in STATUS register are cleared */
	do
	{
		module_qspi->FLAGSCLEAR.U = 0xFFF;  /* PT2F,PT1F,RXF,TXF,ERRORFLAGS */
	} while (module_qspi->STATUS.U & 0xFFF);

	module_qspi->BACONENTRY.U = 0x17A24025;
	module_qspi->GLOBALCON.B.EN = 1;

	/* transfer all commands */
	uint32_t wdt_cmd_size = (uint32_t)(sizeof(wdtdiscmd) / sizeof(uint16_t));
	uint32_t i;
	for (i = 0; i < wdt_cmd_size; ++i)
	{
		/* clear TX flag */
		module_qspi->FLAGSCLEAR.U = 1 << 9;
		module_qspi->DATAENTRY[0].U = (unsigned int)wdtdiscmd[i];
		/* wait until transfer is complete */
		while (!module_qspi->STATUS.B.TXF)
			;
		/* wait for receive is finished */
		while (!module_qspi->STATUS.B.RXF)
			;
		/* clear RX flag */
		module_qspi->FLAGSCLEAR.U = 1 << 10;
		/* read and discard value */
		(void)module_qspi->RXEXIT.U;
	}

	/* Reset the QSPI module */
	unlock_wdtcon();
	module_qspi->KRST1.B.RST = 1;
	module_qspi->KRST0.B.RST = 1;
	while (module_qspi->KRST0.B.RSTSTAT == 0)
		;
	module_qspi->KRSTCLR.B.CLR = 1;
	lock_wdtcon();
}

#endif  /* TRIBOARD_TC377TX_V1_X */
