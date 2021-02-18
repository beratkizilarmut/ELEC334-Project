/* main.c Problem 2, blink timer
    Berat Kizilarmut */
#include "stm32g0xx.h"
volatile uint32_t interval = 1; /* Setting the second interval counter */

void EXTI0_1_IRQHandler (void) { /* Button interrupt */
    if(interval == 10) /* if the interval is 10, return to 1 second intervals */
    interval = 0; /* Setting it zero here, because line below will increase it once */
	interval++; /* Increase the interval time */
    TIM1->ARR = (16000)*(interval); /* Multiply it with 16k to achieve a second */
    EXTI->RPR1 |= (1U << 0); /* Clear pending status */
}

void TIM1_BRK_UP_TRG_COM_IRQHandler (void) /* Configuring TIM1 */
{
    GPIOC->ODR ^= (1U << 6); /* Toggle PC6 */
    TIM1->SR &= ~(1U << 0); /* Clear pending status */
}

void init_timer1(void) /* Setting TIM1 */
{
    RCC->APBENR2 |= (1U << 11); /* Enable TIM1 Clock */
    TIM1->CR1 = 0; /* Clearing the control register */
    TIM1->CR1 |= (1U << 7);  /* Auto Reload Preload Enable */
    TIM1->CNT = 0; /* Zero the counter */
    TIM1->PSC = 999; /* 1000 since we're starting from 0 */
    TIM1->ARR = 16000; /* For 16 MHz */
    TIM1->DIER |= (1U << 0); /* Updating interrupt enabler */
    TIM1->CR1 |= (1U << 0); /* Enable TIM1 */
    NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 3); /* Setting lowest priority */
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); /* Enabling interrupt */
}

int main(void) {
    /* Setting up the external button and interrupt */
    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U);
    /* Setup PA0 as input */
    GPIOA->MODER &= ~(3U);
    /* Setting up the interrupt operation for PA0 */
    EXTI->RTSR1 |= (1U); /* Setting the trigger as rising edge */
    EXTI->EXTICR[0] &= ~(1U << 8*0); /* EXTICR 0 for 0_1 */
    EXTI->IMR1 |= (1U << 0); /* Interrupt mask register */
    EXTI->RPR1 |= (1U << 0); /* Rising edge pending register, Clearing pending PA0 */
    NVIC_SetPriority(EXTI0_1_IRQn, 0); /* Setting highest priority */
    NVIC_EnableIRQ(EXTI0_1_IRQn); /* Enabling the interrupt function */

    /* Setting the onboard LED3, set default as PC6 */
    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);
    /* Set PC6 as output */
    GPIOC->MODER &= ~(3U << 12); /* Setting bits 13 and 12 as zero */
    GPIOC->MODER |= (1U << 12); /* Setting bit 12 as one */

    init_timer1(); /* Calling the timer initializer */

    while(1) { } /* Endless loop */
    return 0;
}