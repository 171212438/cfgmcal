

/**
 * \file IfxSrc_regdef.h
 * \copyright Copyright (c) 2020 Infineon Technologies AG. All rights reserved.
 * Version: TC38XA_UM_V1.5.0.R0
 * Specification: TC3xx User Manual V1.5.0
 * MAY BE CHANGED BY USER [yes/no]: No
 
 * SRC(Service Request Controller)= "外设中断的配电盘+路由器". 外设把"服务请求"(IRQ事件)丢给SRC, SRC负责两件事:
 * 把请求路由给谁(哪个CPU/服务提供者);
 * 以什么优先级送出去, 并决定要不要送(使能/禁止); 
 * 外设:
 *  STM、GTM、EVADC、ASCLIN、QSPI、I2C、MCMCAN、GETH、CCU6、GPT12、DMA、SMU、SCU/PMU等
 * 服务请求:
 * 	STM: 比较器命中(CMP0、CMP1) → SR0/SR1;
 * 	GTM: TOM/ATOM PWM更新或溢出、中断触发; TIM捕获事件; TBU溢出等 → 多条GTM专用SR;
 * 	EVADC/EDSADC: 转换完成、队列/结果FIFO事件、通道/组溢出、错误警报 → 每个组/通道有各自SR;
 * 	ASCLIN(UART/LIN/SPI多模): RX/TX完成、接收超时、帧/校验错误、LIN唤醒/睡眠事件 → RX/TX/ERR专用SR;
 * 	QSPI: TX/RX阈值、水位/下溢、帧结束、错误 → 通常有TX/RX/ERR/FRAME相关SR;
 * 	I2C: 传输完成、仲裁丢失、NACK、错误等;
 * 	MCMCAN: 接收FIFO/缓冲满、传输完成、错误/状态变化等(MCAN有多路SR, 按节点/缓冲区分);
 * 	GETH(千兆以太网): DMA通道RX/TX完成、中断聚合、错误/状态变更;
 * 	CCU6/GPT12: 定时器溢出/比较匹配、捕获事件;
 * 	DMA: 通道传输完成/错误请求(DMA自己也有SR线);
 * 	SMU: 安全报警分组路由到CPU的服务请求(高优先级处理);
 * 	SCU/PMU/RCU: 某些时钟/电源/复位相关的可选服务请求(取决于配置);
 * 	其他模块(MSC、HSSL、QSPI从机等)也各自有TX/RX/ERR类SR;

 * \defgroup IfxSfr_Src_Registers_Bitfields Bitfields
 * \ingroup IfxSfr_Src_Registers
 * 
 * \defgroup IfxSfr_Src_Registers_union Register unions
 * \ingroup IfxSfr_Src_Registers
 * 
 * \defgroup IfxSfr_Src_Registers_struct Memory map
 * \ingroup IfxSfr_Src_Registers */
#ifndef IFXSRC_REGDEF_H
#define IFXSRC_REGDEF_H 1


#include "Ifx_TypesReg.h"

