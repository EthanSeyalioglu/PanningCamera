#include "servo_control.h"
#include <stdlib.h>


ServoDirection thumbstick_get_direction(int thumbstick_val_x)
{
    if (abs(thumbstick_val_x - THUMBSTICK_CENTER) > THUMBSTICK_DEADZONE)
    {
        return (thumbstick_val_x >= THUMBSTICK_CENTER ? RIGHT : LEFT);
    }
    else
    {
        return CENTER;
    }
}

uint16_t update_pulse_width(ServoDirection direction, uint16_t pulse_width)
{
    uint16_t result = pulse_width;

    if (direction == RIGHT)
    {
        result += SERVO_STEP;
        result = (result > SERVO_MAX ? SERVO_MAX : result); // 2400 ceiling
    }
    else if (direction == LEFT)
    {
        result -= SERVO_STEP;
        result = (result < SERVO_MIN ? SERVO_MIN : result); // 600 floor
    }

    return result;
}