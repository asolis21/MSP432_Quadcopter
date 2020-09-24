################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432P401R_FREERTOS.cmd 

SYSCFG_SRCS += \
../drone.syscfg 

C_SRCS += \
../BME280.c \
../GPS.c \
../MPU6050.c \
../MPU9250.c \
../UARTDEBUG.c \
./syscfg/ti_drivers_config.c \
../hal_main.c \
../madgwick.c \
../main.c \
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
./UARTDEBUG.d \
./syscfg/ti_drivers_config.d \
./hal_main.d \
./madgwick.d \
./main.d \
./servo.d 

OBJS += \
./BME280.obj \
./GPS.obj \
./MPU6050.obj \
./MPU9250.obj \
./UARTDEBUG.obj \
./syscfg/ti_drivers_config.obj \
./hal_main.obj \
./madgwick.obj \
./main.obj \
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
"UARTDEBUG.obj" \
"syscfg\ti_drivers_config.obj" \
"hal_main.obj" \
"madgwick.obj" \
"main.obj" \
"servo.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"BME280.d" \
"GPS.d" \
"MPU6050.d" \
"MPU9250.d" \
"UARTDEBUG.d" \
"syscfg\ti_drivers_config.d" \
"hal_main.d" \
"madgwick.d" \
"main.d" \
"servo.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../BME280.c" \
"../GPS.c" \
"../MPU6050.c" \
"../MPU9250.c" \
"../UARTDEBUG.c" \
"./syscfg/ti_drivers_config.c" \
"../hal_main.c" \
"../madgwick.c" \
"../main.c" \
"../servo.c" 

SYSCFG_SRCS__QUOTED += \
"../drone.syscfg" 


