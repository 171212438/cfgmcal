/**************************************************************************************************
 * FILE: board.h
 * BOARD FAMILY:
 *    AppKit TC3x7 ADAS V1.0
 * UC SUPPORT:
 *    TC397  (LFBGA-292 ADAS package)
 * DESCRIPTION
 *    Board support package specification
 * HISTORY
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

/* board implementation guard */
#if defined (APPKIT_TC3X7_ADAS_V1_0)

#ifndef BOARD_H
#define BOARD_H


/* BSP Board low level specification header files */
#include "board_spec.h"
#include "board_types.h"

/* Board LED configuration */
extern const BOARD_LED_S board_led[BOARD_NB_LEDS];


#endif  /* BOARD_H */
#endif  /* APPKIT_TC3X7_ADAS_V1_0 */
