#include "thumbstick.h"
#include "stm32f446xx.h"
#include <stdint.h>

#define GPIO_MODER1_ANALOG      (GPIO_MODER_MODER1_0  | GPIO_MODER_MODER1_1)
#define GPIO_MODER4_ANALOG      (GPIO_MODER_MODER4_0  | GPIO_MODER_MODER4_1)
#define ADC_CR1_RES_10_BIT      (ADC_CR1_RES_0)
#define ADC_SEQ_LEN_2           (ADC_SQR1_L_0)
#define ADC_SQ1_CHANNEL1        (ADC_SQR3_SQ1_0)
#define ADC_SQ2_CHANNEL4        (ADC_SQR3_SQ2_2)

#define DMA_S0_LIFCR_ALL_Msk    (DMA_LIFCR_CFEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTCIF0)
#define DMA_PSIZE_16BIT         (DMA_SxCR_PSIZE_0)
#define DMA_MSIZE_16BIT         (DMA_SxCR_MSIZE_0)

static volatile uint16_t adc_dma_buffer[THUMBSTICK_DATA_BUFF_SIZE];

static void adc_thumbstick_config(void);
static void adc_thumbstick_dma_config(void);


void adc_thumbstick_dma_init(void)
{
    /*** Set PA1 and PA4 for ADC1 ***/ 
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER1;
    GPIOA->MODER |= GPIO_MODER1_ANALOG;

    GPIOA->MODER &= ~GPIO_MODER_MODER4;
    GPIOA->MODER |= GPIO_MODER4_ANALOG;

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    ADC1->CR2 |= ADC_CR2_ADON;
    for (int i = 0; i < 100; i++) {} // delay after ADC ON

    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    adc_thumbstick_config();
    adc_thumbstick_dma_config();

    ADC1->CR2 |= ADC_CR2_SWSTART;
}


static void adc_thumbstick_config(void)
{
    ADC1->CR1 |= ADC_CR1_SCAN;

    ADC1->CR1 &= ~ADC_CR1_RES;
    ADC1->CR1 |= ADC_CR1_RES_10_BIT;

    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SEQ_LEN_2;

    ADC1->SQR3 &= ~(ADC_SQR3_SQ1 | ADC_SQR3_SQ2);
    ADC1->SQR3 |= ADC_SQ1_CHANNEL1;
    ADC1->SQR3 |= ADC_SQ2_CHANNEL4;

    // ADC1->CR2 |= ADC_CR2_EOCS;
    ADC1->CR2 |= ADC_CR2_CONT;
    ADC1->CR2 |= ADC_CR2_DDS;
    ADC1->CR2 |= ADC_CR2_DMA;
}

static void adc_thumbstick_dma_config(void)
{
    DMA2_Stream0->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream0->CR & DMA_SxCR_EN) {}
    DMA2->LIFCR = DMA_S0_LIFCR_ALL_Msk;

    DMA2_Stream0->PAR = (uint32_t) &(ADC1->DR);
    DMA2_Stream0->M0AR = (uint32_t) &(adc_dma_buffer);
    DMA2_Stream0->NDTR = (uint16_t) THUMBSTICK_DATA_BUFF_SIZE;

    DMA2_Stream0->CR &= ~DMA_SxCR_CHSEL_Msk;
    DMA2_Stream0->CR &= ~DMA_SxCR_DIR_Msk;

    DMA2_Stream0->CR |= DMA_SxCR_MINC;

    DMA2_Stream0->CR &= ~DMA_SxCR_PSIZE_Msk;
    DMA2_Stream0->CR |= DMA_PSIZE_16BIT;

    DMA2_Stream0->CR &= ~DMA_SxCR_MSIZE_Msk;
    DMA2_Stream0->CR |= DMA_MSIZE_16BIT;

    DMA2_Stream0->CR |= DMA_SxCR_CIRC;

    DMA2_Stream0->CR |= DMA_SxCR_EN;
}


void adc_thumbstick_read(uint16_t* values)
{
    values[0] = adc_dma_buffer[0];
    values[1] = adc_dma_buffer[1];
}

// void adc_thumbstick_scan(void)
// {
//     ADC1->CR2 |= ADC_CR2_SWSTART;
// }

// void adc_thumbstick_read(int* values)
// {
//     for (int i = 0; i < 2; i++)
//     {
//         while (!(ADC1->SR & (ADC_SR_EOC | ADC_SR_OVR))) {}

//         if (ADC1->SR & ADC_SR_OVR) 
//         {
//             ADC1->SR &= ~ADC_SR_OVR;
//             return;
//         }

//         *values++ = ADC1->DR;
//     }

// }