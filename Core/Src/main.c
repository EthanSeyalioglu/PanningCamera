#include "thumbstick.h"
#include "tim2_pwm.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CENTER      512
#define DEADZONE   10

uint8_t position;
uint16_t pulse_width;
int thumbstick_values[2];
uint8_t direction;

uint16_t get_pulse_width(uint8_t position);
void get_servo_direction(int thumbstick_val_x);

int main(void)
{
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


  pulse_width = 1400;

  while (1)
  {

    adc_thumbstick_scan();
    adc_thumbstick_read(thumbstick_values);

    get_servo_direction(thumbstick_values[0]);

    if (direction == 2)
    {
        pulse_width += 100;
        pulse_width = pulse_width > 2400 ? 2400 : pulse_width; // min
    }
    else if (direction == 0)
    {
        pulse_width -= 100;
        pulse_width = pulse_width < 100 ? 100 : pulse_width; // max
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


void get_servo_direction(int thumbstick_val_x)
{
    if (abs(thumbstick_val_x - CENTER) > DEADZONE)
    {
        if (thumbstick_val_x >= 512)
        {
            direction = 2;
        }
        else
        {
            direction = 0;
        }
    }
    else
    {
        direction = 1;
    }
}