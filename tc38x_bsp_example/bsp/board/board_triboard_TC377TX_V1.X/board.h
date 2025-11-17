/**************************************************************************************************
 * FILE: board.h
 * BOARD FAMILY:
 *    TriBoard TC377TX V1.X
 * UC SUPPORT:
 *    TC377TX  (LFBGA-292 ADAS Package)
 * DESCRIPTION:
 *    Board support package specification and types.
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

#ifndef BOARD_H
#define BOARD_H


/* BSP Board low level specification header files */
#include "board_spec.h"
#include "board_types.h"

/* Board configuration */
extern const BOARD_LED_S board_led[BOARD_NB_LEDS];


#endif  /* BOARD_H */
#endif  /* TRIBOARD_TC377TX_V1_X */
