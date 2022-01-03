#pragma once //
int init_ESC_PWM(int pin, int divider_in, int stop_pulse_in, int min_pulse_in, int max_pulse_in, int define_pulse_in, int esc_calib_low_in, int esc_calib_high_in);
void updateControls(
    int channels[],
    int throttle_chan,
    int roll_chan,
    int pitch_chan,
    int yaw_chan,
    int arm_chan);
void writeESCs(int value, bool calibration);