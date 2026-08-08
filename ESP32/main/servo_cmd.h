#ifndef __SERVO_CMD_H
#define __SERVO_CMD_H

typedef enum
{
    PAN_NONE = 0,
    PAN_RIGHT = 1,
    PAN_LEFT = 2
} pan_direction_t;

typedef enum
{
    TILT_NONE = 0,
    TILT_UP = 1,
    TILT_DOWN = 2
} tilt_direction_t;


typedef struct
{
    uint8_t pan;
    uint8_t tilt;
} servo_cmd_t;


void uart_add_servo_cmd(const servo_cmd_t* cmd);


#endif