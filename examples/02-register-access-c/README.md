# 02-register-access-c - Macro vs struct register access

Blink LED PB8 with SysTick timing.

This example keeps the same behavior as `01-systick-c`. The only change is how the hardware registers are declared and accessed.

Two variants are provided:

- `macro/` uses one `#define` per register.
- `struct/` uses one `typedef struct` per peripheral, plus a base pointer.

No HAL. No CMSIS. No ST startup files.

Demo clip for the whole series lives in the [root README](../../README.md#demo).

## Layout

```text
02-register-access-c/
├── macro/
│   ├── led_blink.c
│   ├── led_blink.h
│   ├── Makefile
│   ├── stm32l151xx.ld
│   └── stm32l_init.gdb
├── struct/
│   ├── led_blink.c
│   ├── led_blink.h
│   ├── Makefile
│   ├── stm32l151xx.ld
│   └── stm32l_init.gdb
└── README.md
```

Each folder is self-contained.

## Diff From 01-Systick-C

`01-systick-c` uses flat register macros:

```c
#define RCC_AHBENR  (*(volatile uint32_t*)0x4002381C)
#define GPIOB_MODER (*(volatile uint32_t*)0x40020400)
#define GPIOB_ODR   (*(volatile uint32_t*)0x40020414)

#define SYST_CSR (*(volatile uint32_t*)0xE000E010)
#define SYST_RVR (*(volatile uint32_t*)0xE000E014)
#define SYST_CVR (*(volatile uint32_t*)0xE000E018)
```

The `macro/` variant keeps that style.

The `struct/` variant groups registers by peripheral:

```c
typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
} GPIO_TypeDef;

#define GPIOB ((GPIO_TypeDef*)0x40020400UL)
```

Then register access changes from a flat macro:

```c
GPIOB_ODR ^= (1U << LED_PIN);
```

to a field access through the peripheral pointer:

```c
GPIOB->ODR ^= (1U << LED_PIN);
```

Both forms access the same address:

```text
GPIOB base address = 0x40020400
ODR offset         = 0x14
GPIOB->ODR         = 0x40020414
GPIOB_ODR          = 0x40020414
```

## Macro Variant

The `macro/` version declares each register as a direct memory address.

Example:

```c
#define GPIOB_ODR (*(volatile uint32_t*)0x40020414)
```

Usage:

```c
GPIOB_ODR ^= (1U << LED_PIN);
```

This is simple and direct. It is easy to see the exact address used by each register.

## Struct Variant

The `struct/` version declares the register layout once, then maps it to the peripheral base address.

Example:

```c
#define GPIOB ((GPIO_TypeDef*)0x40020400UL)
```

Usage:

```c
GPIOB->ODR ^= (1U << LED_PIN);
```

The compiler calculates the final address from the base address plus the field offset.

## Build / Flash / Debug

Build the macro version:

```bash
cd macro
make
make flash
make debug
```

Build the struct version:

```bash
cd struct
make
make flash
make debug
```

## Meaning

This example separates register access style from program behavior.

The LED blink logic is the same in both folders. The startup code, vector table, linker script, SysTick setup, and main loop stay the same.

Only the register declarations change.

The `struct/` style is the pattern used by CMSIS device headers. The next example replaces these hand-written structs with the vendor CMSIS headers.
