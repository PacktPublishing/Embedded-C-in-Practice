ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    .text :
    {
        . = ALIGN(4);

        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)

        . = ALIGN(4);
    } > FLASH

    .data :
    {
        . = ALIGN(4);

        *(.data)
        *(.data*)

        . = ALIGN(4);
    } > RAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);

        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);

        PROVIDE(end = .);
    } > RAM
}