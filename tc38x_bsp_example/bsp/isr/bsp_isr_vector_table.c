

/**************************************************************************************************
 * FILE: bsp_isr_vector_table.c
 * UC ARCHITECTURE:
 *    AURIX TC3x
 * UC FAMILY:
 *    TC3xx
 * DESCRIPTION:
 *  每个核的"中断跳板合集". 硬件一进中断, 先落到此处汇编"跳板", 再由跳板去RAM里取注册的ISR函数指针并跳过去, 因此运行期能热替换ISR, 而不用改闪存里的向量表
 *  外设事件 → 设置对应SRCx.SRPN=i并使能 → CPU进中断, PC落到向量表第i个入口桩(本文件) → 桩从BspIsrRamTable_CpuX[i]取函数指针 → calli真正ISR(业务ISR) → rslcx/rfe返回
 *  这里放的不是的业务ISR, 而是N个通用入口桩(每个优先级一个), 每个桩干三件事:
 *    > bisr i建立中断上下文;
 *    > 从BspIsrRamTable_CpuX[i]取出函数指针;
 *    > calli跳到真正ISR, 回来后rslcx、rfe退出;
 * FILE HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

/* include common BSP header file */
#include "bsp.h"

/* implement only when BSP provides HW Interrupt support */
#ifndef BSP_ISR_SUPPORT_DISABLED

/* ================================================================================================
 * Vector table macros
 * ==============================================================================================*/

/* 创建/切换到一个名为.bsp_isr_vector_table_cpu<i>的段(section), "ax": 链接后占用内存、可执行. 像.text一样被当作代码段处理; @progbits: 该段内容是程序自带的二进制, 不是调试/元数据(对链接器是普通可装入内容);
 * 对齐到2^11 = 2048字节边界; TriCore/AURIX的BIV要求向量表具有固定对齐; 对应官方建议的"2k对齐的中断向量表"; 若对齐不足, BIV指到的表会不合规, bisr取入口时可能直接陷入异常(或跳错地址);
 * 导出一个全局符号, 例如BSP_ISR_VECTOR_TABLE_CPU0; 链接后该符号值即为该核向量表的基址, 初始化代码可用它来设置该核的BIV寄存器, 也便于调试器定位;
 * 定义一个同名标签, 作为段内的起始位置; 随后该宏后面会跟一串由BSP_ISR_DEFINE_HANDLER(cpu, idx)展开的向量入口桩(每个优先级一个entry), 从这个基址顺序排列; */
