#include "tim2_pwm.h"


#define GPIO_MODER5_AF          (GPIO_MODER_MODER5_1)
#define GPIO_PA5_AF1            (GPIO_AFRL_AFSEL5_0)
#define TIM_CCMR1_OC1M_PWM1     (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2)


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
    TIM2->PSC = 16 - 1; // 16 MHz / 16 = 1 MHz

    TIM2->ARR = 20000 - 1; // Set 50 Hz frequency (1 MHz / 20000 = 50 Hz)

    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;

    // Set OC1M to PWM mode 1
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_PWM1;

    TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload

    // Enable auto-reload preload
    TIM2->CR1 |= TIM_CR1_ARPE;

    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CCER &= ~TIM_CCER_CC1P; // OC1 active high

    TIM2->CCR1 = 1500;

    TIM2->EGR |= TIM_EGR_UG; // Update 
}


void tim2_pwm_start(void)
{
    TIM2->CR1 |= TIM_CR1_CEN;
}

void tim2_pwm_stop(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
}


void servo_set_position(uint16_t pulse_width)
{
    TIM2->CCR1 = pulse_width;
}