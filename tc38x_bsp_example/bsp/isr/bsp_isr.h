

/**************************************************************************************************
 * FILE: bsp_isr.h
 * UC ARCHITECTURE:
 *    AURIX TC3x
 * UC FAMILY:
 *    TC39x
 * DESCRIPTION:
 *    BSP Interrupt interface file
 * FILE HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/
#ifndef BSP_ISR_H_
#define BSP_ISR_H_

/* 每个内核可用的"优先级槽(向量号)数量", 也就是RAM向量表BspIsrRamTable_CpuX[]的长度上限
 * TriCore的SRPN理论上可到255
 * 在bsp_isr_vector_table.c把BSP_ISR_DEFINE_HANDLER(cpu, i)的生成范围扩到新上限 */
#define BSP_ISR_MAX_VECTORS 32

/* 函数指针类型, 指向无参数、无返回值的函数 */
typedef void(*BspIsrHandler) (void);

/* Default Interrupt Handler for Undefined Interrupt Handlers (not mapped to any HW module) */
extern void isr_undef_handler(void);


#endif /* BSP_ISR_H_ */

