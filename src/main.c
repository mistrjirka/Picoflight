#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "config.h"

#if defined FLIGHT_FLYING_WING         
    #include "flyingwing.h";
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
    while (true)
    
    {
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
        printf("test");
        printf("channel 12: %d \n ", SBUS_getChannel(12));
        printf("channel 16: %d", SBUS_getChannel(16)); /*
        /*while (1)
        {
            // tight_loop_contents();
        }*/
    }
}