#include "servos_pwm.h"
#include <stdio.h>


#define GPIO_MODER5_AF          (GPIO_MODER_MODER5_1)
#define GPIO_MODER6_AF          (GPIO_MODER_MODER6_1)
#define GPIO_PA5_AF1            (GPIO_AFRL_AFSEL5_0)
#define GPIO_PA6_AF2            (GPIO_AFRL_AFSEL6_1)
#define TIM_CCMR1_OC1M_PWM1     (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2)

#define SERVO_TIMER_PSC         16 - 1
#define SERVO_TIMER_ARR         2000 - 1
#define SERVO_CENTER_PULSE      1400



void tim2_pwm_init(void)
{
    /**** Set PA5 for TIM2 ****/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER |= GPIO_MODER5_AF;

    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
    GPIOA->AFR[0] |= GPIO_PA5_AF1;    

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void tim2_servo_pwm_config(void)
{
    // Set mode to upcounting
    TIM2->CR1 &= ~TIM_CR1_DIR;

    // Edge aligned mode 
    TIM2->CR1 &= ~TIM_CR1_CMS;

    // 16MHz is default clock
    TIM2->PSC = SERVO_TIMER_PSC; // 16 MHz / 16 = 1 MHz

    TIM2->ARR = SERVO_TIMER_ARR; // Set 50 Hz frequency (1 MHz / 20000 = 50 Hz)

    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;

    // Set OC1M to PWM mode 1
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_PWM1;

    TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload

    // Enable auto-reload preload
    TIM2->CR1 |= TIM_CR1_ARPE;

    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CCER &= ~TIM_CCER_CC1P; // OC1 active high

    TIM2->CCR1 = SERVO_CENTER_PULSE;

    TIM2->EGR |= TIM_EGR_UG; // Update 
}
////////////////////

void tim3_pwm_init(void)
{
    /**** Set PA6 for TIM3 ****/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER6;
    GPIOA->MODER |= GPIO_MODER6_AF;

    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
    GPIOA->AFR[0] |= GPIO_PA6_AF2;    

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
}

void tim3_servo_pwm_config(void)
{
    // Set mode to upcounting
    TIM3->CR1 &= ~TIM_CR1_DIR;

    // Edge aligned mode 
    TIM3->CR1 &= ~TIM_CR1_CMS;

    // 16MHz is default clock
    TIM3->PSC = 16 - 1; // 16 MHz / 16 = 1 MHz

    TIM3->ARR = 20000 - 1; // Set 50 Hz frequency (1 MHz / 20000 = 50 Hz)

    TIM3->CCMR1 &= ~TIM_CCMR1_CC1S;

    // Set OC1M to PWM mode 1
    TIM3->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_PWM1;

    TIM3->CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload

    // Enable auto-reload preload
    TIM3->CR1 |= TIM_CR1_ARPE;

    TIM3->CCER |= TIM_CCER_CC1E;
    TIM3->CCER &= ~TIM_CCER_CC1P; // OC1 active high

    TIM3->CCR1 = SERVO_CENTER_PULSE;

    TIM3->EGR |= TIM_EGR_UG; // Update 
}


void pwm_start(void)
{
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void pwm_stop(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM3->CR1 &= ~TIM_CR1_CEN;
}


void servo_x_set_position(uint16_t pulse_width)
{
    TIM2->CCR1 = pulse_width;
    printf("CALLED");
}

void servo_y_set_position(uint16_t pulse_width)
{
    TIM3->CCR1 = pulse_width;
}