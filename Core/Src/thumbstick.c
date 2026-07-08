#include "thumbstick.h"

#define GPIO_MODER1_ANALOG      (GPIO_MODER_MODER1_0  | GPIO_MODER_MODER1_1)

void adc_thumbstick_init(void)
{
    /*** Set PA1 for ADC ***/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER1;
    GPIOA->MODER |= GPIO_MODER1_ANALOG;

    RCC->APB1ENR |= RCC_APB2ENR_ADC1EN;
}


void adc_thumbstick_config(void)
{
    
}


void adc_thumbstick_start(void)
{

}

void adc_thumbstick_stop(void)
{
    
}