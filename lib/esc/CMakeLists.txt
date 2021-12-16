add_library(esc STATIC esc.c)
target_include_directories(esc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
#libraries

target_link_libraries(esc
    pico_stdlib 

    hardware_pwm
)

