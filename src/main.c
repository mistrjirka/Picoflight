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
        int getChannel = SBUS_getChannel();
    #endif

    #if defined FLIGHT_FLYING_WING
        
    #endif
    #ifdef MOTOR1
            pwm_set_enabled(slice_num, true);
            gpio_set_function(MOTOR1, GPIO_FUNC_PWM);
            uint slice_num = pwm_gpio_to_slice_num(MOTOR1);
    #endif
    while (true)
        int throttle = 0;
    {
        throttle = getChannel(THROTTLE);
    }
}