################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Cpu0_Main.c" \
"../Cpu1_Main.c" \
"../Cpu2_Main.c" \
"../my_adc.c" \
"../my_tick.c" 

COMPILED_SRCS += \
"Cpu0_Main.src" \
"Cpu1_Main.src" \
"Cpu2_Main.src" \
"my_adc.src" \
"my_tick.src" 

C_DEPS += \
"./Cpu0_Main.d" \
"./Cpu1_Main.d" \
"./Cpu2_Main.d" \
"./my_adc.d" \
"./my_tick.d" 

OBJS += \
"Cpu0_Main.o" \
"Cpu1_Main.o" \
"Cpu2_Main.o" \
"my_adc.o" \
"my_tick.o" 


# Each subdirectory must supply rules for building sources it contributes
"Cpu0_Main.src":"../Cpu0_Main.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc27xd "-fC:/Users/User/AURIX-v1.10.36-workspace/Lab12_ADC_Stream/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Cpu0_Main.o":"Cpu0_Main.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Cpu1_Main.src":"../Cpu1_Main.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc27xd "-fC:/Users/User/AURIX-v1.10.36-workspace/Lab12_ADC_Stream/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Cpu1_Main.o":"Cpu1_Main.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Cpu2_Main.src":"../Cpu2_Main.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc27xd "-fC:/Users/User/AURIX-v1.10.36-workspace/Lab12_ADC_Stream/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Cpu2_Main.o":"Cpu2_Main.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"my_adc.src":"../my_adc.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc27xd "-fC:/Users/User/AURIX-v1.10.36-workspace/Lab12_ADC_Stream/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"my_adc.o":"my_adc.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"my_tick.src":"../my_tick.c" "subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc27xd "-fC:/Users/User/AURIX-v1.10.36-workspace/Lab12_ADC_Stream/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"my_tick.o":"my_tick.src" "subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean--2e-

clean--2e-:
	-$(RM) ./Cpu0_Main.d ./Cpu0_Main.o ./Cpu0_Main.src ./Cpu1_Main.d ./Cpu1_Main.o ./Cpu1_Main.src ./Cpu2_Main.d ./Cpu2_Main.o ./Cpu2_Main.src ./my_adc.d ./my_adc.o ./my_adc.src ./my_tick.d ./my_tick.o ./my_tick.src

.PHONY: clean--2e-

