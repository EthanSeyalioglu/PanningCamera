#include "uart.h"
#include "stm32f446xx.h"

#define GPIO_MODER2_AF      (GPIO_MODER_MODER2_1)
#define GPIO_PA2_AF7        (GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2)

#define CLK_FREQ            16000000
#define BAUDRATE            115200


static uint16_t calculate_brr(uint32_t periph_clk, uint32_t baudrate);
static void uart_write(int ch);

int __io_putchar(int ch)
{
    uart_write(ch);
    return ch;
}


void uart_init(void)
{
    /*** Set PA2 for USART2 TX ***/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER2;
    GPIOA->MODER |= GPIO_MODER2_AF;

    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
    GPIOA->AFR[0] |= GPIO_PA2_AF7;

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;


    USART2->CR1 |= USART_CR1_UE;
    USART2->CR1 &= ~USART_CR1_M;
    USART2->CR2 &= ~USART_CR2_STOP;

    USART2->BRR = calculate_brr(CLK_FREQ, BAUDRATE);
}

void uart_tx_start(void)
{
    USART2->CR1 |= USART_CR1_TE;
}

void uart_tx_stop(void)
{
    USART2->CR1 &= ~USART_CR1_TE;
}


static void uart_write(int ch)
{
    while (!(USART2->SR & USART_SR_TXE)) {}

    USART2->DR = (ch & 0xFF);
}


static uint16_t calculate_brr(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate / 2U)) / baudrate);
}