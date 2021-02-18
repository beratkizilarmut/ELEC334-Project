#include "stm32g0xx.h"

void HardFault_Handler(void) {
    /* Insert Hardfault Functions Here */
}

void EXTI0_1_IRQHandler (void) {
	HardFault_Handler();
    EXTI->RPR1 |= (1U << 0);
}

int main(void) {
    /* Setting up the button and interruption */
    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U);
    /* Setup PA0 as input */
    GPIOA->MODER &= ~(3U);

    /* Setting up the interrupt operation for PA0 */
    EXTI->RTSR1 |= (1U); /* Setting the trigger as rising edge */
    EXTI->EXTICR[0] &= ~(1U << 8*0); /* EXTICR 0 for 0_1 */
    EXTI->IMR1 |= (1U << 0); /* Interrupt mask register */
    EXTI->RPR1 |= (1U << 0); /* Rising edge pending register, Clearing pending PA0 */
    NVIC_SetPriority(EXTI0_1_IRQn, 0); /* Setting priority */
    NVIC_EnableIRQ(EXTI0_1_IRQn); /* Enabling the interrupt function */

    while(1) { } /* Endless loop */
    return 0;
}
