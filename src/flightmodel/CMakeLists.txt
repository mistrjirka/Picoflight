add_library(flyingwing STATIC flyingwing.c)
target_include_directories(flyingwing PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(flyingwing
    pico_stdlib 
    pico_multicore
    hardware_timer
    picoservo
)
