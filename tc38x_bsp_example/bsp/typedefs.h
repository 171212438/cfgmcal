

/**************************************************************************************************
 * FILE: typedefs.h
 * DESCRIPTION:
 *    Definition of necessary types extensions mainly required by uC register header files.
 * HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/
#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <stdint.h>

/* volatile types */
typedef volatile signed char vint8_t;
typedef volatile unsigned char vuint8_t;

typedef volatile signed short vint16_t;
typedef volatile unsigned short vuint16_t;

typedef volatile signed int vint32_t;
typedef volatile unsigned int vuint32_t;

#endif

