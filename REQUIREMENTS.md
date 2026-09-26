# Requirements

This document describes the software, toolchain, dependencies, and hardware used to build and run the examples from **Embedded C in Practice**.

The repository is designed around a minimal bare-metal development environment and does not require an IDE-specific project or build system.

---

## Recommended Setup

A minimal development environment consists of:

```text
Git
ARM GNU Toolchain
GNU Make
STM32CubeProgrammer
```

An IDE is not required to build the examples.

---

## Programming Language

The firmware examples use:

**C11**

The compiler is invoked using:

```text
-std=c11
```

---

## ARM GNU Toolchain

The projects use the **Arm GNU Toolchain** for bare-metal ARM development.

Required tools:

```text
arm-none-eabi-gcc
arm-none-eabi-objcopy
arm-none-eabi-size
```

Verify the installation with:

```bash
arm-none-eabi-gcc --version
arm-none-eabi-objcopy --version
arm-none-eabi-size --version
```

The target toolchain is:

```text
arm-none-eabi
```

The exact compiler release used for each development environment should be recorded when required for reproducibility.

---

## GNU Make

GNU Make is used as the build system.

Verify the installation:

```bash
make --version
```

No additional build system is required.

---

## CMSIS

The repository uses CMSIS for ARM Cortex-M core definitions and STM32 device definitions. CMSIS is maintained in a shared location in the repository, and individual projects reference it through symbolic links to avoid duplicating the same files.

### CMSIS-Core

Version:

```text
5.9.0
```

CMSIS-Core provides the ARM Cortex-M4 core headers and supporting definitions.

---

### STM32F4xx CMSIS Device

Repository:

[STM32F4xx CMSIS Device](https://github.com/STMicroelectronics/cmsis-device-f4)

Version:

```text
2.6.11
```

The device headers provide the register definitions and device-specific information required by the STM32F4 examples.

For the STM32F401RE target, the relevant device header is:

```text
stm32f401xe.h
```

---

## STM32CubeProgrammer

STM32CubeProgrammer is used to program the firmware onto the target microcontroller through ST-LINK/SWD.

Required command-line tool:

```text
STM32_Programmer_CLI
```

Verify the installation:

```bash
STM32_Programmer_CLI --version
```

The examples use the CLI interface rather than relying on the graphical application.

Typical connection command:

```bash
STM32_Programmer_CLI -c port=SWD
```

---

## Hardware

The examples in this book are developed and tested using the:

[ST NUCLEO-F401RE](https://www.st.com/en/evaluation-tools/nucleo-f401re.html)

The board is based on the **STM32F401RE** microcontroller.

---

## Project Dependencies

Each project contains the files required to build the example.

A typical project contains:

```text
project/
├── Makefile
├── linker.ld
├── main.c
├── drivers/
└── core/
    ├── CMSIS -> shared CMSIS copy
    ├── startup_*.s
    └── system_*.c
```

The projects intentionally keep their infrastructure self-contained where this improves readability and makes the examples easier to follow.
CMSIS is shared across projects and referenced through symbolic links to avoid maintaining multiple copies of the same dependency.

---

## Versioning and Reproducibility

External dependencies are pinned to explicit versions whenever possible.

The current pinned CMSIS dependencies are:

| Dependency             | Version |
| ---------------------- | ------- |
| CMSIS-Core             | 5.9.0   |
| STM32F4xx CMSIS Device | 2.6.11  |

Toolchain versions may vary between development environments. When a specific compiler version is required to reproduce an example exactly, it should be documented alongside the relevant project.
