#ifndef __THUMBSTICK_H
#define __THUMBSTICH_H

#include "stm32f4xx.h"

#define THUMBSTICK_DATA_BUFF_SIZE   2

void adc_thumbstick_dma_init(void);
// void adc_thumbstick_config(void);
// void adc_thumbstick_scan(void);
void adc_thumbstick_read(uint16_t* values);

#endif