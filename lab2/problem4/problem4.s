Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

    THUMB

    AREA    RESET, DATA, READONLY
    EXPORT  __Vectors

__Vectors
    DCD     __initial_sp                   ; Top of Stack
    DCD     Reset_Handler                  ; Reset Handler
    DCD     NMI_Handler                    ; NMI Handler
    DCD     HardFault_Handler              ; Hard Fault Handler

    AREA    |.text|, CODE, READONLY


; nmi handler
NMI_Handler    PROC
    EXPORT  NMI_Handler
    B .
    ENDP


; hardfault handler
HardFault_Handler    PROC
    EXPORT  HardFault_Handler
    B .
    ENDP


; entry function
Reset_Handler    PROC
    EXPORT  Reset_Handler
    ; Edit below this line

    ;Clocking the peripherals
    ;Enabling GPIOA and GPIOB clock, Setting bit0 and 1 on IOPENR as 1
    LDR r3, =(0x40021000 + 0x34)
    LDR r2, [r3]
    LDR r1, =0x3
    ORRS r2, r2, r1
    STR r2, [r3]

    ;Setting odd bits zero, even bits one up until 17th on modder to set the 8 leds on GPIOB modder
    LDR r3, =(0x50000400 + 0x00)
    LDR r2, [r3]
    LDR r1, =0xAAAA
    MVNS r1, r1
    ANDS r2, r2, r1
    LDR r1, =0x5555
    ORRS r2, r2, r1
    STR r2, [r3]

    ;Setting PA0 for input by setting bits 0 and 1 to 00 using modder
    LDR r3, =(0x50000000 + 0x00)
    LDR r2, [r3]
    LDR r1, =0x3
    MVNS r1, r1
    ANDS r2, r2, r1
    STR r2, [r3]

    LDR r6, =0x0 ;Zeroing the Switch value

T0
	LDR r4, =(0x50000000 + 0x10)
    LDR r5, [r4]
    CMP r5, #0x1 ;Checking the button
    BNE Skip0 ;Skip if button is not pressed
    MVNS r6, r6 ;Toggle if button is pressed
Skip0

    LDR r3, =(0x50000400 + 0x14)
    LDR r2, [r3]
    LDR r1, =0x0    ;Turning off the leds
    ANDS r2, r2, r1
    STR r2, [r3]

    LDR r3, =0x50000400+(0x14)
    LDR r2, [r3]
    LDR r1, =0xC1 ;Turning on LED7, LED8 and LED1
    ORRS r2, r2, r1
    STR r2, [r3] 

    LDR r0,=0x208D55
Delayfunc0 ;Delaying for 100 ms
    SUBS r0, #0x1
    BNE Delayfunc0

    CMP r6, #0x0 ;Checking the switch
    BNE T7 ;If the left shift mode is on
    B T1 ;If the right shift mode is on

T1
	LDR r4, =(0x50000000 + 0x10)
    LDR r5, [r4]
    CMP r5, #0x1 ;Checking the button
    BNE Skip1 ;Skip if button is not pressed
    MVNS r6, r6 ;Toggle if button is pressed
Skip1

    LDR r3, =(0x50000400 + 0x14)
    LDR r2, [r3]
    LDR r1, =0x0 ;Turning off the leds
    ANDS r2, r2, r1
    STR r2, [r3]

	LDR r3, =0x50000400+(0x14)
    LDR r2, [r3]
    LDR r1, =0x83 ;Turning on LED8 , LED1 and LED2
    ORRS r2, r2, r1
    STR r2, [r3] 

    LDR r0,=0x208D55
Delayfunc1 ;Delaying for 100 ms
    SUBS r0, #0x1
    BNE Delayfunc1

    CMP r6, #0x0 ;Checking the switch
    BNE T0 ;If the left shift mode is on
    B T2 ;If the right shift mode is on

T2
	LDR r4, =(0x50000000 + 0x10)
    LDR r5, [r4]
    ;LDR r5, =0x1 ;Test fake button press, Delete the comment to test !!!!!!!!!!!!!!!!!!!!!!
    CMP r5, #0x1 ;Checking the button
    BNE Skip2 ;Skip if button is not pressed
    MVNS r6, r6 ;Toggle if button is pressed
Skip2

	LDR r3, =(0x50000400 + 0x14)
    LDR r2, [r3]
    LDR r1, =0x0 ;Turning off the leds
    ANDS r2, r2, r1
    STR r2, [r3]

	LDR r3, =0x50000400+(0x14)
    LDR r2, [r3]
    LDR r1, =0x7 ;Turning on LED1 , LED2 and LED3
    ORRS r2, r2, r1
    STR r2, [r3]

    LDR r0,=0x208D55
Delayfunc2 ;Delaying for 100 ms
    SUBS r0, #0x1
    BNE Delayfunc2

    CMP r6, #0x0 ;Checking the switch
    BNE T1 ;If the left shift mode is on
    B T3 ;If the right shift mode is on
    
