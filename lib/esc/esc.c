#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "esc.h"

int stop_pulse = 1000;
int min_pulse = 1000;
int esc_calib_low = 1000;
int esc_calib_high = 2000;
int max_pulse = 1800;
int idle_pulse = 1065;
int divider = 125;
int escs[] = {};
bool ready = false;

// for now analog, todo: blheli (probably with pio)
void calibrateESCs(int pin)
{
    printf("starting calibration\n");

    writeESCs(esc_calib_high);
    sleep_ms(5000);
    writeESCs(esc_calib_low);
    sleep_ms(5000);
    printf("ending calibration\n");
}

void writeESCs(int value)
{
    printf("writing\n");
    if (value < 1000)
    {
        value = stop_pulse;
    }
    int esc_length = sizeof(escs) / sizeof(escs[0]);
    for (int i = 0; i < esc_length; i++)
    {
        printf("Motor: %d\n", i);
        int esc = escs[i];
        int chann = pwm_gpio_to_channel(esc);
        int slice  = pwm_gpio_to_slice_num(esc;
        pwm_set_chan_level(slice, chann, value);
    }
    printf("end of writing\n");
}
int init_ESC_PWM(int pin, int divider_in, int stop_pulse_in, int min_pulse_in, int max_pulse_in, int idle_pulse_in, int esc_calib_low_in, int esc_calib_high_in)
{
    printf("starting init\n");
    stop_pulse = stop_pulse_in;
    min_pulse = min_pulse_in;
    max_pulse = max_pulse_in;
    idle_pulse = idle_pulse_in;
    divider = divider_in;

    //initializing pwm thanks to pico-examples
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    // clock is 125Mhz, so 1 tick is 8 nanoseconds. I want a cycle of 1 micro, so the clock divider is 125

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, divider);
    pwm_set_wrap(slice_num, 2039);
    int chan = pwm_gpio_to_channel(pin);
    pwm_set_chan_level(slice_num, chan, stop_pulse);
    pwm_set_enabled(slice_num, true);
    calibrateESCs(pin);
    ready = true;
    printf("end of init\n");
}
