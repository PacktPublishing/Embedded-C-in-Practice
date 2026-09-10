.syntax unified
.cpu cortex-m4
.fpu fpv4-sp-d16
.thumb

.extern SystemInit
.extern main

.global Reset_Handler

.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object

g_pfnVectors:
    .word _estack
    .word Reset_Handler

    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler

    .word 0
    .word 0
    .word 0
    .word 0

    .word Default_Handler
    .word Default_Handler
    .word 0
    .word Default_Handler
    .word Default_Handler

    .rept 85
    .word Default_Handler
    .endr

.size g_pfnVectors, . - g_pfnVectors

.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:

    /* Copy .data from FLASH to RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

1:
    cmp r1, r2
    bge 2f

    ldr r3, [r0]
    str r3, [r1]

    adds r0, #4
    adds r1, #4

    b 1b

2:
    /* Zero-initialize .bss */
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0

3:
    cmp r1, r2
    bge 4f

    str r3, [r1]
    adds r1, #4

    b 3b

4:
    bl SystemInit
    bl main

5:
    b 5b

.size Reset_Handler, . - Reset_Handler

.section .text.Default_Handler
.type Default_Handler, %function

Default_Handler:

1:
    b 1b

.size Default_Handler, . - Default_Handler