/* IfxSfr_Src_Registers_Bitfields */
typedef struct _Ifx_SRC_SRCR_Bits {
	Ifx_UReg_32Bit SRPN : 8;				 /* [7:0] 优先级号. 数值越大, 优先级越高; 对CPU来说0x01是最低可服务(0x00不会被CPU服务); 对DMA来说0x00触发通道0, 且SRPN不能大于实现的DMA最大通道号 (rw) */
	Ifx_UReg_32Bit reserved_8 : 2;	 /* [9:8] internal Reserved */
	Ifx_UReg_32Bit SRE : 1; 				 /* [10:10] 使能这根服务请求线. 1=允许参与仲裁; 0=不送去服务提供者(但SRR仍可由硬件/软件置位, 可用于轮询模式) (rw) */
	Ifx_UReg_32Bit TOS : 3; 				 /* [13:11] 路由到谁(服务提供者). 000b→CPU0, 010b→CPU1, 011b→CPU2, 100b→CPU3, 001b→DMA, 其他保留 (rw) */
	Ifx_UReg_32Bit reserved_14 : 2;  /* [15:14] internal Reserved */
	Ifx_UReg_32Bit ECC : 5; 				 /* [20:16] ECC校验位(写入SRC的不同粒度会更新它). 一般不用手改, 由硬件按写入自动更新 (rwh) */
	Ifx_UReg_32Bit reserved_21 : 3;  /* [23:21] internal Reserved */
	Ifx_UReg_32Bit SRR : 1; 				 /* [24:24] 请求标志(只读显示). 1表示当前有待服务的请求; 是否被送去服务提供者取决于SRE (rh) */
	Ifx_UReg_32Bit CLRR : 1;				 /* [25:25] 清请求位. 写1清SRR; 读总是0; 如果同时和SETR一起写1, 则"不改变SRR". 常用于轮询模式的确认, 或初始化时清旧请求 (w) */
	Ifx_UReg_32Bit SETR : 1;				 /* [26:26] 置请求位(软件触发). 写1会置SRR=1且SWS=1; 读总是0; 若和CLRR同时写1, 不改变SRR. 常用于自测中断链路 (w) */
	Ifx_UReg_32Bit IOV : 1; 				 /* [27:27] 触发溢出标志(只读). 当已有挂起请求时又来了新的触发(硬件触发或你写了SETR), 会置1; 用来发现请求堆积 (rh) */
	Ifx_UReg_32Bit IOVCLR : 1;			 /* [28:28] 清上述溢出标志. 写1清IOV (w) */
	Ifx_UReg_32Bit SWS : 1; 				 /* [29:29] 软件置位"粘滞"标志(只读). 当通过SETR置过SRR, 这个位被置1, 用于区分软件触发与硬件触发. 用SWSCLR清 (rh) */
	Ifx_UReg_32Bit SWSCLR : 1;			 /* [30:30] 清SWS. 写1清SWS (w) */
	Ifx_UReg_32Bit reserved_31 : 1;  /* [31:31] internal Reserved */
} Ifx_SRC_SRCR_Bits;

/* IfxSfr_src_Registers_union */
typedef union {
  Ifx_UReg_32Bit U;     /* 把整个寄存器当成无符号32位值一次性读/写 */
  Ifx_SReg_32Bit I;     /* 把整个寄存器当成有符号32位值访问. 几乎不用, 只为类型对齐 */
  Ifx_SRC_SRCR_Bits B;  /* 按位段名访问(SRPN/SRE/TOS/CLRR/SETR/...), 可读性高 */
} Ifx_SRC_SRCR;


/* SRC里"CPU软件断点(Software Breakpoint)服务请求"的入口表——每个CPU核各有一个SB(Software Breakpoint)条目
 * SRC.CPU.CPU[x].SB → 第x个CPU的SB服务请求寄存器(SRCR版);
 * 这些SB条目在SRC地址空间的最前面, 偏移0x00000 + x * 4; 在Debug Reset下复位 */
typedef volatile struct _Ifx_SRC_CPU_CPU {
  Ifx_SRC_SRCR SB;  /* 0, CPUx Software Breakpoint Service Request*/
} Ifx_SRC_CPU_CPU;
typedef volatile struct _Ifx_SRC_CPU {
  Ifx_SRC_CPU_CPU CPU[4];  /* 0, CPUx Software Breakpoint Service Request*/
} Ifx_SRC_CPU;


/* SRC里给"Cerberus(片上安全模块/HSM)"预留的两根服务请求线的寄存器映射 */
typedef volatile struct _Ifx_SRC_CERBERUS_CERBERUS {
  Ifx_SRC_SRCR SR[2];  /* 0, 表示Cerberus有2个SR条目: SRC_CERBERUS0、SRC_CERBERUS1, 每个SR[y]本质就是一个SRCR(Ifx_SRC_SRCR)*/
} Ifx_SRC_CERBERUS_CERBERUS;
typedef volatile struct _Ifx_SRC_CERBERUS
{
  Ifx_SRC_CERBERUS_CERBERUS CERBERUS;  /* 0, */
} Ifx_SRC_CERBERUS;


/* SRC里ASCLIN模块的中断入口表
 * 每个ASCLIN实例(ASCLINx)固定有3根服务请求线: TX(发送)、RX(接收)、ERR(错误)
 * 结构体里ASCLIN[12]意味着最多12个ASCLIN实例(按器件实际数量为准), 每个实例都有这三条SRCR寄存器;
 * 这三个条目都是标准SRCR: 能设优先级(SRPN)、路由/目标(TOS)、使能(SRE), 还支持软件置位/清位(SETR/CLRR)等 */
typedef volatile struct _Ifx_SRC_ASCLIN_ASCLIN
{
  Ifx_SRC_SRCR TX;   /* 0, ASCLINx Transmit Service Request*/
  Ifx_SRC_SRCR RX;   /* 4, ASCLINx Receive Service Request*/
  Ifx_SRC_SRCR ERR;  /* 8, ASCLINx Error Service Request*/
} Ifx_SRC_ASCLIN_ASCLIN;
typedef volatile struct _Ifx_SRC_ASCLIN
{
  Ifx_SRC_ASCLIN_ASCLIN ASCLIN[12];  /* 0, ASCLINx Error Service Request*/
} Ifx_SRC_ASCLIN;


