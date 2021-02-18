/*  Project 3
    main.c
    Digital Voice Recorder
    Berat Kizilarmut, 171024086 */
#include "stm32g0xx.h"
volatile float time, ava,sample, print_value = 0; /* Creating the global variables*/
volatile uint32_t buffer[1600], buffer_counter = 0; /* Microphone input, EEPROM write buffers */
volatile uint32_t delaycount = 0; /* Global delay counter */

enum state_enum /* Setting the state enum */
{
	idle = 0, track = 1, play = 2, pause = 3, record = 4, delete = 5, status = 6, full = 7, start = 8
};
enum state_enum state; /* State object */

void delay_ms(volatile uint32_t ms) /* Set as volatile to prevent optimization */
{
    delaycount = 0; /* Set counter as zero */
    while(1) /* Continuously checks the counter */
    {
        if(delaycount == ms)
        return;
    }
}

void ssd_clear(void) /* Clear the display */
{
    GPIOA->ODR &= ~(15U << 4); /* Clear Digits */
    GPIOB->ODR &= ~(127U); /* Set A B C ... as zero */
}

void ssd_printdigit(uint32_t digit) /* Prints the values depending on the number */
{
    switch(digit)
    {
        case 0: /* Printing 0 */
            GPIOB->ODR |= (0x40U);
            break;

        case 1: /* Printing 1 */
            GPIOB->ODR |= (0x79U);
            break;

        case 2: /* Printing 2 */
            GPIOB->ODR |= (0x24U);
            break;

        case 3: /* Printing 3 */
            GPIOB->ODR |= (0x30U);
            break;

        case 4: /* Printing 4 */
            GPIOB->ODR |= (0x19U);
            break;

        case 5: /* Printing 5 */
            GPIOB->ODR |= (0x12U);
            break;

        case 6: /* Printing 6 */
            GPIOB->ODR |= (0x2U);
            break;

        case 7: /* Printing 7 */
            GPIOB->ODR |= (0x78U);
            break;

        case 8: /* Printing 8 */
            GPIOB->ODR |= (0x0U);
            break;

        case 9: /* Printing 9 */
            GPIOB->ODR |= (0x10U);
            break;
    }
}

