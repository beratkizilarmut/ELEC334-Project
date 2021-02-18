Microsoft Windows [Version 10.0.18363.1198]
(c) 2019 Microsoft Corporation. Tüm hakları saklıdır.

C:\Users\kuros>arm-none-eabi-objdump -D Desktop\2020-hw3.elf

Desktop\2020-hw3.elf:     file format elf32-littlearm


Disassembly of section .text:

08000000 <v>:
 8000000:       10002000        andne   r2, r0, r0 
 8000004:       08000021        stmdaeq r0, {r0, r5} ;Store multiple increments, Decrement Address
 8000008:       0800002b        stmdaeq r0, {r0, r1, r3, r5} ; Same op, including r1 and r3 as well
 800000c:       0800002b        stmdaeq r0, {r0, r1, r3, r5} ; Same op
 8000010:       10000000        andne   r0, r0, r0 ;Does nothing
 8000014:       10000000        andne   r0, r0, r0
 8000018:       10000000        andne   r0, r0, r0
 800001c:       10000000        andne   r0, r0, r0

08000020 <r>:
 8000020:       481b            ldr     r0, [pc, #108]  ; (8000090 <lizard+0x10>) Goes to 8000020 + 108 = 08000080
 8000022:       4685            mov     sp, r0 ;moves that address to stack pointer
 8000024:       f000 f802       bl      800002c <main> ;branch links to main
 8000028:       e7fe            b.n     8000028 <r+0x8> ;indefinite loop

0800002a <d>:
 800002a:       e7fe            b.n     800002a <d> ;indefinite loop

0800002c <main>:
 800002c:       4919            ldr     r1, [pc, #100]  ; (8000094 <lizard+0x14>)  Goes to 800002c + 100 = 8000094
 800002e:       4a1a            ldr     r2, [pc, #104]  ; (8000098 <lizard+0x18>)   Goes to 800002e + 104 = 8000098
 8000030:       2300            movs    r3, #0 ; Move zero to r3

08000032 <rock>:
 8000032:       f000 f807       bl      8000044 <paper> ;Branch link to paper
 8000036:       6010            str     r0, [r2, #0] ;Writes value of the address that r2 points to r0
 8000038:       3104            adds    r1, #4 ;Adds 4 to r1
 800003a:       3204            adds    r2, #4 ;Adds 4 to r2
 800003c:       3301            adds    r3, #1 ;Adds 4 to r3
 800003e:       2b04            cmp     r3, #4 ;Compare r3 to 4
 8000040:       d1f7            bne.n   8000032 <rock> ;Branch to rock if Not Equal
 8000042:       e017            b.n     8000074 <eof>   ;Branch to end of function

08000044 <paper>:
 8000044:       b40e            push    {r1, r2, r3} ;Push r1 r2 and r3 to stack
 8000046:       4e15            ldr     r6, [pc, #84]   ; (800009c <lizard+0x1c>) Goes to 8000044 + 84 = 800009c
 8000048:       00f7            lsls    r7, r6, #3 ;Logic Shift Left r6 by 3 and write to r7, 400004e0
 800004a:       6809            ldr     r1, [r1, #0] ;Writes value of the address that r1 points to r1
 800004c:       4c14            ldr     r4, [pc, #80]   ; (80000a0 <lizard+0x20>) Goes to 800004c + 80 = 80000a0

0800004e <scissors>:
 800004e:       4a15            ldr     r2, [pc, #84]   ; (80000a4 <lizard+0x24>) Gets the program counter + 84 offsett address to r2
 8000050:       6815            ldr     r5, [r2, #0] ;Writes value of the address that r2 points to r5
 8000052:       0108            lsls    r0, r1, #4 ;Logic Shift Left r1 by 4 and write to r0
 8000054:       1940            adds    r0, r0, r5 ;Adds r0 r5 and writes back to r0
 8000056:       b401            push    {r0} ;Pushes r0 to stack
 8000058:       6855            ldr     r5, [r2, #4] ;Writes value of the address that r2 points plus 4 to r4
 800005a:       0948            lsrs    r0, r1, #5 ;Logic Shift Right r1 by 5 and write to r0
 800005c:       1940            adds    r0, r0, r5 ;Add r5 to r0 and write to r0
 800005e:       19ca            adds    r2, r1, r7 ;Add r7 to r1 and write to r2
 8000060:       4050            eors    r0, r2 ;XOR r2 and write to r0
 8000062:       bc04            pop     {r2} ;POP from stack and write to r2
 8000064:       4050            eors    r0, r2 ;XOR r2 and write to r0
 8000066:       1a09            subs    r1, r1, r0 ;Subtract r0 from r1 and write to r1
 8000068:       1bbf            subs    r7, r7, r6  ;Subtract r6 from r7 and write to r7
 800006a:       0864            lsrs    r4, r4, #1 ;Logic Shift Right r4 by 4 and write back to r4
 800006c:       d1ef            bne.n   800004e <scissors> ;Depending on the flag conditions, loop back to scissors
 800006e:       0008            movs    r0, r1 ;Move r1 to r0
 8000070:       bc0e            pop     {r1, r2, r3} ;POP r1 r2 and r3 to stack 
 8000072:       4770            bx      lr ;branch and exchange operation, causes branching to an instruction set by register ARMv6-M

08000074 <eof>: ;   End of function, loops infinitely
 8000074:       e7fe            b.n     8000074 <eof>
 8000076:       46c0            nop                     ; (mov r8, r8)

08000078 <spock>:
 8000078:       138a5b9c        orrne   r5, sl, #156, 22        ; 0x27000
 800007c:       83b19de5                        ; <UNDEFINED> instruction: 0x83b19de5

08000080 <lizard>:
 8000080:       a2390c55        eorsge  r0, r9, #21760  ; 0x5500
 8000084:       113f39fc        teqne   pc, ip  ; <illegal shifter operand>     ; <UNPREDICTABLE>
 8000088:       6140f4fd        strdvs  pc, [r0, #-77]  ; 0xffffffb3
 800008c:       d3926c34        orrsle  r6, r2, #52, 24 ; 0x3400
 8000090:       10002000        andne   r2, r0, r0  
 8000094:       08000080        stmdaeq r0, {r7}
 8000098:       10000200        andne   r0, r0, r0, lsl #4
 800009c:       14159265        ldrne   r9, [r5], #-613 ; 0xfffffd9b
 80000a0:       00000080        andeq   r0, r0, r0, lsl #1
 80000a4:       08000078        stmdaeq r0, {r3, r4, r5, r6}

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:   00002141        andeq   r2, r0, r1, asr #2
   4:   61656100        cmnvs   r5, r0, lsl #2
   8:   01006962        tsteq   r0, r2, ror #18
   c:   00000017        andeq   r0, r0, r7, lsl r0
  10:   726f4305        rsbvc   r4, pc, #335544320      ; 0x14000000
  14:   2d786574        cfldr64cs       mvdx6, [r8, #-464]!     ; 0xfffffe30
  18:   002b304d        eoreq   r3, fp, sp, asr #32
  1c:   4d070c06        stcmi   12, cr0, [r7, #-24]     ; 0xffffffe8
  20:   Address 0x00000020 is out of bounds.
