#pragma once

void initModel(
    int throttle_chan_par,
    int roll_chan_par,
    int pitch_chan_par,
    int yaw_chan_par,
    int (*channelFunction)(int),
    int *channelsPointer[],
    void (*initMotorsPointer)(void),
    void (*setAllMotorsPointer)(void),
    int updatefrequency_par);
void writeESCs(int value, bool facalibrationlse);
bool ready;