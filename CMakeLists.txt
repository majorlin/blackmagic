#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.21)

set(CHIP YTM32B1LE0)
set(SDK SDK)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(BlackMagic C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# for use printf & scanf with float
set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs")

#Uncomment for hardware floating point
add_compile_definitions(CPU_${CHIP})
add_compile_definitions(DEV_ERROR_DETECT)

add_compile_options(-mcpu=cortex-m0plus -mthumb -mfloat-abi=soft)
add_link_options(-mcpu=cortex-m0plus -mthumb -mfloat-abi=soft)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_compile_definitions(ENABLE_DEBUG)
add_compile_definitions(NO_LIBOPENCM3)
add_compile_definitions(PC_HOSTED=0)
add_compile_options(-Werror=format=2)
# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

add_compile_options(-Os -flto -g)
include_directories(
        ${SDK}/CMSIS/Core/Include
        ${SDK}/platform/devices
        ${SDK}/platform/devices/common
        ${SDK}/platform/devices/${CHIP}/include
        ${SDK}/platform/devices/${CHIP}/startup
        ${SDK}/platform/drivers/inc
        ${SDK}/platform/drivers/src/flash
        ${SDK}/platform/drivers/src/uart
        ${SDK}/platform/drivers/src/lin
        ${SDK}/rtos/osif
)

file(GLOB SOURCES
        ${SDK}/platform/devices/${CHIP}/startup/gcc/${CHIP}_startup_gcc.S
        ${SDK}/platform/devices/${CHIP}/startup/system_${CHIP}.c
        ${SDK}/platform/devices/startup.c
        ${SDK}/platform/drivers/src/interrupt/interrupt_manager.c

        ${SDK}/platform/drivers/src/pins/*.c
        ${SDK}/platform/drivers/src/ptmr/*.c
        ${SDK}/rtos/osif/osif_baremetal.c
        )
    file(GLOB SERIES_SOURCES
        ${SDK}/platform/drivers/src/uart/*.c
        ${SDK}/platform/drivers/src/lin/*.c
        ${SDK}/platform/drivers/src/clock/YTM32Z1x/clock_YTM32Z1x.c
        ${SDK}/platform/drivers/src/power/YTM32Z1x/power_manager_YTM32Z1x.c
        )
    include_directories(
		    src/include
            src/platforms/common
            src/platforms/yuntu
            src/target
    )
    file(GLOB APP_SOURCES
		    src/*.c
            src/target/*.c
            src/platforms/common/swdptap.c
            src/platforms/common/jtagtap.c
            src/platforms/yuntu/*.c
        )

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/${SDK}/platform/devices/${CHIP}/linker/gcc/flash.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${SERIES_SOURCES} ${APP_SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
