################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
HORIZON_DEBUG_FILE_OUTPUTS += \
eloyalty.hzx \


# All Target
all: eloyalty

# Tool invocations
	@echo 'No tool found that can build the extension specified with the build artifact name $@'
eloyalty.hzx: $(OBJS)
	@echo 'Invoking: SmartDeck Linker'
	hcl -g -o "eloyalty.hzx" $(OBJS)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(HORIZON_DEBUG_FILE_OUTPUTS) eloyalty
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
