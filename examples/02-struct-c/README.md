# 02-struct-c — Peripheral access via `typedef struct`

Same LED blink as [`01-systick-c`](../01-systick-c/). The only change is how we reach the registers: `typedef struct` + pointer instead of one `#define` per register — the same pattern CMSIS uses.

## Result

<!-- Add LED blink video / gif here -->

## How it works

The whole diff lives in `led_blink.h` and the register access syntax in `led_blink.c`.

**Before (`01-systick-c/led_blink.h`):**
```c
#define GPIOB_MODER (*(volatile uint32_t*)0x40020400)
#define GPIOB_BSRR  (*(volatile uint32_t*)0x40020418)
```

**Now (`02-struct-c/led_blink.h`):**
```c
typedef struct {
    volatile uint32_t MODER;   /* 0x00 */
    volatile uint32_t OTYPER;  /* 0x04 */
    /* ... */
    volatile uint32_t BSRR;    /* 0x18 */
} GPIO_TypeDef;

#define GPIOB ((GPIO_TypeDef*)0x40020400UL)
```

Access:
```c
GPIOB_BSRR = (1U << 8);   // old
GPIOB->BSRR = (1U << 8);  // new
```

The compiler computes each field offset from the struct. As long as the field order and types match the datasheet layout, `GPIOB->BSRR` compiles to `*(uint32_t*)(0x40020400 + 0x18)` — the exact same address as the `#define` version.

Build / flash / debug:
```bash
make && make flash
make debug
```

## Meaning

Moving from raw addresses to a peripheral block. Why bother:

- **Scales.** GPIOA through GPIOI each have around 10 registers. One `GPIO_TypeDef` handles them all instead of hundreds of `#define` lines.
- **Offsets stay in sync.** If ST changes a layout, we fix one struct instead of hunting down individual `#define`s.
- **Same style as CMSIS.** The vendor `stm32l1xx.h` uses exactly this pattern, so `04-cmsis-device-c` will just swap our hand-written struct for the ST one.

The next examples (`03-cmsis-core-c`, `04-cmsis-device-c`, `05-hal-c`) keep climbing the same ladder — each step drops another block of hand-written code.