/* SRC里QSPI模块的中断入口表. 每个QSPIx有5条服务请求线, 分别管不同类型的事件:
 * TX: 发送侧事件(TX FIFO低于阈值、取走BACON/DATA后的"需要补粮"等)走这条线. 支持单次/批量/组合三种触发模式;
 * RX: 接收侧事件(RX FIFO高于阈值、新数据到达等)走这条线, 触发模式与TX类似;
 * ERR: 所有错误条件(TX/RX溢出/下溢、协议错误等)汇总到这条线;
 * PT(Phase Transition): 相位转换事件. QSPI一帧有若干阶段(IdleA/IdleB/Leading/Data/Trailing), 可选择两处相位作为触发点(PT1/PT2), 常用于精确对齐帧边界或做时序测量. 主模式还能在SLSI释放等场景下触发;
 * U(User Defined): 用户自定义中断, 本质是"在PT1事件发生期间, 如果BACON. UINT置1, 就把PT1也送到U这条线上". 用它可以把某些帧的PT1单独分流到另一根SR, 做更灵活的分工; */
typedef volatile struct _Ifx_SRC_QSPI_QSPI
{
  Ifx_SRC_SRCR TX;   /* 0, QSPIx Transmit Service Request 发送侧事件的集合 */
  Ifx_SRC_SRCR RX;   /* 4, QSPIx Receive Service Request 接收侧事件的集合 */
  Ifx_SRC_SRCR ERR;  /* 8, QSPIx Error Service Request 错误/异常的集合 */
  Ifx_SRC_SRCR PT;   /* C, QSPIx Phase Transition Service Request 相位转换事件 */
  Ifx_SRC_SRCR U;    /* 10, QSPIx User Defined Service Request 用户自定义事件*/
} Ifx_SRC_QSPI_QSPI;
typedef volatile struct _Ifx_SRC_QSPI
{
  Ifx_SRC_QSPI_QSPI QSPI[5];  /* 0, QSPIx User Defined Service Request*/
} Ifx_SRC_QSPI;


/* HSCT(高速串行通信收发器)在SRC里的中断入口映射
 * 每个HSCT实例只有一根服务请求线, 结构体里就是SRC.HSCT.HSCT[0].SR这一条
 * 它把HSCT内部的各类可中断事件(收发状态、FIFO阈值、协议/超时/错误等)汇聚到同一根SR线, 由用户在SRC侧决定送到哪个核、用多大优先级、是否使能 */
typedef volatile struct _Ifx_SRC_HSCT_HSCT
{
  Ifx_SRC_SRCR SR;  /* 0, HSCT0 Service Request*/
} Ifx_SRC_HSCT_HSCT;
typedef volatile struct _Ifx_SRC_HSCT
{
  Ifx_SRC_HSCT_HSCT HSCT[1];  /* 0, HSCT0 Service Request*/
} Ifx_SRC_HSCT;


/* HSSL(High-Speed Serial Link)在SRC里的中断入口映射. 每个HSSL实例里有4个通道(CH[0.. 3]), 每个通道各自有四条"服务请求线", 外加实例级的一条异常线: */
typedef volatile struct _Ifx_SRC_HSSL_HSSL_CH
{
  Ifx_SRC_SRCR COK;  /* 0, 该通道上一条命令(读/写/流通道帧等)正常完成时触发. 适合在此处做"命令完成→状态机前进";*/
  Ifx_SRC_SRCR RDI;  /* 4, 对端返回的读数据到达(或到达阈值)时触发. 适合搬运RX FIFO/读结果. 高吞吐时建议配DMA;*/
  Ifx_SRC_SRCR ERR;  /* 8, 错误汇总. 包括超时、CRC/校验、协议/序列错误、FIFO溢出/下溢等都进这根线. 建议设较高优先级并在ISR里做快速收尾/复位;*/
  Ifx_SRC_SRCR TRG;  /* C, 触发中断. 在HSSL内部选择的触发条件(如某状态/阈值/软触发)打到这条线, 用来做轻量的阶段性通知或时间对齐;*/
} Ifx_SRC_HSSL_HSSL_CH;
typedef volatile struct _Ifx_SRC_HSSL_HSSL
{
  Ifx_SRC_HSSL_HSSL_CH CH[4];  /* 0, HSSL0 Channel y Trigger Interrupt Service Request */
  Ifx_SRC_SRCR EXI;            /* 40, 异常(Exception). 跨通道/全局级别事件(严重错误、链路异常)走这条线. 通常放最高优先级, 进入后先停发→复位/重训练; */
} Ifx_SRC_HSSL_HSSL;
typedef volatile struct _Ifx_SRC_HSSL
{
  Ifx_SRC_HSSL_HSSL HSSL[1];   /* 0, HSSLx Exception Service Request*/
} Ifx_SRC_HSSL;


