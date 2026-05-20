cmake_minimum_required(VERSION 3.22)

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

option(MONITOR_INTERNALL_TEMP "Enable internal temp monitoring" OFF)
option(TEST_DMA               "Enable DMA test mode"            OFF)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME test)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
	# Peripheral interface
    Peripheral/Src/adc.c
	Peripheral/Src/gpio.c
	Peripheral/Src/interrupt.c
	Peripheral/Src/rcc.c
	Peripheral/Src/timer.c
	Peripheral/Src/uart.c
	Peripheral/Src/flash.c

	# Debug Version
	$<$<CONFIG:Debug>:Peripheral/Src/dma.c>

	#System Health
	Common/Src/system_health.c

	# FreeRTOS
	Middlewares/ThirdParty/FreeRTOS/portable/GCC/ARM_CM3/port.c
	Middlewares/ThirdParty/FreeRTOS/portable/MemMang/heap_4.c
	Middlewares/ThirdParty/FreeRTOS/croutine.c
	Middlewares/ThirdParty/FreeRTOS/event_groups.c
	Middlewares/ThirdParty/FreeRTOS/list.c
	Middlewares/ThirdParty/FreeRTOS/queue.c
	Middlewares/ThirdParty/FreeRTOS/stream_buffer.c
	Middlewares/ThirdParty/FreeRTOS/tasks.c
	Middlewares/ThirdParty/FreeRTOS/timers.c
	Middlewares/ThirdParty/lwgps/src/lwgps/lwgps.c
	Middlewares/ThirdParty/lwrb/src/lwrb/lwrb_ex.c
	Middlewares/ThirdParty/lwrb/src/lwrb/lwrb.c

	ApplicationTasks/Src/gps.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Peripheral/Inc
	Common/Inc
	Middlewares/ThirdParty/FreeRTOS/include
	Middlewares/ThirdParty/FreeRTOS/portable/GCC/ARM_CM3
	Middlewares/ThirdParty/lwgps/src/include
	Middlewares/ThirdParty/lwrb/src/include
	ApplicationTasks/Inc
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<BOOL:${MONITOR_INTERNALL_TEMP}>:MONITOR_INTERNALL_TEMP>
    $<$<BOOL:${TEST_DMA}>:TEST_DMA>
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex
            $<TARGET_FILE:${PROJECT_NAME}>
            ${PROJECT_NAME}.hex

    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:${PROJECT_NAME}>
            ${PROJECT_NAME}.bin

    COMMAND ${CMAKE_OBJDUMP} -D
            $<TARGET_FILE:${PROJECT_NAME}>
            > ${PROJECT_NAME}.lst

    COMMAND ${CMAKE_SIZE}
            $<TARGET_FILE:${PROJECT_NAME}>
)
