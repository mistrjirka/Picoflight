#include <stdio.h>
#include "pico/stdlib.h"
#include "flyingwing.h" 
#include "hardware/timer.h"

int (*getChannel)(int);
int *channels[];
void (*initMotors)(void);
void (*setAllMotors)(void);
int throttle_chan = 3;
int roll_chan = 1;
int pitch_chan = 2;
int yaw_chan = 4;
int updatefrequency = 20;

void updateControls(
    int channels[],
    int throttle_chan,
    int roll_chan,
    int pitch_chan,
    int yaw_chan,
    int arm_chan)
{
}

void initModel(
    int throttle_chan_par,
    int roll_chan_par,
    int pitch_chan_par,
    int yaw_chan_par,
    int *channelFunction,
    int *channelsPointer[],
    void *initMotorsPointer,
    void *setAllMotorsPointer,
    int updatefrequency_par)
{
    *channels = &channelsPointer;
    getChannel = &channelFunction;
    initMotors = &initMotorsPointer;
    setAllMotors = &setAllMotorsPointer;

    throttle_chan = throttle_chan_par;
    roll_chan = roll_chan_par;
    pitch_chan = pitch_chan_par;
    yaw_chan = yaw_chan_par;

    updatefrequency = updatefrequency_par;

    struct repeating_timer timer;
    add_repeating_timer_ms(updatefrequency, updateControls, NULL, &timer);
}

