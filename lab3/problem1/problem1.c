#include "stm32g0xx.h"

void HardFault_Handler(void) {
    /* Insert Hardfault Functions Here */
}

int main(void) {
    /* Setting up the button and interruption */
    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U);
    /* Setup PA0 as input */
    GPIOA->MODER &= ~(3U);
    while(1) { 

        if(GPIOA->IDR & 1U) { /* Check for button input */
            HardFault_Handler(); /* If button is one, call hardfault handler */
        }
        /*Internal Functions */
    } /* Endless loop */
    return 0;
}