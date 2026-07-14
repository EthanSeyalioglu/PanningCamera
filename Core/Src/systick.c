#include "systick.h"

#define ONE_SEC_AT_16HZ        16000U

static volatile uint32_t system_ticks;


void systick_init_ms(uint32_t period_ms)
{
    system_ticks = 0;

    SysTick->LOAD = (ONE_SEC_AT_16HZ * period_ms) - 1;

    SysTick->VAL = 0;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}


uint32_t system_get_ticks()
{
    return system_ticks;
}


void SysTick_Handler(void)
{
    system_ticks++;
}