

/******************************************************************************
 * FILE: uc_tc38_types.h
 * uC ARCHITECTURE
 *    TC3xx
 * uC FAMILY
 *    TC38x
 * DESCRIPTION
 *    uC derivative specific types that user can use in code.
 *    A usage of these types helps to catch issues when porting between
 *    different uC derivatives.
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

#ifndef UC_TC38_TYPES_H
#define UC_TC38_TYPES_H

/* ----------------------------------------------------------------------------
 * CORE Identification
 * --------------------------------------------------------------------------*/
typedef enum {
  UC_CORE_MAIN_0 = 0, 
  UC_CORE_MAIN_1 = 1, 
  UC_CORE_MAIN_2 = 2, 
  UC_CORE_MAIN_3 = 3, 
} UC_CORE_ID_e;


/* ----------------------------------------------------------------------------
 * SRC Module TOS fields
 * --------------------------------------------------------------------------*/
typedef enum {
  UC_SRC_TOS_CPU0 = 0, 
  UC_SRC_TOS_DMA = 1, 
  UC_SRC_TOS_CPU1 = 2, 
  UC_SRC_TOS_CPU2 = 3, 
  UC_SRC_TOS_CPU3 = 4, 
} UC_SRC_TOS_e;


/* ----------------------------------------------------------------------------
 * CCU CLOCK modules
 * --------------------------------------------------------------------------*/

/* CLOCK SOURCE selection */
typedef enum {
  UC_CCU_CON0_CLKSEL_BACKUP = 0, 
  UC_CCU_CON0_CLKSEL_PLL0 = 1, 
  UC_CCU_CON0_CLKSEL_PLL1 = 2, 
} UC_CCU_CON0_CLKSEL_e;


/* PLLs INPUT selection */
typedef enum {
  UC_CCU_CON1_INSEL_BACKUP = 0, 
  UC_CCU_CON1_INSEL_XOSC = 1, 
} UC_CCU_CON1_INSEL_e;


/* Fsystem clock selection */
typedef enum {
  UC_CCU_SYSTEM_CLOCK_BACKUP = 0, 
  UC_CCU_SYSTEM_CLOCK_PLL0 = 1, 
  UC_CCU_SYSTEM_CLOCK_XOSC = 2, 
} UC_CCU_SYSTEM_CLOCK_e;


/* ----------------------------------------------------------------------------
 * External CLOCK Output
 * --------------------------------------------------------------------------*/

/* Clock selection corresponds to EXTCLK0 */
typedef enum {
  UC_EXTCLK0_SEL_OUT = 0, 
  UC_EXTCLK0_SEL_PLL0 = 1, 
  UC_EXTCLK0_SEL_PLL1 = 2, 
  UC_EXTCLK0_SEL_OSC0 = 3, 
  UC_EXTCLK0_SEL_BACK = 4, 
  UC_EXTCLK0_SEL_PLL2 = 5, 
  UC_EXTCLK0_SEL_BBB = 6, 
  UC_EXTCLK0_SEL_SRI = 8, 
  UC_EXTCLK0_SEL_SPB = 9, 
  UC_EXTCLK0_SEL_FSI = 10, 
  UC_EXTCLK0_SEL_STM = 11, 
  UC_EXTCLK0_SEL_GTM = 12, 
  UC_EXTCLK0_SEL_TCK = 13, 
  UC_EXTCLK0_SEL_FSI2 = 14, 
  UC_EXTCLK0_SEL_MT0 = 15, 
} UC_EXTCLK0_SEL_e;


/* Clock selection corresponds to EXTCLK1 */
typedef enum {
  UC_EXTCLK1_SEL_OUT = 0, 
  UC_EXTCLK1_SEL_PLL0 = 1, 
  UC_EXTCLK1_SEL_PLL1 = 2, 
  UC_EXTCLK1_SEL_EBU = 3, 
  UC_EXTCLK1_SEL_BACK = 4, 
  UC_EXTCLK1_SEL_CAN = 5, 
  UC_EXTCLK1_SEL_ADC = 6, 
  UC_EXTCLK1_SEL_SPI = 7, 
  UC_EXTCLK1_SEL_SRI = 8, 
  UC_EXTCLK1_SEL_SPB = 9, 
  UC_EXTCLK1_SEL_I2C = 10, 
  UC_EXTCLK1_SEL_MSC = 11, 
  UC_EXTCLK1_SEL_ERAY = 12, 
  UC_EXTCLK1_SEL_LINF = 13, 
  UC_EXTCLK1_SEL_LINS = 14, 
  UC_EXTCLK1_SEL_OSC = 15, 
} UC_EXTCLK1_SEL_e;


/* ----------------------------------------------------------------------------
 * STM timer
 * --------------------------------------------------------------------------*/

/* STM CHANNELS interface type */
typedef enum {
  UC_STM_CHANNEL_0 = 0, 
  UC_STM_CHANNEL_1 = 1, 
  UC_STM_CHANNEL_2 = 2, 
  UC_STM_CHANNEL_3 = 3, 
} UC_STM_CHANNEL_e;


/* ----------------------------------------------------------------------------
 * PORTs IO Interface
 * --------------------------------------------------------------------------*/

/* 从最慢到最快的输出沿
 * 作用: 限制上升/下降沿的斜率, 慢一点抗干扰/辐射更好, 快一点时序更紧;
 * 实战建议:
 *   数字通信(SPI时钟、GTM PWM较高频) → 选3~4;
 *   车身开关量/LED/低速信号 → 选1~2, 抑制EMI;
 *   对应寄存器: 端口驱动配置Pn_PDRx.PDx* 的"边沿速率/驱动能力"字段(不同引脚分在PDR0/PDR1的不同位组) */
