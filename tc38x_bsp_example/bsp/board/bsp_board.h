

/**************************************************************************************************
 * FILE: bsp_board.h
 * BOARD FAMILY:
 *    TC3xx EVB Boards
 * DESCRIPTION:
 *    Selection of the EVB board definition files.
 * FILE HISTORY:
 *    V1   05.2020  RO
 * 	  V1.1 10.2022  MF Add radar base board support
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2022 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 ************************************************************************************************/
#ifndef BSP_BOARD_H
#define BSP_BOARD_H

/* ------------------------------------------------------------------------------------------------
 * LED physical port-pin structure
 * ----------------------------------------------------------------------------------------------*/
typedef struct board_led_s {
  uint32_t port;
  uint32_t index;
} BOARD_LED_S;


/* ------------------------------------------------------------------------------------------------
 * Specific Board Interface Header file selection
 *    Controlled by the symbol defined either in the Project Settings or on the build command line
 * ----------------------------------------------------------------------------------------------*/

/* for the check of the board definition */
#define BSP_BOARD_SUPPORT 0

#ifdef APPKIT_TC3X7_V1_0
#include "board_appkit_TC3X7_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef UNT_ZCU_TC387QP_V1_0
#include "board_appkit_TC3X7_V2.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef APPKIT_TC3X7_ADAS_V1_0
#include "board_appkit_TC3X7_ADAS_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef APPKIT_TC3X7_ADAS_V2_0
#include "board_appkit_TC3X7_ADAS_V2.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X2_V1_0
#include "board_triboard_TC3X2_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X3_V1_0
#include "board_triboard_TC3X3_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X4L_V1_0
#include "board_triboard_TC3X4L_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X4T_V1_0
#include "board_triboard_TC3X4T_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X5_V1_X
#include "board_triboard_TC3X5_V1.X/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X6_V1_0
#include "board_triboard_TC3X6_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef RADAR_TC3X6_V1_3
#include "board_radar_TC3X6_V1.3/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X6_ADAS_V1_0
#include "board_triboard_TC3X6_ADAS_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X7_V1_0
#include "board_triboard_TC3X7_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X7_V2_0
#include "board_triboard_TC3X7_V2.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X7_ADAS_V1_0
#include "board_triboard_TC3X7_ADAS_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X7_ADAS_V2_0
#include "board_triboard_TC3X7_ADAS_V2.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC377TX_V1_X
#include "board_triboard_TC377TX_V1.X/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X9_V1_0
#include "board_triboard_TC3X9_V1.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

#ifdef TRIBOARD_TC3X9_V2_0
#include "board_triboard_TC3X9_V2.0/board.h"
#undef BSP_BOARD_SUPPORT
#endif

/* check of existing board definition that the BSP relies on */
#ifdef BSP_BOARD_SUPPORT
#error ... Board Type is not defined in the project setting.
#endif


#endif /* BSP_BOARD_H */

