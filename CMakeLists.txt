cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

include(pico_extras_import_optional.cmake)

project(pico_function_generator C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(BOARD pico_sdk)
set(FAMILY rp2040)


if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()
set(PICO_TINYUSB_PATH "D:/pico-sdk/lib/tinyusb")
# Initialize the SDK

pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_executable(debug
        src/main.c
        src/ad9833.c
        src/usb_descriptors.c
        src/usbtmc_app.c
        src/log.c
        src/commands.c
        src/keypad.c
        src/cursor.c
        lib/ST7735S/st7735s_compat.c
        lib/ST7735S/st7735s.c
        lib/ST7735S/gfx.c
        lib/ST7735S/fonts.c
        )
include(family_support.cmake)

target_include_directories(debug PUBLIC include lib/ST7735S)

# pull in common dependencies
target_link_libraries(debug pico_stdlib tinyusb_device tinyusb_board hardware_spi)
add_definitions(-DPICO_STDIO_USB_CONNECTION_WITHOUT_DTR=1)
add_definitions(-DBOARD_TUD_RHPORT=0)
target_compile_definitions(debug PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)
# create map/bin/hex file etc.
pico_add_extra_outputs(debug)