#define BSP_ISR_VECTOR_TABLE(i) \
    __asm (".section .bsp_isr_vector_table_cpu" #i " , \"ax\", @progbits"); \
    __asm (".align 11"); \
    __asm (".global BSP_ISR_VECTOR_TABLE_CPU" #i); \
    __asm ("BSP_ISR_VECTOR_TABLE_CPU" #i ":")

/* 1. 把当前入口按2^5 = 32字节对齐. 中断向量槽要按固定粒度对齐, 便于硬件定位和取指性能. 整张表按2KB对齐, 此处是槽内对齐;
 * 2. "Begin Interrupt Service Routine"——设置当前中断上下文(含当前优先级号CCPN = i, 并处理嵌套/屏蔽的必要状态更新). 把CPU的中断优先级环境切到"正在处理i优先级"的状态, 确保嵌套策略正确;
 * 3-4. TriCore没有一次性加载32位绝对地址的单指令, 采用高/低半加载的标准序列: movh.a先把地址的高16位装进地址寄存器a14; lea把低16位补齐, 同时完成加法;
 *      BspIsrRamTable_Cpu<cpu>是该核的ISR函数指针表基址; 每项4字节, 偏移4 * i就是第i个优先级槽位的指针地址; 选用%a14作为临时地址寄存器, 后面还要用它做间接跳转;
 * 5. 把内存里的函数指针值(表项内容)读出来, 放回a14; 此时a14变为目标ISR的入口地址(而不是表项地址);
 * 6. 间接调用真正的ISR; 处理函数类型是void(*)(void); BSP要求它带__attribute__((interrupt_handler)), 让编译器生成适配的序言/尾声(保存/恢复寄存器、使用CSA/栈等), 以匹配TriCore的异常/中断调用约定;
 * 7. Restore Lower Context(恢复低上下文), 与硬件/编译器在入口处保存的上下文配对; 组合了"ISR返回路径"的一部分工作. 注意: 宏里没有svlcx, 因为保存工作通常由中断入口或编译器生成的ISR序言完成; 这里在调用真正ISR结束后, 先恢复"低上下文";
 * 8. Return From Exception, 最终从中断返回, 恢复到被打断的现场(PC/PSW等), 继续原先的程序流; */
#define BSP_ISR_DEFINE_HANDLER(cpu, i) \
    __asm (".align 5"); \
    __asm ("bisr " #i); \
    __asm ("movh.a %a14, hi:(BspIsrRamTable_Cpu" #cpu "+ 4 * " #i ")"); \
    __asm ("lea    %a14, [%a14]lo:(BspIsrRamTable_Cpu" #cpu "+ 4 * " #i ")"); \
    __asm ("ld.a   %a14, [%a14]0"); \
    __asm ("calli  %a14"); \
    __asm ("rslcx"); \
    __asm ("rfe");


/* ================================================================================================
 * ISR VECTOR TABLES definition
 * BSP supports (defines) only 32 priority vectors that should be sufficient for evaluation
 * purposes and simple test applications.
 * ==============================================================================================*/

/* ------------------------------------------------------------------------------------------------
 * CORE 0
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 0)

BSP_ISR_VECTOR_TABLE(0);  /* 让接下来的指令被放进名为.bsp_isr_vector_table_cpu0的可执行段里, 并做2KB对齐, 同时打上全局标签BSP_ISR_VECTOR_TABLE_CPU0作为表基址符号" */

BSP_ISR_DEFINE_HANDLER(0, 0);  /* 一小段汇编桩代码, 被放进上面的向量表段里, 等到中断优先级0触发时, CPU的PC才会跳到这里, 做: */
                               /* bisr 0建立中断上下文; 从BspIsrRamTable_Cpu0[0]取出业务ISR指针; calli调用ISR; rslcx、rfe返回 */
BSP_ISR_DEFINE_HANDLER(0, 1);  /* 一小段汇编桩代码, 被放进上面的向量表段里, 等到中断优先级1触发时, CPU的PC才会跳到这里 */
BSP_ISR_DEFINE_HANDLER(0, 2);
BSP_ISR_DEFINE_HANDLER(0, 3);
BSP_ISR_DEFINE_HANDLER(0, 4);
BSP_ISR_DEFINE_HANDLER(0, 5);
BSP_ISR_DEFINE_HANDLER(0, 6);
BSP_ISR_DEFINE_HANDLER(0, 7);
BSP_ISR_DEFINE_HANDLER(0, 8);
BSP_ISR_DEFINE_HANDLER(0, 9);

BSP_ISR_DEFINE_HANDLER(0, 10);
BSP_ISR_DEFINE_HANDLER(0, 11);
BSP_ISR_DEFINE_HANDLER(0, 12);
BSP_ISR_DEFINE_HANDLER(0, 13);
BSP_ISR_DEFINE_HANDLER(0, 14);
BSP_ISR_DEFINE_HANDLER(0, 15);
BSP_ISR_DEFINE_HANDLER(0, 16);
BSP_ISR_DEFINE_HANDLER(0, 17);
BSP_ISR_DEFINE_HANDLER(0, 18);
BSP_ISR_DEFINE_HANDLER(0, 19);

BSP_ISR_DEFINE_HANDLER(0, 20);
BSP_ISR_DEFINE_HANDLER(0, 21);
BSP_ISR_DEFINE_HANDLER(0, 22);
BSP_ISR_DEFINE_HANDLER(0, 23);
BSP_ISR_DEFINE_HANDLER(0, 24);
BSP_ISR_DEFINE_HANDLER(0, 25);
BSP_ISR_DEFINE_HANDLER(0, 26);
BSP_ISR_DEFINE_HANDLER(0, 27);
BSP_ISR_DEFINE_HANDLER(0, 28);
BSP_ISR_DEFINE_HANDLER(0, 29);

BSP_ISR_DEFINE_HANDLER(0, 30);
BSP_ISR_DEFINE_HANDLER(0, 31);

/* ------------------------------------------------------------------------------------------------
 * CORE 1
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 1)

BSP_ISR_VECTOR_TABLE(1);

BSP_ISR_DEFINE_HANDLER(1, 0);
BSP_ISR_DEFINE_HANDLER(1, 1);
BSP_ISR_DEFINE_HANDLER(1, 2);
BSP_ISR_DEFINE_HANDLER(1, 3);
BSP_ISR_DEFINE_HANDLER(1, 4);
BSP_ISR_DEFINE_HANDLER(1, 5);
BSP_ISR_DEFINE_HANDLER(1, 6);
BSP_ISR_DEFINE_HANDLER(1, 7);
BSP_ISR_DEFINE_HANDLER(1, 8);
BSP_ISR_DEFINE_HANDLER(1, 9);

BSP_ISR_DEFINE_HANDLER(1, 10);
BSP_ISR_DEFINE_HANDLER(1, 11);
BSP_ISR_DEFINE_HANDLER(1, 12);
BSP_ISR_DEFINE_HANDLER(1, 13);
BSP_ISR_DEFINE_HANDLER(1, 14);
BSP_ISR_DEFINE_HANDLER(1, 15);
BSP_ISR_DEFINE_HANDLER(1, 16);
BSP_ISR_DEFINE_HANDLER(1, 17);
BSP_ISR_DEFINE_HANDLER(1, 18);
BSP_ISR_DEFINE_HANDLER(1, 19);

BSP_ISR_DEFINE_HANDLER(1, 20);
BSP_ISR_DEFINE_HANDLER(1, 21);
BSP_ISR_DEFINE_HANDLER(1, 22);
BSP_ISR_DEFINE_HANDLER(1, 23);
BSP_ISR_DEFINE_HANDLER(1, 24);
BSP_ISR_DEFINE_HANDLER(1, 25);
BSP_ISR_DEFINE_HANDLER(1, 26);
BSP_ISR_DEFINE_HANDLER(1, 27);
BSP_ISR_DEFINE_HANDLER(1, 28);
BSP_ISR_DEFINE_HANDLER(1, 29);

BSP_ISR_DEFINE_HANDLER(1, 30);
BSP_ISR_DEFINE_HANDLER(1, 31);

/* ------------------------------------------------------------------------------------------------
 * CORE 2
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 2)

BSP_ISR_VECTOR_TABLE(2);

BSP_ISR_DEFINE_HANDLER(2, 0);
BSP_ISR_DEFINE_HANDLER(2, 1);
BSP_ISR_DEFINE_HANDLER(2, 2);
BSP_ISR_DEFINE_HANDLER(2, 3);
BSP_ISR_DEFINE_HANDLER(2, 4);
BSP_ISR_DEFINE_HANDLER(2, 5);
BSP_ISR_DEFINE_HANDLER(2, 6);
BSP_ISR_DEFINE_HANDLER(2, 7);
BSP_ISR_DEFINE_HANDLER(2, 8);
BSP_ISR_DEFINE_HANDLER(2, 9);

BSP_ISR_DEFINE_HANDLER(2, 10);
BSP_ISR_DEFINE_HANDLER(2, 11);
BSP_ISR_DEFINE_HANDLER(2, 12);
BSP_ISR_DEFINE_HANDLER(2, 13);
BSP_ISR_DEFINE_HANDLER(2, 14);
BSP_ISR_DEFINE_HANDLER(2, 15);
BSP_ISR_DEFINE_HANDLER(2, 16);
BSP_ISR_DEFINE_HANDLER(2, 17);
BSP_ISR_DEFINE_HANDLER(2, 18);
BSP_ISR_DEFINE_HANDLER(2, 19);

BSP_ISR_DEFINE_HANDLER(2, 20);
BSP_ISR_DEFINE_HANDLER(2, 21);
BSP_ISR_DEFINE_HANDLER(2, 22);
BSP_ISR_DEFINE_HANDLER(2, 23);
BSP_ISR_DEFINE_HANDLER(2, 24);
BSP_ISR_DEFINE_HANDLER(2, 25);
BSP_ISR_DEFINE_HANDLER(2, 26);
BSP_ISR_DEFINE_HANDLER(2, 27);
BSP_ISR_DEFINE_HANDLER(2, 28);
BSP_ISR_DEFINE_HANDLER(2, 29);

BSP_ISR_DEFINE_HANDLER(2, 30);
BSP_ISR_DEFINE_HANDLER(2, 31);

/* ------------------------------------------------------------------------------------------------
 * CORE 3
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 3)

BSP_ISR_VECTOR_TABLE(3);

BSP_ISR_DEFINE_HANDLER(3, 0);
BSP_ISR_DEFINE_HANDLER(3, 1);
BSP_ISR_DEFINE_HANDLER(3, 2);
BSP_ISR_DEFINE_HANDLER(3, 3);
BSP_ISR_DEFINE_HANDLER(3, 4);
BSP_ISR_DEFINE_HANDLER(3, 5);
BSP_ISR_DEFINE_HANDLER(3, 6);
BSP_ISR_DEFINE_HANDLER(3, 7);
BSP_ISR_DEFINE_HANDLER(3, 8);
BSP_ISR_DEFINE_HANDLER(3, 9);

BSP_ISR_DEFINE_HANDLER(3, 10);
BSP_ISR_DEFINE_HANDLER(3, 11);
BSP_ISR_DEFINE_HANDLER(3, 12);
BSP_ISR_DEFINE_HANDLER(3, 13);
BSP_ISR_DEFINE_HANDLER(3, 14);
BSP_ISR_DEFINE_HANDLER(3, 15);
BSP_ISR_DEFINE_HANDLER(3, 16);
BSP_ISR_DEFINE_HANDLER(3, 17);
BSP_ISR_DEFINE_HANDLER(3, 18);
BSP_ISR_DEFINE_HANDLER(3, 19);

BSP_ISR_DEFINE_HANDLER(3, 20);
BSP_ISR_DEFINE_HANDLER(3, 21);
BSP_ISR_DEFINE_HANDLER(3, 22);
BSP_ISR_DEFINE_HANDLER(3, 23);
BSP_ISR_DEFINE_HANDLER(3, 24);
BSP_ISR_DEFINE_HANDLER(3, 25);
BSP_ISR_DEFINE_HANDLER(3, 26);
BSP_ISR_DEFINE_HANDLER(3, 27);
BSP_ISR_DEFINE_HANDLER(3, 28);
BSP_ISR_DEFINE_HANDLER(3, 29);

BSP_ISR_DEFINE_HANDLER(3, 30);
BSP_ISR_DEFINE_HANDLER(3, 31);

/* ------------------------------------------------------------------------------------------------
 * CORE 4
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 4)

BSP_ISR_VECTOR_TABLE(4);

BSP_ISR_DEFINE_HANDLER(4, 0);
BSP_ISR_DEFINE_HANDLER(4, 1);
BSP_ISR_DEFINE_HANDLER(4, 2);
BSP_ISR_DEFINE_HANDLER(4, 3);
BSP_ISR_DEFINE_HANDLER(4, 4);
BSP_ISR_DEFINE_HANDLER(4, 5);
BSP_ISR_DEFINE_HANDLER(4, 6);
BSP_ISR_DEFINE_HANDLER(4, 7);
BSP_ISR_DEFINE_HANDLER(4, 8);
BSP_ISR_DEFINE_HANDLER(4, 9);

BSP_ISR_DEFINE_HANDLER(4, 10);
BSP_ISR_DEFINE_HANDLER(4, 11);
BSP_ISR_DEFINE_HANDLER(4, 12);
BSP_ISR_DEFINE_HANDLER(4, 13);
BSP_ISR_DEFINE_HANDLER(4, 14);
BSP_ISR_DEFINE_HANDLER(4, 15);
BSP_ISR_DEFINE_HANDLER(4, 16);
BSP_ISR_DEFINE_HANDLER(4, 17);
BSP_ISR_DEFINE_HANDLER(4, 18);
BSP_ISR_DEFINE_HANDLER(4, 19);

BSP_ISR_DEFINE_HANDLER(4, 20);
BSP_ISR_DEFINE_HANDLER(4, 21);
BSP_ISR_DEFINE_HANDLER(4, 22);
BSP_ISR_DEFINE_HANDLER(4, 23);
BSP_ISR_DEFINE_HANDLER(4, 24);
BSP_ISR_DEFINE_HANDLER(4, 25);
BSP_ISR_DEFINE_HANDLER(4, 26);
BSP_ISR_DEFINE_HANDLER(4, 27);
BSP_ISR_DEFINE_HANDLER(4, 28);
BSP_ISR_DEFINE_HANDLER(4, 29);

BSP_ISR_DEFINE_HANDLER(4, 30);
BSP_ISR_DEFINE_HANDLER(4, 31);

/* ------------------------------------------------------------------------------------------------
 * CORE 5
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 5)

BSP_ISR_VECTOR_TABLE(5);

BSP_ISR_DEFINE_HANDLER(5, 0);
BSP_ISR_DEFINE_HANDLER(5, 1);
BSP_ISR_DEFINE_HANDLER(5, 2);
BSP_ISR_DEFINE_HANDLER(5, 3);
BSP_ISR_DEFINE_HANDLER(5, 4);
BSP_ISR_DEFINE_HANDLER(5, 5);
BSP_ISR_DEFINE_HANDLER(5, 6);
BSP_ISR_DEFINE_HANDLER(5, 7);
BSP_ISR_DEFINE_HANDLER(5, 8);
BSP_ISR_DEFINE_HANDLER(5, 9);

BSP_ISR_DEFINE_HANDLER(5, 10);
BSP_ISR_DEFINE_HANDLER(5, 11);
BSP_ISR_DEFINE_HANDLER(5, 12);
BSP_ISR_DEFINE_HANDLER(5, 13);
BSP_ISR_DEFINE_HANDLER(5, 14);
BSP_ISR_DEFINE_HANDLER(5, 15);
BSP_ISR_DEFINE_HANDLER(5, 16);
BSP_ISR_DEFINE_HANDLER(5, 17);
BSP_ISR_DEFINE_HANDLER(5, 18);
BSP_ISR_DEFINE_HANDLER(5, 19);

BSP_ISR_DEFINE_HANDLER(5, 20);
BSP_ISR_DEFINE_HANDLER(5, 21);
BSP_ISR_DEFINE_HANDLER(5, 22);
BSP_ISR_DEFINE_HANDLER(5, 23);
BSP_ISR_DEFINE_HANDLER(5, 24);
BSP_ISR_DEFINE_HANDLER(5, 25);
BSP_ISR_DEFINE_HANDLER(5, 26);
BSP_ISR_DEFINE_HANDLER(5, 27);
BSP_ISR_DEFINE_HANDLER(5, 28);
BSP_ISR_DEFINE_HANDLER(5, 29);

BSP_ISR_DEFINE_HANDLER(5, 30);
BSP_ISR_DEFINE_HANDLER(5, 31);

#endif // CORE 5

#endif // CORE 4

#endif // CORE 3

#endif // CORE 2

#endif // CORE 1

#endif // CORE 0


#endif /* #ifndef BSP_ISR_SUPPORT_DISABLED */

