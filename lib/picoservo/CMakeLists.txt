
add_library(picoservo
	src/pico_servo.c
)
target_include_directories(picoservo PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(picoservo pico_stdlib hardware_pwm)

#I used library pico-servo: https://github.com/irishpatrick/pico-servo/
