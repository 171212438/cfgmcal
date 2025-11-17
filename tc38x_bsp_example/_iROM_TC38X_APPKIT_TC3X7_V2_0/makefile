################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cmd.exe /c @"D:\InstallKits\htc_ide_rel\plugins\com.rt.hightec.resources_1.0.16.202505091130\resources\builder\rm.bat"

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include crt0/subdir.mk
-include bsp/uc/uc_tc38/subdir.mk
-include bsp/uc/subdir.mk
-include bsp/isr/subdir.mk
-include bsp/board/board_triboard_TC3X9_V2.0/subdir.mk
-include bsp/board/board_triboard_TC3X9_V1.0/subdir.mk
-include bsp/board/board_triboard_TC3X7_V2.0/subdir.mk
-include bsp/board/board_triboard_TC3X7_V1.0/subdir.mk
-include bsp/board/board_triboard_TC3X7_ADAS_V2.0/subdir.mk
-include bsp/board/board_triboard_TC3X7_ADAS_V1.0/subdir.mk
-include bsp/board/board_triboard_TC3X6_V1.0/subdir.mk
-include bsp/board/board_triboard_TC3X6_ADAS_V1.0/subdir.mk
-include bsp/board/board_triboard_TC3X5_V1.X/subdir.mk
-include bsp/board/board_triboard_TC3X4T_V1.0/subdir.mk
-include bsp/board/board_triboard_TC3X4L_V1.0/subdir.mk
-include bsp/board/board_triboard_TC3X3_V1.0/subdir.mk
-include bsp/board/board_triboard_TC3X2_V1.0/subdir.mk
-include bsp/board/board_triboard_TC377TX_V1.X/subdir.mk
-include bsp/board/board_radar_TC3X6_V1.3/subdir.mk
-include bsp/board/board_appkit_TC3X7_V2.0/subdir.mk
-include bsp/board/board_appkit_TC3X7_V1.0/subdir.mk
-include bsp/board/board_appkit_TC3X7_ADAS_V2.0/subdir.mk
-include bsp/board/board_appkit_TC3X7_ADAS_V1.0/subdir.mk
-include bsp/board/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(I_DEPS)),)
-include $(I_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPCP_DEPS)),)
-include $(CPCP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := tc38x_bsp_example
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# Rebuild target
rebuild: clean all

# All Target
all: main-build

# Main-build Target
main-build: tc38x_bsp_example.elf

# Tool invocations
tc38x_bsp_example.elf: $(OBJS) $(USER_OBJS) $(ASM) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: TriCore C Linker'
	"D:\Software\HighTec\toolchains\tricore\v4.9.3.0-infineon-1.0/bin/tricore-gcc" -o  "tc38x_bsp_example.elf" -nocrt0 -nostartfiles -T"tc38xA_memory.ld" -Wl,--gc-sections -mtc162 -Wl,--warn-orphan -Wl,@objectlist.optfile -Wl,@libpathlist.optfile -Wl,-Map="$(basename $(notdir $@)).map" -Wl,--cref -fshort-double -Wl,-n -Wl,--extmap="a"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	$(foreach __cleandir__,${SUBDIRS},$(call do-clean-dir,$(__cleandir__)))
	@echo 'Cleaning: tc38x_bsp_example.elf'
	@$(RM) tc38x_bsp_example.elf
	@echo ' '

define do-clean-dir
	@echo 'Cleaning: $1/*.o *.d *.rel'
	@$(RM) $1/*.o $1/*.d $1/*.rel

endef

legacy-clean:
	-$(RM) $(I_DEPS)$(EXECUTABLES)$(OBJS)$(C_DEPS)$(CPCP_DEPS) tc38x_bsp_example.elf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
