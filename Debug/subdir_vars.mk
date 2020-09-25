################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432P401R_FREERTOS.cmd 

SYSCFG_SRCS += \
../quadcopter.syscfg 

C_SRCS += \
../BME280.c \
../GPS.c \
../MPU6050.c \
../MPU9250.c \
../QMC5883.c \
../UARTDEBUG.c \
../hal_main.c \
../madgwick.c \
../main.c \
./syscfg/ti_drivers_config.c \
../servo.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./BME280.d \
./GPS.d \
./MPU6050.d \
./MPU9250.d \
./QMC5883.d \
./UARTDEBUG.d \
./hal_main.d \
./madgwick.d \
./main.d \
./syscfg/ti_drivers_config.d \
./servo.d 

OBJS += \
./BME280.obj \
./GPS.obj \
./MPU6050.obj \
./MPU9250.obj \
./QMC5883.obj \
./UARTDEBUG.obj \
./hal_main.obj \
./madgwick.obj \
./main.obj \
./syscfg/ti_drivers_config.obj \
./servo.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"BME280.obj" \
"GPS.obj" \
"MPU6050.obj" \
"MPU9250.obj" \
"QMC5883.obj" \
"UARTDEBUG.obj" \
"hal_main.obj" \
"madgwick.obj" \
"main.obj" \
"syscfg\ti_drivers_config.obj" \
"servo.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"BME280.d" \
"GPS.d" \
"MPU6050.d" \
"MPU9250.d" \
"QMC5883.d" \
"UARTDEBUG.d" \
"hal_main.d" \
"madgwick.d" \
"main.d" \
"syscfg\ti_drivers_config.d" \
"servo.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../BME280.c" \
"../GPS.c" \
"../MPU6050.c" \
"../MPU9250.c" \
"../QMC5883.c" \
"../UARTDEBUG.c" \
"../hal_main.c" \
"../madgwick.c" \
"../main.c" \
"./syscfg/ti_drivers_config.c" \
"../servo.c" 

SYSCFG_SRCS__QUOTED += \
"../quadcopter.syscfg" 


