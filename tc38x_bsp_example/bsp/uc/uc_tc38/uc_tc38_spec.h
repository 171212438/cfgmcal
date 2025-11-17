

/******************************************************************************
 * FILE: uc_tc39_spec.h
 * uC ARCHITECTURE
 *    TC3xx
 * uC FAMILY
 *    TC38x
 * DESCRIPTION
 *    Configuration and description of uC derivative.
 *    Parameters are taken from uC Datasheet and Reference manual.
 * FILE HISTORY
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

#ifndef UC_TC38_SPEC_H
#define UC_TC38_SPEC_H

/* uC derivative specific types */
#include "uc_tc38_types.h"


/* ============================================================================
 * CORES SPECIFICATION
 * ==========================================================================*/
#define UC_NB_CORES 4    /* Number of Cores instances */
#define UC_RESET_CORE 0  /* RESET CORE: core that starts after Power On */


/* ============================================================================
 * DEFAULT uC CLOCK SETTING
 * Target is MAX system clock for Core frequency following recommended rules.
 * User ** should not ** change it unless he is certain what he is doing
 * ==========================================================================*/
#define UC_PLL0_CLOCK 300    /* 系统主PLL(SYS PLL/PLL0)的目标输出频率; 它主要喂: 各个CPU core的时钟; SRI/SPB等主总线(经分频); STM等一堆"平台时钟" */

#define UC_PLL1_CLOCK 320    /* 外设PLL1(PER PLL1)的目标频率, 专门给部分高速外设用(比如QSPI) */
#define UC_PLL2_CLOCK 200    /* 外设PLL2(PER PLL2)**的目标频率, 可以给外设(比如QSPI、GETH等)当时钟源 */


#define UC_BACKUP_CLOCK 100  /* 内部备份振荡器的频率: 100MHz; 当外部晶振挂了, 或PLL没锁住时, 系统可以切回这颗内部时钟做"救命时钟", 保证MCU不死透 */

/* Platform clocks */
#define UC_SRI_CLOCK  300    /* 大号主总线(CPU ↔ 内存/大外设), 设成300MHz, 正常来源是PLL0=300, 出事时可切到100MHz的BACKUP */
#define UC_SPB_CLOCK  100    /* 次级外设总线, 设成100 MHz; 很多外设挂在SPB上(DMA、GTM子块等), 手册限制MAX=100 */
#define UC_FSI_CLOCK  100    /* 一些内部互连/桥的时钟(Family Specific Interconnect); FSI设成100MHz, FSI2设成300MHz, 通过CCU由SRI分频出来 */
#define UC_FSI2_CLOCK 300    /*  */

/* Peripheral clocks needed in BSP example */
#define UC_QSPI_CLOCK 200   /* QSPI的模块时钟频率, 来源可以是PERPLL1(320)、PERPLL2(200)、或BACKUP. 上限是200MHz*/
#define UC_STM_CLOCK  100   /* STM(系统定时器)的时钟频率; 来源是PLL0或BACKUP, 通过CCUCON0.STMDIV分出来; 选100MHz, 精度高(10ns/tick), 又不至于太快(溢出前还能跑挺久) */

/* BSP supportive time macros */
#define UC_NB_TICKS_1US (UC_STM_CLOCK)            /* 1微秒 = UC_STM_CLOCK个tick */
#define UC_NB_TICKS_1MS (UC_NB_TICKS_1US * 1000)  /* 1毫秒 = UC_STM_CLOCK * 1000个tick */


/* ============================================================================
 * PFLASH SPECIFICATION
 * ==========================================================================*/
/* UC_PFLASH_PF = 30 ns
 *   Program Flash本体一次访问的最小时间: 从给地址, 到取出数据稳定在总线上的时间, 大约30ns
 * UC_PFLASH_PFECC = 10 ns
 *   ECC(错误检测纠正)逻辑额外需要的时间: 读出原始数据后, 要算ECC、做校验/纠错, 再把"最终正确数据"给CPU, 大约10ns
 * 因此PFlash单次读取总时间 ≈ 30ns + 10ns = 40ns(最小)
 * 比如:
 *   若CPU核时钟fCPU = 300 MHz, 一个时钟周期约3. 33 ns;
 *   想拿到PFlash上的那40ns数据, 至少要12个左右的CPU周期时间;
 *   在Flash配置寄存器里: 插入一定数量的Wait States, 让取指/读常量不出错 */
/* PFLASH selection of minimum Wait states for given SYSTEM_CLOCK
 * DS Chapter 3.16 - Flash Memory */
#define UC_PFLASH_PF    30     /* [ns] Program Flash access delay */
#define UC_PFLASH_PFECC 10     /* [ns] Program Flash ECC delay */

/* DFlash单次读取总时间 ≈ 100ns + 20ns = 120ns
 * 即便把DFlash也挂在200MHz左右的时钟上, 一个周期5ns, 120ns也要24个周期;
 * DFlash从设计上就是"慢且贵的存储", 适合存参数/EEPROM仿真, 不适合跑紧凑代码 */
