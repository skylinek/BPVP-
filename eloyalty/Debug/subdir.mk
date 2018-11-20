################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../eloyalty.c 

OBJS += \
./eloyalty.hzo 


# Each subdirectory must supply rules for building sources it contributes
%.hzo: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SmartDeck Compiler'
	hcl -I"C:/temp/demo-workspace/eloyalty" -c -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


