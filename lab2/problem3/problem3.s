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
    ;Enabling GPIOB clock, Setting bit1 on IOPENR 1
    LDR r3, =(0x40021000 + 0x34)
    LDR r2, [r3]
    LDR r1, =0x2
    ORRS r2, r2, r1
    STR r2, [r3]

    ;Setting odd bits zero, even bits one up until 17th on modder
    LDR r3, =(0x50000400 + 0x00)
    LDR r2, [r3]
    LDR r1, =0xAAAA
    MVNS r1, r1
    ANDS r2, r2, r1
    LDR r1, =0x5555
    ORRS r2, r2, r1
    STR r2, [r3]

START
    ;Turning on the leds
	LDR r3, =0x50000400+(0x14)
    LDR r2, [r3]
    LDR r1, =0xFF
    ORRS r2, r2, r1
    STR r2, [r3] 
    ;Delaying for one second
    LDR r0,=0x1458555
Delayfunc
    SUBS r0, #0x1
    BNE Delayfunc
    ;Turning off the leds
    LDR r3, =(0x50000400 + 0x14)
    LDR r2, [r3]
    LDR r1, =0xFFFFFF00
    ANDS r2, r2, r1
    STR r2, [r3]
    ;Delaying for one second
    LDR r0,=0x1458555
Delayfunc2
    SUBS r0, #0x1
    BNE Delayfunc2
    B START

    ; Edit above this line
    B .
    ENDP

    END