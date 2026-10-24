.syntax unified
.cpu cortex-m4
.fpu fpv4-sp-d16
.thumb

.extern SystemInit
.extern main
.extern EXTI15_10_IRQHandler
.extern USART2_IRQHandler
.extern SysTick_Handler
.extern TIM4_IRQHandler
.extern ADC_IRQHandler

.global Reset_Handler

.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object
.size g_pfnVectors, . - g_pfnVectors

g_pfnVectors:
    /* Cortex-M core exceptions */
    .word _estack
    .word Reset_Handler
    .word Default_Handler          /* NMI */
    .word Default_Handler          /* HardFault */
    .word Default_Handler          /* MemManage */
    .word Default_Handler          /* BusFault */
    .word Default_Handler          /* UsageFault */
    .word 0
    .word 0
    .word 0
    .word 0
    .word Default_Handler          /* SVC */
    .word Default_Handler          /* DebugMon */
    .word 0
    .word Default_Handler          /* PendSV */
    .word SysTick_Handler          /* SysTick */

    /* External interrupts */
    .word Default_Handler              /* 0  WWDG */
    .word Default_Handler              /* 1  PVD */
    .word Default_Handler              /* 2  TAMP_STAMP */
    .word Default_Handler              /* 3  RTC_WKUP */
    .word Default_Handler              /* 4  FLASH */
    .word Default_Handler              /* 5  RCC */
    .word Default_Handler              /* 6  EXTI0 */
    .word Default_Handler              /* 7  EXTI1 */
    .word Default_Handler              /* 8  EXTI2 */
    .word Default_Handler              /* 9  EXTI3 */
    .word Default_Handler              /* 10 EXTI4 */
    .word Default_Handler              /* 11 DMA1_Stream0 */
    .word Default_Handler              /* 12 DMA1_Stream1 */
    .word Default_Handler              /* 13 DMA1_Stream2 */
    .word Default_Handler              /* 14 DMA1_Stream3 */
    .word Default_Handler              /* 15 DMA1_Stream4 */
    .word Default_Handler              /* 16 DMA1_Stream5 */
    .word Default_Handler              /* 17 DMA1_Stream6 */
    .word ADC_IRQHandler               /* 18 ADC */
    .word 0                             /* 19 Reserved */
    .word 0                             /* 20 Reserved */
    .word 0                             /* 21 Reserved */
    .word 0                             /* 22 Reserved */
    .word Default_Handler              /* 23 EXTI9_5 */
    .word Default_Handler              /* 24 TIM1_BRK_TIM9 */
    .word Default_Handler              /* 25 TIM1_UP_TIM10 */
    .word Default_Handler              /* 26 TIM1_TRG_COM_TIM11 */
    .word Default_Handler              /* 27 TIM1_CC */
    .word Default_Handler              /* 28 TIM2 */
    .word Default_Handler              /* 29 TIM3 */
    .word TIM4_IRQHandler              /* 30 TIM4 */
    .word Default_Handler              /* 31 I2C1_EV */
    .word Default_Handler              /* 32 I2C1_ER */
    .word Default_Handler              /* 33 I2C2_EV */
    .word Default_Handler              /* 34 I2C2_ER */
    .word Default_Handler              /* 35 SPI1 */
    .word Default_Handler              /* 36 SPI2 */
    .word Default_Handler              /* 37 USART1 */
    .word USART2_IRQHandler            /* 38 USART2 */
    .word Default_Handler              /* 39 USART6 */
    .word EXTI15_10_IRQHandler         /* 40 EXTI15_10 */

    /*
     * Remaining external interrupt vectors.
     * They are not used by this project.
     */
    .rept 29
    .word Default_Handler
    .endr

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