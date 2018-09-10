################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
CodeStartBranch.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/cpu/src/32b/f28x/f2806x/CodeStartBranch.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="CodeStartBranch.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Main.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/Main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

adc.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/adc/src/32b/f28x/f2806x/adc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="adc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

clarke.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/clarke/src/32b/clarke.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="clarke.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

clk.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/clk/src/32b/f28x/f2806x/clk.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="clk.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

cpu.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/cpu/src/32b/f28x/f2806x/cpu.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="cpu.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ctrlQEP.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/ctrl/src/32b/ctrlQEP.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="ctrlQEP.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

datalog.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/datalog/src/32b/datalog.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="datalog.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

enc.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/enc/src/32b/enc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="enc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

filter.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/filter.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="filter.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

filter_fo.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/filter/src/32b/filter_fo.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="filter_fo.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

flash.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/flash/src/32b/f28x/f2806x/flash.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="flash.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

gpio.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/gpio/src/32b/f28x/f2806x/gpio.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="gpio.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

hal.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/hal/boards/hvkit_rev1p1/f28x/f2806x/src/hal.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="hal.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

hallbldc.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/hallbldc/src/32b/hallbldc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="hallbldc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

i2c.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/i2c/src/32b/f28x/f2806x/i2c.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="i2c.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

i2c_24lc32.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/i2c_24lc32.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="i2c_24lc32.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

i2c_lcd.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/i2c_lcd.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="i2c_lcd.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

i2c_mcp23017.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/i2c_mcp23017.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="i2c_mcp23017.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

i2c_message.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/i2c_message.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="i2c_message.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ipark.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/ipark/src/32b/ipark.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="ipark.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

memCopy.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/memCopy/src/memCopy.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="memCopy.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

offset.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/offset/src/32b/offset.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="offset.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

osc.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/osc/src/32b/f28x/f2806x/osc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="osc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

park.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/park/src/32b/park.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="park.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

pid.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/pid/src/32b/pid.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="pid.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

pie.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/pie/src/32b/f28x/f2806x/pie.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="pie.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

pll.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/pll/src/32b/f28x/f2806x/pll.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="pll.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

pwm.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/pwm/src/32b/f28x/f2806x/pwm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="pwm.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

pwr.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/pwr/src/32b/f28x/f2806x/pwr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="pwr.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

qep.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/qep/src/32b/f28x/f2806x/qep.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="qep.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sci.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/sci/src/32b/f28x/f2806x/sci.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sci.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sci_message.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/sci_message.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sci_message.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sci_modbus.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/sci_modbus.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sci_modbus.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sci_operator.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/sci_operator.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="sci_operator.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

spi.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/spi/src/32b/f28x/f2806x/spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="spi.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

svgen.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/svgen/src/32b/svgen.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="svgen.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

throttle.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/throttle/src/32b/throttle.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="throttle.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

timer.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/timer/src/32b/f28x/f2806x/timer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="timer.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

traj.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/traj/src/32b/traj.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="traj.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

usDelay.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/usDelay/src/32b/f28x/usDelay.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="usDelay.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

user.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/modules/user/src/32b/user.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="user.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

user_data.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/Sources/user_data.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="user_data.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

wdog.obj: D:/My\ Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32/sw/drivers/wdog/src/32b/f28x/f2806x/wdog.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 -O2 --include_path="D:/My Documents/Desktop/NCTU_IMLAB/Ti_doc_Motor_doc/code/EV_28069M_Fpu32_0910/EV_28069M_Fpu32" --include_path="D:/TI/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --advice:performance=all --define=QEP --define=FLASH --define=FAST_ROM_V1p6 -g --diag_suppress=16002 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="wdog.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