/* PORT PAD output edge rate control */
typedef enum {
  UC_PORT_PD_SPEED_1 = 0, 
  UC_PORT_PD_SPEED_2 = 1, 
  UC_PORT_PD_SPEED_3 = 2, 
  UC_PORT_PD_SPEED_4 = 3, 
} UC_PORT_PD_SPEED_e;


/* PAD电平特性(AL vs TTL)
 * AL(Automotive Level): 汽车级阈值/滞回, 抗噪更强, 适合5V/噪声环境;
 * TTL: TTL/CMOS风格阈值, 更贴近3.3V数字逻辑;
 * 用途: 给该PAD选择门限/电平规范, 保证与外部器件的兼容与抗扰;
 * 实战建议:
 *   接车身线束/继电器边等脏环境 → AL;
 *   接MCU ↔ 外设3.3V逻辑(如外部GPIO扩展/FPGA) → TTL;
 *   精确门限/滞回见数据手册电气特性表, 各PAD支持的模式以具体引脚能力为准 */
/* PORT PAD output level control */
typedef enum {
  UC_PORT_PL_LEVEL_AL = 0, 
  UC_PORT_PL_LEVEL_TTL = 1, 
} UC_PORT_PL_LEVEL_e;


/* 1) TRISTATE(高阻)
 *   像把手松开, 不拉不拽: 不接内部上拉/下拉, 脚子处于"漂"的状态; 外部不驱动时, 脚会乱飘(感应噪声→0/1乱跳); 功耗低;
 *   什么时候用: 外部电路自己已经有明确的上拉/下拉(比如外部电阻、分压器、传感器带推挽输出), 那就用高阻, 别和外部抢;
 * 2) PULL_UP(上拉)
 *   给脚子轻轻拴一根细橡皮筋到电源: 没人拉时它会待在"1"; 内部有个弱上拉电阻, 默认读到1; 外部如果拉到0(比如按键接地、开漏下拉), 能把它拽成0;
 *   什么时候用: 线路可能会悬空、但你希望默认=1;
 *   常见: 按钮一端接地、松手为1; 开漏/集电极开路输出(open-drain/open-collector)需要上拉; UART RX线空闲为高时可上拉稳定;
 * 3) PULL_DOWN(下拉)
 *   反过来, 弱下拉电阻把脚默认拉到0;
 *   什么时候用: 线路可能会悬空、但你希望默认=0;
 *   常见: 按钮一端接电源、松手为0; 外设闲置会浮空但你想读0的场景;
 * "弱"的意思: 上/下拉电阻值比较大(阻值高、电流小), 外部的强驱动会覆盖它. 所以它只用来给"没人驱动时"定个默认值, 不是用来对抗外部强驱动的;
 * 会不会打架?
 *   外部也上拉、内部又下拉? → 形成分压, 电平不可靠;
 *   外部推挽输出成"高", 内部还开下拉? → 会有小电流白白消耗;
 *   原则: 内部上/下拉只在外部可能悬空时使用; 外部已有明确电平就用TRISTATE;
 * 噪声/抖动解决: 悬空线很容易感应到0/1抖; 需要稳定就开PULL_UP或PULL_DOWN. 长线束, 还可以外加RC滤波或用AL输入阈值更抗噪;
 * 功耗影响: 打开上/下拉会有微小电流(特别在被外部拉到相反电平时), 低功耗场景要评估; 不需要就关(TRISTATE);
 * 按键怎么接? 两种等价接法:
 *   上拉 + 按键接地: 松手读1, 按下读0;
 *   下拉 + 按键接电源: 松手读0, 按下读1;
 *   选你代码更顺手的逻辑, 记得软件里做消抖;
 * 一眼决策表
 *   外部有推挽输出 → TRISTATE
 *   外部开漏/集电极开路、可能悬空 → PULL_UP(或按需要选下拉)
 *   按钮/拨码开关, 松手需要固定值 → PULL_UP或PULL_DOWN
 *   线长、噪声大, 但外部无确定电平 → PULL_UP/PULL_DOWN(必要时再加外部电阻/滤波) */
/* 输入模式(上下拉/三态)
 *   TRISTATE: 高阻输入(不加内部上下拉);
 *   PULL_DOWN: 下拉输入(内部弱下拉);
 *   PULL_UP: 上拉输入(内部弱上拉);
 * 实战建议: 浮空开关量/未接时要有确定状态 → 选上拉或下拉; 外部已有强上下拉/分压 → 三态;
 * 对应寄存器: Pn_IOCRx.PCx输入配置/上下拉选择与Pn_PDISC(输入禁用)等组合 */
/* PORT PAD input mode control */
typedef enum {
  UC_PORT_INPUT_MODE_TRISTATE = 0, 
  UC_PORT_INPUT_MODE_PULL_DOWN = 1, 
  UC_PORT_INPUT_MODE_PULL_UP = 2, 
} UC_PORT_INPUT_MODE_e;


/* 输出寄存器的快捷操作
 *   NO_CHANGE: 不改动当前输出;
 *   SET: 置1(把引脚驱动为高);
 *   CLEAR: 置0(把引脚驱动为低);
 *   TOGGLE: 翻转当前电平;
 * 对应寄存器: 输出修改寄存器Pn_OMR(具有原子性的PSx/PCx/PPx字段), 比直接写Pn_OUT更安全, 避免读-改-写竞态; */
/* PORT OUTPUT control */
typedef enum {
  UC_PORT_OUTPUT_NO_CHANGE = 0, 
  UC_PORT_OUTPUT_SET = 1, 
  UC_PORT_OUTPUT_CLEAR = 2, 
  UC_PORT_OUTPUT_TOGGLE = 3, 
} UC_PORT_OUTPUT_e;


#endif /* UC_TC38_TYPES_H */

#endif /* uC protection guard */

