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
    ;Enabling GPIOB and GPIOC clock, Setting bit1 and bit2 on IOPENR 1
    LDR r3, =(0x40021000 + 0x34)
    LDR r2, [r3]
    LDR r1, =0x6
    ORRS r2, r2, r1
    STR r2, [r3]

    ;Setting PB6 for input by setting bits 13 and 12 to 00 using modder
    LDR r3, =(0x50000400 + 0x00)
    LDR r2, [r3]
    LDR r1, =0x3000
    MVNS r1, r1
    ANDS r2, r2, r1
    STR r2, [r3]

    ;Setting PC6 for output by setting bits 13 and 12 to 01 using modder
    LDR r3, =(0x50000800 + 0x00)
    LDR r2, [r3]
    LDR r1, =0x2000
    MVNS r1, r1
    ANDS r2, r2, r1
    LDR r1, =0x1000
    ORRS r2, r2, r1
    STR r2, [r3]

    ;Turning off the onboard led
LEDOFF
    LDR r3, =(0x50000800 + 0x14)
    LDR r2, [r3]
    LDR r1, =0xFFFFFFBF
    ANDS r2, r2, r1
    STR r2, [r3]

    ;Checking if the button is pressed
BUTTON
    LDR r3, =(0x50000400 + 0x10)
    LDR r2, [r3]
    LDR r1, =0x40
    MVNS r0, r1
    ORRS r2, r2, r0
    CMP r0, r3
    BEQ LEDOFF

    ;Turning on the onboard led
LEDON
	LDR r3, =(0x50000800+0x14)
    LDR r2, [r3]
    LDR r1, =0x40
    ORRS r2, r2, r1
    STR r2, [r3] 
    B BUTTON

    ; Edit above this line
    B .
    ENDP

    END