typedef volatile struct _Ifx_SRC_I2C_I2C
{
  Ifx_SRC_SRCR DTR;             /* 0, I2Cx Data Transfer Request*/
  Ifx_SRC_SRCR ERR;             /* 4, I2Cx Error Service Request*/
  Ifx_SRC_SRCR P;               /* 8, I2Cx Protocol Service Request*/
  Ifx_UReg_8Bit reserved_C[4];  /* C, internal Reserved */
} Ifx_SRC_I2C_I2C;
typedef volatile struct _Ifx_SRC_I2C
{
  Ifx_SRC_I2C_I2C I2C[2];       /* 0, */
} Ifx_SRC_I2C;


typedef volatile struct _Ifx_SRC_SENT_SENT
{
  Ifx_SRC_SRCR SR;              /* 0, SENT TRIGx Service Request*/
} Ifx_SRC_SENT_SENT;
typedef volatile struct _Ifx_SRC_SENT
{
  Ifx_SRC_SENT_SENT SENT[10];   /* 0, SENT TRIGx Service Request*/
} Ifx_SRC_SENT;


/* MSC(Motor/Sensor串行通信)在SRC里的中断入口表
 * 每个MSC实例(MSC0.. MSC2, 最多3个)各有5条服务请求线, 用来把不同类别的事件从 MSC模块送到CPU/DMA
 *  上行(US/Upstream)事件: 从外设→MCU的接收就绪/阈值到达、新数据到达等;
 *  下行(DS/Downstream)事件: MCU→外设的发送就绪/TX FIFO低水位等;
 *  错误/异常: 如CRC/帧/奇偶、超时、冲突、下/上溢;
 *  触发/相位/状态变化: 协议状态机的阶段性触发(如帧边界、采样点), 常用于对齐时间;
 *  ABRA/波特自适应或特殊控制: 与波特率自动调整、链路"粘连"事件相关的汇总; */
typedef volatile struct _Ifx_SRC_MSC_MSC
{
  Ifx_SRC_SRCR SR[5];           /* 0, MSCx Service Request y*/
} Ifx_SRC_MSC_MSC;
typedef volatile struct _Ifx_SRC_MSC
{
  Ifx_SRC_MSC_MSC MSC[3];       /* 0, */
} Ifx_SRC_MSC;


typedef volatile struct _Ifx_SRC_CCU6_CCU
{
  Ifx_SRC_SRCR SR[4];           /* 0, CCUx Service Request y*/
} Ifx_SRC_CCU6_CCU;
typedef volatile struct _Ifx_SRC_CCU6
{
  Ifx_SRC_CCU6_CCU CCU[2];      /* 0, */
} Ifx_SRC_CCU6;


typedef volatile struct _Ifx_SRC_GPT12_GPT12
{
  Ifx_SRC_SRCR CIRQ;  /* 0, GPT120 CAPREL Service Request*/
  Ifx_SRC_SRCR T2;    /* 4, GPT120 Timer 2 Service Request*/
  Ifx_SRC_SRCR T3;    /* 8, GPT120 Timer 3 Service Request*/
  Ifx_SRC_SRCR T4;    /* C, GPT120 Timer 4 Service Request*/
  Ifx_SRC_SRCR T5;    /* 10, GPT120 Timer 5 Service Request*/
  Ifx_SRC_SRCR T6;    /* 14, GPT120 Timer 6 Service Request*/
} Ifx_SRC_GPT12_GPT12;
typedef volatile struct _Ifx_SRC_GPT12
{
  Ifx_SRC_GPT12_GPT12 GPT12[1];  /* 0, GPT120 Timer 6 Service Request*/
} Ifx_SRC_GPT12;


