################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/isr/bsp_isr.c \
../bsp/isr/bsp_isr_vector_table.c \
../bsp/isr/bsp_traps_tc3.c 

OBJS += \
./bsp/isr/bsp_isr.o \
./bsp/isr/bsp_isr_vector_table.o \
./bsp/isr/bsp_traps_tc3.o 

C_DEPS += \
./bsp/isr/bsp_isr.d \
./bsp/isr/bsp_isr_vector_table.d \
./bsp/isr/bsp_traps_tc3.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/isr/%.o: ../bsp/isr/%.c bsp/isr/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TriCore C Compiler'
	"D:\Software\HighTec\toolchains\tricore\v4.9.3.0-infineon-1.0/bin/tricore-gcc" -c @ccompilerincludelist.optfile -fno-common -fno-short-enums -O0 -g2 -W -Wall -Wextra -Wdiv-by-zero -Warray-bounds -Wcast-align -Wignored-qualifiers -Wformat -Wformat-security -pipe -DAPPKIT_TC3X7_V2_0 -D__TC38XX__ -fshort-double -mtc162 -mversion-info -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