void ssd_print(uint32_t op, float value)
{
	uint32_t d1=0, d2=0, d3=0, d4=0;
    switch(op) /* Printing operation depending on the operational input */
    {
        case 0: /* Printing idLE */
            ssd_clear();
            /* Print idLE (Idle) on SSD */
            GPIOB->ODR |= (123U); /* (123U = 0b1111011) for i */
            GPIOA->ODR |= (1U << 4); /* Set D1 High */
            ssd_clear();
            GPIOB->ODR |= (33U); /* (33U = 0b0100001) for d */
            GPIOA->ODR |= (1U << 5); /* Set D2 High */
            ssd_clear();
            GPIOB->ODR |= (71U); /* (71U = 0b1000111) for L */
            GPIOA->ODR |= (1U << 6); /* Set D3 High */
            ssd_clear();
            GPIOB->ODR |= (6U); /* (6U = 0b0000110) for E */
            GPIOA->ODR |= (1U << 7); /* Set D4 High */
            ssd_clear();
            break;

        case 1: /* Printing Track Number */
            ssd_clear();
            /* Print TrA (Track) on SSD */
            GPIOB->ODR |= (78U); /* (78U = 0b1001110) for T */
            GPIOA->ODR |= (1U << 4); /* Set D1 High */
            ssd_clear();
            GPIOB->ODR |= (47U); /* (47U = 0b0101111) for r */
            GPIOA->ODR |= (1U << 5); /* Set D2 High */
            ssd_clear();
            GPIOB->ODR |= (8U); /* (8U = 0b0001000) for A */
            GPIOA->ODR |= (1U << 6); /* Set D3 High */
            ssd_clear();
            ssd_printdigit(value); /* Print Value to D4 */
            GPIOA->ODR |= (1U << 7); /* Set D4 High */
            ssd_clear();
            break;

        case 2: /* Printing Played back track */
            ssd_clear();
            /* Print PLb (Playback) on SSD */
            GPIOB->ODR |= (12U); /* (12U = 0b0001100) for P */
            GPIOA->ODR |= (1U << 4); /* Set D1 High */
            ssd_clear();
            GPIOB->ODR |= (71U); /* (71U = 0b1000111) for L */
            GPIOA->ODR |= (1U << 5); /* Set D2 High */
            ssd_clear();
            GPIOB->ODR |= (3U); /* (3U = 0b0000011) for b */
            GPIOA->ODR |= (1U << 6); /* Set D3 High */
            ssd_clear();
            ssd_printdigit(value); /* Print Value to D4 */
            GPIOA->ODR |= (1U << 7); /* Set D4 High */
            ssd_clear();
            break;

        case 3: /* Printing Pause */
            ssd_clear();
            /* Print PAUS (Pause) on SSD */
            GPIOB->ODR |= (12U); /* (12U = 0b0001100) for P */
            GPIOA->ODR |= (1U << 4); /* Set D1 High */
            ssd_clear();
            GPIOB->ODR |= (8U); /* (8U = 0b0001000) for A */
            GPIOA->ODR |= (1U << 5); /* Set D2 High */
            ssd_clear();
            GPIOB->ODR |= (65U); /* (65U = 0b1000001) for U */
            GPIOA->ODR |= (1U << 6); /* Set D3 High */
            ssd_clear();
            GPIOB->ODR |= (18U); /* (18U = 0b0010010) for S */
            GPIOA->ODR |= (1U << 7); /* Set D4 High */
            ssd_clear();
            break;

        case 4: /* Printing Recording */
            ssd_clear();
            /* Print rcd (Record) on SSD */
            GPIOB->ODR |= (47U); /* (47U = 0b0101111) for r */
            GPIOA->ODR |= (1U << 4); /* Set D1 High */
            ssd_clear();
            GPIOB->ODR |= (39U); /* (39U = 0b0100111) for c */
            GPIOA->ODR |= (1U << 5); /* Set D2 High */
            ssd_clear();
            GPIOB->ODR |= (33U); /* (33U = 0b0100001) for d */
            GPIOA->ODR |= (1U << 6); /* Set D3 High */
            ssd_clear();
            ssd_printdigit(value); /* Print Value to D4 */
            GPIOA->ODR |= (1U << 7); /* Set D4 High */
            ssd_clear();
            break;

        case 5: /* Dont print anything for delete */
            break;

        case 6: /* Printing Status availability */
            ssd_clear();
            /* Print AvA (Available) on SSD */
            GPIOB->ODR |= (8U); /* (8U = 0b0001000) for A */
            GPIOA->ODR |= (1U << 4); /* Set D1 High */
            ssd_clear();
            GPIOB->ODR |= (99U); /* (99U = 0b1100011) for v */
            GPIOA->ODR |= (1U << 5); /* Set D2 High */
            ssd_clear();
            GPIOB->ODR |= (8U); /* (8U = 0b0001000) for A */
            GPIOA->ODR |= (1U << 6); /* Set D3 High */
            ssd_clear();
            ssd_printdigit(value); /* Print Value to D4 */
            GPIOA->ODR |= (1U << 7); /* Set D4 High */
            ssd_clear();
            break;

        case 7: /* Printing Full */
            ssd_clear();
            /* Print FULL (Full) on SSD */
            GPIOB->ODR |= (14U); /* (14U = 0b0001110) for F */
            GPIOA->ODR |= (1U << 4); /* Set D1 High */
            ssd_clear();
            GPIOB->ODR |= (65U); /* (65U = 0b1000001) for U */
            GPIOA->ODR |= (1U << 5); /* Set D2 High */
            ssd_clear();
            GPIOB->ODR |= (71U); /* (71U = 0b1000111) for L */
            GPIOA->ODR |= (1U << 6); /* Set D3 High */
            ssd_clear();
            GPIOB->ODR |= (71U); /* (71U = 0b1000111) for L */
            GPIOA->ODR |= (1U << 7); /* Set D4 High */
            ssd_clear();
            break;

        case 8: /* Printing 4 digit ID number */
            ssd_clear();
            /* Print idLE (Idle) on SSD */
            GPIOB->ODR |= (0x79U); /* 1 */
            GPIOA->ODR |= (1U << 4); /* Set D1 High */
            ssd_clear();
            GPIOB->ODR |= (0x78U); /* 7 */
            GPIOA->ODR |= (1U << 5); /* Set D2 High */
            ssd_clear();
            GPIOB->ODR |= (0x0U); /* 8 */
            GPIOA->ODR |= (1U << 6); /* Set D3 High */
            ssd_clear();
            GPIOB->ODR |= (0x2U); /* 6 */
            GPIOA->ODR |= (1U << 7); /* Set D4 High */
            ssd_clear();
            break;
    }

}

