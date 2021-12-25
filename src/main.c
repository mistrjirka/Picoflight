#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "config.h"


#if defined FLIGHT_FLYING_WING         
    #include "flyingwing.h"
#endif
long mapThrottle(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("starting init\n");
    init_ESC_PWM(MOTOR1, DIVIDER, STOP_PULSE_PWM, MIN_PULSE_PWM, MAX_PULSE_PWM, IDLE_PULSE_PWM, ESC_CALIB_LOW, ESC_CALIB_HIGH);
    int throttle = 0;
    int mappedThrottle = 0;
#if defined RC_SBUS
    SBUS_init();
    int (*getChannel)(int) = &SBUS_getChannel;
#else
    SBUS_init();
    int (*getChannel)(int) = &SBUS_getChannel;
#endif

#if defined FLIGHT_FLYING_WING

#endif

    //irq_set_exclusive_handler(TIMER_IRQ_0, );
    while (true)
    {
        #ifdef MOTOR1   
            if (ready)
            {
            
                throttle = (*getChannel)(THROTTLE);
                mappedThrottle = mapThrottle(throttle, 172, 1810, 1000, 1800);
                writeESCs(mappedThrottle, false);
                //printf("%d\n", throttle);
                sleep_ms(20);
            }
        #endif
    }

}
