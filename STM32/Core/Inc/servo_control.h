#ifndef __SERVO_CONTROL
#define __SERVO_CONTROL

#include "stm32f4xx.h"

typedef enum
{
    DIR_NONE = 0,
    DIR_RIGHT = 1,
    DIR_LEFT = 2
} ServoDirection;


#define THUMBSTICK_CENTER       512
#define THUMBSTICK_DEADZONE     50

#define SERVO_MIN               400
#define SERVO_CENTER            1400
#define SERVO_MAX               2400
#define SERVO_STEP              300


ServoDirection thumbstick_get_direction(int thumbstick_val_x);
uint16_t update_pulse_width(ServoDirection direction, uint16_t pulse_width);


#endif
