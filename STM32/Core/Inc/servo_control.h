#ifndef __SERVO_CONTROL
#define __SERVO_CONTROL

#include "stm32f4xx.h"

typedef enum
{
    LEFT = -1,
    CENTER = 0,
    RIGHT = 1
} ServoDirection;


#define THUMBSTICK_CENTER       512
#define THUMBSTICK_DEADZONE     50

#define SERVO_MIN               500
#define SERVO_CENTER            1400
#define SERVO_MAX               2400
#define SERVO_STEP              20


ServoDirection thumbstick_get_direction(int thumbstick_val_x);
uint16_t update_pulse_width(ServoDirection direction, uint16_t pulse_width);


#endif
