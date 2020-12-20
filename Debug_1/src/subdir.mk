################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BlinkLed.c \
../src/FreeRTOS_helper.c \
../src/_initialize_hardware.c \
../src/_write.c \
../src/main.c 

OBJS += \
./src/BlinkLed.o \
./src/FreeRTOS_helper.o \
./src/_initialize_hardware.o \
./src/_write.o \
./src/main.o 

C_DEPS += \
./src/BlinkLed.d \
./src/FreeRTOS_helper.d \
./src/_initialize_hardware.d \
./src/_write.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"/home/vlad/eclipse-workspace/f407-disc-freertos/freertos/include" -I"/home/vlad/eclipse-workspace/f407-disc-freertos/freertos/portable/GCC/ARM_CM3" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


