#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

int stop_pulse = 1000;
int min_pulse = 1000;
int esc_calib_low = 1000;
int esc_calib_high = 2000;
int max_pulse = 1800;
int idle_pulse = 1065;
int divider = 125;
bool ready = false;
int escs[] = {};

// for now analog, todo: blheli (probably with pio)
void calibrateESCs(int pin)
{
    writeESCs(esc_calib_high);
    delay(5000);
    writeESCs(esc_calib_low);
    delay(5000);
}

void writeESCs(int value)
{
    if (value < 1000)
    {
        value = stop_pulse;
    }
    int esc_length = sizeof(escs) / sizeof(escs[0]);
    for (int i = 0; i < esc_length; i++)
    {
        int esc = escs[i];
        int chann = pwm_gpio_to_slice_num(esc);
        pwm_set_chan_level(escs[i], chann, value);
    }
}
int init_ESC_PWM(int pin, int divider_in, int stop_pulse_in, int min_pulse_in, int max_pulse_in, int idle_pulse_in, int esc_calib_low_in, int esc_calib_high_in)
{
    stop_pulse = stop_pulse_in;
    min_pulse = min_pulse_in;
    max_pulse = max_pulse_in;
    idle_pulse = idle_pulse_in;
    divider = divider_in;

    //initializing pwm thanks to pico-examples
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    // clock is 125Mhz, so 1 tick is 8 nanoseconds. I want a cycle of 1 micro, so the clock divider is 125
    pwm_set_clkdiv(pin, divider); // copied from https://github.com/and-b/arduino-pico_esc/blob/master/arduino-pico_esc.ino
    // this is the maximum counter (16 bit 0-65535). set period of 2040 micros, 490.19 Hz
    pwm_set_wrap(pin, 2039);
    int chan = pwm_gpio_to_slice_num(pin);
    pwm_set_chan_level(pin, chan, stop_pulse);
    pwm_set_enabled(pin, true);
}

