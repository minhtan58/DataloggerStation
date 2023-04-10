################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Threads/Network/ftplib.c \
../Threads/Network/network_handler.c \
../Threads/Network/tcp_echoclient.c \
../Threads/Network/tcp_echoserver.c 

OBJS += \
./Threads/Network/ftplib.o \
./Threads/Network/network_handler.o \
./Threads/Network/tcp_echoclient.o \
./Threads/Network/tcp_echoserver.o 

C_DEPS += \
./Threads/Network/ftplib.d \
./Threads/Network/network_handler.d \
./Threads/Network/tcp_echoclient.d \
./Threads/Network/tcp_echoserver.d 


# Each subdirectory must supply rules for building sources it contributes
Threads/Network/%.o Threads/Network/%.su Threads/Network/%.cyclo: ../Threads/Network/%.c Threads/Network/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/LwIP/system -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../Middlewares/Third_Party/LwIP/src/include/netif -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../../Middlewares/Third_Party/LwIP/system/arch -I../LWIP/Target -I../LWIP/App -I../Threads -I../Threads/SerialPortCOM -I../Threads/Network -I../Threads/LCD_Display -I../Threads/AnalogSensor -I../Threads/DigitalSensor -I../Peripherals -I../Common -I../FATFS/APP -I../FATFS/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/FatFs/src/option -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../USB_DEVICE -I../USB_DEVICE/App -I../USB_DEVICE/Target -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Threads-2f-Network

clean-Threads-2f-Network:
	-$(RM) ./Threads/Network/ftplib.cyclo ./Threads/Network/ftplib.d ./Threads/Network/ftplib.o ./Threads/Network/ftplib.su ./Threads/Network/network_handler.cyclo ./Threads/Network/network_handler.d ./Threads/Network/network_handler.o ./Threads/Network/network_handler.su ./Threads/Network/tcp_echoclient.cyclo ./Threads/Network/tcp_echoclient.d ./Threads/Network/tcp_echoclient.o ./Threads/Network/tcp_echoclient.su ./Threads/Network/tcp_echoserver.cyclo ./Threads/Network/tcp_echoserver.d ./Threads/Network/tcp_echoserver.o ./Threads/Network/tcp_echoserver.su

.PHONY: clean-Threads-2f-Network

