# 01-systick-c — SysTick-based millisecond delay

Blink LED trên PB8 của **AK Embedded Base Kit (STM32L151C8T6)** dùng **SysTick timer** để đo thời gian chính xác — vẫn hoàn toàn bare-metal, không HAL, không CMSIS.

## Result

<!-- Chèn video/gif blink LED ở đây (đề xuất: đặt trong resources/images/) -->

## How it works

Kế thừa toàn bộ nền tảng từ [`00-minimal-c`](../00-minimal-c/) (vector table, Reset_Handler, linker script, memory-mapped I/O). Điểm mới:

1. **Vector table mở rộng** — tăng từ 2 lên 16 phần tử. Slot `[15]` chứa `SysTick_Handler` (vị trí ARM cố định cho SysTick exception).
2. **SysTick timer** (`led_blink.c`, `led_blink.h`) — 3 thanh ghi trong System Control Space (`0xE000_E010`):
   - `SYSTICK_LOAD = SYSCLK_HZ / TICK_HZ - 1 = 2096` → chu kỳ interrupt 1ms (MSI mặc định 2.097 MHz).
   - `SYSTICK_CTRL = 0b111` → bật counter, bật interrupt, dùng SYSCLK.
3. **`SysTick_Handler`** — mỗi 1ms tăng biến `volatile uint32_t g_tick`.
4. **`delay_ms(ms)`** — poll `g_tick`, dùng phép trừ unsigned nên an toàn khi biến tràn 32-bit.
5. **`main()`** — setup GPIO như bài trước, thêm setup SysTick, rồi blink với `delay_ms(100)`.

Build + flash + debug:
```bash
make          # build .elf và .bin
make flash    # ghi qua ST-Link (STM32CubeProgrammer)
make debug    # openocd + arm-none-eabi-gdb
```

## Ý nghĩa

Đây là bài đầu tiên **có interrupt**. So với `00-minimal-c` chỉ dùng busy-loop `nop` (thời gian không xác định, phụ thuộc tần số CPU và optimizer), bài này giới thiệu:

- **Cortex-M exception mechanism** — cách CPU tự động lưu context, jump vào ISR khi timer đếm về 0.
- **Timing chính xác** — delay 100ms nghĩa là *thực sự* 100ms, không phụ thuộc code trong loop.
- **Chia sẻ dữ liệu giữa ISR và main** — `volatile` để compiler không cache biến `g_tick`.