typedef volatile struct _Ifx_SRC_STM_STM
{
  Ifx_SRC_SRCR SR[2];      /* 0, System Timer x Service Request y*/
} Ifx_SRC_STM_STM;
typedef volatile struct _Ifx_SRC_STM
{
  Ifx_SRC_STM_STM STM[4];  /* 0, */
} Ifx_SRC_STM;


typedef volatile struct _Ifx_SRC_FCE_FCE0
{
  Ifx_SRC_SRCR SR;        /* 0, FCE0 Error Service Request*/
} Ifx_SRC_FCE_FCE0;
typedef volatile struct _Ifx_SRC_FCE
{
  Ifx_SRC_FCE_FCE0 FCE0;  /* 0, FCE0 Error Service Request*/
} Ifx_SRC_FCE;


typedef volatile struct _Ifx_SRC_DMA_DMA
{
  Ifx_SRC_SRCR ERR[4];            /* 0, DMA Error Service Request y*/
  Ifx_UReg_8Bit reserved_10[32];  /* 10, internal Reserved */
  Ifx_SRC_SRCR CH[128];           /* 30, DMA Channel  y Service Request*/
} Ifx_SRC_DMA_DMA;
typedef volatile struct _Ifx_SRC_DMA
{
  Ifx_SRC_DMA_DMA DMA[1];         /* 0, */
} Ifx_SRC_DMA;


typedef volatile struct _Ifx_SRC_GETH_GETH
{
  Ifx_SRC_SRCR SR[10];        /* 0, GETH Service Request y*/
} Ifx_SRC_GETH_GETH;
typedef volatile struct _Ifx_SRC_GETH
{
  Ifx_SRC_GETH_GETH GETH[1];  /* 0, */
} Ifx_SRC_GETH;


typedef volatile struct _Ifx_SRC_CAN_CAN
{
  Ifx_SRC_SRCR INT[16];    /* 0, CANx Service Request y*/
} Ifx_SRC_CAN_CAN;
typedef volatile struct _Ifx_SRC_CAN
{
  Ifx_SRC_CAN_CAN CAN[3];  /* 0, */
} Ifx_SRC_CAN;


typedef volatile struct _Ifx_SRC_VADC_G
{
  Ifx_SRC_SRCR SR0;  /* 0, EVADC Group x Service Request y*/
  Ifx_SRC_SRCR SR1;  /* 4, EVADC Group x Service Request y*/
  Ifx_SRC_SRCR SR2;  /* 8, EVADC Group x Service Request y*/
  Ifx_SRC_SRCR SR3;  /* C, EVADC Group x Service Request y*/
} Ifx_SRC_VADC_G;
typedef volatile struct _Ifx_SRC_VADC_FC
{
  Ifx_SRC_SRCR SR0;  /* 0, EVADC Fast Compare x Service Request SR0*/
} Ifx_SRC_VADC_FC;
typedef volatile struct _Ifx_SRC_VADC
{
  Ifx_SRC_VADC_G G[12];           /* 0, */
  Ifx_SRC_VADC_FC FC[4];          /* C0, EVADC Fast Compare x Service Request SR0*/
  Ifx_UReg_8Bit reserved_D0[16];  /* D0, internal Reserved */
  Ifx_SRC_VADC_G CG[2];           /* E0, */
} Ifx_SRC_VADC;


typedef volatile struct _Ifx_SRC_DSADC_DSADC
{
  Ifx_SRC_SRCR SRM;               /* 0, DSADC SRMx Service Request*/
  Ifx_SRC_SRCR SRA;               /* 4, DSADC SRAx Service Request*/
} Ifx_SRC_DSADC_DSADC;
typedef volatile struct _Ifx_SRC_DSADC
{
  Ifx_SRC_DSADC_DSADC DSADC[10];  /* 0, DSADC SRAx Service Request*/
} Ifx_SRC_DSADC;


typedef volatile struct _Ifx_SRC_ERAY_ERAY
{
  Ifx_SRC_SRCR INT0;   /* 0, E-RAY x Service Request 0*/
  Ifx_SRC_SRCR INT1;   /* 4, E-RAY x Service Request 1*/
  Ifx_SRC_SRCR TINT0;  /* 8, E-RAY x Timer Interrupt 0 Service Request*/
  Ifx_SRC_SRCR TINT1;  /* C, E-RAY x Timer Interrupt 1 Service Request*/
  Ifx_SRC_SRCR NDAT0;  /* 10, E-RAY x New Data 0 Service Request*/
  Ifx_SRC_SRCR NDAT1;  /* 14, E-RAY x New Data 1 Service Request*/
  Ifx_SRC_SRCR MBSC0;  /* 18, E-RAY x Message Buffer Status Changed 0 Service Request*/
  Ifx_SRC_SRCR MBSC1;  /* 1C, E-RAY x Message Buffer Status Changed 1 Service Request*/
  Ifx_SRC_SRCR OBUSY;  /* 20, E-RAY x Output Buffer Busy*/
  Ifx_SRC_SRCR IBUSY;  /* 24, E-RAY x Input Buffer Busy*/
  Ifx_UReg_8Bit reserved_28[8];  /* 28, internal Reserved */
} Ifx_SRC_ERAY_ERAY;
typedef volatile struct _Ifx_SRC_ERAY
{
  Ifx_SRC_ERAY_ERAY ERAY[2];  /* 0, */
} Ifx_SRC_ERAY;


