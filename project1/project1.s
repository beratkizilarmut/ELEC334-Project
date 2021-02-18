/*
 * project1.s
 *
 * author: Berat Kizilarmut
 *
 * description: Countdown clock which utilizes pseudo random to countdown 10 to 1 seconds at random.
 * 
 */


.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb


/* make linker see this */
.global Reset_Handler

/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss


/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE,         (0x40021000)          // RCC base address
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset

.equ GPIOC_BASE,       (0x50000800)          // GPIOC base address
.equ GPIOC_MODER,      (GPIOC_BASE + (0x00)) // GPIOC MODER register offset
.equ GPIOC_ODR,        (GPIOC_BASE + (0x14)) // GPIOC ODR register offset


/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	.word _estack             /*     Stack pointer */
	.word Reset_Handler +1    /*     Reset handler */
	.word Default_Handler +1  /*       NMI handler */
	.word Default_Handler +1  /* HardFault handler */
	/* add rest of them here if needed */


/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr r0, =_estack
	mov sp, r0

	/* initialize data and bss 
	 * not necessary for rom only code 
	 * */
	bl init_data
	/* call main */
	bl main
	/* trap if returned */
	b .


/* initialize data and bss sections */
.section .text
init_data:

	/* copy rom to ram */
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_sidata
	movs r3, #0
	b LoopCopyDataInit

	CopyDataInit:
		ldr r4, [r2, r3]
		str r4, [r0, r3]
		adds r3, r3, #4

	LoopCopyDataInit:
		adds r4, r0, r3
		cmp r4, r1
		bcc CopyDataInit

	/* zero bss */
	ldr r2, =_sbss
	ldr r4, =_ebss
	movs r3, #0
	b LoopFillZerobss

	FillZerobss:
		str  r3, [r2]
		adds r2, r2, #4

	LoopFillZerobss:
		cmp r2, r4
		bcc FillZerobss

	bx lr


/* default handler */
.section .text
Default_Handler:
	b Default_Handler


/* main function */
.section .text
main:
    /* Clocking the peripherals */
    /* Enabling GPIOA and GPIOB clock, Setting bits 0 and 1 on IOPENR as 1 */
    LDR r3, =(0x40021000 + 0x34)
    LDR r2, [r3]
    LDR r1, =0x3
    ORRS r2, r2, r1
    STR r2, [r3]

    /* Setting 8 pins as output on GPIOB modder, 7 of them for SSD A,B,C... legs, 1 for output led */
    LDR r3, =(0x50000400 + 0x00) /* GPIOB Base Address + Modder Offset */
    LDR r2, [r3]
    LDR r1, =0xFFFF5555 /* Binary 11111111111111110101010101010101 */
    ANDS r2, r2, r1 /* Setting odd bits zero */
    LDR r1, =0x5555 /* Binary 101010101010101 */
    ORRS r2, r2, r1 /* Setting even bits one */
    STR r2, [r3]

    /* Setting PA0 for input, And PA4-7 for output on GPIOA Modder (Because PA2-3 is absent on board pinout) */
    LDR r3, =(0x50000000 + 0x00) /* GPIOA Base Address + Modder Offset */
    LDR r2, [r3]
    LDR r1, =0xFFFFFFFC /* Binary 11111111111111111111111111111100 */
    ANDS r2, r2, r1 /* Setting PA0 as input to connect a Push Button */
    LDR r1, =0xFFFF55FF /* Setting PA4-7 as output to use as SSD Digit Inputs */
    ANDS r2, r2, r1 /* Setting odd bits zero */
    LDR r1, =0x5500 /* Binary 101010101010101 */
    ORRS r2, r2, r1 /* Setting even bits one */
    STR r2, [r3]

