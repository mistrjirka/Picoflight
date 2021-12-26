#pragma once 

void initModel(
    int throttle_chan_par,
    int roll_chan_par,
    int pitch_chan_par,
    int yaw_chan_par,
    int *channelFunction,
    int *channelsPointer[],
    void *initMotorsPointer,
    void *setAllMotorsPointer,
    int updatefrequency);
void writeESCs(int value, bool facalibrationlse);
bool ready;