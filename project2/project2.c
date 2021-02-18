/* main.c Probject 2, Scientific Calculator
    Berat Kizilarmut, 171024086 */
#include "stm32g0xx.h"
#include <math.h>
volatile float number1 = 1786, number2 = 0, newnumber, floatnumber; /* Creating the global variables*/
volatile char input; /* Input return variable */
volatile uint32_t column, row; /* Column and row trackers */
volatile uint32_t decimalpoint = 0; /* Global decimal point tracker */
volatile uint32_t delaycount = 0; /* Global delay counter */

enum state_enum /* Setting the state tracker enum */
{
	idle = 0, first_input = 1, next_input = 2, scientific = 3, submenu = 4 , result = 5, error = 6, result_printing = 7
};
enum state_enum state; /* State object */
enum state_enum previous_state; /* Previous State object */

enum button_type_enum /* Creating button type trackers */
{
    number = 0, operation = 1
};
enum button_type_enum button_type; /* Button_type object */

enum operation_type_enum /* Creating operation type trackers */
{
    addition = 0, subtraction = 1, multiplication = 2, division = 3, logarithmic = 4, ln = 5,
    squareroot = 6, squared = 7, sinus = 8, cosinus = 9, tangent = 10, cotangent = 11, pi = 12
};
enum  operation_type_enum operation_type; /* Operation_type object */

void ssd_clear(void) /* Clear the display */
{
    GPIOA->ODR &= ~(15U << 4); /* Clear Digits */
    GPIOB->ODR |= (255U); /* Clear A B C... */
}

void delay_ms(volatile uint32_t s) /* Set as volatile to prevent optimization */
{
    delaycount = 0; /* Set counter as zero */
    while(1) /* Continuously checks the counter */
    {
        if(delaycount == s)
        return;
    }
}