T3
	LDR r4, =(0x50000000 + 0x10)
    LDR r5, [r4]
    CMP r5, #0x1 ;Checking the button
    BNE Skip3 ;Skip if button is not pressed
    MVNS r6, r6 ;Toggle if button is pressed
Skip3

	LDR r3, =(0x50000400 + 0x14)
    LDR r2, [r3]
    LDR r1, =0x0 ;Turning off the leds
    ANDS r2, r2, r1
    STR r2, [r3]

	LDR r3, =0x50000400+(0x14)
    LDR r2, [r3]
    LDR r1, =0xE ;Turning on LED2 , LED3 and LED4
    ORRS r2, r2, r1
    STR r2, [r3]

    LDR r0,=0x208D55
Delayfunc3 ;Delaying for 100 ms
    SUBS r0, #0x1
    BNE Delayfunc3

    CMP r6, #0x0 ;Checking the switch
    BNE T2 ;If the left shift mode is on
    B T4 ;If the right shift mode is on

T4
	LDR r4, =(0x50000000 + 0x10)
    LDR r5, [r4]
    CMP r5, #0x1 ;Checking the button
    BNE Skip4 ;Skip if button is not pressed
    MVNS r6, r6 ;Toggle if button is pressed
Skip4

	LDR r3, =(0x50000400 + 0x14)
    LDR r2, [r3]
    LDR r1, =0x0 ;Turning off the leds
    ANDS r2, r2, r1
    STR r2, [r3]

	LDR r3, =0x50000400+(0x14)
    LDR r2, [r3]
    LDR r1, =0x1C ;Turning on LED3 , LED4 and LED5
    ORRS r2, r2, r1
    STR r2, [r3]

    LDR r0,=0x208D55
Delayfunc4 ;Delaying for 100 ms
    SUBS r0, #0x1
    BNE Delayfunc4

    CMP r6, #0x0 ;Checking the switch
    BNE T3 ;If the left shift mode is on
    B T5 ;If the right shift mode is on

T5
	LDR r4, =(0x50000000 + 0x10)
    LDR r5, [r4]
    CMP r5, #0x1 ;Checking the button
    BNE Skip5 ;Skip if button is not pressed
    MVNS r6, r6 ;Toggle if button is pressed
Skip5

	LDR r3, =(0x50000400 + 0x14)
    LDR r2, [r3]
    LDR r1, =0x0 ;Turning off the leds
    ANDS r2, r2, r1
    STR r2, [r3]

	LDR r3, =0x50000400+(0x14)
    LDR r2, [r3]
    LDR r1, =0x38 ;Turning on LED4 , LED5 and LED6
    ORRS r2, r2, r1
    STR r2, [r3]

    LDR r0,=0x208D55
Delayfunc5 ;Delaying for 100 ms
    SUBS r0, #0x1
    BNE Delayfunc5

    CMP r6, #0x0 ;Checking the switch
    BNE T4 ;If the left shift mode is on
    B T6 ;If the right shift mode is on

T6
	LDR r4, =(0x50000000 + 0x10)
    LDR r5, [r4]
    CMP r5, #0x1 ;Checking the button
    BNE Skip6 ;Skip if button is not pressed
    MVNS r6, r6 ;Toggle if button is pressed
Skip6

	LDR r3, =(0x50000400 + 0x14)
    LDR r2, [r3]
    LDR r1, =0x0 ;Turning off the leds
    ANDS r2, r2, r1
    STR r2, [r3]

	LDR r3, =0x50000400+(0x14)
    LDR r2, [r3]
    LDR r1, =0x70 ;Turning on LED5 , LED6 and LED7
    ORRS r2, r2, r1
    STR r2, [r3]

    LDR r0,=0x208D55
Delayfunc6 ;Delaying for 100 ms
    SUBS r0, #0x1
    BNE Delayfunc6


    CMP r6, #0x0 ;Checking the switch
    BNE T5 ;If the left shift mode is on
    B T7 ;If the right shift mode is on

T7
	LDR r4, =(0x50000000 + 0x10)
    LDR r5, [r4]
    CMP r5, #0x1 ;Checking the button
    BNE Skip7 ;Skip if button is not pressed
    MVNS r6, r6 ;Toggle if button is pressed
Skip7

	LDR r3, =(0x50000400 + 0x14)
    LDR r2, [r3]
    LDR r1, =0x0 ;Turning off the leds
    ANDS r2, r2, r1
    STR r2, [r3]

	LDR r3, =0x50000400+(0x14)
    LDR r2, [r3]
    LDR r1, =0xE0 ;Turning on LED6 , LED7 and LED8
    ORRS r2, r2, r1
    STR r2, [r3] 

    LDR r0,=0x208D55
Delayfunc7 ;Delaying for 100 ms
    SUBS r0, #0x1
    BNE Delayfunc7

    CMP r6, #0x0 ;Checking the switch
    BNE T6 ;If the left shift mode is on
    B T0 ;If the right shift mode is on   

    ; Edit above this line
    B .
    ENDP

    END