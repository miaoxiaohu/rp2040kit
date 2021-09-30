# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "D:/pico/pico-sdk")


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
project(RP2040 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(RP2040 
  main.c 
  led.c 
  dut.c 
  icp.c 
  tmp117.c 
  program.c
  task_usb.c
  )

pico_set_program_name(RP2040 "RP2040")
pico_set_program_version(RP2040 "0.1")

pico_enable_stdio_uart(RP2040 1)
pico_enable_stdio_usb(RP2040 1)

# no_flash means the target is to run from RAM
# pico_set_binary_type(RP2040 no_flash)

# Add the standard library to the build
target_link_libraries(RP2040 pico_stdlib)

# Add any user requested libraries
target_link_libraries(RP2040
  pico_stdlib
  hardware_spi
  hardware_i2c
  hardware_dma
  hardware_pio
  hardware_interp
  hardware_timer
  hardware_watchdog
  hardware_clocks
  hardware_flash
  )

pico_add_extra_outputs(RP2040)

