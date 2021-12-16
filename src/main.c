#include <stdio.h>
#include "pico/stdlib.h"
#include "sbus.h"
#include "hardware/pwm.h"

int main()
{
    stdio_init_all();
    SBUS_init();
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
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