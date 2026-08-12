#include "stm32f446xx.h"
#include "uart.h"
#include "servo_control.h"

#define GPIO_MODER10_AF     (GPIO_MODER_MODER10_1)
#define GPIO_PA10_AF7       (GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2)

#define CLK_FREQ            16000000
#define BAUDRATE            115200


static volatile uint8_t cmd_rx_index = 0;
static volatile bool servo_cmd_ready = false;
static volatile servo_cmd_pckt_t servo_cmd = {
    .pan = DIR_NONE,
    .tilt = DIR_NONE
};


void uart_esp_init(void)
{
    /*** Set PA10 for USART1 RX ***/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER10;
    GPIOA->MODER |= GPIO_MODER10_AF;

    GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10;
    GPIOA->AFR[1] |= GPIO_PA10_AF7;

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;


    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 &= ~USART_CR1_M;
    USART1->CR1 |= USART_CR1_RXNEIE;

    USART1->CR2 &= ~USART_CR2_STOP;

    USART1->BRR = calculate_brr(CLK_FREQ, BAUDRATE);

    NVIC_EnableIRQ(USART1_IRQn);

    uart_esp_rx_start();
}


void uart_esp_rx_start(void)
{
    USART1->CR1 |= USART_CR1_RE;
}

void uart_esp_rx_stop(void)
{
    USART1->CR1 &= ~USART_CR1_RE;
}

bool new_esp_servo_cmd_ready(void)
{
    return servo_cmd_ready;
}

void get_new_servo_cmd(servo_cmd_pckt_t* cmd)
{
    *cmd = servo_cmd;
}

void reset_cmd_ready_status(void)
{
    servo_cmd_ready = false;
}


void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_SR_RXNE)
    {
        if (cmd_rx_index == 0)
        {
            servo_cmd.pan = USART1->DR;
            cmd_rx_index = 1;
        }
        else
        {
            servo_cmd.tilt = USART1->DR;
            cmd_rx_index = 0;
            servo_cmd_ready = true;
        }
    }
}