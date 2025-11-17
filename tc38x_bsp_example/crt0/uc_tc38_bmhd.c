/**************************************************************************************************
 * FILE: uc_tc38_bmhd.c
 * ARCHITECTURE:
 *    TC3xx
 * UC FAMILY:
 *    TC38x
 * DESCRIPTION:
 *    Boot Header Mode read by uC firmware after the reset at fixed location.
 *    Each uC has fixed structure how BMHD array looks like
 * FILE HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

/* uC derivative protection guard */
#ifdef __TC38XX__

#include <stdint.h>

/* Boot Mode Header Structure type */
typedef struct
{
	uint16_t bmi;				/* Boot Mode Index (BMI)*/
	uint16_t bmhdid;			/* Boot Mode Header ID (CODE) = B359H*/
	uint32_t stad;			    /* User Code start address*/
	uint32_t crc;				/* Check Result for the BMI Header (offset 000H - 007H)*/
	uint32_t crcInv;			/* Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	uint32_t reserved[120];	    /* Reserved area till the offset 0x1F0*/
	uint32_t confirmation;	    /* 32-bit CODE, (always same)*/
} IfxCpu_Bmhd;

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = OFF */
#pragma section ".bmhd_0_orig" a
const IfxCpu_Bmhd bmhd_0_orig=
{
	.bmi=				0x003E,		/* Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/* Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/* User Code start address*/
	.crc=			0xD86CBDAB,		/* Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x27934254,		/* Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/* 32-bit CODE, (always same)*/
};
#pragma section

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = OFF */
#pragma section ".bmhd_1_orig" a
const IfxCpu_Bmhd bmhd_1_orig=
{
	.bmi=				0x003E,		/* Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/* Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/* User Code start address*/
	.crc=			0xD86CBDAB,		/* Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x27934254,		/* Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/* 32-bit CODE, (always same)*/
};
#pragma section

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = ON */
#pragma section ".bmhd_2_orig" a
const IfxCpu_Bmhd bmhd_2_orig=
{
	.bmi=				0x003E,		/* Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/* Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/* User Code start address*/
	.crc=			0xD86CBDAB,		/* Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x27934254,		/* Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/* 32-bit CODE, (always same)*/
};
#pragma section

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = ON */
#pragma section ".bmhd_3_orig" a
const IfxCpu_Bmhd bmhd_3_orig=
{
	.bmi=				0x003E,		/* Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/* Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/* User Code start address*/
	.crc=			0xD86CBDAB,		/* Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x27934254,		/* Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/* 32-bit CODE, (always same)*/
};
#pragma section

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = OFF */
#pragma section ".bmhd_0_copy" a
const IfxCpu_Bmhd bmhd_0_copy=
{
	.bmi=				0x003E,		/* Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/* Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/* User Code start address*/
	.crc=			0xD86CBDAB,		/* Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x27934254,		/* Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/* 32-bit CODE, (always same)*/
};
#pragma section

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = OFF */
#pragma section ".bmhd_1_copy" a
const IfxCpu_Bmhd bmhd_1_copy=
{
	.bmi=				0x003E,		/* Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/* Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/* User Code start address*/
	.crc=			0xD86CBDAB,		/* Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x27934254,		/* Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/* 32-bit CODE, (always same)*/
};
#pragma section

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = ON */
#pragma section ".bmhd_2_copy" a
const IfxCpu_Bmhd bmhd_2_copy=
{
	.bmi=				0x003E,		/* Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/* Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/* User Code start address*/
	.crc=			0xD86CBDAB,		/* Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x27934254,		/* Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/* 32-bit CODE, (always same)*/
};
#pragma section

/* BMHD Setting
 * Core[0-3] = LOCKSTEP ON
 * Core[4-5] = LOCKSTEP OFF
 * LBIST = ON */
#pragma section ".bmhd_3_copy" a
const IfxCpu_Bmhd bmhd_3_copy=
{
	.bmi=				0x003E,		/* Boot Mode Index (BMI)*/
	.bmhdid=			0xB359,		/* Boot Mode Header ID (CODE) = B359H*/
	.stad=			0xA0000000,		/* User Code start address*/
	.crc=			0xD86CBDAB,		/* Check Result for the BMI Header (offset 000H - 007H)*/
	.crcInv=		0x27934254,		/* Inverted Check Result for the BMI Header (offset 000H - 007H)*/
	.confirmation=	0x43211234,		/* 32-bit CODE, (always same)*/
};
#pragma section

#endif  /* uC protection guard */
