################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./BME280.obj" \
"./GPS.obj" \
"./MPU6050.obj" \
"./MPU9250.obj" \
"./QMC5883.obj" \
"./UARTDEBUG.obj" \
"./hal_main.obj" \
"./madgwick.obj" \
"./main.obj" \
"./syscfg/ti_drivers_config.obj" \
"./servo.obj" \
"./EasyHal/adc_dev.obj" \
"./EasyHal/i2c_dev.obj" \
"./EasyHal/pwm_dev.obj" \
"./EasyHal/spi_dev.obj" \
"./EasyHal/time_dev.obj" \
"./EasyHal/uart_dev.obj" \
"../MSP_EXP432P401R_FREERTOS.cmd" \
$(GEN_CMDS__FLAG) \
-l"ti/display/lib/display.aem4f" \
-l"ti/grlib/lib/ccs/m4f/grlib.a" \
-l"third_party/spiffs/lib/ccs/m4f/spiffs.a" \
-l"ti/drivers/lib/drivers_msp432p401x.aem4f" \
-l"third_party/fatfs/lib/ccs/m4f/fatfs.a" \
-lfreertos_builds_MSP_EXP432P401R_release_ccs.lib \
-l"ti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include EasyHal/subdir_vars.mk
-include subdir_rules.mk
-include EasyHal/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Quadcopter_HAL.out \

EXE_OUTPUTS__QUOTED += \
"Quadcopter_HAL.out" \

BIN_OUTPUTS += \
Quadcopter_HAL.hex \

BIN_OUTPUTS__QUOTED += \
"Quadcopter_HAL.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Quadcopter_HAL.out"

dependents:
	-cd C:/Users/Alex/workspace_v9_2/freertos_builds_MSP_EXP432P401R_release_ccs/Debug && $(MAKE) all

# Tool invocations
Quadcopter_HAL.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) C:/Users/Alex/workspace_v9_2/freertos_builds_MSP_EXP432P401R_release_ccs/Debug/freertos_builds_MSP_EXP432P401R_release_ccs.lib
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on -z -m"Quadcopter_HAL.map" -i"C:/ti/simplelink_msp432p4_sdk_3_40_01_02/source" -i"C:/Users/Alex/workspace_v9_2/freertos_builds_MSP_EXP432P401R_release_ccs/Debug" -i"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/lib" --diag_suppress=10063 --diag_wrap=off --display_error_number --warn_sections --xml_link_info="Quadcopter_HAL_linkInfo.xml" --rom_model -o "Quadcopter_HAL.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Quadcopter_HAL.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armhex"  -o "Quadcopter_HAL.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "BME280.obj" "GPS.obj" "MPU6050.obj" "MPU9250.obj" "QMC5883.obj" "UARTDEBUG.obj" "hal_main.obj" "madgwick.obj" "main.obj" "syscfg\ti_drivers_config.obj" "servo.obj" "EasyHal\adc_dev.obj" "EasyHal\i2c_dev.obj" "EasyHal\pwm_dev.obj" "EasyHal\spi_dev.obj" "EasyHal\time_dev.obj" "EasyHal\uart_dev.obj" 
	-$(RM) "BME280.d" "GPS.d" "MPU6050.d" "MPU9250.d" "QMC5883.d" "UARTDEBUG.d" "hal_main.d" "madgwick.d" "main.d" "syscfg\ti_drivers_config.d" "servo.d" "EasyHal\adc_dev.d" "EasyHal\i2c_dev.d" "EasyHal\pwm_dev.d" "EasyHal\spi_dev.d" "EasyHal\time_dev.d" "EasyHal\uart_dev.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
C:/Users/Alex/workspace_v9_2/freertos_builds_MSP_EXP432P401R_release_ccs/Debug/freertos_builds_MSP_EXP432P401R_release_ccs.lib:

-include ../makefile.targets

