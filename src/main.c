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
    sleep_ms(6000);
    printf("starting init\n");
    init_ESC_PWM(MOTOR1, DIVIDER, STOP_PULSE_PWM, MIN_PULSE_PWM, MAX_PULSE_PWM, IDLE_PULSE_PWM, ESC_CALIB_LOW, ESC_CALIB_HIGH);
    int throttle = 0;
#if defined RC_SBUS
        SBUS_init(); 
        int (*getChannel)(int) = &SBUS_getChannel;
    #else
        SBUS_init();
        int (*getChannel)(int) = &SBUS_getChannel;
    #endif

    #if defined FLIGHT_FLYING_WING
        
    #endif
    

    while (true)
    {
        #ifdef MOTOR1   
            if (ready)
            {
            
                throttle = (*getChannel)(THROTTLE);
                writeESCs(throttle);
                //printf("%d\n", throttle);
                sleep_ms(1000);
            }
        #endif
    }

}