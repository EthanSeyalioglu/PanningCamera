#ifndef __SERVO_CMD_H
#define __SERVO_CMD_H

typedef enum
{
    PAN_NONE = 0,
    PAN_RIGHT,
    PAN_LEFT
} pan_direction_t;

typedef enum
{
    TILT_NONE = 0,
    TILT_UP,
    TILT_DOWN
} tilt_direction_t;


typedef struct
{
    pan_direction_t pan;
    tilt_direction_t tilt;
} servo_cmd_t;


void uart_add_servo_cmd(const servo_cmd_t* cmd);


#endif