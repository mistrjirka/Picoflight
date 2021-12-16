#pragma once
#include "sbus.h"
#include "esc.h"

#define RC_SBUS
#define FLIGHT_FLYING_WING 

#define RC_CHANNELS 

#define ROLL 0
#define PITCH 1
#define THROTTLE 2
#define YAW 3

#define MOTOR1 10
#define DIVIDER 125 // divider 125mhz
#define MIN_PULSE_PWM  1000
#define MAX_PULSE_PWM  1800
#define STOP_PULSE_PWM 1000
#define IDLE_PULSE_PWM 1060
#define ESC_CALIB_LOW 1000
#define ESC_CALIB_HIGH 2000