################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Libraries/iLLD/TC27D/Tricore/_PinMap/subdir.mk
-include Libraries/iLLD/TC27D/Tricore/_Impl/subdir.mk
-include Libraries/iLLD/TC27D/Tricore/Src/Std/subdir.mk
-include Libraries/iLLD/TC27D/Tricore/Scu/Std/subdir.mk
-include Libraries/iLLD/TC27D/Tricore/Port/Std/subdir.mk
-include Libraries/iLLD/TC27D/Tricore/Mtu/Std/subdir.mk
-include Libraries/iLLD/TC27D/Tricore/Gtm/Std/subdir.mk
-include Libraries/iLLD/TC27D/Tricore/Cpu/Trap/subdir.mk
-include Libraries/iLLD/TC27D/Tricore/Cpu/Std/subdir.mk
-include Libraries/iLLD/TC27D/Tricore/Cpu/CStart/subdir.mk
-include Libraries/Infra/Platform/Tricore/Compilers/subdir.mk
-include Configurations/Debug/subdir.mk
-include subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Lab11_ADC_Pot
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
Lab11_ADC_Pot.siz \


# All Target
all: main-build

# Main-build Target
main-build: Lab11_ADC_Pot.elf secondary-outputs

# Tool invocations
Lab11_ADC_Pot.elf: $(OBJS) makefile $(OPTIONAL_TOOL_DEPS) .Lab11_ADC_Pot.elf.opt
	@echo 'Building target: $@'
	@echo 'Invoking: TASKING Linker'
	cctc -f .Lab11_ADC_Pot.elf.opt
	@echo 'Finished building target: $@'
	@echo ' '

.Lab11_ADC_Pot.elf.opt:
	@argfile .Lab11_ADC_Pot.elf.opt --lsl-file="../Lcf_Tasking_Tricore_Tc.lsl" -Wl-Oc -Wl-OL -Wl-Ot -Wl-Ox -Wl-Oy -Wl--map-file="Lab11_ADC_Pot.map" -Wl-mc -Wl-mf -Wl-mi -Wl-mk -Wl-ml -Wl-mm -Wl-md -Wl-mr -Wl-mu --no-warnings= -Wl--error-limit=42 --fp-model=3 -lrt --lsl-core=vtc --exceptions --strict --anachronisms --force-c++ -Ctc27xd -o"Lab11_ADC_Pot.elf" -Wl-o"Lab11_ADC_Pot.hex:IHEX" $(OBJS)

.PHONY: .Lab11_ADC_Pot.elf.opt

Lab11_ADC_Pot.siz: Lab11_ADC_Pot.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	elfsize  "Lab11_ADC_Pot.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Lab11_ADC_Pot.siz Lab11_ADC_Pot.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
