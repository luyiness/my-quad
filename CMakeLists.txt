cmake_minimum_required(VERSION 3.22)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(ArmGNUToolchain /Applications/ArmGNUToolchain/13.3.rel1/arm-none-eabi)
set(CMAKE_C_COMPILER   ${ArmGNUToolchain}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${ArmGNUToolchain}/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${ArmGNUToolchain}/bin/arm-none-eabi-gcc)
#include_directories(${ArmGNUToolchain}/arm-none-eabi/include)      //建议删掉。不要手动加。 gcc 自己会找。你手动加，反而可能：找错 multilib,找到 ARM mode libc,导致 Thumb/ARM mismatch
#link_directories(${ArmGNUToolchain}/arm-none-eabi/lib)
#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
set(CMAKE_BUILD_TYPE Debug)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME quad-test)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb  -mfloat-abi=soft")
set(CMAKE_C_FLAGS "${CPU_FLAGS} -ffreestanding")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS} -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS}")

# Core project settings
project(${CMAKE_PROJECT_NAME} C ASM)

message("Build type: " ${CMAKE_BUILD_TYPE})

## Enable CMake support for ASM and C languages
#enable_language(C ASM)

#add_compile_options(
#        -mcpu=cortex-m3
#        -mthumb
#)

add_link_options(
        -mcpu=cortex-m3
        -mthumb
        --specs=nosys.specs
        -Wl,--gc-sections
)


# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")   #生成物加上.elf后缀

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
        -mcpu=cortex-m3
        -mthumb
        -mfloat-abi=soft
        -ffreestanding
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103XX_FLASH.ld)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
        -mcpu=cortex-m3
        -mthumb
        -mfloat-abi=soft
        -T${LINKER_SCRIPT}
        --specs=nano.specs
        --specs=nosys.specs
        -Wl,-Map=${CMAKE_PROJECT_NAME}.map
        -Wl,--gc-sections
)

set_source_files_properties(
        startup_stm32f103xe.s
        PROPERTIES
        LANGUAGE ASM
        COMPILE_FLAGS "-x assembler-with-cpp -mthumb -mcpu=cortex-m3"
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32cubemx

        # Add user defined libraries
)

#生成bin,hex文件
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND arm-none-eabi-objcopy -O binary
        ${CMAKE_PROJECT_NAME}.elf
        ${CMAKE_PROJECT_NAME}.bin

        COMMAND arm-none-eabi-objcopy -O ihex
        ${CMAKE_PROJECT_NAME}.elf
        ${CMAKE_PROJECT_NAME}.hex
)