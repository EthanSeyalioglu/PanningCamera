#include "tim2_pwm.h"
#include <stdint.h>

uint8_t position;
uint16_t pulse_width;

uint16_t get_pulse_width(uint8_t position);

int main(void)
{
  tim2_pwm_init();
  tim2_servo_pwm_config();
  tim2_pwm_start();

  position = 1;

  while (1)
  {
    for (int i = 0; i < 900000; i++) {}

    pulse_width = get_pulse_width(position);

    servo_set_position(1400);

    if (position > 0)
    {
        position = 0;
    }
    else
    {
        position = 2;
    }
  }
}

uint16_t get_pulse_width(uint8_t position)
{
    if (position == 0)
    {
        return 500;
    }
    else if (position == 1)
    {
        return 1500;
    }
    else
    {
        return 2500;
    }
}