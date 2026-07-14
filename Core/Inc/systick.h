#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h"

void systick_init_ms(uint32_t period_ms);
uint32_t system_get_ticks();

#endif