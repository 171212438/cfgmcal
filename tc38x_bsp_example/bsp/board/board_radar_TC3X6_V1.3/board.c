/**************************************************************************************************
 * FILE: board.c
 * BOARD FAMILY:
 *    Radar Base board TC3X6 V1.3
 * UC SUPPORT:
 *    TC356, TC336_EXT
 * DESCRIPTION:
 *    Implementation of Board support routines.
 * HISTORY:
 *    V1  10.2022  MF
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2022 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

/* board implementation guard */
#if defined (RADAR_TC3X6_V1_3)

/* include common BSP header file */
#include "bsp.h"


/* ------------------------------------------------------------------------------------------------
 * Board LED port-pin setup
 *    The EVB has only 4 on-board LEDs, the rest is GPIOs
 * ----------------------------------------------------------------------------------------------*/

const BOARD_LED_S board_led[BOARD_NB_LEDS] =
{
	{02, 1},  /* LED_0 Blue */
	{02, 2},  /* LED_1 Red */
	{02, 8},  /* LED_2 Green */
};

#endif  /* RADAR_TC3X6_V1_3 */
