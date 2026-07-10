#include "thumbstick.h"
#include "tim2_pwm.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define THUMBSTICK_CENTER       512
#define THUMBSTICK_DEADZONE     20

#define SERVO_MIN               500
#define SERVO_CENTER            1400
#define SERVO_MAX               2400
#define SERVO_STEP              100

typedef enum
{
    LEFT = -1,
    CENTER = 0,
    RIGHT = 1
} ServoDirection;

uint16_t get_pulse_width(uint8_t position);
ServoDirection thumbstick_get_direction(int thumbstick_val_x);

int main(void)
{
    uint16_t pulse_width;
    int thumbstick_values[2];
    ServoDirection direction;

    uart_init();
    uart_tx_start();
    printf("UART Initialized\n\n");

    adc_thumbstick_init();
    adc_thumbstick_config();
    printf("ADC Initialized\n\n");

    tim2_pwm_init();
    tim2_servo_pwm_config();
    tim2_pwm_start();
    printf("PWM Initialized\n\n");


    pulse_width = SERVO_CENTER;

    while (1)
    {
        adc_thumbstick_scan();
        adc_thumbstick_read(thumbstick_values);

        direction = thumbstick_get_direction(thumbstick_values[0]);

        if (direction == RIGHT)
        {
            pulse_width += SERVO_STEP;
            pulse_width = (pulse_width > SERVO_MAX ? SERVO_MAX : pulse_width); // 2400 ceiling
        }
        else if (direction == LEFT)
        {
            pulse_width -= SERVO_STEP;
            pulse_width = (pulse_width < SERVO_MIN ? SERVO_MIN : pulse_width); // 600 floor
        }

        servo_set_position(pulse_width);

        for (int i = 0; i < 10000; i++) {}

        // printf("Pulse Width: %d\n", pulse_width);

        // if (pulse_width > 2500 || pulse_width < 500)
        // {
        //     polarity = (polarity - 1) * -1;
        // }

        printf("Pulse Width: %d\n", pulse_width);
        printf("Thumbstick X: %d\n", thumbstick_values[0]);
        printf("Servo Direction: %d\n\n", direction);
        // printf("Thumbstick X: %d\nThumbstick Y: %d\n\n", thumbstick_values[0], thumbstick_values[1]);
    }
}

// pulse width 500 is starting position, 2500 is ending


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