#ifndef TIM2_PWM_H
#define TIM2_PWM_H

#include <stdint.h>
#include "stm32f4xx.h"

void tim2_pwm_init(void);
void tim2_servo_pwm_config(void);
void tim2_pwm_start(void);
void tim2_pwm_stop(void);
void servo_set_position(uint16_t pulse_width);

#endif
