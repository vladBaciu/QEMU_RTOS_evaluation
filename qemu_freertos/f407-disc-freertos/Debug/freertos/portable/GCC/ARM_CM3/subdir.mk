################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/portable/GCC/ARM_CM3/port.c 

OBJS += \
./freertos/portable/GCC/ARM_CM3/port.o 

C_DEPS += \
./freertos/portable/GCC/ARM_CM3/port.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/portable/GCC/ARM_CM3/%.o: ../freertos/portable/GCC/ARM_CM3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"C:\Users\baciu\Downloads\qemu-eclipse-test-projects-master\qemu-eclipse-test-projects-master\f407-disc-freertos\freertos\include" -I"C:\Users\baciu\Downloads\qemu-eclipse-test-projects-master\qemu-eclipse-test-projects-master\f407-disc-freertos\freertos\portable\GCC\ARM_CM3" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


