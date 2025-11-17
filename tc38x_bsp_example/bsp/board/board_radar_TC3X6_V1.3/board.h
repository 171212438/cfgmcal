/**************************************************************************************************
 * FILE: board.h
 * BOARD FAMILY:
 *     Radar Board TC3X6 V1.3
 * UC SUPPORT:
 *     TC356, TC336_EXT
 * DESCRIPTION:
 *     Board support package specification and types.
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

#ifndef BOARD_H
#define BOARD_H


/* BSP Board low level specification header files */
#include "board_spec.h"
#include "board_types.h"

/* Board configuration */
extern const BOARD_LED_S board_led[BOARD_NB_LEDS];


#endif  /* BOARD_H */
#endif  /* RADAR_TC3X6_V1_3 */
