

/**************************************************************************************************
 * FILE: bsp_isr.c
 * UC ARCHITECTURE:
 *    AURIX TC3xx
 * UC FAMILY:
 *    TC3xx
 * DESCRIPTION:
 *    Initialization of the Core's interrupt system (BIV register).
 *    Each core has its own interrupt vector table provided by this BSP.
 * FILE HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 ************************************************************************************************/

/* include BSP API interface */
#include "bsp.h"

/* implement only when BSP provides HW Interrupt support */
#ifndef BSP_ISR_SUPPORT_DISABLED

/* 兜底的"未注册中断处理函数". 谁没被正确注册/没写ISR, 就会掉进这里*/
void isr_undef_handler(void)
{
  __asm("debug");  /* 触发断点. 连着仿真器/调试器时, CPU会停在这行, 当场抓现行(看PC、寄存器、堆栈、ICR. CCPN等); */

  for(; ; ) {  /* 没有接调试器, 则进入死循环, 直到看门狗复位 */
    ;
  }
}


/* ================================================================================================
 * ISR Vector table for each uC core
 * ==============================================================================================*/
/* BSP_ISR_VECTOR_TABLE_CPU0不是C变量, 是在bsp_isr_vector_table.c里由宏BSP_ISR_VECTOR_TABLE(0)生成的
 * 汇编标签(symbol/label), 被放进名为.bsp_isr_vector_table_cpu0的可执行段中, 并且2KB对齐 */
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU0;  /* 只是为了在C里"声明这个符号存在", 以便取它的地址作为BIV的基址 */

#if (UC_NB_CORES > 1)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU1;

#if (UC_NB_CORES > 2)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU2;

#if (UC_NB_CORES > 3)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU3;

#if (UC_NB_CORES > 4)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU4;

#if (UC_NB_CORES > 5)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU5;
#endif  // CORE 5

#endif  // CORE 4

#endif  // CORE 3

#endif  // CORE 2

#endif  // CORE 1

/* ================================================================================================
 * BSP ISR Handlers table for each core
 * in case the RAM version is configured
 * ==============================================================================================*/
#pragma section ".BspIsrRamTable_Cpu0" aw 4  /* 切换当前输出节到名为.BspIsrRamTable_Cpu0的段; 参与映像/装载且可写; 确保后续对象至少4字节对齐, 便于32位原子访问 */
BspIsrHandler BspIsrRamTable_Cpu0[BSP_ISR_MAX_VECTORS];  /* 定义Core0的32×4B = 128B的RAM向量表, TriCore是32位地址空间, 默认函数指针就是32位 = 4字节 */
#pragma section  /* 恢复默认节 */

#if (UC_NB_CORES > 1)
#pragma section ".BspIsrRamTable_Cpu1" aw 4
BspIsrHandler BspIsrRamTable_Cpu1[BSP_ISR_MAX_VECTORS];
#pragma section

#if (UC_NB_CORES > 2)
#pragma section ".BspIsrRamTable_Cpu2" aw 4
BspIsrHandler BspIsrRamTable_Cpu2[BSP_ISR_MAX_VECTORS];
#pragma section

#if (UC_NB_CORES > 3)
#pragma section ".BspIsrRamTable_Cpu3" aw 4
BspIsrHandler BspIsrRamTable_Cpu3[BSP_ISR_MAX_VECTORS];
#pragma section

#if (UC_NB_CORES > 4)
#pragma section ".BspIsrRamTable_Cpu4" aw 4
BspIsrHandler BspIsrRamTable_Cpu4[BSP_ISR_MAX_VECTORS];
#pragma section

#if (UC_NB_CORES > 5)
#pragma section ".BspIsrRamTable_Cpu5" aw 4
BspIsrHandler BspIsrRamTable_Cpu5[BSP_ISR_MAX_VECTORS];
#pragma section
#endif // CORE 5

#endif // CORE 4

#endif // CORE 3

#endif // CORE 2

#endif // CORE 1

/* ================================================================================================
 * ISR API
 * ==============================================================================================*/

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: isr_Init
 *     It sets ISR Vector Table in the corresponding Core's BIV register.
 *     It initializes RAM ISR Table with all priority vectors pointing
 *     to Undefined Handler routine.
 *     如果是多核中断, 每个个初始化时都要调用
 * ------------------------------------------------------------------------------------------------ */
void bsp_isr_Init(void)
{
  uint32_t isr_vector_table;  /* holds the vector table for the requested Core */
  uint32_t coreId = bsp_uc_core_GetCurrentCore();  /* Get current Core logical number */

  switch(coreId)
  {
    case UC_CORE_MAIN_0:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU0;
      break;

#if (UC_NB_CORES > 1)
    case UC_CORE_MAIN_1:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU1;
      break;

#if (UC_NB_CORES > 2)
    case UC_CORE_MAIN_2:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU2;
      break;

#if (UC_NB_CORES > 3)
    case UC_CORE_MAIN_3:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU3;
      break;

#if (UC_NB_CORES > 4)
    case UC_CORE_MAIN_4:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU4;
      break;

#if (UC_NB_CORES > 5)
    case UC_CORE_MAIN_5:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU5;
      break;

#endif // CORE 5

#endif // CORE 4

#endif // CORE 3

#endif // CORE 2

#endif // CORE 1

    default:
      return; // nothing to do in case of wrong coreId parameter
  }

  bsp_uc_intc_SetBIV(isr_vector_table, 0);  /* (解锁保护) → 把BIV指到向量表基址, 并设置入口步长VSS → (上锁保护) → 指令同步 */

  uint32_t i;  /* Initialize RAM Handler table entries */
  for(i = 0; i < BSP_ISR_MAX_VECTORS; i++) {
    bsp_isr_RegisterHandler(coreId, (BspIsrHandler) isr_undef_handler, i);  /* 每个业务ISR默认为isr_undef_handler */
  }
}


/* 把指定核心coreId的RAM向量表中编号为prio的入口, 写成传入的ISR函数指针handler. 超范围或非法核心则直接返回, 不做任何事 */
void bsp_isr_RegisterHandler(uint32_t coreId, BspIsrHandler handler, uint32_t prio)
{
  BspIsrHandler * handler_table;  /* holds RAM Handler table for requested core */

  switch(coreId)  /* Select right vector table */
  {
    case UC_CORE_MAIN_0:
      handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu0;
      break;

#if (UC_NB_CORES > 1)
    case UC_CORE_MAIN_1:
      handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu1;
      break;

#if (UC_NB_CORES > 2)
    case UC_CORE_MAIN_2:
      handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu2;
      break;

#if (UC_NB_CORES > 3)
    case UC_CORE_MAIN_3:
      handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu3;
      break;

#if (UC_NB_CORES > 4)
    case UC_CORE_MAIN_4:
      handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu4;
      break;

#if (UC_NB_CORES > 5)
    case UC_CORE_MAIN_5:
      handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu5;
      break;

#endif  // CORE 5

#endif  // CORE 4

#endif  // CORE 3

#endif  // CORE 2

#endif  // CORE 1

    default:
      return; // nothing to do in case of wrong coreId parameter
  }

  /* Update table entry, only for supported priorities, otherwise nothing happens */
  if(prio < BSP_ISR_MAX_VECTORS) {
    handler_table[prio] = handler;
  }
}


#endif  /* #ifndef BSP_ISR_SUPPORT_DISABLED */

