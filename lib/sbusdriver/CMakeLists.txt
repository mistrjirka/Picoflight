add_library(sbus STATIC sbus.c)
target_include_directories(sbus PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
#libraries

target_link_libraries(sbus
    pico_stdlib 
    pico_multicore
    hardware_uart
    hardware_pio
)

