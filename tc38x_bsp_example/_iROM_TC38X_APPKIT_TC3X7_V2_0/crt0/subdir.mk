################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../crt0/uc_tc38_bmhd.c 

S_UPPER_SRCS += \
../crt0/tc3x_crt0.S 

OBJS += \
./crt0/tc3x_crt0.o \
./crt0/uc_tc38_bmhd.o 

C_DEPS += \
./crt0/uc_tc38_bmhd.d 


# Each subdirectory must supply rules for building sources it contributes
crt0/%.o: ../crt0/%.S crt0/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TriCore Assembler'
	"D:\Software\HighTec\toolchains\tricore\v4.9.3.0-infineon-1.0/bin/tricore-gcc" -c @asmincludelist.optfile -Wa,--gdwarf-2 -mtc162 -Wa,--insn32-preferred -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

crt0/%.o: ../crt0/%.c crt0/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TriCore C Compiler'
	"D:\Software\HighTec\toolchains\tricore\v4.9.3.0-infineon-1.0/bin/tricore-gcc" -c @ccompilerincludelist.optfile -fno-common -fno-short-enums -O0 -g2 -W -Wall -Wextra -Wdiv-by-zero -Warray-bounds -Wcast-align -Wignored-qualifiers -Wformat -Wformat-security -pipe -DAPPKIT_TC3X7_V2_0 -D__TC38XX__ -fshort-double -mtc162 -mversion-info -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


