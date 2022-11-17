cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(pio_pioscope C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_executable(pio_pioscope)

target_sources(pio_pioscope PRIVATE pioscope.c registers.h)

target_link_libraries(pio_pioscope PRIVATE pico_stdlib hardware_pio hardware_dma)
pico_add_extra_outputs(pio_pioscope)

pico_enable_stdio_usb(pio_logic_analyser 1)
pico_enable_stdio_uart(pio_pioscope 0)

# add url via pico_set_program_url
# example_auto_set_url(pio_pioscope)