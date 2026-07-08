# HAL pattern

Peripheral driver patterns: raw register access (macro and struct), CMSIS-Device, HAL blocking, and HAL non-blocking.

Same LED blink behavior in every subfolder — only the way registers are declared and touched changes.

## Subfolders

| # | Folder | Topic |
|---|--------|-------|
| 00 | [`00-register-macro/`](00-register-macro/) | Macro-based register access |
| 01 | [`01-register-struct/`](01-register-struct/) | Struct-based register access |
| 02 | [`02-cmsis-device/`](02-cmsis-device/) | CMSIS-Device vendor headers |
| 03 | [`03-hal-blocking/`](03-hal-blocking/) | HAL with blocking `HAL_Delay` |
| 04 | [`04-hal-nonblocking/`](04-hal-nonblocking/) | HAL with non-blocking `HAL_GetTick` |

## Prerequisite

Read [`arm-cortex-m/00-systick`](../arm-cortex-m/00-systick/) first — every folder here uses SysTick for the 1 ms tick.

## Reading order

Read them 00 → 01 → 02 → 03 → 04 — each step wraps the previous one.

