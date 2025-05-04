# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

add_compile_definitions(PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=1)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(internal_temperature_adc C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(internal_temperature_adc 
    src/main.c 
    src/ssd1306.c
    )

pico_set_program_name(internal_temperature_adc "internal_temperature_adc")
pico_set_program_version(internal_temperature_adc "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(internal_temperature_adc 0)
pico_enable_stdio_usb(internal_temperature_adc 1)

# Add the standard library to the build
target_link_libraries(internal_temperature_adc
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(internal_temperature_adc PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# Add any user requested libraries
target_link_libraries(internal_temperature_adc 
        hardware_adc
        hardware_i2c
        )

pico_add_extra_outputs(internal_temperature_adc)