float float_disassembly(float floatnumber)
{
    if(floatnumber >= 1000)
    {
    	decimalpoint=0;
        floatnumber = floatnumber; /* Do nothing, it's already filled up to 4 digits */
    }
    else if (floatnumber >= 100)
    {
        decimalpoint=3; /* Put the decimal point to the third position from left */
        floatnumber = (10 * floatnumber);
    }
    else if (floatnumber >= 10)
    {
        decimalpoint=2; /* Put the decimal point to the second position from left */
        floatnumber = (100 * floatnumber);
    }
    else
    {
        decimalpoint=1; /* Put the decimal point to the first position from left */
        floatnumber = (1000 * floatnumber);
    }
    return floatnumber;
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

void ssd_print(float value)
{   /* Check if there is overflow */
    if(value > 9999 || value < -999)
    {
    	ssd_clear();
        /* Print ouFL (Overflow) on SSD */
        GPIOA->ODR |= (1U << 4); /* Set D1 High */
        GPIOB->ODR &= (163U); /* (163U = 0b10100011) for o */
        ssd_clear();
        GPIOA->ODR |= (1U << 5); /* Set D2 High */
        GPIOB->ODR &= (227U); /* (227U = 0b11100011) for v */
        ssd_clear();
        GPIOA->ODR |= (1U << 6); /* Set D3 High */
        GPIOB->ODR &= (142U); /* (142U = 0b10001110) for F */
        ssd_clear();
        GPIOA->ODR |= (1U << 7); /* Set D4 High */
        GPIOB->ODR &= (199U); /* (199U = 0b11000111) for L */
        ssd_clear();
        state = error;
        delaycount=0;
    }

    /* Check if the value is negative */
    if(value < 0)
    {
        value= (-10) * value; /* Turn the value positive and multiply it by 10 */
        value=float_disassembly(value); /* Disassemble the input to get decimal point */
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
        GPIOB->ODR &= (191U); /* (191U = 0b10111111) for - sign*/
        if(decimalpoint == 1) /* Put the decimal point if requested */
        GPIOB->ODR &= ~(1U << 7);
        ssd_clear();
        /* Print Hundreds Digit */
        GPIOA->ODR |= (1U << 5); /* Set D2 High */
        ssd_printdigit(d1); /* Print Value to D2 */
        if(decimalpoint == 2) /* Put the decimal point if requested */
        GPIOB->ODR &= ~(1U << 7);
        ssd_clear();
        /* Print Tens Digit */
        GPIOA->ODR |= (1U << 6); /* Set D3 High */
        ssd_printdigit(d2); /* Print Value to D3 */
        if(decimalpoint == 3) /* Put the decimal point if requested */
        GPIOB->ODR &= ~(1U << 7);
        ssd_clear();
        /* Print Ones Digit */
        GPIOA->ODR |= (1U << 7); /* Set D4 High */
        ssd_printdigit(d3); /* Print Value to D4 */
        ssd_clear();
    }

    else
    {
    value=float_disassembly(value); /* Disassemble the input to get decimal point */
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
    if(decimalpoint == 1) /* Put the decimal point if requested */
    GPIOB->ODR &= ~(1U << 7);
    ssd_clear();
    /* Print Hundreds Digit */
    GPIOA->ODR |= (1U << 5); /* Set D2 High */
    ssd_printdigit(d2); /* Print Value to D2 */
    if(decimalpoint == 2) /* Put the decimal point if requested */
    GPIOB->ODR &= ~(1U << 7);
    ssd_clear();
    /* Print Tens Digit */
    GPIOA->ODR |= (1U << 6); /* Set D3 High */
    ssd_printdigit(d3); /* Print Value to D3 */
    if(decimalpoint == 3) /* Put the decimal point if requested */
    GPIOB->ODR &= ~(1U << 7);
    ssd_clear();
    /* Print Ones Digit */
    GPIOA->ODR |= (1U << 7); /* Set D4 High */
    ssd_printdigit(d4); /* Print Value to D4 */
    ssd_clear();
    }
}

void error_function(void)
{
    if(number1 > 9999 || number1 < -999)
    {
        /* Print ouFL (Overflow) on SSD */
        GPIOA->ODR |= (1U << 4); /* Set D1 High */
        GPIOB->ODR &= (163U); /* (163U = 0b10100011) for o */
        ssd_clear();
        GPIOA->ODR |= (1U << 5); /* Set D2 High */
        GPIOB->ODR &= (227U); /* (227U = 0b11100011) for v */
        ssd_clear();
        GPIOA->ODR |= (1U << 6); /* Set D3 High */
        GPIOB->ODR &= (142U); /* (142U = 0b10001110) for F */
        ssd_clear();
        GPIOA->ODR |= (1U << 7); /* Set D4 High */
        GPIOB->ODR &= (199U); /* (199U = 0b11000111) for L */
        ssd_clear();
        state = error;
    }
    else
    {
        /* Print Invd (Invalid) on SSD */
        GPIOA->ODR |= (1U << 4); /* Set D1 High */
        GPIOB->ODR &= (207U); /* (207U = 0b11001111) for I */
        ssd_clear();
        GPIOA->ODR |= (1U << 5); /* Set D2 High */
        GPIOB->ODR &= (171U); /* (171U = 0b10101011) for n */
        ssd_clear();
        GPIOA->ODR |= (1U << 6); /* Set D3 High */
        GPIOB->ODR &= (227U); /* (227U = 0b11100011) for v */
        ssd_clear();
        GPIOA->ODR |= (1U << 7); /* Set D4 High */
        GPIOB->ODR &= (161U); /* (161U = 0b10100001) for d */
        ssd_clear();
        state = error;
    }
}

void clear_rows(void)
{
    /* Setting all the rows as low */
    GPIOA->ODR &= ~(36867U); /* ~(36867U) = ~(1001000000000011) */
}

void activate_rows(void)
{
    /* Setting all the rows as high */
    GPIOA->ODR |= (36867U); /* 36867U = 1001000000000011 */
}

void operation_execute(void)
{
    switch(operation_type)
    {
        case addition: /* Complete the addition operation and print */
            number1 = number2 + number1;
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case subtraction: /* Complete the subtraction operation and print */
            number1 = number2 - number1;
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case multiplication: /* Complete the multiplication operation and print */
            number1 = number2 * number1;
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case division: /* Complete the division operation and print */
            if(number2 == 0) /* Cannot divide by zero */
            {
                number1=0; /* Zero the number */
                delaycount = 0; /* Set delay */
                state = error; /* Set Error State */
                error_function(); /* Go to error */
            }
            number1 = number2 / number1;
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case logarithmic: /* Complete the logarithmic operation and print */
            number1 = log10(number2);
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case ln: /* Complete the ln operation and print */
            number1 = log(number2);
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case squareroot: /* Complete the square root operation and print */
            number1 = sqrt(number2);
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case squared: /* Complete the squared operation and print */
            number1= number2 * number2;
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case sinus: /* Complete the sinus operation and print */
            number1 = sin(number2);
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case cosinus: /* Complete the cosinus operation and print */
            number1 = cos(number2);
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case tangent: /* Complete the tangent operation and print */
            number1 = tan(number2);
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;

        case cotangent: /* Complete the cotangent operation and print */
            number1 = 1 / tan(number2);
            ssd_print(number1);
            state = result_printing;
            delaycount = 0;
            break;
    }
}

void keypad_check(void)
{   /* This function checks for the button row and column */
    /* Also informs about the type of button press, is it a number or an operation */

    /* Setting Row 1 (PA0) as 1, others as zero to check */
    clear_rows();
    GPIOA->ODR |= (1U);
    if((GPIOA->IDR >> 8) & (1U)) /* Checking CR1, PA8 */
    { /* C1 and R1 means button "1" */
        column = 1;
        row = 1;
        input = '1';
        newnumber = 1;
        button_type = number;
    }

    if((GPIOA->IDR >> 9) & (1U)) /* Checking CR2, PA9 */
    { /* C2 and R1 means button "2" */
        column = 2;
        row = 1;
        input = '2';
        newnumber = 2;
        button_type = number;
    }

    if((GPIOA->IDR >> 10) & (1U)) /* Checking CR3, PA10 */
    { /* C3 and R1 means button "3" */
        column = 3;
        row = 1;
        input = '3';
        newnumber = 3;
        button_type = number;
    }

    if((GPIOA->IDR >> 11) & (1U)) /* Checking CR4, PA11 */
    { /* C4 and R1 means button "A" */
        column = 4;
        row = 1;
        input = 'A';
        operation_type = addition;
        button_type = operation;
    }

    /* Setting Row 2 (PA1) as 1, others as zero to check */
    clear_rows();
    GPIOA->ODR |= (1U << 1);
    if((GPIOA->IDR >> 8) & (1U)) /* Checking CR1, PA8 */
    { /* C1 and R2 means button "4" */
        column = 1;
        row = 2;
        input = '4';
        newnumber = 4;
        button_type = number;
    }

    if((GPIOA->IDR >> 9) & (1U)) /* Checking CR2, PA9 */
    { /* C2 and R2 means button "5" */
        column = 2;
        row = 2;
        input = '5';
        newnumber = 5;
        button_type = number;
    }

    if((GPIOA->IDR >> 10) & (1U)) /* Checking CR3, PA10 */
    { /* C3 and R2 means button "6" */
        column = 3;
        row = 2;
        input = '6';
        newnumber = 6;
        button_type = number;
    }

    if((GPIOA->IDR >> 11) & (1U)) /* Checking CR4, PA11 */
    { /* C4 and R2 means button "B" */
        column = 4;
        row = 2;
        input = 'B';
        operation_type = subtraction;
        button_type = operation;
    }

    /* Setting Row 3 (PA12) as 1, others as zero to check */
    clear_rows();
    GPIOA->ODR |= (1U << 12);
    if((GPIOA->IDR >> 8) & (1U)) /* Checking CR1, PA8 */
    { /* C1 and R3 means button "7" */
        column = 1;
        row = 3;
        input = '7';
        newnumber = 7;
        button_type = number;
    }

    if((GPIOA->IDR >> 9) & (1U)) /* Checking CR2, PA9 */
    { /* C2 and R3 means button "8" */
        column = 2;
        row = 3;
        input = '8';
        newnumber = 8;
        button_type = number;
    }

    if((GPIOA->IDR >> 10) & (1U)) /* Checking CR3, PA10 */
    { /* C3 and R3 means button "9" */
        column = 3;
        row = 3;
        input = '9';
        newnumber = 9;
        button_type = number;
    }

    if((GPIOA->IDR >> 11) & (1U)) /* Checking CR4, PA11 */
    { /* C4 and R3 means button "C" */
        column = 4;
        row = 3;
        input = 'C';
        operation_type = multiplication;
        button_type = operation;
    }


    /* Setting Row 4 (PA15) as 1, others as zero to check */
    clear_rows();
    GPIOA->ODR |= (1U << 15);
    if((GPIOA->IDR >> 8) & (1U)) /* Checking CR1, PA8 */
    { /* C1 and R4 means button "*" */
        column = 1;
        row = 4;
        input = '*';
        state = scientific;
        button_type = operation;
    }

    if((GPIOA->IDR >> 9) & (1U)) /* Checking CR2, PA9 */
    { /* C2 and R4 means button "0" */
        column = 2;
        row = 4;
        input = '0';
        newnumber = 0;
        button_type = number;
    }

    if((GPIOA->IDR >> 10) & (1U)) /* Checking CR3, PA10 */
    { /* C3 and R4 means button "#" */
        column = 3;
        row = 4;
        input = '#';
        state = result;
        button_type = operation;
    }

    if((GPIOA->IDR >> 11) & (1U)) /* Checking CR4, PA11 */
    { /* C4 and R4 means button "D" */
        column = 4;
        row = 4;
        input = 'D';
        operation_type = division;
        button_type = operation;
    }
    activate_rows(); /* Setting the rows high before leaving the check function*/
}

void EXTI4_15_IRQHandler (void) {
    delaycount = 0; /* Setting up for timeout */
    keypad_check(); /* Get button information */
    /* Check for button type */
    if (button_type == number) /* Pressed button is a number */
    {
        switch(state)
        {
            case idle: /* If the last state was idle, we have to reset the number1 value */
                state=first_input; /* Since button type is number, this is first number input state */
                previous_state = idle; /* Setting Previous State */
                number1 = newnumber; /* Carry the input to the number */
                ssd_print(number1); /* Print the current number */
                break;

            case first_input:
                if(previous_state == scientific || previous_state == submenu)
                {
                    state = next_input; /* Setting State */
                    number2 = number1; /* Carry the number to secondary number */
                    number1 = newnumber; /* Carry the input to the number */
                    ssd_print(number1); /* Print the current number */
                }
                else
                {
                    previous_state = first_input; /* Setting Previous State */
                    number1 = (10 * number1) + newnumber; /* Shift and carry the input to the number */
                    ssd_print(number1); /* Print the current number */
                }
                break;

            case next_input:
                if(previous_state == next_input)
                {
                    number1 = (10 * number1) + newnumber; /* Shift and carry the input to the number */
                    ssd_print(number1); /* Print the current number */
                }
                else
                {
                   /* number2 = number1; /* Switching values */
                    state = next_input; /* Setting State */
                    number1 = newnumber; /* Carry the input to the number */
                    previous_state = next_input; /* Setting Previous State */
                    ssd_print(number1); /* Print the current number */
                }
                break;
        }
    }

    if (button_type == operation) /* Pressed button is an operation */
    {
        switch(state)
        {
            case idle:
                number1=0; /* Zeroing number */
                state=next_input; /* Setting State */
                previous_state = idle; /* Setting Previous State */
                ssd_print(number1); /* Print the current number */
                break;

            case first_input:
                state=next_input; /* Setting State */
                previous_state = first_input; /* Setting Previous State */
                number2=number1; /* Switching values */
                number1=0; /* Zeroing number to get new inputs */
                ssd_print(number1); /* Print the current number */
                break;

            case next_input:
                number2=number1; /* Switching values */
                number1=0; /* Zeroing number to get new inputs */
                previous_state = first_input; /* Setting Previous State */
                state = first_input; /* Switching state to get new input */
                ssd_print(number1); /* Print the current number */
                break;

            case scientific:
                previous_state = scientific; /* Setting Previous State */
                if(column == 4 && row == 1)
                    operation_type = logarithmic; /* Setting operation type */
                if(column == 4 && row == 2)
                    operation_type = ln; /* Setting operation type */
                if(column == 4 && row == 3)
                    operation_type = squareroot; /* Setting operation type */
                if(column == 4 && row == 4)
                    operation_type = squared; /* Setting operation type */
                if(column == 1 && row == 4)
                    state = submenu;
                else
                {   /* Something went wrong */
                    number1=0; /* Zero the number */
                    delaycount = 0; /* Set delay */
                    state = error; /* Set Error State */
                    error_function(); /* Go to error */
                }
                break;

            case submenu:
                state = next_input; /* Setting State */
                previous_state = submenu; /* Setting Previous State */
                if(column == 4 && row == 1)
                    operation_type = sinus; /* Setting operation type */
                if(column == 4 && row == 2)
                    operation_type = cosinus; /* Setting operation type */
                if(column == 4 && row == 3)
                    operation_type = tangent; /* Setting operation type */
                if(column == 4 && row == 4)
                    operation_type = cotangent; /* Setting operation type */
                if(column == 1 && row == 4)
                    number1 = 3.14 ; /* Setting number as pi */
                else
                {   /* Something went wrong */
                    number1=0; /* Zero the number */
                    delaycount = 0; /* Set delay */
                    state = error; /* Set Error State */
                    error_function(); /* Go to error */
                }
                break;

            case result:
                state = result_printing; /* Setting State */
                previous_state = result; /* Setting Previous State */
                operation_execute(); /* Execute the operations and print result */
                break;
        }
    }
    delay_ms(500); /* Delay so function does not run continiously */
    EXTI->RPR1 |= (15U << 8); /* Clearing pending PA8 to PA11 */
}

void TIM1_BRK_UP_TRG_COM_IRQHandler (void) /* Configuring TIM1 */
{
    delaycount++;
    TIM1->SR &= ~(1U << 0); /* Clear pending status */
}

void system_initialize(void) /* Setting up the board and subsystems */
{
    /* Enable GPIOA and GPIOB clock */
    RCC->IOPENR |= (3U); /* 3U = 11 */
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
    NVIC_SetPriority(EXTI4_15_IRQn, 1); /* Setting priority */
    NVIC_EnableIRQ(EXTI4_15_IRQn); /* Enabling the interrupt function */
    /* Setting up the pins PA0, PA1, PA12 and PA15 as output to use a rows on the keypad */
    GPIOA->MODER &= ~(3U << 0*2); /* Zero PA0 */
    GPIOA->MODER |= (1U << 0*2); /* Set first bit one on PA0 */
    GPIOA->MODER &= ~(3U << 1*2); /* Zero PA1 */
    GPIOA->MODER |= (1U << 1*2); /* Set first bit one on PA1 */
    GPIOA->MODER &= ~(3U << 12*2); /* Zero PA12 */
    GPIOA->MODER |= (1U << 12*2); /* Set first bit one on PA12 */
    GPIOA->MODER &= ~(3U << 15*2); /* Zero PA15 */
    GPIOA->MODER |= (1U << 15*2); /* Set first bit one on PA15 */
    /* Have to set the rows as high to detect an initial input */
    GPIOA->ODR |= (36867U); /* 36867U = 1001000000000011 */

    /* Setting up the Seven Segment Display*/
    /* Set PB0 to PB7 as output to use as A B C D E F G and decimal point pins of the SSD */
    GPIOB->MODER &= ~(65535U); /* Setting first 16 bits as zero */
    GPIOB->MODER |= (21845U); /* Setting odd bits as one */
    /* Set PA4 to PA7 as output to use as D1 D2 D3 D4 pins of the SSD */
    GPIOA->MODER &= ~(65280U); /* Setting bits 8th to 16th as zero */
    GPIOA->MODER |= (21760U); /* Setting odd bits as one from 8th to 16th */

    /* Setting TIM1 */
    RCC->APBENR2 |= (1U << 11); /* Enable TIM1 Clock */
    TIM1->CR1 = 0; /* Clearing the control register */
    TIM1->CR1 |= (1U << 7);  /* Auto Reload Preload Enable */
    TIM1->CNT = 0; /* Zero the counter */
    TIM1->PSC = 15999; /* Setting prescaler as 16000 to achieve a millisecond on my delay function */
    TIM1->ARR = 1;
    TIM1->DIER |= (1U << 0); /* Updating interrupt enabler */
    TIM1->CR1 |= (1U << 0); /* Enable TIM1 */
    NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0); /* Setting highest priority */
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); /* Enabling interrupt */
}

int main(void) {
    system_initialize(); /* Calling the system initializer */
    while(1) {
        if(state == idle) 
        { /* If state is idle set my school number and keep printing it */
            number1 = 1786;
            ssd_print(number1);
        }
        else if (state == error)
        { /* If state is error, keep printing error message for 5 seconds */
            if(delaycount==5000)
            {
                state=idle;
            }
        	error_function();
        }
        else if (state == result_printing)
        { /* If state is result printing, keep printing result for 5 seconds */
            if(delaycount==5000)
            {
                state=idle;
            }
            ssd_print(number1);
        }
        else{
            if(delaycount==10000) /* If nothing happens for 10 seconds, go back to idling */
            {
                state=idle;
            }
            ssd_print(number1);
        }
    } /* Endless loop */
    return 0;
}
