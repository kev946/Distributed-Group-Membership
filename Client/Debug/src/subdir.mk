################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LogCreator.cpp \
../src/Server.cpp \
../src/dgrep-server.cpp 

OBJS += \
./src/LogCreator.o \
./src/Server.o \
./src/dgrep-server.o 

CPP_DEPS += \
./src/LogCreator.d \
./src/Server.d \
./src/dgrep-server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


