#ifndef __UART_H
#define __UART_H

#include "stm32f4xx.h"
#include <stdbool.h>

#define ESP_MSG_LEN     12


// typedef enum
// {
//     PAN_NONE = 0,
//     PAN_RIGHT = 1,
//     PAN_LEFT = 2
// } pan_direction_t;

// typedef enum
// {
//     TILT_NONE = 0,
//     TILT_UP = 1,
//     TILT_DOWN = 2
// } tilt_direction_t;

typedef struct 
{
    uint8_t pan;
    uint8_t tilt;
} servo_cmd_pckt_t;


uint16_t calculate_brr(uint32_t periph_clk, uint32_t baudrate);

void uart_debug_init(void);
void uart_debug_init(void);
void uart_debug_tx_start(void);
void uart_debug_tx_stop(void);

void uart_esp_init(void);
void uart_esp_rx_start(void);
void uart_esp_rx_stop(void);

bool new_esp_servo_cmd_ready(void);
void get_new_servo_cmd(servo_cmd_pckt_t* cmd);
void reset_cmd_ready_status(void);

#endif