typedef volatile struct _Ifx_SRC_HSM_HSM
{
  Ifx_SRC_SRCR HSM[2];     /* 0, HSM Service Request y*/
} Ifx_SRC_HSM_HSM;
typedef volatile struct _Ifx_SRC_HSM
{
  Ifx_SRC_HSM_HSM HSM[1];  /* 0, */
} Ifx_SRC_HSM;


typedef volatile struct _Ifx_SRC_SCU
{
  Ifx_SRC_SRCR SCUERU[4];  /* 0, SCU ERU Service Request x*/
} Ifx_SRC_SCU;


typedef volatile struct _Ifx_SRC_PMS_PMS
{
  Ifx_SRC_SRCR SR;         /* 0, Power Management System Service Request x*/
} Ifx_SRC_PMS_PMS;
typedef volatile struct _Ifx_SRC_PMS
{
  Ifx_SRC_PMS_PMS PMS[4];  /* 0, Power Management System Service Request x*/
} Ifx_SRC_PMS;


typedef volatile struct _Ifx_SRC_SMU_SMU
{
  Ifx_SRC_SRCR SR[3];  /* 0, SMU Service Request y*/
} Ifx_SRC_SMU_SMU;
typedef volatile struct _Ifx_SRC_SMU
{
  Ifx_SRC_SMU_SMU SMU[1];  /* 0, */
} Ifx_SRC_SMU;


typedef volatile struct _Ifx_SRC_PSI5_PSI5
{
  Ifx_SRC_SRCR SR[8];  /* 0, PSI5 Service Request y*/
} Ifx_SRC_PSI5_PSI5;
typedef volatile struct _Ifx_SRC_PSI5
{
  Ifx_SRC_PSI5_PSI5 PSI5[1];  /* 0, */
} Ifx_SRC_PSI5;


typedef volatile struct _Ifx_SRC_DAM_DAM
{
  Ifx_SRC_SRCR LI0;  /* 0, DAMx Limit 0 Service Request*/
  Ifx_SRC_SRCR RI0;  /* 4, DAMx Ready 0 Service Reques*/
  Ifx_SRC_SRCR LI1;  /* 8, DAMx Limit 1 Service Request*/
  Ifx_SRC_SRCR RI1;  /* C, DAMx Ready 1 Service Request*/
  Ifx_SRC_SRCR DR;   /* 10, DAMx DMA Ready Service Request*/
  Ifx_SRC_SRCR ERR;  /* 14, DAMx Error Service Request*/
} Ifx_SRC_DAM_DAM;
typedef volatile struct _Ifx_SRC_DAM
{
  Ifx_SRC_DAM_DAM DAM[1];  /* 0, */
} Ifx_SRC_DAM;


typedef volatile struct _Ifx_SRC_PSI5S_PSI5S
{
  Ifx_SRC_SRCR SR[8];  /* 0, PSI5-S Service Request y*/
} Ifx_SRC_PSI5S_PSI5S;
typedef volatile struct _Ifx_SRC_PSI5S
{
  Ifx_SRC_PSI5S_PSI5S PSI5S[1];  /* 0, */
} Ifx_SRC_PSI5S;


typedef volatile struct _Ifx_SRC_GPSR_GPSR
{
  Ifx_SRC_SRCR SR[8];  /* 0, General Purpose Group x Service Request y*/
} Ifx_SRC_GPSR_GPSR;
typedef volatile struct _Ifx_SRC_GPSR
{
  Ifx_SRC_GPSR_GPSR GPSR[4];  /* 0, */
} Ifx_SRC_GPSR;


