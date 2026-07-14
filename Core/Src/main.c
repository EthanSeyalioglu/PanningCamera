#include "thumbstick.h"
#include "servos_pwm.h"
#include "servo_control.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


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

        for (int i = 0; i < 5000; i++) {}

        printf("Pulse Width: %d\n", pulse_width_x);
        printf("Thumbstick X: %d\n", thumbstick_values[0]);
        printf("Servo Direction: %d\n\n", direction_x);
        // printf("Thumbstick X: %d\nThumbstick Y: %d\n\n", thumbstick_values[0], thumbstick_values[1]);
    }
}