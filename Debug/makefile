################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
-lrts2800_fpu32.lib \
"./CodeStartBranch.obj" \
"./Main.obj" \
"./adc.obj" \
"./clarke.obj" \
"./clk.obj" \
"./cpu.obj" \
"./ctrlQEP.obj" \
"./datalog.obj" \
"./enc.obj" \
"./filter.obj" \
"./filter_fo.obj" \
"./flash.obj" \
"./gpio.obj" \
"./hal.obj" \
"./hallbldc.obj" \
"./i2c.obj" \
"./i2c_24lc32.obj" \
"./i2c_lcd.obj" \
"./i2c_mcp23017.obj" \
"./i2c_message.obj" \
"./ipark.obj" \
"./memCopy.obj" \
"./offset.obj" \
"./osc.obj" \
"./park.obj" \
"./pid.obj" \
"./pie.obj" \
"./pll.obj" \
"./pwm.obj" \
"./pwr.obj" \
"./qep.obj" \
"./sci.obj" \
"./sci_message.obj" \
"./sci_modbus.obj" \
"./sci_operator.obj" \
"./spi.obj" \
"./svgen.obj" \
"./throttle.obj" \
"./timer.obj" \
"./traj.obj" \
"./usDelay.obj" \
"./user.obj" \
"./user_data.obj" \
"./wdog.obj" \
"../sw/modules/fast/lib/32b/f28x/f2806x/2806xRevB_FastSpinROMSymbols_fpu32.lib" \
"../F28069.cmd" \
"../sw/modules/iqmath/lib/f28x/float/IQmath_fpu32.lib" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
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
EV_28069M_Fpu32.out \

EXE_OUTPUTS__QUOTED += \
"EV_28069M_Fpu32.out" \

BIN_OUTPUTS += \
EV_28069M_Fpu32.hex \

BIN_OUTPUTS__QUOTED += \
"EV_28069M_Fpu32.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "EV_28069M_Fpu32.out"

# Tool invocations
EV_28069M_Fpu32.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number -z -m"EV_28069M_Fpu32.map" --stack_size=0x3B0 --warn_sections -i"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/lib" -i"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --priority --reread_libs --disable_auto_rts --diag_wrap=off --display_error_number --xml_link_info="EV_28069M_Fpu32_linkInfo.xml" --rom_model -o "EV_28069M_Fpu32.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

EV_28069M_Fpu32.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/hex2000"  -o "EV_28069M_Fpu32.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "CodeStartBranch.obj" "Main.obj" "adc.obj" "clarke.obj" "clk.obj" "cpu.obj" "ctrlQEP.obj" "datalog.obj" "enc.obj" "filter.obj" "filter_fo.obj" "flash.obj" "gpio.obj" "hal.obj" "hallbldc.obj" "i2c.obj" "i2c_24lc32.obj" "i2c_lcd.obj" "i2c_mcp23017.obj" "i2c_message.obj" "ipark.obj" "memCopy.obj" "offset.obj" "osc.obj" "park.obj" "pid.obj" "pie.obj" "pll.obj" "pwm.obj" "pwr.obj" "qep.obj" "sci.obj" "sci_message.obj" "sci_modbus.obj" "sci_operator.obj" "spi.obj" "svgen.obj" "throttle.obj" "timer.obj" "traj.obj" "usDelay.obj" "user.obj" "user_data.obj" "wdog.obj" 
	-$(RM) "Main.d" "adc.d" "clarke.d" "clk.d" "cpu.d" "ctrlQEP.d" "datalog.d" "enc.d" "filter.d" "filter_fo.d" "flash.d" "gpio.d" "hal.d" "hallbldc.d" "i2c.d" "i2c_24lc32.d" "i2c_lcd.d" "i2c_mcp23017.d" "i2c_message.d" "ipark.d" "memCopy.d" "offset.d" "osc.d" "park.d" "pid.d" "pie.d" "pll.d" "pwm.d" "pwr.d" "qep.d" "sci.d" "sci_message.d" "sci_modbus.d" "sci_operator.d" "spi.d" "svgen.d" "throttle.d" "timer.d" "traj.d" "user.d" "user_data.d" "wdog.d" 
	-$(RM) "CodeStartBranch.d" "usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