/* IDLE FUNCTION */
Idle: /* My ID is 171024086 */
    BL Clear
    /* Turning on the indicator led */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x80 /* Turning on the 8th bit, PB7 */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    BL Clear
    /* Printing Decimal 4 */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x99 /* Setting 1 1 0 0 1 1 0, G to A */
    ORRS r6, r6, r7 /* Set ones */
    ANDS r6, r6, r7 /* Set zeros */
    STR r6, [r5] /* Store */
    /* Setting Digit 1 */
    LDR r5, =(0x50000000 + 0x14) /* GPIOA Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x10 /* PA4, Binary 10000 to Hex 10 */
    ORRS r6, r6, r7 /* Set 5th bit 1 */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    BL Clear
    /* Printing Decimal 0 */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0xC0 /* Setting 0 1 1 1 1 1 1, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    /* Setting Digit 2 */
    LDR r5, =(0x50000000 + 0x14) /* GPIOA Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x20 /* PA5, Binary 100000 to Hex 20 */
    ORRS r6, r6, r7 /* Set 6th bit 1 */
    ANDS r6, r6, r7 /* Set everything else zero */ 
    STR r6, [r5] /* Store */
    BL Clear
    /* Printing Decimal 8 */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x80 /* Setting 1 1 1 1 1 1 1, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    /* Setting Digit 3 */
    LDR r5, =(0x50000000 + 0x14) /* GPIOA Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x40 /* PA6, Binary 1000000 to Hex 40 */
    ORRS r6, r6, r7 /* Set 7th bit 1 */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    BL Clear
    /* Printing Decimal 6 */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x2 /* Setting 1 1 1 1 1 0 1, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    /* Setting Digit 4 */
    LDR r5, =(0x50000000 + 0x14) /* GPIOA Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x80 /* PA7, Binary 10000000 to Hex 80 */
    ORRS r6, r6, r7 /* Set 8th bit 1 */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    BL Clear
    /* Checking for Button Press */
    LDR r3, =(0x50000000 + 0x10) /* GPIOA Base Address + IDR Offset */
    LDR r2, [r3]
    LDR r1, =0x1
    ANDS r2, r2, r1 
    CMP r2, r1
    BNE Idle /* Reach imm8 */
    B Random /* Reach imm11 */

Clear: /* Clearing the screen before writing */
    PUSH {LR}
    LDR r7, =0x2555
Delayfunc3: /* Delaying */
    SUBS r7, #0x1
    BNE Delayfunc3

    LDR r5, =(0x50000000 + 0x14) /* GPIOA Base Address + ODR Offset */
    LDR r7, =0xFFFF0000 /* Setting 0 0 0 0 0 0 0, G to A */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

/* PRINT FUNCTIONS */
Print: /* Print function that has many switch cases to determine the correct way to decode the decimal info to SSD */
    LDR r7, =0x12555
Delayfunc4: /* Delaying */
    SUBS r7, #0x1
    BNE Delayfunc4
PrintD1: /* Thousands Decimal Point, Switch Case to compare counter numbers */
    BL Clear
    CMP r1, #0x0 /* Compare the value */
    BNE Skip11 /* If it's not the same, skip the Branch Link */
    BL Print0 /* If it's same, go to Branch Link and Print out the value */
Skip11: /* Skip to Digit 1, Number 1 */

    CMP r1, #0x1
    BNE Skip12
    BL Print1
Skip12:  

    CMP r1, #0x2
    BNE Skip13
    BL Print2
Skip13:

    CMP r1, #0x3
    BNE Skip14
    BL Print3
Skip14:

    CMP r1, #0x4
    BNE Skip15
    BL Print4
Skip15:

    CMP r1, #0x5
    BNE Skip16
    BL Print5
Skip16:

    CMP r1, #0x6
    BNE Skip17
    BL Print6
Skip17:

    CMP r1, #0x7
    BNE Skip18
    BL Print7
Skip18:

    CMP r1, #0x8
    BNE Skip19
    BL Print8
Skip19:
    BL Digit1 /* Set the correct digit */
    CMP r1, #0x9
    BNE PrintD2  /* Move on to Hundrends Decimal Point */
    BL Print9

PrintD2: /* Hundreds Decimal Point, Switch Case to compare counter numbers */
    BL Clear
    CMP r2, #0x0 /* Compare the value */
    BNE Skip21 /* If it's not the same, skip the Branch Link */
    BL Print0 /* If it's same, go to Branch Link and Print out the value */
Skip21: /* Skip to Digit 2, Number 1 */

    CMP r2, #0x1
    BNE Skip22
    BL Print1
Skip22:

    CMP r2, #0x2
    BNE Skip23
    BL Print2
Skip23:

    CMP r2, #0x3
    BNE Skip24
    BL Print3
Skip24:

    CMP r2, #0x4
    BNE Skip25
    BL Print4
Skip25:

    CMP r2, #0x5
    BNE Skip26
    BL Print5
Skip26:

    CMP r2, #0x6
    BNE Skip27
    BL Print6
Skip27:

    CMP r2, #0x7
    BNE Skip28
    BL Print7
Skip28:

    CMP r2, #0x8
    BNE Skip29
    BL Print8
Skip29:
    BL Digit2 /* Set the correct digit */
    CMP r2, #0x9
    BNE PrintD3  /* Move on to Tens Decimal Point */
    BL Print9

