#include "tim2_pwm.h"
// #include "stm32f446xx.h"

// #define GPIOAEN     (1U << 0)
// #define TIM2EN      (1U << 0)

// #define TIM_CR1_ARPE    (1U << 7)
// #define TIM_CR1_DIR     (1U << 4)
// #define TIM_CCMR1_OC1PE (1U << 3)
// #define TIM_EGR_UG      (1U << 0)
// #define TIM_CCER_CC1E   (1U << 0)
// #define TIM_CCER_CC1P   (1U << 1)

// #define TIM_CR1_CEN    (1U << 0)

#define GPIO_MODER5_AF          (GPIO_MODER_MODER5_1)
#define GPIO_PA5_AF1            (GPIO_AFRL_AFSEL5_0)
#define TIM_CCMR1_OC1M_PWM1     (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2)


void tim2_pwm_init(void)
{
    /**** Set PA5 for TIM2 ****/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER |= GPIO_MODER5_AF;
    // GPIOA->MODER &= ~(1U << 10);
    // GPIOA->MODER |= (1U << 11);

    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
    GPIOA->AFR[0] |= GPIO_PA5_AF1;    
    // GPIOA->AFR[0] |= (1U << 20);    // CHNAGE TO USE CMSIS!
    // GPIOA->AFR[0] &= ~(1U << 21);   // CHNAGE TO USE CMSIS!
    // GPIOA->AFR[0] &= ~(1U << 22);   // CHNAGE TO USE CMSIS!
    // GPIOA->AFR[0] &= ~(1U << 23);   // CHNAGE TO USE CMSIS!

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /**** ****/
}

void tim2_servo_pwm_config(void)
{
    // Set mode to upcounting
    TIM2->CR1 &= ~TIM_CR1_DIR;

    // Edge aligned mode 
    TIM2->CR1 &= ~TIM_CR1_CMS;
    // TIM2->CR1 &= ~(1U << 5); // CHNAGE TO USE CMSIS!
    // TIM2->CR1 &= ~(1U << 6); // CHNAGE TO USE CMSIS!

    // 16MHz is default clock
    TIM2->PSC = 16 - 1; // 16 MHz / 16 = 1 MHz

    TIM2->ARR = 20000 - 1; // Set 50 Hz frequency (1 MHz / 20000 = 50 Hz)

    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;
    // TIM2->CCMR1 &= ~(1U << 0);
    // TIM2->CCMR1 &= ~(1U << 1);


    // Set OC1M to PWM mode 1
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_PWM1;

    // TIM2->CCMR1 &= ~(1U << 4);
    // TIM2->CCMR1 |= (1U << 5);
    // TIM2->CCMR1 |= (1U << 6);

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