add_library(crsf STATIC crsf.c)
target_include_directories(crsf PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
#libraries

target_link_libraries(crsf
    pico_stdlib 
    pico_multicore
    hardware_uart
    hardware_pio
)

