################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Threads/SerialPortCOM/command_process.c \
../Threads/SerialPortCOM/sdi12.c \
../Threads/SerialPortCOM/sensor_process.c \
../Threads/SerialPortCOM/sim4g_process.c \
../Threads/SerialPortCOM/uart_handler.c 

OBJS += \
./Threads/SerialPortCOM/command_process.o \
./Threads/SerialPortCOM/sdi12.o \
./Threads/SerialPortCOM/sensor_process.o \
./Threads/SerialPortCOM/sim4g_process.o \
./Threads/SerialPortCOM/uart_handler.o 

C_DEPS += \
./Threads/SerialPortCOM/command_process.d \
./Threads/SerialPortCOM/sdi12.d \
./Threads/SerialPortCOM/sensor_process.d \
./Threads/SerialPortCOM/sim4g_process.d \
./Threads/SerialPortCOM/uart_handler.d 


# Each subdirectory must supply rules for building sources it contributes
Threads/SerialPortCOM/%.o Threads/SerialPortCOM/%.su Threads/SerialPortCOM/%.cyclo: ../Threads/SerialPortCOM/%.c Threads/SerialPortCOM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/LwIP/system -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../Middlewares/Third_Party/LwIP/src/include/netif -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../../Middlewares/Third_Party/LwIP/system/arch -I../LWIP/Target -I../LWIP/App -I../Threads -I../Threads/SerialPortCOM -I../Threads/Network -I../Threads/LCD_Display -I../Threads/AnalogSensor -I../Threads/DigitalSensor -I../Peripherals -I../Common -I../FATFS/APP -I../FATFS/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/FatFs/src/option -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../USB_DEVICE -I../USB_DEVICE/App -I../USB_DEVICE/Target -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Threads-2f-SerialPortCOM

clean-Threads-2f-SerialPortCOM:
	-$(RM) ./Threads/SerialPortCOM/command_process.cyclo ./Threads/SerialPortCOM/command_process.d ./Threads/SerialPortCOM/command_process.o ./Threads/SerialPortCOM/command_process.su ./Threads/SerialPortCOM/sdi12.cyclo ./Threads/SerialPortCOM/sdi12.d ./Threads/SerialPortCOM/sdi12.o ./Threads/SerialPortCOM/sdi12.su ./Threads/SerialPortCOM/sensor_process.cyclo ./Threads/SerialPortCOM/sensor_process.d ./Threads/SerialPortCOM/sensor_process.o ./Threads/SerialPortCOM/sensor_process.su ./Threads/SerialPortCOM/sim4g_process.cyclo ./Threads/SerialPortCOM/sim4g_process.d ./Threads/SerialPortCOM/sim4g_process.o ./Threads/SerialPortCOM/sim4g_process.su ./Threads/SerialPortCOM/uart_handler.cyclo ./Threads/SerialPortCOM/uart_handler.d ./Threads/SerialPortCOM/uart_handler.o ./Threads/SerialPortCOM/uart_handler.su

.PHONY: clean-Threads-2f-SerialPortCOM