void ADC_COMP_IRQHandler(void)
{   /* Analog to Digital Converter Comparator Handler */
	uint8_t microphone_input = (uint8_t) (ADC1->DR);
	buffer[buffer_counter] = microphone_input;
	ADC1->ISR |= (1U << 3); /* Clear pending status */
}

void op_play(void)
{   /* Selected track playback function */
    
}

void op_record(void)
{   /* Selected track recording function */

}

void op_status(void)
{   /* Track availibity check function */

}

void op_delete(void)
{   /* Track deletion function */

}

void state_machine(void)
{   /* Continiously running state machine */
    if(state != play)
    TIM17->CCR1 = 0; /* Set output duty cycle as zero to silence */

    switch(state) /* Operations depending on the states */
    {
        case 0: /* Idle State*/
            ssd_print(idle,0);
            break;

        case 1: /* Track Selected State */
            ssd_print(track,sample);
            break;

        case 2: /* Track Playback State*/
            op_play();
            ssd_print(play,sample);
            break;

        case 3: /* Program Pause State */
            ssd_print(pause,0);
            break;

        case 4: /* Track Recording State */
            op_record();
            ssd_print(record,time);
            break;

        case 5: /* Track Deletion State */
            op_delete();
            ssd_print(delete,0);
            break;

        case 6: /* Program Status Check State */
            op_status();
            ssd_print(status,ava);
            break;

        case 7: /* Cannot record to track, track is full state */
            ssd_print(full,0);
            break;

        case 8: /* Starting State */
            ssd_print(start,0);
            break;
    }
}

void clear_rows(void)
{
    /* Setting all the rows as low */
    GPIOA->ODR &= ~(36864U); /* ~(36864U) = ~(1001000000000000) */
}

void activate_rows(void)
{
    /* Setting all the rows as high */
    GPIOA->ODR |= (36864U); /* 36864U = 1001000000000000 */
}

void keypad_check(void)
{   /* This function checks for the button row and column */
    if (state == start) /* We should not be in start state if we arrive here */
    state = idle;
    /* Setting Row 1 (PA12) as 1, others as zero to check */
    clear_rows();
    GPIOA->ODR |= (1U << 12);
    if((GPIOA->IDR >> 8) & (1U)) /* Checking CR1, PA8 */
    { /* C1 and R1 means button "1" */
        if(state == idle)
        {
            sample = 1;
            state = track;
        }
    }

    if((GPIOA->IDR >> 9) & (1U)) /* Checking CR2, PA9 */
    { /* C2 and R1 means button "2" */
        if(state == idle)
        {
            sample = 2;
            state = track;
        }
    }

    if((GPIOA->IDR >> 10) & (1U)) /* Checking CR3, PA10 */
    { /* C3 and R1 means button "3" */
        if(state == idle)
        {
            sample = 3;
            state = track;
        }
    }

    if((GPIOA->IDR >> 11) & (1U)) /* Checking CR4, PA11 */
    { /* C4 and R1 means button "A" */
        if(state == track)
        state = record;
    }

    delay_ms(50); /* Waiting 50 ms to stabilze*/
    /* Setting Row 2 (PA15) as 1, others as zero to check */
    clear_rows();
    GPIOA->ODR |= (1U << 15);
    if((GPIOA->IDR >> 8) & (1U)) /* Checking CR1, PA8 */
    { /* C1 and R2 means button "4" */
        if(state == idle)
        {
            sample = 4;
            state = track;
        }
    }

    if((GPIOA->IDR >> 9) & (1U)) /* Checking CR2, PA9 */
    { /* C2 and R2 means button "5" */
        if(state == idle)
        state = status;
    }

    if((GPIOA->IDR >> 10) & (1U)) /* Checking CR3, PA10 */
    { /* C3 and R2 means button "6" */
        if(state == track)
        state = delete;
    }

    if((GPIOA->IDR >> 11) & (1U)) /* Checking CR4, PA11 */
    { /* C4 and R2 means button "B" */
        if(state == track)
        state = play;

        if(state == play)
        state = pause;
    }
    activate_rows(); /* Setting the rows high before leaving the check function*/
}



void EXTI4_15_IRQHandler (void)
{
    delaycount = 0; /* Setting up for timeout */
    keypad_check(); /* Get button information */
    delay_ms(500); /* Delay so function does not run continiously */
    EXTI->RPR1 |= (15U << 8); /* Clearing pending PA8 to PA11 */
}

