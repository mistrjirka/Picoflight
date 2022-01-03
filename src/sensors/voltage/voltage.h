#pragma once

void initModel(
    int throttle_chan_par,
    int roll_chan_par,
    int pitch_chan_par,
    int yaw_chan_par,
    int arm_chan_par,
    int arm_above_par,
    int (*channelFunction)(int),
    int channelsPointer[],
    int channelsLength,
    void (*setAllMotorsPointer)(int, _Bool),
    int updatefrequency_par,
    bool (*failsafeFunction_par)(void),
    int servoleft_par,
    int servoright_par);

bool ready;