PrintD3: /* Tens Decimal Point, Switch Case to compare counter numbers */
    BL Clear
    CMP r3, #0x0 /* Compare the value */
    BNE Skip31 /* If it's not the same, skip the Branch Link */
    BL Print0 /* If it's same, go to Branch Link and Print out the value */
Skip31: /* Skip to Digit 3, Number 1 */

    CMP r3, #0x1
    BNE Skip32
    BL Print1
Skip32:

    CMP r3, #0x2
    BNE Skip33
    BL Print2
Skip33:

    CMP r3, #0x3
    BNE Skip34
    BL Print3
Skip34:

    CMP r3, #0x4
    BNE Skip35
    BL Print4
Skip35:

    CMP r3, #0x5
    BNE Skip36
    BL Print5
Skip36:

    CMP r3, #0x6
    BNE Skip37
    BL Print6
Skip37:

    CMP r3, #0x7
    BNE Skip38
    BL Print7
Skip38:

    CMP r3, #0x8
    BNE Skip39
    BL Print8
Skip39:
    BL Digit3 /* Set the correct digit */
    CMP r3, #0x9
    BNE PrintD4  /* Move on to Ones Decimal Point */
    BL Print9

PrintD4: /* Ones Decimal Point, Switch Case to compare counter numbers */
    BL Clear
    CMP r4, #0x0 /* Compare the value */
    BNE Skip41 /* If it's not the same, skip the Branch Link */
    BL Print0 /* If it's same, go to Branch Link and Print out the value */
Skip41: /* Skip to Digit 4, Number 1 */

    CMP r4, #0x1
    BNE Skip42
    BL Print1
Skip42:

    CMP r4, #0x2
    BNE Skip43
    BL Print2
Skip43:

    CMP r4, #0x3
    BNE Skip44
    BL Print3
Skip44:

    CMP r4, #0x4
    BNE Skip45
    BL Print4
Skip45:

    CMP r4, #0x5
    BNE Skip46
    BL Print5
Skip46:

    CMP r4, #0x6
    BNE Skip47
    BL Print6
Skip47:

    CMP r4, #0x7
    BNE Skip48
    BL Print7
Skip48:

    CMP r4, #0x8
    BNE Skip49
    BL Print8
Skip49:
    BL Digit4 /* Set the correct digit */
    CMP r4, #0x9
    BNE Skip50
    BL Print9
Skip50:
    /* Checking Button Input */
    LDR r3, =(0x50000000 + 0x10) /* GPIOA Base Address + IDR Offset */
    LDR r2, [r3]
    LDR r1, =0x1
    ANDS r2, r2, r1
    CMP r2, r1
    BEQ Pause
    B Countdown

/* RANDOM FUNCTIONS */
Random: /* Utilizing pseudo random number algorithm from HW2, Problem1 */
    /* Turning off the indicator led */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x80 /* Turning off the 8th bit, PB7 */
    MVNS r7, r7
    ORRS r6, r6, r7
    ANDS r6, r6, r7
    STR r6, [r5] /* Store */

    LDR r0, =0x1B67 /* Set seed */
    LDR r1, =0x41A7 /* a = 16807 */
    LDR r2, =0x7FFFFFFF /* b = 2147483647 */
    MULS r0, r1, r0 /* next = (seed * a) */
    BL Modulo /* random = next % b */

    LDR r2, =0x2327 /*  random = random %8999 */
    BL Modulo

    LDR r3, =0x3E8
    ADDS r0, r0, r3 /* Add 1000 to make the random value fall within 1000 and 9999 */
    PUSH {r0} /* Push to use on Countdown */
    B Decimal_Conversion /* Branch to Decimal Conversion Function */

Modulo: /* Modulo Op to use in Random Op */
    PUSH {LR} /* Pushing lr to return to branching point on the future */
Modulo_Loop:
    CMP r0, r2
    BLT Leave /* If r0 is lower than r2, modulo op is completed, leave function */
    SUBS r0, r0, r2
    BGE Modulo_Loop /* Loop if greater or equal */
Leave: /* Leave the function and return back */
    POP {PC}

Countdown:
    BL Clear
    LDR r7, =0x12555
Delayfunc1: /* Delaying */
    SUBS r7, #0x1
    BNE Delayfunc1
    /* Checking Button Input */
    LDR r5, =(0x50000000 + 0x10) /* GPIOA Base Address + IDR Offset */
    LDR r6, [r5]
    LDR r7, =0x1
    ANDS r6, r6, r7
    CMP r6, r7
    BEQ Pause
    CMP r0, #0x0
    BEQ Idle_Jump /* Cannot branch to "Idle", need another operation */
    SUBS r0, #0x1
    B Decimal_Conversion

