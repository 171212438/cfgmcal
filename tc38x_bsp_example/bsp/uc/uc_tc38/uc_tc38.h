/******************************************************************************
 * FILE: uc_tc38.h
 * ARCHITECTURE
 *    AURIX TC3xx
 * FAMILY
 *    TC38x
 * DESCRIPTION
 *    Include of target uC header files used by BSP
 * HISTORY
 *    V1  05.2020  RO
 *    - Base reference example
 ******************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

/* uC derivative protection guard */
#ifdef __TC38XX__

#ifndef UC_TC38_H
#define UC_TC38_H

/* Check supported uC Steps, if not defined in the project setting by the user
 * the most recent uC step is selected */
#if !defined(TC38XA)
#define TC38XA
#endif

/* Import of uC Specific header files */
#include "uc_tc38_spec.h"

/* DECLARATION OF FAMILY SPECIFIC API IMPLEMENTATIONS
 * Definition of specific MACRO disables function in generic uc_bsp.c and replaces it
 * with uc_<derivative> implementation. This allows to use different header files etc.
 * that is not possible in a WEAK function system.
 * MACRO:  #define <uc_fce>_UC_SPECIFIC */
#if defined (TC38XA)

#endif

/* Include of peripheral register headers files */
#if defined (TC38XA)

#include "TC38x_RegCHeaders/_Reg/Ifx_reg.h"

#include "TC38x_RegCHeaders/_Reg/IfxAsclin_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxCan_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxCbs_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxCcu6_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxConverter_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxCpu_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxDam_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxDma_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxDmu_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxDom_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxEdsadc_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxEray_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxEvadc_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxFce_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxFsi_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxGeth_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxGpt12_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxGtm_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxHsct_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxHssl_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxI2c_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxInt_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxIom_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxLmu_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxMinimcds_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxMsc_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxMtu_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxPfi_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxPms_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxPmu_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxPort_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxPsi5_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxPsi5s_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxQspi_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxSbcu_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxScu_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxSent_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxSmu_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxSrc_bf.h"
#include "TC38x_RegCHeaders/_Reg/IfxStm_bf.h"

#endif

#endif  /* UC_TC38_H */
#endif  /* uC protection guard */
