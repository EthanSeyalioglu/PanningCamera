#ifndef SERVOS_PWM_H
#define SERVOS_PWM_H

#include <stdint.h>
#include "stm32f4xx.h"

void tim2_pwm_init(void);
void tim2_servo_pwm_config(void);

void tim3_pwm_init(void);
void tim3_servo_pwm_config(void);

void pwm_start(void);
void pwm_stop(void);

void servo_x_set_position(uint16_t pulse_width);
void servo_y_set_position(uint16_t pulse_width);

#endif
