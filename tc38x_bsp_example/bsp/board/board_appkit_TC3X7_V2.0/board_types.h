/**************************************************************************************************
 * FILE: board_types.h
 * BOARD FAMILY:
 *    AppKit TC3X7 V2.0
 * UC SUPPORT:
 *    TC397, TC387  (LFBA-292 Package)
 * DESCRIPTION:
 *    Board specific types.
 * FILE HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

/* board implementation guard */
#if defined (UNT_ZCU_TC387QP_V1_0)

#ifndef BOARD_TYPES_H
#define BOARD_TYPES_H


/* PORT LEDs */
typedef enum
{
	BOARD_LED_0 = 0,
	BOARD_LED_1,
	BOARD_LED_2,
	BOARD_LED_3,
	BOARD_LED_4,
	BOARD_LED_5,
	BOARD_LED_6,
	BOARD_LED_7,
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
#endif  /* UNT_ZCU_TC387QP_V1_0 */
