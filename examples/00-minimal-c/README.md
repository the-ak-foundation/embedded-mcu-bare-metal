# 00-minimal-c — Bare-metal LED blink (no HAL, no CMSIS)

Blink LED trên PB8 của **AK Embedded Base Kit (STM32L151C8T6)** hoàn toàn bare-metal: tự viết vector table, Reset_Handler, linker script — không dùng HAL, CMSIS, hay bất kỳ file nào từ ST.

## Result

<!-- Chèn video/gif blink LED ở đây -->

## How it works

1. **Vector table** (`led_blink.c`) — 2 word đầu ở địa chỉ 0x0800_0000: initial SP (`_estack`) và Reset_Handler. CPU đọc 2 word này khi power-on.
2. **Reset_Handler** — zero `.bss`, copy `.data` từ FLASH sang RAM, gọi `main()`.
3. **Linker script** (`stm32l151xx.ld`) — định nghĩa FLASH (128KB @ 0x0800_0000) và RAM (16KB @ 0x2000_0000), place `.isr_vector` ở đầu FLASH, xuất các symbol `_sbss/_ebss/_sdata/_edata/_sidata` cho startup dùng.
4. **`main()`** — bật clock GPIOB qua `RCC_AHBENR`, set PB8 output qua `GPIOB_MODER`, rồi toggle bằng `GPIOB_BSRR` với busy-loop delay.

Build + flash:
```bash
make          # build .elf và .bin
make flash    # ghi qua ST-Link
make debug    # openocd + gdb
