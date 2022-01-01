#pragma once

void initModel(
    int throttle_chan_par,
    int roll_chan_par,
    int pitch_chan_par,
    int yaw_chan_par,
    int (*channelFunction)(int),
    int channelsPointer[],
    int channelsLength,
    void (*setAllMotorsPointer)(int, _Bool),
    int updatefrequency_par);
void writeESCs(int value, bool facalibrationlse);
bool ready;