/**************************************************************************************************
 * FILE: board_spec.h
 * BOARD FAMILY:
 *    TriBoard TC3X6 V1.0
 * UC SUPPORT:
 *    TC366, TC336  (LFBGA-180 Package)
 * DESCRIPTION:
 *    Board specification file
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
#if defined (TRIBOARD_TC3X6_V1_0)

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

/* TLF presence on the board */
#define BOARD_TLF35584                 1

/* TLF configuration if TLF is present on the board */
#if (BOARD_TLF35584)

#define BOARD_TLF35584_CUT             2     /* 0=A, 1=B, 2=C, ... */


#define BOARD_TLF35584_QSPI_MODULE     2

#define BOARD_TLF35584_SLS_PORT        14    /* Slave Select Signal */
#define BOARD_TLF35584_SLS_INDEX       6     /* Pin index in port */
#define BOARD_TLF35584_SLS_PC          0x13  /* SLS Output function for I/O */

#define BOARD_TLF35584_SCLK_PORT       15    /* Serial Clock */
#define BOARD_TLF35584_SCLK_INDEX      3     /* Pin index in port */
#define BOARD_TLF35584_SCLK_PC         0x13  /* SCLK Output function for I/O */

#define BOARD_TLF35584_MTSR_PORT       15    /* Master Transmit Slave Receive*/
#define BOARD_TLF35584_MTSR_INDEX      6     /* Pin index in port */
#define BOARD_TLF35584_MTSR_PC         0x13  /* MTSR Output function for I/O */

#define BOARD_TLF35584_MRST_PORT       15    /* Master Receive Slave Transmit */
#define BOARD_TLF35584_MRST_INDEX      7     /* Pin index in port */

#endif

#endif  /* BOARD_SPEC_H */
#endif  /* TRIBOARD_TC3X6_V1_0 */
