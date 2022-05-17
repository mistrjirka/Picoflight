#include <stdio.h>
#include "pico/stdlib.h"
#include "flyingwing.h" 
#include "hardware/timer.h"
#include "picoservo.h"

int (*getChannel)(int);
int *channels[32];
void (*setAllMotors)(int, _Bool);
bool (*failsafeFunction)(void);

int throttle_chan = 3;
int roll_chan = 1;
int pitch_chan = 2;
int yaw_chan = 4;
int arm_chan = 5;
int arm_above = 1000;
int updatefrequency = 20;

int servoLeft = 7;
int servoRight = 6;

int servoLeftAngleMAX = 55;
int servoLeftAngleMIN = 20;
int servoLeftMultiplier = 1;

int servoRightAngleMAX = 55;
int servoRightAngleMIN = 20;
int servoRightMultiplier = 1;

bool arm = false;
bool failsafe = true;

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

long mapThrottle(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

repeating_timer_callback_t updateControls(repeating_timer_t *rt)
{
    int arm_val = (*getChannel)(arm_chan);
    arm = arm_val > arm_above;
    failsafe = (*failsafeFunction)();
    if(failsafe){
        gpio_put(LED_PIN, 0);
    }else{

        gpio_put(LED_PIN, 1);


    }
    printf("arm %d\n", arm);

    if (arm && !failsafe)
    {
        int throttle = (*getChannel)(throttle_chan);
        long mappedThrottle = mapThrottle(throttle, 172, 1810, 1000, 2000);
        printf("%d %d\n", throttle, mappedThrottle);
        (*setAllMotors)(mappedThrottle, false);
        servo_microseconds(10, mappedThrottle);
        int roll = (*getChannel)(roll_chan);
        int mappedRollLeft = mapThrottle(roll, 172, 1810, servoLeftAngleMIN, servoLeftAngleMAX);
        int mappedRollRight = mapThrottle(roll, 172, 1810, servoRightAngleMIN, servoRightAngleMAX);

        int pitch = (*getChannel)(pitch_chan);

        int mappedPitchLeft = mapThrottle(pitch, 172, 1810, servoLeftAngleMIN - (servoLeftAngleMIN + servoLeftAngleMAX) / 2, servoLeftAngleMAX - (servoLeftAngleMIN + servoLeftAngleMAX) / 2);
        int mappedPitchRight = mapThrottle(pitch, 1810, 172, servoRightAngleMIN - (servoRightAngleMIN + servoRightAngleMAX) / 2, servoRightAngleMAX - (servoRightAngleMIN + servoRightAngleMAX) / 2);
        //printf("Mapped pitch left: %d, pitch right: %d\n", mappedPitchLeft, mappedPitchRight);
        //printf("Mapped roll left: %d, roll right: %d\n", mappedRollLeft, mappedRollRight);
        int servoValLeft = mappedRollLeft + mappedPitchLeft;
        int servoValRight = mappedRollRight + mappedPitchRight;

        if(servoValLeft < servoLeftAngleMIN){
            servoValLeft = servoLeftAngleMIN;
        }
        if (servoValLeft > servoLeftAngleMAX)
        {
            servoValLeft = servoLeftAngleMAX;
        }

        if (servoValRight < servoRightAngleMIN)
        {
            servoValRight = servoRightAngleMIN;
        }
        if (servoValRight > servoRightAngleMAX)
        {
            servoValRight = servoRightAngleMAX;
        }

        servo_move_to(servoLeft, servoValLeft);
        servo_move_to(servoRight, servoValRight);

        //printf("servo left: %d servoright: %d \n", servoValLeft, servoValRight);
    }else{
        servo_move_to(servoLeft, (servoLeftAngleMIN + servoLeftAngleMAX) / 2);
        servo_move_to(servoRight, (servoRightAngleMIN + servoRightAngleMAX) / 2);
        (*setAllMotors)(1000, true);
    }
    return true;
}

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
    int servoright_par)
{
    *channels = channelsPointer;
    getChannel = *channelFunction;
    failsafeFunction = *failsafeFunction_par;
    setAllMotors = setAllMotorsPointer;
    arm_chan=arm_chan_par;
    arm_above = arm_above_par;
    throttle_chan = throttle_chan_par;
    roll_chan = roll_chan_par;
    pitch_chan = pitch_chan_par;
    yaw_chan = yaw_chan_par;

    updatefrequency = updatefrequency_par;
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    struct repeating_timer timer;
    servo_init();
    servo_clock_manual(50);

    servo_attach(servoLeft);
    //servo_attach(10);
    servo_attach(servoRight);

    add_repeating_timer_ms((1/updatefrequency)*1000, updateControls, NULL, &timer);
}
