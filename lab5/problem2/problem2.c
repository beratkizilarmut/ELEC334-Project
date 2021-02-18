/* main.c Problem 2, Pulse Width Modulation
    Berat Kizilarmut */
#include "stm32g0xx.h"

void init_timer3(void) /* Setting TIM3 */
{
    RCC->APBENR1 |= (1U << 1); /* Enable TIM3 Clock */
    TIM3->CCMR1 = 1;
    TIM3->CR1 = 0; /* Clearing the control register */
    TIM3->CR1 |= (1U << 7);  /* Auto Reload Preload Enable */
    TIM3->CNT = 0; /* Zero the counter */
    TIM3->PSC = 7999;
    TIM3->ARR = 1;
    TIM3->DIER |= (1U << 0); /* Updating interrupt enabler */
    TIM3->CR1 |= (1U << 0); /* Enable TIM3 */
    NVIC_SetPriority(TIM3_BRK_UP_TRG_COM_IRQn, 3); /* Setting lowest priority */
    NVIC_EnableIRQ(TIM3_BRK_UP_TRG_COM_IRQn); /* Enabling interrupt */
}

int main(void) {
    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U);
    /* Setup PA6 as Alternate function mode */
    GPIOA->MODER &= ~(3U << 2*6);
    GPIOA->MODER |= (2U << 2*6);
    /* Choose AF1 from the Mux */
    GPIOA->AFR[0] &= ~(0xFU << 4*6);
    GPIOA->AFR[0] |= (1U << 4*6);
    init_timer3;
    while(1) { } /* Endless loop */
    return 0;
}