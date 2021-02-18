/* main.c Problem 1, UART Protocol
    Berat Kizilarmut */
#include "stm32g0xx.h"
static volatile uint32_t tick = 0;

void SysTick_Handler(void) /* Overriding Systick Handler */
{
    if(tick > 0)
    {
        --tick;
    }
}

void BSP_UART_init(uint32_t baud){ /*Initializing UART */
    RCC->IOPENR |= (1U << 0); /* Enable A */
    RCC->APBENR1 |= (1U << 17); /*Enable USART2 */
    /* Setup PA2 as Alternate function mode */
    GPIOA->MODER &= ~(3U << 2*2);
    GPIOA->MODER |= (2U << 2*2);
    /* Choose AF1 from the Mux */
    GPIOA->AFR[0] &= ~(0xFU << 4*2);
    GPIOA->AFR[0] |= (1U << 4*2);
    /* Setup PA3 as Alternate function mode */
    GPIOA->MODER &= ~(3U << 2*3);
    GPIOA->MODER |= (2U << 2*3);
    /* Choose AF1 from the Mux */
    GPIOA->AFR[0] &= ~(0xFU << 4*3);
    GPIOA->AFR[0] |= (1U << 4*3);
    /* Setup UART2 */
    /* Reset UART2 CR1 */
    USART2->CR1 = 0;
    USART2->CR1 |= (1U << 3); /* TE */
    USART2->CR1 |= (1U << 2); /* RE */
    USART2->BRR = (uint16_t)(SystemCoreClock / baud);
    USART2->CR1 |= (1U << 0); /* UE */

}

void uart_tx(uint8_t c){ /* Setting Transmission of Data */
    USART2->TDR = (uint16_t)c; /* Transmit Data Register */
    while (!(USART2->ISR & (1U << 6))); 
}

uint8_t uart_rx(void){ /* Setting Receiving of Data */
    uint8_t data = (uint8_t)USART2->RDR;
    return data;
}

int main(void) {
	SysTick_Config(SystemCoreClock / 1000); /* Setting systick timing */
    BSP_UART_init(9600); /* Calling the UART initializer */

    while(1) {
        uart_tx(uart_rx()); /* Continuous data stream */
     } /* Endless loop */
    return 0;
}