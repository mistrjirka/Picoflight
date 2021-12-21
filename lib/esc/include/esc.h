#pragma once //
int init_ESC_PWM(int pin, int divider_in, int stop_pulse_in, int min_pulse_in, int max_pulse_in, int define_pulse_in, int esc_calib_low_in, int esc_calib_high_in);
void writeESCs(int value);
bool ready;