/* IfxSfr_Src_Registers_struct  SRC object */
typedef volatile struct _Ifx_SRC
{
	Ifx_SRC_CPU CPU;									 /* 0, */
	Ifx_UReg_8Bit reserved_10[16];		 /* 10, internal Reserved */
	Ifx_SRC_SRCR SBCU;								 /* 20, SBCU Service Request (SPB Bus Control Unit) */
	Ifx_UReg_8Bit reserved_24[12];		 /* 24, internal Reserved */
	Ifx_SRC_SRCR XBAR0; 							 /* 30, SRI Domain 0 Service Request */
	Ifx_UReg_8Bit reserved_34[12];		 /* 34, internal Reserved */
	Ifx_SRC_CERBERUS CERBERUS;				 /* 40, */
	Ifx_UReg_8Bit reserved_48[8]; 		 /* 48, internal Reserved */
	Ifx_SRC_ASCLIN ASCLIN;						 /* 50, */
	Ifx_UReg_8Bit reserved_E0[12];		 /* E0, internal Reserved */
	Ifx_SRC_SRCR MTUDONE; 						 /* EC, MTU Done Service Request */
	Ifx_SRC_QSPI QSPI;								 /* F0, */
	Ifx_UReg_8Bit reserved_154[44]; 	 /* 154, internal Reserved */
	Ifx_SRC_HSCT HSCT;								 /* 180, */
	Ifx_UReg_8Bit reserved_184[12]; 	 /* 184, internal Reserved */
	Ifx_SRC_HSSL HSSL;								 /* 190, */
	Ifx_UReg_8Bit reserved_1D4[76]; 	 /* 1D4, internal Reserved */
	Ifx_SRC_I2C I2C;									 /* 220, */
	Ifx_SRC_SENT SENT;								 /* 240, */
	Ifx_UReg_8Bit reserved_268[8];		 /* 268, internal Reserved */
	Ifx_SRC_MSC MSC;									 /* 270, */
	Ifx_UReg_8Bit reserved_2AC[20]; 	 /* 2AC, internal Reserved */
	Ifx_SRC_CCU6 CCU6;								 /* 2C0, */
	Ifx_SRC_GPT12 GPT12;							 /* 2E0, */
	Ifx_UReg_8Bit reserved_2F8[8];		 /* 2F8, internal Reserved */
	Ifx_SRC_STM STM;									 /* 300, */
	Ifx_UReg_8Bit reserved_320[16]; 	 /* 320, internal Reserved */
	Ifx_SRC_FCE FCE;									 /* 330, */
	Ifx_UReg_8Bit reserved_334[12]; 	 /* 334, internal Reserved */
	Ifx_SRC_DMA DMA;									 /* 340, */
	Ifx_UReg_8Bit reserved_570[16]; 	 /* 570, internal Reserved */
	Ifx_SRC_GETH GETH;								 /* 580, */
	Ifx_UReg_8Bit reserved_5A8[8];		 /* 5A8, internal Reserved */
	Ifx_SRC_CAN CAN;									 /* 5B0, */
	Ifx_SRC_VADC VADC;								 /* 670, */
	Ifx_SRC_DSADC DSADC;							 /* 770, */
	Ifx_UReg_8Bit reserved_7C0[32]; 	 /* 7C0, internal Reserved */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN12; 	 /* 7E0, ASCLIN12 Error Service Request */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN13; 	 /* 7EC, ASCLIN13 Error Service Request */
	Ifx_UReg_8Bit reserved_7F8[8];		 /* 7F8, internal Reserved */
	Ifx_SRC_ERAY ERAY;								 /* 800, */
	Ifx_SRC_SRCR DMUHOST; 						 /* 860, DMU Host Service Request */
	Ifx_SRC_SRCR DMUFSI;							 /* 864, DMU FSI Service Request */
	Ifx_UReg_8Bit reserved_868[8];		 /* 868, internal Reserved */
	Ifx_SRC_HSM HSM;									 /* 870, */
	Ifx_UReg_8Bit reserved_878[8];		 /* 878, internal Reserved */
	Ifx_SRC_SCU SCU;									 /* 880, */
	Ifx_UReg_8Bit reserved_890[28]; 	 /* 890, internal Reserved */
	Ifx_SRC_SRCR PMSDTS;							 /* 8AC, PMS DTS Service Request */
	Ifx_SRC_PMS PMS;									 /* 8B0, */
	Ifx_SRC_SRCR SCR; 								 /* 8C0, Stand By Controller Service Request */
	Ifx_UReg_8Bit reserved_8C4[12]; 	 /* 8C4, internal Reserved */
	Ifx_SRC_SMU SMU;									 /* 8D0, */
	Ifx_UReg_8Bit reserved_8DC[4];		 /* 8DC, internal Reserved */
	Ifx_SRC_PSI5 PSI5;								 /* 8E0, */
	Ifx_UReg_8Bit reserved_900[16]; 	 /* 900, internal Reserved */
	Ifx_SRC_DAM DAM;									 /* 910, */
	Ifx_UReg_8Bit reserved_928[40]; 	 /* 928, internal Reserved */
	Ifx_SRC_PSI5S PSI5S;							 /* 950, */
	Ifx_UReg_8Bit reserved_970[32]; 	 /* 970, internal Reserved */
	Ifx_SRC_GPSR GPSR;								 /* 990, */
	Ifx_UReg_8Bit reserved_A10[64]; 	 /* A10, internal Reserved */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN14; 	 /* A50, ASCLIN14 Error Service Request */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN15; 	 /* A5C, ASCLIN15 Error Service Request */
	Ifx_UReg_8Bit reserved_A68[8];		 /* A68, internal Reserved */
	Ifx_SRC_SRCR GTM_AEIIRQ;					 /* A70, AEI Shared Service Request */
	Ifx_SRC_SRCR GTM_ARUIRQ[3]; 			 /* A74, ARU Shared Service Request ${w} */
	Ifx_SRC_SRCR GTM_BRCIRQ;					 /* A80, BRC Shared Service Request */
	Ifx_SRC_SRCR GTM_CMBIRQ;					 /* A84, CMP Shared Service Request */
	Ifx_SRC_SRCR GTM_SPEIRQ[4]; 			 /* A88, SPE${w} Shared Service Request */
	Ifx_UReg_8Bit reserved_A98[8];		 /* A98, internal Reserved */
	Ifx_SRC_SRCR GTM_PSM[2][8]; 			 /* AA0, */
	Ifx_UReg_8Bit reserved_AE0[32]; 	 /* AE0, internal Reserved */
	Ifx_SRC_SRCR GTM_DPLL[27];				 /* B00, DPLL Service Request ${w} */
	Ifx_UReg_8Bit reserved_B6C[4];		 /* B6C, internal Reserved */
	Ifx_SRC_SRCR GTM_ERR; 						 /* B70, Error Service Request */
	Ifx_UReg_8Bit reserved_B74[28]; 	 /* B74, internal Reserved */
	Ifx_SRC_SRCR GTM_TIM[7][8]; 			 /* B90, */
	Ifx_UReg_8Bit reserved_C70[32]; 	 /* C70, internal Reserved */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN16; 	 /* C90, ASCLIN16 Error Service Request */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN17; 	 /* C9C, ASCLIN17 Error Service Request */
	Ifx_UReg_8Bit reserved_CA8[8];		 /* CA8, internal Reserved */
	Ifx_SRC_SRCR GTM_MCS[7][8]; 			 /* CB0, */
	Ifx_UReg_8Bit reserved_D90[96]; 	 /* D90, internal Reserved */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN18; 	 /* DF0, ASCLIN18 Error Service Request */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN19; 	 /* DFC, ASCLIN19 Error Service Request */
	Ifx_UReg_8Bit reserved_E08[8];		 /* E08, internal Reserved */
	Ifx_SRC_SRCR GTM_TOM[5][8]; 			 /* E10, */
	Ifx_UReg_8Bit reserved_EB0[32]; 	 /* EB0, internal Reserved */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN20; 	 /* ED0, ASCLIN20 Error Service Request */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN21; 	 /* EDC, ASCLIN21 Error Service Request */
	Ifx_UReg_8Bit reserved_EE8[8];		 /* EE8, internal Reserved */
	Ifx_SRC_SRCR GTM_ATOM[9][4];			 /* EF0, */
	Ifx_UReg_8Bit reserved_F80[48]; 	 /* F80, internal Reserved */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN22; 	 /* FB0, ASCLIN22 Error Service Request */
	Ifx_SRC_ASCLIN_ASCLIN ASCLIN23; 	 /* FBC, ASCLIN23 Error Service Request */
	Ifx_UReg_8Bit reserved_FC8[8];		 /* FC8, internal Reserved */
	Ifx_SRC_SRCR GTM_MCSW[10];				 /* FD0, GTM Multi Channel Sequencer Service Request ${w} */
	Ifx_UReg_8Bit reserved_FF8[4104];  /* FF8, internal Reserved */
} Ifx_SRC;

#endif /* IFXSRC_REGDEF_H */