void TIM1_BRK_UP_TRG_COM_IRQHandler (void) /* Configuring TIM1 */
{
    delaycount++;
    if(delaycount > 10000)
    state = idle;
    TIM1->SR &= ~(1U << 0); /* Clear pending status */
}

void system_initialize(void) /* Setting up the board and subsystems */
{   /* Enable GPIOA and GPIOB clock */
    RCC->IOPENR |= (3U); /* 3U = 11 */

    /* Setting TIM1 */
    RCC->APBENR2 |= (1U << 11); /* Enable TIM1 Clock */
    TIM1->CR1 = 0; /* Clearing the control register */
    TIM1->CR1 |= (1U << 7);  /* Auto Reload Preload Enable */
    TIM1->CNT = 0; /* Zero the counter */
    TIM1->PSC = 15999; /* Setting prescaler as 16000 to achieve a millisecond on my delay function */
    TIM1->ARR = 1;
    TIM1->DIER |= (1U << 0); /* Updating interrupt enabler */
    TIM1->CR1 |= (1U << 0); /* Enable TIM1 */
    NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1); /* Setting priority */
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); /* Enabling interrupt */

    /* Setting up the 4x4 Keypad */
    /* Setting up the external button interrupts */
    /* Setup PA8 to PA11 as input to use as columns on the keypad */
    GPIOA->MODER &= ~(255U << 8*2); /* ~(255U) = ~(11111111) */
    GPIOA->PUPDR &= ~(85U << 8*2); /* ~(85U) = ~(01010101) */
    GPIOA->PUPDR |= (170U << 8*2); /* 170U = 10101010 */
    /* Setting up the interrupt operation for columns */
    EXTI->RTSR1 |= (15U << 8); /* Setting the trigger as rising edge */
    EXTI->EXTICR[2] &= ~(1U << 8*0); /* Setting for PA8 */
    EXTI->EXTICR[2] &= ~(1U << 8*1); /* Setting for PA9 */
    EXTI->EXTICR[2] &= ~(1U << 8*2); /* Setting for PA10 */
    EXTI->EXTICR[2] &= ~(1U << 8*3); /* Setting for PA11 */
    EXTI->IMR1 |= (15U << 8); /* Interrupt mask register */
    EXTI->RPR1 |= (15U << 8); /* Rising edge pending register, Clearing pending PA8 to PA11 */
    NVIC_SetPriority(EXTI4_15_IRQn, 3); /* Setting priority */
    NVIC_EnableIRQ(EXTI4_15_IRQn); /* Enabling the interrupt function */
    /* Setting up the pins PA12 and PA15 as output to use a rows on the keypad */
    GPIOA->MODER &= ~(3U << 12*2); /* Zero PA12 */
    GPIOA->MODER |= (1U << 12*2); /* Set first bit one on PA12 */
    GPIOA->MODER &= ~(3U << 15*2); /* Zero PA15 */
    GPIOA->MODER |= (1U << 15*2); /* Set first bit one on PA15 */
    /* Have to set the rows as high to detect an initial input */
    GPIOA->ODR |= (36864U); /* 36864U = 1001000000000000 */

    /* Setting up the Seven Segment Display*/
    /* Set PB0 to PB6 as output to use as A B C D E F G pins of the SSD */
    GPIOB->MODER &= ~(16383U); /* Setting first 16 bits as zero */
    GPIOB->MODER |= (5461U); /* Setting odd bits as one */
    /* Set PA4 to PA7 as output to use as D1 D2 D3 D4 pins of the SSD */
    GPIOA->MODER &= ~(65280U); /* Setting bits 8th to 16th as zero */
    GPIOA->MODER |= (21760U); /* Setting odd bits as one from 8th to 16th */

    /* Setting up the ADC */
    /* Setup PA0 as Analog mode */
    GPIOA->MODER |= (3U << 0);
    /* Setting up ADC1/0 for Analog input on PA0 */
    RCC->APBENR2 |= (1U << 20); /* Enable ADC Clock */
    ADC1->CR = 0; /* Clearing the control register */
    ADC1->CFGR1 = 0; /* Clearing the configurator register */
    ADC1->CR |= (1U << 28); /* Enabling voltage regulator */
    delay_ms(1000); /* Waiting for second to voltage to regulate */
    ADC1->CR |= (1U << 31); /* Initialize Calibration Operation */
    while(((ADC1->CR >> 31) & 1U)); /* Check Calibration Bit */
    ADC1->CFGR1 |= (2U << 3); /* Setting resolution on configration register as 8 bit */
    ADC1->CFGR1 &= ~(1U << 13); /* Single conversion mode enabled */
    ADC1->SMPR |= (0 << 0); /* Sampling Time Register is set as 1.5 ADC Clock Cycles */
    ADC1->CFGR1 |= (3U << 6); /* External trigger selected as TRG3 */
    ADC1->CFGR1 |= (1U << 10); /* External trigger set as rising edge */
    ADC1->CHSELR |= (1U << 0); /* Channel is set as PA0 */
    ADC1->IER |= (1U << 3); /* End of conversion sequence interrupt enable is set as 1*/
    ADC1->CR |= (1U << 0); /* ADC Enable bit is set as 1 */
    while( !(ADC1->ISR & (1 << 0))); /* If ISR is 0 break */
    NVIC_SetPriority(ADC1_IRQn, 2); /* Setting priority */
    NVIC_EnableIRQ(ADC1_IRQn); /* Enabling the ADC interrupt function */
    ADC1->CR |= (1U << 2); /* ADC start conversion bit is set as 1 */
    /* Setting TIM3 to use on microphone input operations */
    RCC->APBENR1 |= (1U << 1); /* Enable TIM3 Clock */
    TIM3->CR1 = 0; /* Clearing the control register */
    TIM3->CR1 |= (1U << 7);  /* Auto Reload Preload Enable */
    TIM3->CNT = 0; /* Zero the counter */
    TIM3->PSC = 1599; /* Setting prescaler as 16000 to achieve a millisecond on my delay function */
    TIM3->ARR = 1; /* Auto Reload Register */
    TIM3->CR2 |= (2U << 4); /* Master Mode Selection is set as Update */
    TIM3->CR1 |= (1U << 0); /* Enable TIM3 */

    /* Setting up the PWM */
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
    TIM17->PSC = 5; /* Prescaler */
    TIM17->ARR = 255; /* Auto Reload Register */
    TIM17->CR1 |= (1U << 0); /* Enable TIM17 */
    TIM17->CCMR1 |= (6U << 4); /* Capture/Compare mode register */
    TIM17->CCMR1 |= (1U << 3);
    TIM17->CCER |= (1U << 0); /* Capture/Compare enable register */
    TIM17->CCR1 |= (1U << 0); /* Capture/Compare register 1 */
    TIM17->BDTR |= (1U << 15); /* Break and Dead-Time register */

    /* Setting up I2C */
    /* PB7 will be used as SDA */
    GPIOB->MODER &= ~(3U << 2*7); /* Setup PB7 as Alternate function mode */
    GPIOB->MODER |= (2U << 2*7);
    GPIOB->OTYPER |= (1U << 7); /* Output type set as open-drain */
    GPIOB->AFR[0] &= (0xFU << 4*7);
    GPIOB->AFR[0] &= (6U << 4*7); /* Choosing AF6 */
    /* PB8 will be used as SCL */
    GPIOB->MODER &= ~(3U << 2*8); /* Setup PB8 as Alternate function mode */
    GPIOB->MODER |= (2U << 2*8);
    GPIOB->OTYPER |= (1U << 8); /* Output type set as open-drain */
    GPIOB->AFR[1] &= (0xFU << 4*0);
    GPIOB->AFR[1] &= (6U << 4*0); /* Choosing AF6 */
    RCC->APBRSTR1 |= (1U << 21); /* Enabling I2C Clock */
    I2C1->CR1 = 0; /* Clearing I2C Control Register */
    I2C1->CR1 |= (1U << 7); /* Error Interrupt Enabled */
    I2C1->TIMINGR |= (3U << 28); /* Timing Prescaler */
    I2C1->TIMINGR |= (0x13U << 0); /* SCL Low Period */
    I2C1->TIMINGR |= (0xFU << 8); /* SCL High Period */
    I2C1->TIMINGR |= (2U << 16); /* Data Hold Time */
    I2C1->TIMINGR |= (4U << 20); /* Data Setup Time */
    I2C1->CR1 |= (1U << 0); /* Peripheral Enable */
    NVIC_SetPriority(I2C1_IRQn, 0); /* Setting priority */
    NVIC_EnableIRQ(I2C1_IRQn); /* Enabling the I2C interrupt function */


    state = start; /* Return to idle state */
    /* Active Running */
    while(1)
    {
    	state_machine();
    }
}

int main(void) {
    system_initialize(); /* Calling the system initializer */
    while(1) { } /* Endless loop */
    return 0;
}