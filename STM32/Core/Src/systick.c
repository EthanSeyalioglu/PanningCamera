#include "systick.h"

#define ONE_MS_AT_16MHZ        16000U

static volatile uint32_t system_ticks;


void systick_init_ms(uint8_t period_ms)
{
    system_ticks = 0;

    SysTick->LOAD = (ONE_MS_AT_16MHZ * period_ms) - 1;
    // SysTick->LOAD = 16000 - 1;

    SysTick->VAL = 0;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    // SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

uint8_t systick_cycle_complete()
{
    return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) ? 1 : 0);
}


uint32_t system_get_ticks()
{
    return system_ticks;
}


// void SysTick_Handler(void)
// {
//     system_ticks++;
// }