#include "thumbstick.h"
#include "servos_pwm.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define THUMBSTICK_CENTER       512
#define THUMBSTICK_DEADZONE     50

#define SERVO_MIN               500
#define SERVO_CENTER            1400
#define SERVO_MAX               2400
#define SERVO_STEP              20

typedef enum
{
    LEFT = -1,
    CENTER = 0,
    RIGHT = 1
} ServoDirection;

ServoDirection thumbstick_get_direction(int thumbstick_val_x);
uint16_t update_pulse_width(ServoDirection direction, uint16_t pulse_width);

int main(void)
{
    uint16_t pulse_width_x, pulse_width_y;
    int thumbstick_values[2];
    ServoDirection direction_x, direction_y;

    uart_init();
    uart_tx_start();
    printf("UART Initialized\n\n");

    adc_thumbstick_init();
    adc_thumbstick_config();
    printf("ADC Initialized\n\n");

    tim2_pwm_init();
    tim2_servo_pwm_config();
    tim3_pwm_init();
    tim3_servo_pwm_config();
    pwm_start();
    printf("PWM Initialized\n\n");


    pulse_width_x = pulse_width_y = SERVO_CENTER;

    servo_x_set_position(pulse_width_x);
    servo_y_set_position(pulse_width_y);

    while (1)
    {
        adc_thumbstick_scan();
        adc_thumbstick_read(thumbstick_values);

        direction_x = thumbstick_get_direction(thumbstick_values[0]);
        direction_y = thumbstick_get_direction(thumbstick_values[1]);

        pulse_width_x = update_pulse_width(direction_x, pulse_width_x);
        pulse_width_y = update_pulse_width(direction_y, pulse_width_y);

        servo_x_set_position(pulse_width_x);
        servo_y_set_position(pulse_width_y);

        for (int i = 0; i < 10000; i++) {}

        printf("Pulse Width: %d\n", pulse_width_x);
        printf("Thumbstick X: %d\n", thumbstick_values[0]);
        printf("Servo Direction: %d\n\n", direction_x);
        // printf("Thumbstick X: %d\nThumbstick Y: %d\n\n", thumbstick_values[0], thumbstick_values[1]);
    }
}


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