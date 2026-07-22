#include "thumbstick.h"
#include "stm32f446xx.h"

#define GPIO_MODER1_ANALOG      (GPIO_MODER_MODER1_0  | GPIO_MODER_MODER1_1)
#define GPIO_MODER4_ANALOG      (GPIO_MODER_MODER4_0  | GPIO_MODER_MODER4_1)
#define ADC_CR1_RES_10_BIT      (ADC_CR1_RES_0)
#define ADC_SEQ_LEN_2           (ADC_SQR1_L_0)
#define ADC_SQ1_CHANNEL1        (ADC_SQR3_SQ1_0)
#define ADC_SQ2_CHANNEL4        (ADC_SQR3_SQ2_2)

void adc_thumbstick_init(void)
{
    /*** Set PA1 and PA4 for ADC ***/ 
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER1;
    GPIOA->MODER |= GPIO_MODER1_ANALOG;

    GPIOA->MODER &= ~GPIO_MODER_MODER4;
    GPIOA->MODER |= GPIO_MODER4_ANALOG;

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    ADC1->CR2 |= ADC_CR2_ADON;
    for (int i = 0; i < 100; i++) {} // delay after ADC ON
}


void adc_thumbstick_config(void)
{
    ADC1->CR1 |= ADC_CR1_SCAN;

    ADC1->CR1 &= ~ADC_CR1_RES;
    ADC1->CR1 |= ADC_CR1_RES_10_BIT;

    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SEQ_LEN_2;

    ADC1->SQR3 &= ~(ADC_SQR3_SQ1 | ADC_SQR3_SQ2);
    ADC1->SQR3 |= ADC_SQ1_CHANNEL1;
    ADC1->SQR3 |= ADC_SQ2_CHANNEL4;

    ADC1->CR2 |= ADC_CR2_EOCS;
    // ADC1->CR2 |= ADC_CR2_CONT;
}


void adc_thumbstick_scan(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

void adc_thumbstick_read(int* values)
{
    for (int i = 0; i < 2; i++)
    {
        while (!(ADC1->SR & (ADC_SR_EOC | ADC_SR_OVR))) {}

        if (ADC1->SR & ADC_SR_OVR) 
        {
            ADC1->SR &= ~ADC_SR_OVR;
            return;
        }

        *values++ = ADC1->DR;
    }

}