/**************************************************************************************************
 * FILE: board_spec.h
 * BOARD FAMILY:
 *    Radar board TC3X6 V1.3
 * UC SUPPORT:
 *    TC356, TC336_EXT
 * DESCRIPTION:
 *    Board specification file
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

#ifndef BOARD_SPEC_H
#define BOARD_SPEC_H


/* ------------------------------------------------------------------------------------------------
 * External crystal clock in [MHz]
 *    It can be overridden by the user, in case the board is populated with a different crystal
 * ----------------------------------------------------------------------------------------------*/

#ifndef BOARD_XTAL_CLOCK
#define BOARD_XTAL_CLOCK               20
#endif


/* ------------------------------------------------------------------------------------------------
 * OnBoard External WATCHDOG specification (SBC chip)
 *    Board QSPI module and GPIO mapping
 * ----------------------------------------------------------------------------------------------*/

/* External WATCHDOG not presence on the board */
#define BOARD_NO_EXTERNAL_WDG          1


#endif  /* BOARD_SPEC_H */
#endif  /* RADAR_TC3X6_V1_3 */
