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

`01-systick-c` declares each register as a flat macro. The `struct/` variant groups registers by peripheral instead.

```diff
-#define GPIOB_MODER (*(volatile uint32_t*)0x40020400)
-#define GPIOB_ODR   (*(volatile uint32_t*)0x40020414)
+typedef struct
+{
+    volatile uint32_t MODER;
+    volatile uint32_t OTYPER;
+    volatile uint32_t OSPEEDR;
+    volatile uint32_t PUPDR;
+    volatile uint32_t IDR;
+    volatile uint32_t ODR;
+} GPIO_TypeDef;
+
+#define GPIOB ((GPIO_TypeDef*)0x40020400UL)
```

The same change is used for RCC:

```diff
-#define RCC_AHBENR (*(volatile uint32_t*)0x4002381C)
+typedef struct
+{
+    volatile uint32_t CR;
+    volatile uint32_t ICSCR;
+    volatile uint32_t CFGR;
+    volatile uint32_t CIR;
+    volatile uint32_t AHBRSTR;
+    volatile uint32_t APB2RSTR;
+    volatile uint32_t APB1RSTR;
+    volatile uint32_t AHBENR;
+} RCC_TypeDef;
+
+#define RCC ((RCC_TypeDef*)0x40023800UL)
```

And for SysTick:

```diff
-#define SYST_CSR (*(volatile uint32_t*)0xE000E010)
-#define SYST_RVR (*(volatile uint32_t*)0xE000E014)
-#define SYST_CVR (*(volatile uint32_t*)0xE000E018)
+typedef struct
+{
+    volatile uint32_t SYST_CSR;
+    volatile uint32_t SYST_RVR;
+    volatile uint32_t SYST_CVR;
+    volatile uint32_t SYST_CALIB;
+} SysTick_TypeDef;
+
+#define SysTick ((SysTick_TypeDef*)0xE000E010UL)
```

Register access also changes:

```diff
-RCC_AHBENR |= (1U << 1);
-GPIOB_MODER |= (1U << (LED_PIN * 2));
-SYST_RVR = (SYSCLK_HZ / TICK_HZ) - 1U;
-SYST_CVR = 0U;
-SYST_CSR = (1U << 0) | (1U << 1) | (1U << 2);
-GPIOB_ODR ^= (1U << LED_PIN);
+RCC->AHBENR |= (1U << 1);
+GPIOB->MODER |= (1U << (LED_PIN * 2));
+SysTick->SYST_RVR = (SYSCLK_HZ / TICK_HZ) - 1U;
+SysTick->SYST_CVR = 0U;
+SysTick->SYST_CSR = (1U << 0) | (1U << 1) | (1U << 2);
+GPIOB->ODR ^= (1U << LED_PIN);
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

```diff
+#define GPIOB_ODR (*(volatile uint32_t*)0x40020414)
+GPIOB_ODR ^= (1U << LED_PIN);
```

This is simple and direct. It is easy to see the exact address used by each register.

## Struct Variant

The `struct/` version declares the register layout once, then maps it to the peripheral base address.

Example:

```diff
+#define GPIOB ((GPIO_TypeDef*)0x40020400UL)
+GPIOB->ODR ^= (1U << LED_PIN);
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
