################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP_MC2.c \
../Timer0.c \
../buzzer.c \
../external_eeprom.c \
../gpio.c \
../motor.c \
../timer_1.c \
../twi.c \
../uart.c 

OBJS += \
./APP_MC2.o \
./Timer0.o \
./buzzer.o \
./external_eeprom.o \
./gpio.o \
./motor.o \
./timer_1.o \
./twi.o \
./uart.o 

C_DEPS += \
./APP_MC2.d \
./Timer0.d \
./buzzer.d \
./external_eeprom.d \
./gpio.d \
./motor.d \
./timer_1.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


