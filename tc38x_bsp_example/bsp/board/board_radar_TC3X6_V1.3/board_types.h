/**************************************************************************************************
 * FILE: board_types.h
 * BOARD FAMILY:
 *    Radar Board TC3X6 V1.3
 * UC SUPPORT:
 *    TC356, TC336_EXT
 * DESCRIPTION:
 *    Board specific types.
 * FILE HISTORY:
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

#ifndef BOARD_TYPES_H
#define BOARD_TYPES_H


/* PORT LEDs */
typedef enum
{
	BOARD_LED_0 = 0,
	BOARD_LED_1,
	BOARD_LED_2,
	BOARD_NB_LEDS
} BOARD_LED_e;

/* PORT OUTPUT control */
typedef enum
{
	BOARD_LED_SET_NO_CHANGE = 0,
	BOARD_LED_SET_OFF = 1,
	BOARD_LED_SET_ON = 2,
	BOARD_LED_SET_TOGGLE = 3,
} BOARD_LED_SET_e;


#endif  /* BOARD_TYPES_H */
#endif  /* RADAR_TC3X6_V1_3 */
