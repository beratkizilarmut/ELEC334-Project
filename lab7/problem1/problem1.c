/* problem1.c Lab 7, Problem 1, Led Dimmer
    Berat Kizilarmut, 171024086 */
#include "stm32g0xx.h"
volatile uint32_t delaycount = 0; /* Global delay counter */

void delay_ms(volatile uint32_t s) /* Set as volatile to prevent optimization */
{
    delaycount = 0; /* Set counter as zero */
    while(1) /* Continuously checks the counter */
    {
        if(delaycount == s)
        return;
    }
}

void TIM2_BRK_UP_TRG_COM_IRQHandler (void) /* Configuring TIM2 */
{
    delaycount++;
    TIM2->SR &= ~(1U << 0); /* Clear pending status */
}

void system_initialize(void) /* Setting up the board and subsystems */
{
    /* Enable GPIOA and GPIOB clock */
    RCC->IOPENR |= (3U); /* 3U = 11 */

    /* Setup PA8 as Alternate function mode */
    GPIOA->MODER &= ~(3U << 2*8);
    GPIOA->MODER |= (2U << 2*8);
    /* Choose AF2 from the Mux */
    GPIOA->AFR[1] &= ~(0xFU << 4*0);
    GPIOA->AFR[1] |= (2U << 4*0);
    /* Setting TIM1 for PWM on PA8 */
    RCC->APBENR2 |= (1U << 11); /* Enable TIM1 Clock */
    TIM1->CR1 = 0; /* Clearing the control register */
    TIM1->CR1 |= (1U << 7);  /* Auto Reload Preload Enable */
    TIM1->CNT = 0; /* Zero the counter */
    TIM1->PSC = 15999; /* Setting prescaler as 16000 to achieve a millisecond on my delay function */
    TIM1->ARR = 1; /* Auto Reload Register */
    TIM1->DIER |= (1U << 0); /* Updating interrupt enabler */
    TIM1->CR1 |= (1U << 0); /* Enable TIM1 */
    TIM1->CCMR1 |= (6U << 4); /* Capture/Compare mode register */
    TIM1->CCMR1 |= (1U << 3);
    TIM1->CCER |= (1U << 0); /* Capture/Compare enable register */
    TIM1->CCR1 |= (1U << 0); /* Capture/Compare register 1 */
    TIM1->BDTR |= (1U << 15); /* Break and Dead-Time register */    

    /* Setup PB9 as Alternate function mode */
    GPIOB->MODER &= ~(3U << 2*9);
    GPIOB->MODER |= (2U << 2*9);
    /* Choose AF2 from the Mux */
    GPIOB->AFR[1] &= ~(0xFU << 4*1);
    GPIOB->AFR[1] |= (2U << 4*1);
    /* Setting TIM17 for PWM on PB9 */
    RCC->APBENR2 |= (1U << 18); /* Enable TIM17 Clock */
    TIM17->CR1 = 0; /* Clearing the control register */
    TIM17->CR1 |= (1U << 7);  /* Auto Reload Preload Enable */
    TIM17->CNT = 0; /* Zero the counter */
    TIM17->PSC = 15999; /* Setting prescaler as 16000 to achieve a millisecond on my delay function */
    TIM17->ARR = 1; /* Auto Reload Register */
    TIM17->CR1 |= (1U << 0); /* Enable TIM17 */
    TIM17->CCMR1 |= (6U << 4); /* Capture/Compare mode register */
    TIM17->CCMR1 |= (1U << 3);
    TIM17->CCER |= (1U << 0); /* Capture/Compare enable register */
    TIM17->CCR1 |= (1U << 0); /* Capture/Compare register 1 */
    TIM17->BDTR |= (1U << 15); /* Break and Dead-Time register */

    /* Setting TIM2 for Delays*/
    RCC->APBENR1 |= (1U << 0); /* Enable TIM2 Clock */
    TIM2->CR1 = 0; /* Clearing the control register */
    TIM2->CR1 |= (1U << 7);  /* Auto Reload Preload Enable */
    TIM2->CNT = 0; /* Zero the counter */
    TIM2->PSC = 15999; /* Setting prescaler as 16000 to achieve a millisecond on my delay function */
    TIM2->ARR = 1; /* Auto Reload Register */
    TIM2->DIER |= (1U << 0); /* Updating interrupt enabler */
    TIM2->CR1 |= (1U << 0); /* Enable TIM2 */
    NVIC_SetPriority(TIM2_BRK_UP_TRG_COM_IRQn, 0); /* Setting highest priority */
    NVIC_EnableIRQ(TIM2_BRK_UP_TRG_COM_IRQn); /* Enabling interrupt */

    /* Setup PA0 as Analog mode */
    GPIOA->MODER |= (3U << 0);
    /* Setting up ADC1/0 for Analog input on PA0 */
    RCC->APBENR2 |= (1U << 20); /* Enable ADC Clock */
    ADC1->CR = 0; /* Clearing the control register */
    ADC1->CFGR1 = 0; /* Clearing the configrator register */
    ADC1->CR |= (1U << 28); /* Enabling voltage regulator */
    delay_ms(1000); /* Waiting for second to voltage to regulate */
    ADC1->CR |= (1U << 31); /* Initialize Calibration Operation */
    while(!((ADC1->ISR >> 11) & 1)); /* If ISR is 1 break */
    ADC1->IER |= (1U << 11); /* Stop Calibration Operation */
    ADC1->CFGR1 |= (2U << 3); /* Setting resolution on configration register */
    ADC1->CFGR1 |= (1U << 13); /* Continuous conversion mode enabled */
    ADC1->CFGR1 &= ~(1U << 16); /* Discontinuous mode disabled */
    ADC1->SMPR |= (0 << 0); /* Sampling Time Register is set as 1.5 ADC Clock Cycles */
    ADC1->CFGR1 &= ~(3U << 10); /* External trigger is disabled */
    ADC1->CFGR1 &= ~(7U << 6); /* External trigger selected as TRG0 */
    ADC1->CHSELR |= (1U << 0); /* Channel is set as PA0 */
    ADC1->ISR |= (1 << 0); /* ADC Ready bit is set as 1 */
    ADC1->CR |= (1 << 0); /* ADC Enable bit is set as 1 */
    while( !(ADC1->ISR & (1 << 0))); /* If ISR is 0 break */ 
    ADC1->CR |= (1U << 2); /* ADC start conversion bit is set as 1 */
    active_run(); /* Run active running function */
}

void active_run(void)
{
    while (1)
    {
        /* Read ADC voltage here */
        /* Set PWM of the leds */
    }
}

int main(void) {
    system_initialize(); /* Calling the system initializer */
    while(1) { } /* Endless loop */
    return 0;
}
