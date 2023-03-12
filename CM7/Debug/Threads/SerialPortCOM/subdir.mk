################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Threads/SerialPortCOM/command_process.c \
../Threads/SerialPortCOM/sensor_process.c \
../Threads/SerialPortCOM/uart_handler.c 

OBJS += \
./Threads/SerialPortCOM/command_process.o \
./Threads/SerialPortCOM/sensor_process.o \
./Threads/SerialPortCOM/uart_handler.o 

C_DEPS += \
./Threads/SerialPortCOM/command_process.d \
./Threads/SerialPortCOM/sensor_process.d \
./Threads/SerialPortCOM/uart_handler.d 


# Each subdirectory must supply rules for building sources it contributes
Threads/SerialPortCOM/%.o: ../Threads/SerialPortCOM/%.c Threads/SerialPortCOM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/LwIP/system -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../Middlewares/Third_Party/LwIP/src/include/netif -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../../Middlewares/Third_Party/LwIP/system/arch -I../../Middlewares/Third_Party/FatFs/src -I../LWIP/Target -I../LWIP/App -I../Threads -I../Threads/SerialPortCOM -I../Threads/Network -I../Threads/LCD_Display -I../Peripherals -I../Common -I../FATFS/APP -I../FATFS/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/lan8742 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Threads-2f-SerialPortCOM

clean-Threads-2f-SerialPortCOM:
	-$(RM) ./Threads/SerialPortCOM/command_process.d ./Threads/SerialPortCOM/command_process.o ./Threads/SerialPortCOM/sensor_process.d ./Threads/SerialPortCOM/sensor_process.o ./Threads/SerialPortCOM/uart_handler.d ./Threads/SerialPortCOM/uart_handler.o

.PHONY: clean-Threads-2f-SerialPortCOM

