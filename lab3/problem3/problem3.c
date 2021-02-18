/* main.c
    Berat Kizilarmut
    Problem 3, two button interrupt */
#include "stm32g0xx.h"
#define LEDDELAY    16000000 /* 10 Second Delay */

void delay(volatile uint32_t s)
{
    /* Set as volatile to prevent optimization */
    for(; s>0; s--);
}

void EXTI0_1_IRQHandler (void) { /* Overriding default interrupt handler func */
    GPIOB->ODR |= (1U); /* Turning the led on */
    delay(LEDDELAY); /* Delaying for 10 seconds */
    GPIOB->ODR &= ~(1U); /* Turning the led off */
    EXTI->RPR1 |= (1U << 0);
}

void EXTI4_15_IRQHandler (void) {
    GPIOB->ODR |= (2U); /* Turning the led on */
    delay(LEDDELAY); /* Delaying for 10 seconds */
    GPIOB->ODR &= ~(2U); /* Turning the led off */
    EXTI->RPR1 |= (1U << 4);
}

int main(void) { /* Overriding default interrupt handler func */
    /* Setting up the buttons and interruptions */
    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U);
    /* Setup PA0 and PA4 as input */
    /* Used PA4 rather than PA2 or PA3 because pins are absent on the test board */
    GPIOA->MODER &= ~(771U);

    /* Setting up the interrupt operation for PA0 */
    EXTI->RTSR1 |= (1U); /* Setting the trigger as rising edge */
    EXTI->EXTICR[0] &= ~(1U << 8*0); /* EXTICR 0 for 0_1 */
    EXTI->IMR1 |= (1U << 0); /* Interrupt mask register */
    EXTI->RPR1 |= (1U << 0); /* Rising edge pending register, Clearing pending PA0 */
    NVIC_SetPriority(EXTI0_1_IRQn, 0); /* Setting priority */
    NVIC_EnableIRQ(EXTI0_1_IRQn); /* Enabling the interrupt function */

    /* Setting up the interrupt operation for PA4 */
    EXTI->RTSR1 |= (1U << 4); /* Setting the trigger as rising edge */
    EXTI->EXTICR[1] &= ~(1U << 0); /* EXTICR[1] and offset 0 for PA4 */
    EXTI->IMR1 |= (1U << 4); /* Interrupt mask register */
    EXTI->RPR1 |= (1U << 4); /* Rising edge pending register, Clearing pending PA4 */
    NVIC_SetPriority(EXTI4_15_IRQn, 1); /* Setting priority */
    NVIC_EnableIRQ(EXTI4_15_IRQn); /* Enabling the interrupt function */

    /* Setting the output leds */
    /* Enable GPIOB clock */
    RCC->IOPENR |= (2U);
    /* Setup PB0 and PB1 as output */
    GPIOB->MODER &= ~(15U);
    GPIOB->MODER |= (5U);

    while(1) { } /* Endless loop */
    return 0;
}