#define UC_DFLASH_DF    100    /* [ns] Data Flash access delay */
#define UC_DFLASH_DFECC 20     /* [ns] Data Flash ECC Delay */


/* ============================================================================
 * PLL0 SPECIFICATION
 * ==========================================================================*/
/* SYSTEM PLL parameters */
/* System PLL只保证在16–24 MHz的参考频率下工作是合法的、参数表是有效的
 * PLL0内部VCO的工作频率必须落在400–800 MHz之间 
 * VCO = Voltage Controlled Oscillator */
#define UC_PLL0_INCLK_MIN 16
#define UC_PLL0_INCLK_MAX 24
#define UC_PLL0_VCO_MIN   400
#define UC_PLL0_VCO_MAX   800

/* Typical Clock Divider recommendations */
/* 这个是"中间最舒服的频点", 官方推荐的VCO目标值
 * VCO频率fVCO是先把输入时钟fOSC除以P, 再乘N: fVCO = (N * fOSC) / P
 * 输出频率fPLL0是在VCO基础上再除以K2: fPLL0 = (N * fOSC) / (P * K2) */
#define UC_PLL0_VCO_REC   600


/* ============================================================================
 * PLL1 SPECIFICATION
 * ==========================================================================*/
/* PERIPHERAL PLL parameters */
#define UC_PLL1_INCLK_MIN 10   /* 喂给PLL1的输入时钟(INCLK)要在10–24 MHz之间, 工程里一般是用20 MHz晶振 */
#define UC_PLL1_INCLK_MAX 24
#define UC_PLL1_VCO_MIN   400  /* PLL1内部VCO(压控振荡器)允许工作的频率区间 */
#define UC_PLL1_VCO_MAX   800

/* Typical Clock Divider recommendations */
#define UC_PLL1_VCO_REC   640  /* 推荐VCO频率640 MHz */


/* ============================================================================
 * MODULE BASE ADDRESSES IN MEMORY MAP
 * use of semiconductor peripheral file to get peripheral modules' base
 * addresses for those modules that will access a module using their own
 * macros instead of peripheral header bit fields
 * ==========================================================================*/
#define UC_PORT_BASE (uint32_t) &MODULE_P00


/* ============================================================================
 * PLL SUBSYSTEM CALCULATION
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * SYTEM PLL0 CALCULATION
 * --------------------------------------------------------------------------*/
/* XTAL divider setting to fulfill PLL0 input clock */
#if (BOARD_XTAL_CLOCK > UC_PLL0_INCLK_MAX)
	#define PLL0_PDIV 2
#else
	#define PLL0_PDIV 1
#endif

/* PLL0 input clock */
#define UC_PLL0_INCLK          (BOARD_XTAL_CLOCK / PLL0_PDIV)

/* PLL0 MIN/MAX values to make following macros more readable */
#define PLL0_VCO_MIN           (UC_PLL0_VCO_MIN - 1)
#define PLL0_VCO_MAX           (UC_PLL0_VCO_MAX + 1)

/* PLL0_VCO_REC Recommended: */
#define PLL0_K2                (UC_PLL0_VCO_REC / UC_PLL0_CLOCK)
#define PLL0_NDIV_A            (UC_PLL0_VCO_REC / UC_PLL0_INCLK)
#define PLL0_NDIV_A_NOT_VALID  ((PLL0_NDIV_A * UC_PLL_INCLK) - UC_PLL_VCO_REC)
#define PLL0_K2_NOT_VALID      ((PLL0_K2 * UC_PLL0_CLOCK) - UC_PLL0_VCO_REC)

#if ((UC_PLL0_VCO_REC > PLL0_VCO_MIN) && (UC_PLL0_VCO_REC < PLL0_VCO_MAX) && !(PLL0_K2_NOT_VALID) )
	#define PLL0_NDIV PLL0_NDIV_A
#else
	#error Required PLL0 cannot be achieved with predefined PLL0_VCO_REC and board XTAL
#endif  /* UC_PLL0_VCO_REC */

/* Values for PLL registers */
#define UC_PLL0_PDIV           (PLL0_PDIV - 1)
#define UC_PLL0_NDIV           (PLL0_NDIV - 1)
#define UC_PLL0_K2             (PLL0_K2 - 1)

/* ----------------------------------------------------------------------------
 * PERIPHERAL PLL1 CALCULATION
 * --------------------------------------------------------------------------*/
/* XTAL divider setting to fulfill PLL1 input clock */
#if (BOARD_XTAL_CLOCK > UC_PLL1_INCLK_MAX)
	#define PLL1_PDIV 2
#else
	#define PLL1_PDIV 1  /* In using */
#endif

#define UC_PLL1_INCLK          (BOARD_XTAL_CLOCK / PLL1_PDIV)

#define PLL1_VCO_MIN           (UC_PLL1_VCO_MIN - 1)
#define PLL1_VCO_MAX           (UC_PLL1_VCO_MAX + 1)

