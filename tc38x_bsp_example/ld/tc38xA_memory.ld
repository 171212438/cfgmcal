/**************************************************************************************************
 * FILE: tc38xA_memory.ld
 * UC ARCHITECTURE:
 *    TC3xx
 * UC FAMILY:
 *    TC38xA
 * DESCRIPTION:
 *    uC family memory region definition file.
 *    Not all available memory regions are defined, only those used in BSP example.
 *    It includes the application linker files locating sections to memory regions at the end.
 * FILE HISTORY:
 *    V1  05.2020  RO
 *    - Base reference example
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

OUTPUT_FORMAT("elf32-tricore")  /* 告诉链接器: 最终产物是32位TriCore的ELF文件. */
OUTPUT_ARCH(tricore)  /* 给ELF标注"这程序属于TriCore架构" */
ENTRY(_crt0_reset)  /* 定义程序的"入口符号"——ELF的起始地址由这个符号决定 */
                    /* 如果把ENTRY改成main, 调试器可能能直接跑到main, 但启动汇编被跳过, 堆栈/内存未初始化, 必炸 */

/* ================================================================================================
 * TC38XA MEMORY REGIONS
 * ==============================================================================================*/

MEMORY
{
  /* r/w/x/l: 读/写/可执行/可做装载区 */
  /* User configuration block - BMHD headers only */
  ucb_bmhd_orig (rx): org = 0xaf400000, len = 2K
  ucb_bmhd_copy (rx): org = 0xaf401000, len = 2K

  /* Program Flash memory - cached region 这些地址区间是带I/D Cache的别名, CPU取指/读常量更快 */
  /* 四块PFlash(flash0..3)是同一片片上程序闪存被分成的四个Bank/地址段, 所有CPU都能访问任意Bank */
  int_flash0 (rx): org = 0x80000000, len = 3M
  int_flash1 (rx): org = 0x80300000, len = 3M
  int_flash2 (rx): org = 0x80600000, len = 3M
  int_flash3 (rx): org = 0x80900000, len = 1M

  /* Program scratchpad memories 用来放: 极度时间敏感的中断/临界函数, 启动小段代码 */
  pspr_cpu0 (rx): org = 0x70100000, len = 64K
  pspr_cpu1 (rx): org = 0x60100000, len = 64K
  pspr_cpu2 (rx): org = 0x50100000, len = 64K
  pspr_cpu3 (rx): org = 0x40100000, len = 64K

  /* Data scratchpad memories 用来放: 栈(USTACK/ISTACK), CSA(上下文保存区), 小而频繁访问的全局/局部数据, 紧凑环形缓冲 */
  dspr_cpu0 (w!x): org = 0x70000000, len = 240K
  dspr_cpu1 (w!x): org = 0x60000000, len = 240K
  dspr_cpu2 (w!x): org = 0x50000000, len = 96K
  dspr_cpu3 (w!x): org = 0x40000000, len = 96K

  /* Distributed LMU RAM - Non-Cached regions selected, Local core access is always non-cached */
  /* 用来放: 多核间共享但实时一致性要求高的数据(标志位、IPC邮箱、信号量、轻量环形Buffer). 不用担心cache同步, 因为这里天生non-cache. */
  dlmu_cpu0 (wx): org = 0x90000000, len = 64K
  dlmu_cpu1 (wx): org = 0x90010000, len = 64K
  dlmu_cpu2 (wx): org = 0x90020000, len = 64K
  dlmu_cpu3 (wx): org = 0x90030000, len = 64K

  /* Global LMU - Local memory Unit 同一块物理RAM(比如128 KB), 两套地址能访问:一套经缓存(cached), 一套旁路缓存(non‑cached) */
  lmu0_cached (wx): org = 0x90040000, len = 128K  /* CPU自己用、追求速度 → 放lmu0_cached(有cache) */
  lmu0_noncached (wx): org = 0xB0040000, len = 128K  /* 和DMA/外设/别的核共享, 怕cache不一致 */

  /* Periphery memory space region 只是标记外设空间地址范围(0xF0000000…0xFFFFF000),len=0说明不放节 */
  periphery_base : org = 0xF0000000, len = 0
  periphery_end : org = 0xFFFF0000, len = 0
}

/* memory mirrors describe same physical memory accessible by different addresses */
/* 告诉链接器"这俩是同一块物理内存的镜像地址". 工具就不会当两块独立RAM乱分配 */ 
REGION_MIRROR("lmu0_cached", "lmu0_noncached")

/* ================================================================================================
 * MEMORY REGION SYMBOLS
 * ==============================================================================================*/

/* Internal Flash memory */
INT_FLASH_MEMORY_BASE = ORIGIN(int_flash0);  /* 把符号INT_FLASH_MEMORY_BASE设成MEMORY{}里区域int_flash0的起始地址.ORIGIN(name)是ld的内置函数, 返回指定内存区域的起点 */
                                             /* 给后面的段放置规则(通常在SECTIONS{}里)提供"Flash从哪开始"的基准地址, 避免写死魔法数 */
INT_FLASH_MEMORY_SIZE = 10M;  /* 10M就是10×1024×1024 */

/* ================================================================================================
 * INCLUDE OF APPLICATION LINKER FILE
 * ==============================================================================================*/

INCLUDE tc38x_bsp_example.ld  /* 把另一个链接脚本文件的内容原地并入当前脚本.ld的INCLUDE会在当前目录和-L指定的路径里搜, 最多能嵌套10层 */
