################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Peripherals/dma.c \
../Peripherals/flash.c \
../Peripherals/gpio.c \
../Peripherals/i2c.c \
../Peripherals/quadspi.c \
../Peripherals/rtc.c \
../Peripherals/sdcard.c \
../Peripherals/spi.c \
../Peripherals/timer.c \
../Peripherals/uart.c 

OBJS += \
./Peripherals/dma.o \
./Peripherals/flash.o \
./Peripherals/gpio.o \
./Peripherals/i2c.o \
./Peripherals/quadspi.o \
./Peripherals/rtc.o \
./Peripherals/sdcard.o \
./Peripherals/spi.o \
./Peripherals/timer.o \
./Peripherals/uart.o 

C_DEPS += \
./Peripherals/dma.d \
./Peripherals/flash.d \
./Peripherals/gpio.d \
./Peripherals/i2c.d \
./Peripherals/quadspi.d \
./Peripherals/rtc.d \
./Peripherals/sdcard.d \
./Peripherals/spi.d \
./Peripherals/timer.d \
./Peripherals/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Peripherals/%.o Peripherals/%.su Peripherals/%.cyclo: ../Peripherals/%.c Peripherals/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/LwIP/system -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../Middlewares/Third_Party/LwIP/src/include/netif -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../../Middlewares/Third_Party/LwIP/system/arch -I../LWIP/Target -I../LWIP/App -I../Threads -I../Threads/SerialPortCOM -I../Threads/Network -I../Threads/LCD_Display -I../Threads/AnalogSensor -I../Threads/DigitalSensor -I../Peripherals -I../Common -I../FATFS/APP -I../FATFS/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/FatFs/src/option -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../USB_DEVICE -I../USB_DEVICE/App -I../USB_DEVICE/Target -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Peripherals

clean-Peripherals:
	-$(RM) ./Peripherals/dma.cyclo ./Peripherals/dma.d ./Peripherals/dma.o ./Peripherals/dma.su ./Peripherals/flash.cyclo ./Peripherals/flash.d ./Peripherals/flash.o ./Peripherals/flash.su ./Peripherals/gpio.cyclo ./Peripherals/gpio.d ./Peripherals/gpio.o ./Peripherals/gpio.su ./Peripherals/i2c.cyclo ./Peripherals/i2c.d ./Peripherals/i2c.o ./Peripherals/i2c.su ./Peripherals/quadspi.cyclo ./Peripherals/quadspi.d ./Peripherals/quadspi.o ./Peripherals/quadspi.su ./Peripherals/rtc.cyclo ./Peripherals/rtc.d ./Peripherals/rtc.o ./Peripherals/rtc.su ./Peripherals/sdcard.cyclo ./Peripherals/sdcard.d ./Peripherals/sdcard.o ./Peripherals/sdcard.su ./Peripherals/spi.cyclo ./Peripherals/spi.d ./Peripherals/spi.o ./Peripherals/spi.su ./Peripherals/timer.cyclo ./Peripherals/timer.d ./Peripherals/timer.o ./Peripherals/timer.su ./Peripherals/uart.cyclo ./Peripherals/uart.d ./Peripherals/uart.o ./Peripherals/uart.su

.PHONY: clean-Peripherals

