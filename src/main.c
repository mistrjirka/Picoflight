#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "config.h"


#if defined FLIGHT_FLYING_WING         
    #include "flyingwing.h"
#endif

int main()
{
    stdio_init_all();
    
    #if defined RC_SBUS
        SBUS_init(); 
        int (*getChannel)(int) = &SBUS_getChannel;
    #else
        SBUS_init();
        int (*getChannel)(int) = &SBUS_getChannel;
    #endif

    #if defined FLIGHT_FLYING_WING
        
    #endif
    #ifdef MOTOR1
        init_ESC_PWM(MOTOR1, DIVIDER, STOP_PULSE_PWM, MIN_PULSE_PWM, MAX_PULSE_PWM, IDLE_PULSE_PWM, ESC_CALIB_LOW, ESC_CALIB_HIGH);
#endif
    int throttle = 0;

        while (true)
        {

#ifdef MOTOR1
            throttle = (*getChannel)(THROTTLE);

#endif
        }
}