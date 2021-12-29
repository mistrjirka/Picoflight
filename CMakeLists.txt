cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
# Pull in SDK (must be before project)
project (Picoflight C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)



if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif() 

#pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/uart_rx.pio)


# pull in common dependencies


# create map/bin/hex file etc.
if (TARGET tinyusb_device)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/include)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/flightmodel)
    add_executable(${PROJECT_NAME}
        src/main.c
    )
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/picoflight)
    
    #libraries
    target_link_libraries(${PROJECT_NAME} PUBLIC
        pico_stdlib 
        pico_multicore
        hardware_uart
        hardware_pio
        hardware_pwm
        sbus
        crsf
        flyingwing
        esc
    )

    pico_enable_stdio_usb(${PROJECT_NAME} 1)
    pico_enable_stdio_uart(${PROJECT_NAME} 0)

    pico_add_extra_outputs(${PROJECT_NAME})
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()


