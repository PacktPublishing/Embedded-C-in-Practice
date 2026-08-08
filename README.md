# Embedded C in Practice

Source code and projects accompanying the **Embedded C in Practice** book.

The repository is organized by chapter. Each chapter contains one or more independent projects that can be built and flashed from the command line.

The goal of the book is to understand embedded C from the hardware up, progressively introducing the concepts and abstractions commonly used in embedded firmware development.

---

## Repository structure

    Embedded-C-In-Practice/
    │
    ├── README.md
    ├── requirements.md
    │
    ├── chapter_01/
    ├── chapter_02/
    ├── chapter_03/
    │   ├── blink/
    │   └── button-blink/
    │
    └── ...

Each project is self-contained.

For example:

    chapter_03/
    ├── blink/
    │   ├── Makefile
    │   ├── linker.ld
    │   ├── main.c
    │   └── core/
    │
    └── button-blink/
        ├── Makefile
        ├── linker.ld
        ├── main.c
        ├── drivers/
        └── core/

---

# Requirements

The examples are developed and tested using:

| Component | Version | Purpose |
|---|---|---|
| C | C11 | Firmware language |
| ARM GNU Toolchain | See below | Compilation and linking |
| GNU Make | System version | Build system |
| CMSIS-Core | **5.9.0** | ARM Cortex-M4 core support |
| STM32F4xx CMSIS Device | **2.6.11** | STM32F401 device definitions |
| STM32CubeProgrammer | **2.23.0** | Flash programming |
| STM32 HAL | **Not used** | — |
| STM32CubeMX | **Not required** | — |
| STM32CubeIDE | **Not required for build** | — |

The development environment used for the book is **macOS**.

---

## ARM GNU Toolchain

The firmware is compiled using the ARM GNU Toolchain for bare-metal ARM targets.

Required executables:

    arm-none-eabi-gcc
    arm-none-eabi-objcopy
    arm-none-eabi-size

Verify the installation:

    arm-none-eabi-gcc --version
    arm-none-eabi-objcopy --version
    arm-none-eabi-size --version

The projects target:

    arm-none-eabi
    ARM Cortex-M4

The exact ARM GNU Toolchain release used for the book should be recorded in `requirements.md`.

---

## GNU Make

GNU Make is used as the build system.

Verify the installation:

    make --version

No IDE-specific project files or build systems are required.

---

## CMSIS

The projects use CMSIS for the Cortex-M4 core definitions and STM32F4 device register definitions.

STM32 HAL is intentionally **not used**.

### CMSIS-Core

Version:

    5.9.0

CMSIS-Core provides the ARM Cortex-M4 core headers used by the projects.

### STM32F4xx CMSIS Device

Repository:

    STMicroelectronics/cmsis-device-f4

Version:

    2.6.11

Git tag:

    v2.6.11

Commit:

    0fa0e489e053fa1ca7790bb40b4d76458f64c55d

For the STM32F401RE, the relevant device header is:

    stm32f401xe.h

---

# Target hardware

The initial chapters target the:

**ST NUCLEO-F401RE**

Microcontroller:

    STM32F401RE

Main characteristics:

    ARM Cortex-M4
    512 KB Flash
    96 KB SRAM

On-board peripherals used by Chapter 3:

    LD2 LED   → PA5
    B1 USER   → PC13

No external components are required for the Chapter 3 examples.

Programming and debugging are performed through the onboard:

    ST-LINK
    SWD

---

# Development approach

The examples intentionally start from a minimal bare-metal environment.

The initial projects do not use:

- STM32 HAL
- STM32CubeMX generated code
- STM32CubeIDE as a build system
- RTOS
- vendor middleware

Instead, the projects use:

    C11
    +
    ARM GNU Toolchain
    +
    CMSIS
    +
    custom startup code
    +
    custom linker script
    +
    GNU Make

This makes the fundamental components of an embedded firmware project explicit and allows the reader to understand what happens underneath higher-level abstractions.

---

# Building a project

Enter the directory of the project you want to build.

For example:

    cd chapter_03/blink

Then:

    make

A successful build produces:

    build/
    ├── blink.elf
    ├── blink.bin
    └── blink.map

The `build/` directory contains generated files and is excluded from version control.

---

# Flashing a project

Connect the NUCLEO-F401RE through the ST-LINK USB connector.

Verify that the board is detected:

    STM32_Programmer_CLI -c port=SWD

Then flash the project:

    make flash

The firmware is programmed at:

    0x08000000

STM32CubeProgrammer is used only as the programming interface. It is not part of the firmware build process.

---

# Cleaning a project

Remove generated build artifacts:

    make clean

A clean rebuild can be performed with:

    make clean
    make

---

# Chapter 3

Chapter 3 introduces the first complete bare-metal firmware projects.

## blink

A minimal LED blink application introducing:

- Cortex-M4 startup
- vector table
- reset handler
- linker script
- GPIO clock enable
- GPIO configuration
- direct register access
- GPIO output control

The application runs directly on the STM32F401RE and blinks the onboard LD2 LED connected to PA5.

---

## button-blink

The second project extends the previous example with an onboard user button.

It introduces:

- GPIO input
- active-low input handling
- button driver
- LED driver
- polling
- simple debounce
- edge detection

The application toggles LD2 once for every button press.

The two Chapter 3 projects are intentionally independent. At this stage, common infrastructure is duplicated between projects so that each example can be understood and built in isolation.

A common project infrastructure may be introduced in later chapters as the repository evolves.

---

# Reproducibility

The repository is designed to keep the development environment reproducible.

External dependencies are pinned to explicit versions whenever possible.

In particular:

    CMSIS-Core              5.9.0
    STM32F4xx CMSIS Device  2.6.11
    STM32CubeProgrammer     2.23.0

For source dependencies, the repository version and commit are documented when applicable.

The intention is that the examples remain buildable and understandable even when newer versions of the underlying tools become available.

See [`requirements.md`](requirements.md) for the detailed dependency information.

---

# License

License information will be added as the repository is finalized.