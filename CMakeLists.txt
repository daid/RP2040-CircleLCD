cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico CACHE STRING "Board type")

if (DEFINED ENV{PICO_SDK_PATH} OR PICO_SDK_PATH)
  include(pico_sdk_import.cmake)

  if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
  endif()
endif()
set(PROJECT_NAME "CircleLCD")
project(${PROJECT_NAME} C CXX ASM)

if (PICO_SDK_PATH)
  # Initialise the Raspberry Pi Pico SDK
  pico_sdk_init()
endif()

include_directories(./LCD)

add_executable(${PROJECT_NAME} main.cpp HW_Config.h HW_Config.cpp LCD/LCD_1in28.h LCD/LCD_1in28.cpp accel/QMI8658.cpp accel/QMI8658.h)

if (PICO_SDK_PATH)
  pico_enable_stdio_uart(${PROJECT_NAME} 0)
  pico_enable_stdio_usb(${PROJECT_NAME}  1)

  add_compile_options(-O2)

  target_link_libraries(${PROJECT_NAME}
                        pico_stdlib
                        hardware_spi
                        hardware_i2c
                        hardware_dma
                        hardware_pwm
  )
else()
  target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
  target_include_directories(${PROJECT_NAME} PRIVATE sim)
  file(GLOB_RECURSE SIM_SOURCES sim/*.cpp sim/*.h)
  find_package(SDL2 REQUIRED)
  add_library(${PROJECT_NAME}-sim STATIC ${SIM_SOURCES})
  target_link_libraries(${PROJECT_NAME}-sim PUBLIC ${SDL2_LIBRARIES} -lssp)
  target_compile_options(${PROJECT_NAME}-sim PUBLIC -Wall)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-sim)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

if(PICO_SDK_PATH)
  pico_add_extra_outputs(${PROJECT_NAME})
endif()