Idle_Jump: /* Added a jump branch to solve the problem*/
    B Idle

Pause:
    LDR r7, =0x12555
Delayfunc2: /* Delaying */
    SUBS r7, #0x1
    BNE Delayfunc2

    LDR r5, =(0x50000000 + 0x10) /* GPIOA Base Address + IDR Offset */
    LDR r6, [r5]
    LDR r7, =0x1
    ANDS r6, r6, r7
    CMP r6, r7
    BEQ Countdown
    B Pause

/* DECIMAL CONVERSION FUNCTIONS */
Decimal_Conversion: /* Function to convert the random hex value to decimal */
    LDR r1, =0x0 /* Thousands Counter */
    MOVS r2, r1 /* Hundreds Counter */
    MOVS r3, r1 /* Tens Counter */
    MOVS r4, r1 /* Ones Counter */

Thousand:
    LDR r5, =0x3E8 /* Loading 1000 decimal to compare to */
    CMP r0, r5 /* Comparing the random value to 1000 */
    BLT Hundred /* If the value is lower than thousand, skip to checking hundreds */
    SUBS r0, r0, r5 /* Subtract a thousand */
    ADDS r1, #0x1 /* Increasing the Thousands Counter */
    B Thousand /* Loop */

Hundred:
    LDR r5, =0x64 /* Loading 100 decimal to compare to */
    CMP r0, r5 /* Comparing the random value to 100 */
    BLT Ten /* If the value is lower than hundred, skip to checking tens */
    SUBS r0, r0, r5 /* Subtract a hundred */
    ADDS r2, #0x1 /* Increasing the Hundreds Counter */
    B Hundred /* Loop */

Ten:
    LDR r5, =0xA /* Loading 10 decimal to compare to */
    CMP r0, r5 /* Comparing the random value to 10 */
    BLT One /* If the value is lower than ten, skip to checking ones */
    SUBS r0, r0, r5 /* Subtract ten */
    ADDS r3, #0x1 /* Increasing the Tens Counter */
    B Ten /* Loop */

One:
    LDR r5, =0x1 /* Loading 1 to compare to */
    CMP r0, r5 /* Comparing the random value to 1 */
    BLT Print_Jump /* If the value is lower than one, jump to printing. Can't because of imm8 need another branch to assist in jump */
    SUBS r0, r0, r5 /* Subtract one */
    ADDS r4, #0x1 /* Increasing the Ones Counter */
    B One /* Loop */

Print_Jump: /* Branch to extend the reach */
    B Print

/* Setting Seven Segment Display Digits */
Digit1:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000000 + 0x14) /* GPIOA Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x10 /* PA4, Binary 10000 to Hex 10 */
    ORRS r6, r6, r7 /* Set 5th bit 1 */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Digit2:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000000 + 0x14) /* GPIOA Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x20 /* PA5, Binary 100000 to Hex 20 */
    ORRS r6, r6, r7 /* Set 6th bit 1 */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Digit3:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000000 + 0x14) /* GPIOA Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x40 /* PA6, Binary 1000000 to Hex 40 */
    ORRS r6, r6, r7 /* Set 7th bit 1 */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Digit4:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000000 + 0x14) /* GPIOA Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x80 /* PA7, Binary 10000000 to Hex 80 */
    ORRS r6, r6, r7 /* Set 8th bit 1 */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

/* Seven Segment Display Number Encoding */
/* Using Pins PB0 to PB6 as A to G on Seven Segment Display */
Print0:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0xC0 /* Setting 1000000, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Print1:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0xF9 /* Setting 1111001, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Print2:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0xA4 /* Setting 0100100, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Print3:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0xB0 /* Setting 0110000, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Print4:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x99 /* Setting 0011001, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Print5:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x92 /* Setting 0010010, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Print6:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x82 /* Setting 0000010, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Print7:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0xF8 /* Setting 1111000, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Print8:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x80 /* Setting 0000000, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

Print9:
    PUSH {LR} /* Pushing lr to return to branching point on the future */
    LDR r5, =(0x50000400 + 0x14) /* GPIOB Base Address + ODR Offset */
    LDR r6, [r5]
    LDR r7, =0x90 /* Setting 0010000, G to A */
    ORRS r6, r6, r7 /* Set the value */
    ANDS r6, r6, r7 /* Set everything else zero */
    STR r6, [r5] /* Store */
    POP {PC} /* Return back */

	/* for(/* /* )/*  */
	b .

	/* this should never get executed */
	nop

