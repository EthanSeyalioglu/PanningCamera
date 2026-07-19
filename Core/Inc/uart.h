#ifndef __UART_H
#define __UART_H

#include "stm32f4xx.h"

#define ESP_MSG_LEN     12

extern volatile char msg_buffer[ESP_MSG_LEN];
extern volatile uint8_t msg_ready;

uint16_t calculate_brr(uint32_t periph_clk, uint32_t baudrate);

void uart_debug_init(void);
void uart_debug_init(void);
void uart_debug_tx_start(void);
void uart_debug_tx_stop(void);

void uart_esp_init(void);
void uart_esp_rx_start(void);
void uart_esp_rx_stop(void);

#endif