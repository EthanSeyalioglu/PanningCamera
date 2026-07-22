#ifndef __THUMBSTICK_H
#define __THUMBSTICH_H

#include "stm32f4xx.h"

void adc_thumbstick_init(void);
void adc_thumbstick_config(void);
void adc_thumbstick_scan(void);
void adc_thumbstick_read(int* values);

#endif