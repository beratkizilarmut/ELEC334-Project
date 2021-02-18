/* main.c Problem 3, SSD Counter
    Berat Kizilarmut */
#include "stm32g0xx.h"
volatile uint32_t counter = 0; /* Setting the counter */
volatile uint32_t counting_state = 1; /* Setting the state tracker */
/* Zero for idle, one for counting */

void ssd_clear(void) /* Clear the display */
{
    GPIOA->ODR &= ~(15U << 4); /* Clear Digits */
    GPIOB->ODR |= (127U); /* Clear A B C... */
}

void ssd_printdigit(uint32_t s) /* Prints the values depending on the number */
{
    switch(s)
    {
        case 0: /* Printing 0 */
            GPIOB->ODR &= (0xC0U);
            break;

        case 1: /* Printing 1 */
            GPIOB->ODR &= (0xF9U);
            break;

        case 2: /* Printing 2 */
            GPIOB->ODR &= (0xA4U);
            break;

        case 3: /* Printing 3 */
            GPIOB->ODR &= (0xB0U);
            break;

        case 4: /* Printing 4 */
            GPIOB->ODR &= (0x99U);
            break;

        case 5: /* Printing 5 */
            GPIOB->ODR &= (0x92U);
            break;

        case 6: /* Printing 6 */
            GPIOB->ODR &= (0x82U);
            break;

        case 7: /* Printing 7 */
            GPIOB->ODR &= (0xF8U);
            break;

        case 8: /* Printing 8 */
            GPIOB->ODR &= (0x80U);
            break;

        case 9: /* Printing 9 */
            GPIOB->ODR &= (0x90U);
            break;
    }
}

void ssd_print(uint32_t value)
{
    /* Gather the digit by digit values using integer rounding */
    uint32_t d1=0, d2=0, d3=0, d4=0;
    d1=value/1000;
    value=value-(d1*1000);
    d2=value/100;
    value=value-(d2*100);
    d3=value/10;
    value=value-(d3*10);
    d4=value;
    ssd_clear();
    /* Print Thousands Digit */
    GPIOA->ODR |= (1U << 4); /* Set D1 High */
    ssd_printdigit(d1); /* Print Value to D1 */
    ssd_clear();
    /* Print Hundreds Digit */
    GPIOA->ODR |= (1U << 5); /* Set D2 High */
    ssd_printdigit(d2); /* Print Value to D2 */
    ssd_clear();
    /* Print Tens Digit */
    GPIOA->ODR |= (1U << 6); /* Set D3 High */
    ssd_printdigit(d3); /* Print Value to D3 */
    ssd_clear();
    /* Print Ones Digit */
    GPIOA->ODR |= (1U << 7); /* Set D4 High */
    ssd_printdigit(d4); /* Print Value to D4 */
    ssd_clear();
}

void EXTI0_1_IRQHandler (void) { /* Button interrupt */
    if(counting_state == 1)
    {
        EXTI->RPR1 |= (1U << 0); /* Clear pending status */
    }
    else
    {
        GPIOC->ODR |= (1U << 6); /* Turn on PC6 */
        counter=0;
        counting_state=1;
        EXTI->RPR1 |= (1U << 0); /* Clear pending status */
    }
}

void TIM1_BRK_UP_TRG_COM_IRQHandler (void) /* Configuring TIM1 */
{
    if(counter == 9999)
    {
        GPIOC->ODR |= (1U << 6); /* Turn on PC6 */
        counting_state = 0;
    }
    else
    {
        GPIOC->ODR &= ~(1U << 6); /* Turn off PC6 */
    for(int i=0; i<25; i++) /* Refreshing the display again and again to make it visible */
    {
    	ssd_print(counter);
    }
    counter++;
    }
    TIM1->SR &= ~(1U << 0); /* Clear pending status */
}

void init_timer1(void) /* Setting TIM1 */
{
    RCC->APBENR2 |= (1U << 11); /* Enable TIM1 Clock */
    TIM1->CR1 = 0; /* Clearing the control register */
    TIM1->CR1 |= (1U << 7);  /* Auto Reload Preload Enable */
    TIM1->CNT = 0; /* Zero the counter */
    TIM1->PSC = 7999;
    TIM1->ARR = 1;
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

    /* Setting up the Seven Segment Display*/
    /* Enable GPIOB clock */
    RCC->IOPENR |= (1U << 1);
    /* Set PB0 to PB6 as output to use as A B C D E F G pins of the SSD */
    GPIOB->MODER &= ~(65535U); /* Setting first 16 bits as zero */
    GPIOB->MODER |= (21845U); /* Setting odd bits as one */
    /* Set PA4 to PA7 as output to use as D1 D2 D3 D4 pins of the SSD */
    GPIOA->MODER &= ~(65280U); /* Setting bits 8th to 16th as zero */
    GPIOA->MODER |= (21760U); /* Setting odd bits as one from 8th to 16th */

    init_timer1(); /* Calling the timer initializer */

    while(1) { } /* Endless loop */
    return 0;
}
