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

    bl SystemInit
    bl main

1:
    b 1b

.size Reset_Handler, . - Reset_Handler

.section .text.Default_Handler
.type Default_Handler, %function

Default_Handler:

1:
    b 1b

.size Default_Handler, . - Default_Handler
