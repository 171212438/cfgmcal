

/**************************************************************************************************
 * FILE: bsp_uc.c
 * UC ARCHITECTURE:
 *     TC3xx
 * UC FAMILY:
 *     TC3xx
 * DESCRIPTION:
 *     Low Level routines for TC3x architecture.
 *     They represent family generic implementation, that might be replaced
 *     by a family particular version in bsp_uc_<family>.c file
 * FILE HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

/* Include its own API interface and types */
#include "bsp.h"

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_scu_wdt_DisableSafetyWatchdog()
 *    Disables Safety WATCHDOG module (one in TC3x uC)
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_SCU_WDT_DISABLESAFETYWATCHDOG_UC_SPECIFIC
/* 暂时打开Safety ENDINIT锁 → 把Safety WDT的"禁用请求DR"置1 → 再把锁关回去: 把安全看门狗停掉(进入Disabled Mode); */
void bsp_uc_scu_wdt_DisableSafetyWatchdog(void)
{
	/* TriCore/AURIX有两个"看门狗 + ENDINIT"体系:
   *  普通WDT + ENDINIT(保护BIV/BTV、各模块CLC等), 对应unlock_wdtcon();
   *  Safety WDT(WDTS) + SAFETY ENDINIT(保护SCU/CCU/PLL、PMS、电源、安全相关寄存器), 对应unlock_safety_wdtcon(); */
  unlock_safety_wdtcon();  /* 开"Safety ENDINIT锁" */
  SCU_WDTS_CON1.B.DR = 1;
  lock_safety_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_scu_wdt_DisableCpuWatchdog()
 *    Disables Core WATCHDOG module.
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_SCU_WDT_DISABLECPUWATCHDOG_UC_SPECIFIC
/* 按当前核的ID, 关掉这个核自己的CPU Watchdog */
void bsp_uc_scu_wdt_DisableCpuWatchdog()
{
  uint32_t coreId = bsp_uc_core_GetCurrentCore();

  unlock_wdtcon();
  MODULE_SCU.WDTCPU[coreId].CON1.B.DR = 1;
  lock_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_scu_EnableXOSC()
 *    It enables an external oscillator.
 *    To wait for stabilization of the XOSC it shall be enough to use
 *    OSC_WDG PLLLV and PLLHV bits. For that XOSC must be selected as source for PLL
 *       1. Select XTAL as input to PLL
 *       2. Set right foscref frequency through OSCVAL
 *       3. Restart OSC WDG in PLL
 *       4. Wait for PLLLV and PLLHV status bits from OSC WDG
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_SCU_ENABLEXOSC_UC_SPECIFIC
void bsp_uc_scu_EnableXOSC(void)
{
  unlock_safety_wdtcon();

  /* 如果现在系统还不是跑在备份时钟上, 先切到备份时钟 */
  if(SCU_CCUCON0.B.CLKSEL != UC_CCU_CON0_CLKSEL_BACKUP) {
    while(SCU_CCUCON0.B.LCK != 0U) {  /* request the change */
      ;
    }
    SCU_CCUCON0.B.CLKSEL = UC_CCU_CON0_CLKSEL_BACKUP;
    SCU_CCUCON0.B.UP = 1;
    while(SCU_CCUCON0.B.LCK != 0U) {  /* wait for the change to complete */
      ;
    }
  }

  SCU_OSCCON.B.MODE = 0U;  /* 用"外部晶振模式"启用XOSC */
  SCU_SYSPLLCON0.B.INSEL = UC_CCU_CON1_INSEL_XOSC;  /* 选择PLL的输入时钟源 */

  /* OSCVAL defines the divider value that generates the reference clock 'fOSCREF'
   * that is supervised by the oscillator WDG.
   * fOSCREF = fOSC / (OSCVAL + 1) = ~ 2.5Mhz  => OSCVAL = (fOSC / 2.5Mhz) - 1 */
  SCU_OSCCON.B.OSCVAL = ((uint32_t) (BOARD_XTAL_CLOCK * 1000000) / 2500000) - 1;
  SCU_OSCCON.B.OSCRES = 1U;  /* 重启振荡器看门狗 */

  lock_safety_wdtcon();  /* set safety ENDINIT bit */

  while((SCU_OSCCON.B.PLLLV == 0) || (SCU_OSCCON.B.PLLHV == 0)) {  /* 在安全窗口外, 慢慢等XOSC稳定, 数据手册说XOSC稳定可能要最多5ms左右 */
    ;
  }
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_scu_EnablePLL0()
 *    It enables SYSTEM-PLL with required parameters.
 * Input
 *    pdiv       : Input divider
 *    ndiv       : VCO feedback divider
 *    k2         : end output divider
 * Note
 *    It waits for PLL stabilization in infinitive loop!
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_SCU_ENABLEPLL0_UC_SPECIFIC
/* 在PLL里, 输入时钟: fIN(一般是XOSC); 前分频P: PDIV = pdiv; 反馈倍频N: NDIV = ndiv; 输出分频K2: K2DIV = k2 */
void bsp_uc_scu_EnablePLL0(uint32_t pdiv, uint32_t ndiv, uint32_t k2)
{
  unlock_safety_wdtcon();

  /* 先强制把PLL关机到"省电/重置"状态 */
  if(0 == SCU_SYSPLLSTAT.B.PWDSTAT) {
    SCU_SYSPLLCON0.B.PLLPWD = 0;
    while(0 == SCU_SYSPLLSTAT.B.PWDSTAT) {
      ;
    }
  }

  /* 在关机状态下配置NDIV/PDIV, 并让PLL上电 */
  SCU_SYSPLLCON0.B.NDIV = ndiv;
  SCU_SYSPLLCON0.B.PDIV = pdiv;
  SCU_SYSPLLCON0.B.PLLPWD = 1;

  /* 等PLL电源/电路真正起来 */
  while(SCU_SYSPLLSTAT.B.PWDSTAT == 1) {
    ;
  }

  /* 重启锁定机制, 等LOCK = 1 */
  SCU_SYSPLLCON0.B.RESLD = 1;
  while(0 == SCU_SYSPLLSTAT.B.LOCK) {
    ;
  }

  /* 等K2分频器准备好, 再写K2DIV */
  while(SCU_SYSPLLSTAT.B.K2RDY == 0U) {
    ;
  }
  SCU_SYSPLLCON1.B.K2DIV = k2;  /* 这里没做"逐级渐升"的K2 stepping(那种推荐的平滑ramp-up没实现), 而是一次性把K2设到目标值 */

  lock_safety_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_scu_EnablePLL1()
 *    It enables PERIPHERAL-PLL with required parameters.
 * Input
 *    pdiv      : Input divider
 *    ndiv      : VCO feedback divider
 *    k2        : fPLL1 output divider
 *    k3        : fPLL2 output divider
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_SCU_ENABLEPLL1_UC_SPECIFIC
/* 把外设用的PLL1这套多路时钟: 关机→重配→开机→等稳定 */
void bsp_uc_scu_EnablePLL1(uint32_t pdiv, uint32_t ndiv, uint32_t k2, uint32_t k3)
{
  unlock_safety_wdtcon();  /* reset safety ENDINIT bit */

  /* 1) 如果外设PLL当前不是省电/关机状态, 先强制关机并等它真关掉 */
  if(0 == SCU_PERPLLSTAT.B.PWDSTAT) {
    SCU_PERPLLCON0.B.PLLPWD = 0;
    while(0 == SCU_PERPLLSTAT.B.PWDSTAT) {
      ;
    }
  }

  /* 2) 在关机状态下配置倍频/分频参数, 并设置DIVBY, 最后点火上电 */
  SCU_PERPLLCON0.B.NDIV = ndiv;
  SCU_PERPLLCON0.B.PDIV = pdiv;
  SCU_PERPLLCON0.B.DIVBY = UC_PLL1_DIBBY;
  SCU_PERPLLCON0.B.PLLPWD = 1;

  /* 3) 等PLL1上电完成(退出Powerdown) */
  while(SCU_PERPLLSTAT.B.PWDSTAT == 1) {
    ;
  }

  /* 4) 重启锁定检测逻辑, 然后等LOCK=1(频率锁定) */
  SCU_PERPLLCON0.B.RESLD = 1;
  while(0 == SCU_PERPLLSTAT.B.LOCK) {
    ;
  }

  /* 5) 配两路输出分频: K2→fPLL1, K3→fPLL2(没有做渐进ramp) */
  while(SCU_PERPLLSTAT.B.K2RDY == 0U) {
    ;
  }
  SCU_PERPLLCON1.B.K2DIV = k2;
  while(SCU_PERPLLSTAT.B.K3RDY == 0U) {
    ;
  }
  SCU_PERPLLCON1.B.K3DIV = k3;

  lock_safety_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_ccu_SetSystemClock()
 *    Selects requested clock input for SYSTEM clock. This operation shall be executed when
 *    the requested input clock is stable and System clock dividers are set properly.
 * Input
 *    sysclk : clock input from fixed set in uc_types.h
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_SCU_SWITCHSYSTEMCLOCK_UC_SPECIFIC
/* 把系统主时钟FSYSTEM在BACKUP <-> PLL0之间切换的总控函数 */
void bsp_uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_e sysclk)
{
  switch(sysclk)  /* 根据传进来的枚举决定切到哪路时钟 */
  {
    case UC_CCU_SYSTEM_CLOCK_BACKUP:
      /* 1)目标是BACKUP, 如果本来就已经是BACKUP, 就什么都不干 */
      if(SCU_CCUCON0.B.CLKSEL!= UC_CCU_CON0_CLKSEL_BACKUP) {
        unlock_safety_wdtcon();

        /* 2)等CCUCON0不忙(LCK=0), 避免在上一次更新没完时又写 */
        while(SCU_CCUCON0.B.LCK!= 0U) {  /* request the change */
          ;
        }

        /* 3)把系统时钟源CLKSEL切成BACKUP(内部备份时钟) */
        SCU_CCUCON0.B.CLKSEL = UC_CCU_CON0_CLKSEL_BACKUP;

        /* 4)写UP=1, 告诉CCU: 刚改的CLKSEL真正生效 */
        SCU_CCUCON0.B.UP = 1;

        /* 5)再次等LCK=0, 确认切钟动作做完了 */
        while(SCU_CCUCON0.B.LCK!= 0U) {
          ;
        }
        lock_safety_wdtcon();
      }
      break;
    case UC_CCU_SYSTEM_CLOCK_PLL0:
      /* 1)目标是PLL0, 但在切到PLL之前, 要先把系统时钟切回BACKUP做缓冲, 如果当前不是BACKUP, 就递归调用把系统先切到BACKUP */
      if(SCU_CCUCON0.B.CLKSEL!= UC_CCU_CON0_CLKSEL_BACKUP) {
        bsp_uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_BACKUP);
      }

      /* 2)检查XOSC(外部晶振)是否已经稳定: PLLLV/PLLHV任意一个为0就说明XOSC还没振好, 于是调用EnableXOSC */
      if((SCU_OSCCON.B.PLLLV == 0) || (SCU_OSCCON.B.PLLHV == 0)) {
        bsp_uc_scu_EnableXOSC();
      }

      /* 3)检查System PLL0有没有锁上: 如果LOCK=0说明还没锁, 调用EnablePLL0用UC_PLL0_* 参数点亮PLL0 */
      if(SCU_SYSPLLSTAT.B.LOCK == 0) {
        bsp_uc_scu_EnablePLL0(UC_PLL0_PDIV, UC_PLL0_NDIV, UC_PLL0_K2);
      }

      /* 4)同样检查Peripheral PLL(PERPLL1)有没有锁上, 没锁就调用EnablePLL1点亮, 顺便配置fPLL1/fPLL2(给QSPI/GETH/CANH用) */
      if(SCU_PERPLLSTAT.B.LOCK == 0) {
        bsp_uc_scu_EnablePLL1(UC_PLL1_PDIV, UC_PLL1_NDIV, UC_PLL1_K2, UC_PLL1_K3);
      }

      /* 5)到这里: XOSC已振稳 + SYSPLL/PERPLL都锁好了, 现在才真正把系统主时钟从BACKUP切到PLL0输出 */
      unlock_safety_wdtcon();
      while(SCU_CCUCON0.B.LCK!= 0U) {  /* 5.1等CCUCON0不忙(LCK=0) */
        ;
      }
      SCU_CCUCON0.B.CLKSEL = UC_CCU_CON0_CLKSEL_PLL0;  /* 5.2把CLKSEL切到PLL0 */
      SCU_CCUCON0.B.UP = 1;  /* 5.3写UP=1, 让切钟请求生效 */
      while(SCU_CCUCON0.B.LCK!= 0U) {  /* 5.4再等LCK=0, 确认已经完成从BACKUP→PLL0的切换 */
        ;
      }
      lock_safety_wdtcon();
      break;
    default:
      break;
  }
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_scu_SetCcuCon()
 *    Setting of System clock dividers. Their values depend on System clock frequency.
 *    Their default (reset) values are set to safe values.
 *    No need to change it when running on max clock.
 * Input
 *    reg     : pointer to right SC_CCU_CONx register
 *    value   : content to write to the register
 *    update  : request to update CCU module with the content of the CONx
 *              register(s)
 * Note
 *    It shall work on all CONx registers, even those for CPU not having
 *    LCK bit (shall be read as 0 - meaning ready for update)
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_SCU_SETCCUCON_UC_SPECIFIC
void bsp_uc_scu_SetCcuCon(volatile unsigned int * reg, uint32_t value, uint32_t update)
{
  volatile uint32_t * pcon = (volatile uint32_t *) reg;

  unlock_safety_wdtcon();  /* CCUCONx是Safety ENDINIT保护寄存器 */
  while((*pcon & (uint32_t) 0x80000000) != 0) {  /* LCK = 1表示: 这个CCUCON寄存器当前被"锁着", 正在更新/忙不能写 */
    ;
  }
  if(update) {  /* request CCU module to take content of CON register(s) or prepare only the value in CON */
    value = value | (uint32_t) 0x40000000;  /* 设置各个DIV/CLKSEL, 且本次写操作顺带触发一次更新 */
  } else {
    value = value & ~(uint32_t) 0x40000000;  /* 只把CCUCON的内容预写好, 不拉UP */
  }
  *pcon = value;  /* write the SCU_CCU_CONx register */
  lock_safety_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_ccu_SelectExtClk0()
 *    Sets requested input clock to EXTCLK0 and enable.
 * Input
 *    inclk : clock selection from fixed set
 *    div   : divider for fout, the others do not have it
 *            div is divided by 2 to get user expected clock
 *            fout = ((fspb / n) / 2)
 *            div is expected to be even
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_CCU_SELECTEXTCLK0_UC_SPECIFIC
void bsp_uc_ccu_SelectExtClk0(UC_EXTCLK0_SEL_e inclk, uint32_t div)
{
  unlock_safety_wdtcon();
  SCU_FDR.B.STEP = 1024 - ((div >> 1) & 0x3ff);
  SCU_FDR.B.DM = 1;
  SCU_EXTCON.B.SEL0 = inclk;
  SCU_EXTCON.B.EN0 = 1;
  lock_safety_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_ccu_SelectExtClk1()
 *    Sets requested input clock to EXTCLK0 and enable.
 * Input
 *    inclk : clock selection from fixed set
 *    div   : divider only for Fout frequency (Fspb / (DIV + 1)),
 *            others will ignore it
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_CCU_SELECTEXTCLK1_UC_SPECIFIC
void bsp_uc_ccu_SelectExtClk1(UC_EXTCLK1_SEL_e inclk, uint32_t div)
{
  unlock_safety_wdtcon();
  SCU_EXTCON.B.SEL1 = inclk;
  SCU_EXTCON.B.DIV1 = div - 1;
  SCU_EXTCON.B.EN1 = 1;
  lock_safety_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_core_StartCore()
 *    Starting requested core from given reset_vector
 * Input
 *    coreId       : [0..n] logical core index
 *    reset_vector : pointer to function address where the Core will start
 *                   after the reset
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_CORE_STARTCORE_UC_SPECIFIC
void bsp_uc_core_StartCore(uint32_t coreId, uint32_t * reset_vector)
{
  volatile Ifx_CPU_PC * corePC = &CPU0_PC;
  volatile Ifx_CPU_SYSCON * coreSYSCON = &CPU0_SYSCON;

  if(coreId == bsp_uc_core_GetCurrentCore()) {  /* do not switch current core */
    return;
  }

  if(coreId >= UC_NB_CORES) {  /* check coreId limit */
    return;
  }

  if(coreId == 5) {  /* Adjust logical CoreId to Physical index */
    coreId++;
  }

  corePC = (Ifx_CPU_PC *) ((uint32_t) corePC + ((uint32_t) 0x20000 * coreId));  /* adjust register pointers */
  coreSYSCON = (Ifx_CPU_SYSCON *) ((uint32_t) coreSYSCON + ((uint32_t) 0x20000 * coreId));

  if(coreSYSCON->B.BHALT == 0) {  /* do not start already active core */
    return;
  }

  corePC->U = (unsigned int) reset_vector;  /* Set the start address */  /* update registers */
  coreSYSCON->B.BHALT = 0;  /* Reset HALT sate */
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_core_StartAllCores()
 *    Starts all inactive cores on multi-core uC derivative.
 *    Active ones are left untouched.
 *    All inactive cores will start at _crt0_reset entry point.
 *    All cores share the same crt0 startup routine.
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_CORE_STARTALLCORES_UC_SPECIFIC
extern void _crt0_reset(void);
void bsp_uc_core_StartAllCores(void)
{
  uint32_t i;

  for(i = 0; i < UC_NB_CORES; i++) {
    bsp_uc_core_StartCore(i, (uint32_t *) &_crt0_reset);
  }
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_port_EnableInput()
 *    Configures requested pin to Input mode
 * Input
 *    port     : port number
 *    pinIndex : pin index in the selected port (from 0 till 15)
 *    mode     : selection of mode
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_PORT_ENABLEINPUT_UC_SPECIFIC
void bsp_uc_port_EnableInput(uint32_t port, uint32_t pinIndex, UC_PORT_INPUT_MODE_e mode)
{
  uint32_t bitOffset = (8 * (pinIndex % 4));

  __builtin_tricore_ldmst(UC_PORT_IOCR_REG_BASE(port, pinIndex), ((mode << 3) << bitOffset), (0xff << bitOffset));
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_port_EnableOutput()
 *    Configures requested pin to output mode with desired characteristics.
 * Input
 *    port     : port number
 *    pinIndex : pin index in the selected port
 *    source   : Port Control for Port's Pin (Register Pn_IOCRy.PCx).
 *               Selected Output Function
 *               (see Target user manual - PCx Coding table)
 *    pl       : Pad Level Selection, Pad Driver Mode (Register Pn_PDRy.PLx),
 *               (see Target user manual -  Pad Level Selection table)
 *    pd       : Pad Driver Mode Selection, Pad Driver Mode (Register Pn_PDRy.PDx),
 *               (see Target user manual - Pad Driver Mode and Hysteresis
 *               Selection for Pads table)
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_PORT_ENABLEOUTPUT_UC_SPECIFIC
void bsp_uc_port_EnableOutput(uint32_t port, uint32_t pinIndex, uint32_t source, UC_PORT_PL_LEVEL_e pl, UC_PORT_PD_SPEED_e pd)
{
  uint32_t bitOffset = (8 * (pinIndex % 4));  /* Set Pin control (IOCR register) to output using required signal source */
  __builtin_tricore_ldmst(UC_PORT_IOCR_REG_BASE(port, pinIndex), (((0x10 | source) << 3) << bitOffset), (0xff << bitOffset));

  unlock_wdtcon();  /* Set Pin output strength and levels in PDR register */
  bitOffset = (4 * (pinIndex % 8));
  __builtin_tricore_ldmst(UC_PORT_PDR_REG_BASE(port, pinIndex), (((pl << 3) | pd) << bitOffset), (0xf << bitOffset));
  lock_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_port_SetGPIO()
 *    Sets output value to the GPIO pin
 * Input
 *    port     : port number
 *    pinIndex : pin index in the selected port
 *    value    : one bit value clear-2, set-1, toggle-3
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_PORT_SETGPIO_UC_SPECIFIC
void bsp_uc_port_SetGPIO(uint32_t port, uint32_t pinIndex, UC_PORT_OUTPUT_e value)
{
  uint32_t pcl = (((uint32_t) value) & 0x2) << (pinIndex + 15);
  uint32_t ps = ((uint32_t) value & 0x1) << pinIndex;
  UC_PORT_OMR_REG(port, pinIndex) = (pcl | ps);
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_port_GetGPIO()
 *    Reads value from the GPIO pin
 * Input
 *    port     : port number
 *    pinIndex : pin index in the selected port
 * Output
 *    return   : bit value (0/1)
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_PORT_GETGPIO_UC_SPECIFIC
uint8_t bsp_uc_port_GetGPIO(uint32_t port, uint32_t pinIndex)
{
  uint32_t input = ((UC_PORT_IN_REG(port, pinIndex) | (1 << pinIndex))) >> pinIndex;
  return (uint8_t) input;
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_pflash_SetWs()
 *    Wait States Configuration of PLASH controller Port0.
 * Input
 *    wspflash    : Wait States for read access to PFlash
 *    wsecpf      : Wait States for Error Correction of PFlash
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_PFLASH_SETWS_UC_SPECIFIC
void bsp_uc_pflash_SetWs(uint32_t wspflash, uint32_t wsecpf)
{  /* 设置PFLASH读的总时间 = RFLASH对应的"本体时间" + RECC对应的"ECC时间" */
  unlock_wdtcon();
  DMU_HF_PWAIT.B.RFLASH = (uint32_t) wspflash;
  DMU_HF_PWAIT.B.RECC = (uint32_t) wsecpf;
  lock_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_dflash_SetWs()
 *    Wait States Configuration of PLASH controller Port0.
 * Input
 *    wsdflash    : Wait States for read access to DFlash
 *    wsecdf      : Wait States for Error Correction of DFlash
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_DFLASH_SETWS_UC_SPECIFIC
void bsp_uc_dflash_SetWs(uint32_t wsdflash, uint32_t wsecdf)
{  /* 设置DFLASH读的总时间 = RFLASH对应的"本体时间" + RECC对应的"ECC时间" */
  unlock_wdtcon();
  DMU_HF_DWAIT.B.RFLASH = (uint32_t) wsdflash;
  DMU_HF_DWAIT.B.RECC = (uint32_t) wsecdf;
  lock_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_core_EnableICache()
 *    Enables Instruction cache on the current core
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_CORE_ENABLEICACHE_UC_SPECIFIC
void bsp_uc_core_EnableICache(void)
{
  Ifx_CPU_PCON1 pcon1;  /* Initiate invalidation of current cache contents if any */
  pcon1.U = 0;
  pcon1.B.PCINV = 1;  // 清除I-Cache
  _mtcr(CPU_PCON1, pcon1.U);

  unlock_wdtcon();  /* PCACHE enable */
  Ifx_CPU_PCON0 pcon0;
  pcon0.U = 0;
  pcon0.B.PCBYP = 0;  // 取消Program Cache Bypass, 即真正启用I-Cache功能
  _mtcr(CPU_PCON0, pcon0.U);
  _isync();
  lock_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_core_EnableDCache()
 *    Enables Data cache on the current core
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_CORE_ENABLEDCACHE_UC_SPECIFIC
void bsp_uc_core_EnableDCache(void)
{
  Ifx_CPU_DCON0 dcon0;
  unlock_wdtcon();
  dcon0.U = 0;
  dcon0.B.DCBYP = 0;
  __mtcr(CPU_DCON0, dcon0.U);
  _isync();
  lock_wdtcon();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_core_GetCurrentCore()
 *    Reads value of the current execution Core calling this function.
 * Output
 *    return : logical CoreId (linear sequence number)
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_CORE_GETCURRENTCORE_UC_SPECIFIC
uint32_t bsp_uc_core_GetCurrentCore(void)
{
	/* 在核内用: 继续_mfcr(0xFE1C); 这是对的
	 * 跨核/调试器用: 按文档公式把CSFR基址 + 0xFE1C拼出绝对地址再load/store */
  uint32_t coreId = _mfcr(CPU_CORE_ID) & (uint32_t) 0x7;  /* retrieve physical Core ID */

  /* Adjust physical index to logical index */
  if(coreId == 6) {  /* 第6颗TriCore在硬件里报的是6, 而不是以为的连续0..5的5 */
    coreId--;
  }

  return coreId;
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: GetCurrentInterruptPriority()
 *    Reads value of the current core interrupt priority
 * Output
 *    return : Current Interrupt priority
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_CORE_GETCURRENTINTERRUPTPRIORITY_UC_SPECIFIC
uint32_t bsp_uc_core_GetCurrentInterruptPriority(void)
{
  Ifx_CPU_ICR reg;
  reg.U = _mfcr(CPU_ICR);
  return (uint32_t) reg.B.CCPN;
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_stm_ReloadChannel()
 *    Initializes requested channel with reload value.
 * Input
 *    channel      : STM channel to initialize
 *    reload_value : period in timer ticks (PER_CLOCK)
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_STM_RELOADCHANNEL_UC_SPECIFIC
void bsp_uc_stm_ReloadChannel(uint32_t channel, uint32_t reload_value)  /* 把STM的CMP0(比较寄存器0)在原值基础上加上一个周期 */
{                                                                       /* 它不做首次初始化、不改窗口配置, 也不清中断标志. 等价于"相对自增下一次触发点" */
  Ifx_STM * const module_stm = ((Ifx_STM *) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
  module_stm->CMP[0].U += reload_value;  /* 把下一次比较目标定在"上一次目标 + 周期". 这是锁相的正确姿势(不会因ISR抖动漂移) */
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_stm_InitChannelIsr()
 *    Enable STM Compare interrupt
 * Input
 *    channel : STM channel to initialize
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_STM_ENABLECHANNELISR_UC_SPECIFIC
void bsp_uc_stm_EnableChannelIsr(uint32_t channel)
{
  Ifx_STM * const module_stm = ((Ifx_STM *) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
  module_stm->CMCON.B.MSIZE0 = 31;
  module_stm->ISCR.U = (IFX_STM_ISCR_CMP0IRR_MSK << IFX_STM_ISCR_CMP0IRR_OFF);
  module_stm->ISCR.B.CMP0IRR = 1;
  module_stm->ICR.B.CMP0EN = 0x1;
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_stm_ClearChannelIsrFlag()
 *    Clear active interrupt flag on requested channel
 * Input
 *    channel : STM channel to initialize
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_STM_CLEARCHANNELISRFLAG_UC_SPECIFIC
void bsp_uc_stm_ClearChannelIsrFlag(uint32_t channel)
{
  Ifx_STM * const module_stm = ((Ifx_STM *) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
  module_stm->ISCR.B.CMP0IRR = 1;
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_stm_IsChannelIsrFlag()
 *    Reads the interrupt flag status
 * Input
 *    channel : STM channel
 * Output
 *    return : 1 = timeout occurred
 *             0 = not yet from last clear
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_STM_ISCHANNELISRFLAG_UC_SPECIFIC
uint32_t bsp_uc_stm_IsChannelIsrFlag(uint32_t channel)
{
  Ifx_STM * const module_stm = ((Ifx_STM *) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
  return (module_stm->ICR.B.CMP0IR);
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_stm_GetChannelCurrentValue()
 *    Reads current value of the timer from requested channel
 * Input
 *    channel : STM channel
 * Output
 *    return : STM counter value
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_STM_GETCHANNELCURRENTVALUE_UC_SPECIFIC
uint32_t bsp_uc_stm_GetChannelCurrentValue(uint32_t channel)  /* 可以理解为每个核对应一个STM模块, 设计上用于给每个CPU的OS tick提供时基 */
{
  Ifx_STM * const module_stm = ((Ifx_STM *) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
  return (module_stm->TIM0.U);  // 只读低32位

	/* 64位版本(原型保留、内部"自动无撕裂") */
	// uint32_t lo = module_stm->TIM0SV.U;
  // uint32_t hi = module_stm->CAPSV.U;
  // return ((uint64_t)hi << 32) | lo;
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_Wait(uint32_t time)
 *    Wait for <time> in micro seconds using core local STM channel.
 * Input
 *    time           : time in microseconds
 *    stm_src_clk    : clock source frequency [MHz]
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_STM_WAIT_UC_SPECIFIC
void bsp_uc_stm_Wait(uint32_t stm_src_clk, uint32_t time)
{
  int coreId = bsp_uc_core_GetCurrentCore();
  uint32_t stm_val = bsp_uc_stm_GetChannelCurrentValue(coreId);
  uint32_t stm_div = SCU_CCUCON0.B.STMDIV;

  while((uint32_t) (bsp_uc_stm_GetChannelCurrentValue(coreId) -stm_val) < (time * stm_src_clk / stm_div)) {
    ;
  }
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_intc_SetBIV()
 *    Register Interrupt vector table in the current Core.
 * Input
 *    table  : vector table base address
 *    vss    : 32B/8B offset between interrupt entries (1/0)
 * ----------------------------------------------------------------------------------------------*/
/* VSS = 0 → 向量间距32字节(本BSP采用的模式, 入口用. align 5)
 * VSS = 1 → 向量间距4字节(更密, 但入口桩必须按4B布局) */
#ifndef BSP_UC_INTC_SETBIV_UC_SPECIFIC
void bsp_uc_intc_SetBIV(uint32_t table, uint32_t vss)
{
  uint32_t biv = ((uint32_t) 0xFFFFFFFE & table) | ((uint32_t) 0x1 & vss);  /* 清掉bit0, 把table(向量表基址)放到BIV[31:1]; BIV[0] = VSS(Vector Spacing Select), 只取vss的最低位 */
  unlock_wdtcon();      /* 临时关闭ENDINIT */
  _mtcr(CPU_BIV, biv);  /* 写Core寄存器BIV = biv. 告诉核心"中断来了先跳哪张表" */
  lock_wdtcon();        /* 临时恢复ENDINIT */
  _isync();             /* 指令同步屏障. 确保改完BIV后, 后续的取指/异常处理马上以新配置生效, 避免流水线/缓存导致的"老配置"短暂残留 */
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_intc_SetSRC()
 *    Configure STM Interrupt in SRC Interrupt module
 * Input
 *    channel  : stm channel
 *    coreId   : which core will handle the interrupt
 *    prio     : priority level
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_INTC_STM_SETSRC_UC_SPECIFIC
void bsp_uc_intc_stm_SetSRC(uint32_t channel, uint32_t coreId, uint32_t prio)  /* 给STM[channel]的SR0配"送到哪个核、什么优先级、开关启用" */
{
  Ifx_SRC * const module_src = (Ifx_SRC *) &MODULE_SRC;  /* 选中STM的第channel个实例的SR0(STM每个实例通常有SR0/SR1两根服务请求线) */

  /* Convert CoreId to correct TOS ID - see TC3x architecture */
  if(coreId != 0) {  /* 把"核号"转换成TOS编码; */
    coreId = coreId + 1;  /* TOS=2 → CPU1(所以coreId=1要+1变成2), 历史兼容原因 */
  }

  /* set processor and priority values */
  module_src->STM.STM[channel].SR[0].B.TOS = coreId;
  module_src->STM.STM[channel].SR[0].B.SRPN = prio;

  /* ... and enable it */
  module_src->STM.STM[channel].SR[0].B.SRE = 1;
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_intc_EnableExternalInterrupts()
 *    Enables external interrupt source in the current Core's ICR register
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_INTC_ENABLEEXTERNALINTERRUPTS_UC_SPECIFIC
void bsp_uc_intc_EnableExternalInterrupts(void)
{
  _enable();
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_InitClock()
 *    It configures the uC clock system to the optimum setting for the max clock
 *    and peripherals needed by BSP example.
 *    The other peripheral clock are left in their default state to be modified
 *    by the user in the time of its need.
 *    CPUx clocks are set to fSRI by default in CCUCON registers, no need to set them.
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_UC_INITCLOCK_UC_SPECIFIC
void bsp_uc_InitClock(void)
{
  /* SCU CCUCON0 Clock distribution (dividers) */
  Ifx_SCU_CCUCON0 con0;
  con0.U = SCU_CCUCON0.U;
  con0.B.STMDIV = UC_CCU_STM_DIV;
  con0.B.SRIDIV = UC_CCU_SRI_DIV;
  con0.B.SPBDIV = UC_CCU_SPB_DIV;
  con0.B.FSI2DIV = UC_CCU_FSI2_DIV;
  con0.B.FSIDIV = UC_CCU_FSI_DIV;
  bsp_uc_scu_SetCcuCon(&SCU_CCUCON0.U, con0.U, 1);

  /* SCU CCUCON1 Clock distribution (dividers) */
  Ifx_SCU_CCUCON1 con1;
  con1.U = SCU_CCUCON1.U;
  con1.B.QSPIDIV = UC_CCU_QSPI_DIV;
  con1.B.CLKSELQSPI = UC_CCU_QSPI_CLKSEL;
  bsp_uc_scu_SetCcuCon(&SCU_CCUCON1.U, con1.U, 1);

  /* Flash modules - timing */
  bsp_uc_pflash_SetWs(UC_FCON_WSPFLASH, UC_FCON_WSECPF);
  bsp_uc_dflash_SetWs(UC_FCON_WSDFLASH, UC_FCON_WSECDF);

  /* Clock Init */
  bsp_uc_scu_EnableXOSC();
  bsp_uc_scu_EnablePLL0(UC_PLL0_PDIV, UC_PLL0_NDIV, UC_PLL0_K2);
  bsp_uc_scu_EnablePLL1(UC_PLL1_PDIV, UC_PLL1_NDIV, UC_PLL1_K2, UC_PLL1_K3);

  /* Switch System Clock to PLL */
  bsp_uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_PLL0);
}
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: bsp_uc_intc_InitBIV
 *    It initializes BSP provided ISR Vector Table to the current Core's BIV register.
 *    The function can be removed from build be definition of the macro either in the project
 *    setting or on the command line.
 *       - BSP_ISR_SUPPORT_DISABLED
 *    This is the case when user has its own ISR vector implementation (like RTOS)
 * ----------------------------------------------------------------------------------------------*/
#ifndef BSP_ISR_SUPPORT_DISABLED

/* BSP Interrupt vector tables are defined in 'bsp_isr_vector_table.c' */
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU0;

#if ( UC_NB_CORES > 1)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU1;

#if ( UC_NB_CORES > 2)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU2;

#if ( UC_NB_CORES > 3)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU3;

#if ( UC_NB_CORES > 4)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU4;

#if ( UC_NB_CORES > 5)
extern uint32_t * BSP_ISR_VECTOR_TABLE_CPU5;
#endif

#endif

#endif

#endif

#endif

#ifndef BSP_UC_INITCLOCK_UC_SPECIFIC
void bsp_uc_intc_InitBIV(void)
{
  uint32_t isr_vector_table;
  int coreId = bsp_uc_core_GetCurrentCore();

  switch(coreId)  /* Select right vector table */
  {
    case UC_CORE_MAIN_0:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU0;
      break;

#if ( UC_NB_CORES > 1)
    case UC_CORE_MAIN_1:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU1;
      break;

#if ( UC_NB_CORES > 2)
    case UC_CORE_MAIN_2:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU2;
      break;

#if ( UC_NB_CORES > 3)
    case UC_CORE_MAIN_3:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU3;
      break;

#if ( UC_NB_CORES > 4)
    case UC_CORE_MAIN_4:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU4;
      break;

#if ( UC_NB_CORES > 5)
    case UC_CORE_MAIN_5:
      isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU5;
      break;

#endif

#endif

#endif

#endif

#endif

    default:
      return; // nothing to do in case of wrong coreId parameter
  }

  /* register ISR Vector Table (dispatch table) to Core BIV register
   * VSS = 0 to get a BSP expected 32B spacing in between vectors */
  bsp_uc_intc_SetBIV(isr_vector_table, 0);
}
#endif  /* #ifndef BSP_UC_INITCLOCK_UC_SPECIFIC */

#endif  /* #ifndef BSP_ISR_SUPPORT_DISABLED */

