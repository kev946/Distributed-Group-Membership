################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MembershipList.cpp \
../src/MessageCreator.cpp \
../src/Server.cpp \
../src/main.cpp 

OBJS += \
./src/MembershipList.o \
./src/MessageCreator.o \
./src/Server.o \
./src/main.o 

CPP_DEPS += \
./src/MembershipList.d \
./src/MessageCreator.d \
./src/Server.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