/* PLL1_VCO_REC Recommended: */
#define PLL1_K2                (UC_PLL1_VCO_REC / UC_PLL1_CLOCK)
#define PLL1_K3                (UC_PLL1_VCO_REC / 1.6 / UC_PLL2_CLOCK)
#define PLL1_NDIV_A            (UC_PLL1_VCO_REC / UC_PLL1_INCLK)
#define PLL1_NDIV_A_NOT_VALID  ((PLL1_NDIV_A * UC_PLL1_INCLK)-UC_PLL1_VCO_REC)
#define PLL1_K2_NOT_VALID      ((PLL1_K2 * UC_PLL1_CLOCK) - UC_PLL1_VCO_REC)

#if ((UC_PLL1_VCO_REC > PLL1_VCO_MIN) &&  (UC_PLL1_VCO_REC < PLL1_VCO_MAX) && !(PLL1_NDIV_A_NOT_VALID) && !(PLL1_K2_NOT_VALID) )
	#define PLL1_NDIV PLL1_NDIV_A
#else
	#error Required PLL1 cannot be achieved with predefined PLL1_VCO and board XTAL
#endif /* PLL_VCO_A */

/* Values for PLL registers */
#define UC_PLL1_PDIV           (PLL1_PDIV - 1)
#define UC_PLL1_NDIV           (PLL1_NDIV - 1)
#define UC_PLL1_DIBBY          0  /* to divide PLL1 VCO (640MHz / 1.6) => for K3 divider */
#define UC_PLL1_K2             (PLL1_K2 - 1)
#define UC_PLL1_K3             (PLL1_K3 - 1)


/* ============================================================================
 * FLASH REGISTER VALUES
 * ==========================================================================*/
/* 根据当前SRI时钟有多快, 把Flash需要的xx ns转成: FCON里要插几个wait state */
/* 为什么还要-1? 因为FCON里对应的字段设计成"寄存器值+1 = 实际等待周期数" */
/* PFLASH */
#define UC_FCON_WSPFLASH       (((UC_PFLASH_PF * UC_SRI_CLOCK) / 1000) - 1)     /* Program Flash本体的wait state数(写给PFlash的某个WSP字段); */
#define UC_FCON_WSECPF         (((UC_PFLASH_PFECC * UC_SRI_CLOCK) / 1000) - 1)  /* PFlash ECC相关的附加wait state(写给ECC那个字段); */

/* DFLASH */
#define UC_FCON_WSDFLASH       (((UC_DFLASH_DF * UC_SRI_CLOCK) / 1000) - 1)     /* Data Flash本体的wait state; */
#define UC_FCON_WSECDF         (((UC_DFLASH_DFECC * UC_SRI_CLOCK) / 1000) - 1)  /* DFlash ECC部分的wait state */


/* ============================================================================
 *  CCUCONx VALUES FOR BSP SELECTED CLOCKS
 * ==========================================================================*/
/* 根据你前面选好的"目标时钟频率", 算出CCUCON里该填的分频数, 并检查是不是"能整除".
 * 只要不能整除, 就直接 #error给你报编译错误, 防止时钟配置搞成奇怪的小数; */
#define UC_CCU_SRI_DIV (UC_PLL0_CLOCK / UC_SRI_CLOCK)  /* SRI时钟分频 */
#if ((UC_CCU_SRI_DIV * UC_SRI_CLOCK) != UC_PLL0_CLOCK)
#error Wrong SRI clock setting, not a whole number divider
#endif

#define UC_CCU_SPB_DIV (UC_PLL0_CLOCK / UC_SPB_CLOCK)  /* SPB时钟分频 */
#if ((UC_CCU_SPB_DIV * UC_SPB_CLOCK) != UC_PLL0_CLOCK)
#error Wrong SPB clock setting, not a whole number divider
#endif

#define UC_CCU_FSI2_DIV (UC_SRI_CLOCK / UC_FSI2_CLOCK)  /* FSI/FSI2时钟分频(从SRI来) */
#if ((UC_CCU_FSI2_DIV * UC_FSI2_CLOCK) != UC_SRI_CLOCK)
#error Wrong FSI2 clock setting, not a whole number divider
#endif

#define UC_CCU_FSI_DIV (UC_SRI_CLOCK / UC_FSI_CLOCK)
#if ((UC_CCU_FSI_DIV * UC_FSI_CLOCK) != UC_SRI_CLOCK)
#error Wrong FSI2 clock setting, not a whole number divider
#endif

#define UC_CCU_STM_DIV (UC_PLL0_CLOCK / UC_STM_CLOCK)  /* STM时钟分频(从PLL0来) */
#if ((UC_CCU_STM_DIV * UC_STM_CLOCK) != UC_PLL0_CLOCK)
#error Wrong STM clock setting, not a whole number divider
#endif

#define UC_CCU_QSPI_DIV (UC_PLL2_CLOCK / UC_QSPI_CLOCK)  /* QSPI时钟分频(从PLL2来, 带CLKSEL) */
#define UC_CCU_QSPI_CLKSEL 2  /* fPLL2 use as clock source for fSOURCEQSPI */
#if ((UC_CCU_QSPI_DIV * UC_QSPI_CLOCK) != UC_PLL2_CLOCK)
#error Wrong QSPI clock setting, not a whole number divider
#endif

#endif  /* UC_TC38_SPEC_H */

#endif  /* uC protection guard */

