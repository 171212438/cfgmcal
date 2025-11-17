

/**************************************************************************************************
 * FILE: bsp.h
 * ARCHITECTURE:
 *    TC3xx
 * FAMILY:
 *    TC3xx
 * DESCRIPTION:
 *    BSP interface file common for the TC3xx architecture.
 *    [Configuration macros]
 *    BSP_ISR_SUPPORT_DISABLED
 *       If defined, it disables generation of BSP interrupt vector
 *       tables and uc_intc_InitBIV APU routine to allow user
 *       to implement and register its interrupt system, like the case of RTOS
 * HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/
#ifndef BSP_H
#define BSP_H

/* BSP C API in C++ */
#ifdef __cplusplus
extern "C"
{
#endif

  /* Include of BSP sub-modules header files */
#include "typedefs.h"
#include "board/bsp_board.h"
#include "uc/bsp_uc.h"
#include "isr/bsp_isr.h"


  /* ================================================================================================
   * uC sub-module API
   * ==============================================================================================*/

  /* uC derivative specific API */
  extern void bsp_uc_InitClock(void);
  extern void bsp_uc_EnableExtClk0(UC_EXTCLK0_SEL_e clk, uint32_t div);
  extern void bsp_uc_EnableExtClk1(UC_EXTCLK1_SEL_e clk, uint32_t div);

  /* SCU WDT (Watchdog) */
  extern void bsp_uc_scu_wdt_DisableSafetyWatchdog();
  extern void bsp_uc_scu_wdt_DisableCpuWatchdog();

  /* SCU CCU (Clock) */
  extern void bsp_uc_scu_EnableXOSC(void);
  extern void bsp_uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_e sysclk);
  extern void bsp_uc_scu_SetCcuCon(volatile unsigned int * reg, uint32_t value, uint32_t update);
  extern void bsp_uc_scu_EnablePLL0(uint32_t pdiv, uint32_t ndiv, uint32_t k2);
  extern void bsp_uc_scu_EnablePLL1(uint32_t pdiv, uint32_t ndiv, uint32_t k2, uint32_t k3);
  extern void bsp_uc_ccu_SelectExtClk0(UC_EXTCLK0_SEL_e inclk, uint32_t div);
  extern void bsp_uc_ccu_SelectExtClk1(UC_EXTCLK1_SEL_e inclk, uint32_t div);

  /* CORE */
  extern void bsp_uc_core_EnableICache(void);
  extern void bsp_uc_core_EnableDCache(void);
  extern void bsp_uc_core_StartCore(uint32_t core_nb, uint32_t * reset_vector);
  extern void bsp_uc_core_StartAllCores(void);
  extern uint32_t bsp_uc_core_GetCurrentCore(void);
  extern uint32_t bsp_uc_core_GetCurrentInterruptPriority(void);

  /* PORT */
  extern void bsp_uc_port_EnableOutput(uint32_t port, uint32_t pinIndex, uint32_t source, UC_PORT_PL_LEVEL_e pl, UC_PORT_PD_SPEED_e pd);
  extern void bsp_uc_port_EnableInput(uint32_t port, uint32_t pinIndex, UC_PORT_INPUT_MODE_e mode);
  extern void bsp_uc_port_SetGPIO(uint32_t port, uint32_t pinIndex, UC_PORT_OUTPUT_e value);
  extern uint8_t bsp_uc_port_GetGPIO(uint32_t port, uint32_t pinIndex);

  /* FLASH */
  extern void bsp_uc_pflash_SetWs(uint32_t wspflash, uint32_t wsecpf);
  extern void bsp_uc_dflash_SetWs(uint32_t wsdflash, uint32_t wsecdf);

  /* STM (timer) */
  extern void bsp_uc_stm_ReloadChannel(uint32_t channel, uint32_t reload_value);
  extern void bsp_uc_stm_EnableChannelIsr(uint32_t channel);
  extern void bsp_uc_stm_ClearChannelIsrFlag(uint32_t channel);
  extern void bsp_uc_stm_Wait(uint32_t stm_src_clk, uint32_t time);
  extern uint32_t bsp_uc_stm_IsChannelIsrFlag(uint32_t channel);
  extern uint32_t bsp_uc_stm_GetChannelCurrentValue(uint32_t channel);

  /* INTC */
  extern void bsp_uc_intc_SetBIV(uint32_t table, uint32_t vss);
  extern void bsp_uc_intc_stm_SetSRC(uint32_t channel, uint32_t coreId, uint32_t prio);
  extern void bsp_uc_intc_EnableExternalInterrupts(void);


  /* ================================================================================================
   * INTERRUPT sub-module API
   * ==============================================================================================*/
#ifndef BSP_ISR_SUPPORT_DISABLED
  extern void bsp_isr_Init(void);
  extern void bsp_isr_RegisterHandler(uint32_t coreId, BspIsrHandler handler, uint32_t prio);
#endif


  /* ================================================================================================
   * BOARD sub-module API
   * ==============================================================================================*/

  /* Board WATCHDOG */
  extern void bsp_board_wdg_Disable(void);

  /* Board LEDs */
  extern void bsp_board_led_InitAll(BOARD_LED_SET_e set);
  extern void bsp_board_led_Init(BOARD_LED_e led, BOARD_LED_SET_e set);
  extern void bsp_board_led_Set(BOARD_LED_e led, BOARD_LED_SET_e set);

  /* BSP C API in C++ */
#ifdef __cplusplus
}


#endif

#endif /* BSP_H */

