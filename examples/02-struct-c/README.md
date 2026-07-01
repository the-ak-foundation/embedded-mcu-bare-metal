# 02-struct-c — Peripheral register access via `typedef struct`

Blink LED trên PB8 của **AK Embedded Base Kit (STM32L151C8T6)**. **Kết quả giống hệt** [`01-systick-c`](../01-systick-c/), chỉ khác **cách tổ chức register** — thay `#define` từng địa chỉ bằng `typedef struct` + pointer, đúng chuẩn CMSIS.

## Result

<!-- Chèn video/gif blink LED ở đây (giống 01-systick-c) -->

## How it works

Diff duy nhất so với `01-systick-c` nằm ở `led_blink.h` và cú pháp truy cập register trong `led_blink.c`:

**Trước (`01-systick-c/led_blink.h`):**
```c
#define GPIOB_MODER (*(volatile uint32_t*)0x40020400)
#define GPIOB_BSRR  (*(volatile uint32_t*)0x40020418)
```

**Sau (`02-struct-c/led_blink.h`):**
```c
typedef struct {
    volatile uint32_t MODER;   /* 0x00 */
    volatile uint32_t OTYPER;  /* 0x04 */
    /* ... */
    volatile uint32_t BSRR;    /* 0x18 */
} GPIO_TypeDef;

#define GPIOB ((GPIO_TypeDef*)0x40020400UL)
```

Truy cập:
```c
GPIOB_BSRR = (1U << 8);   // cũ
GPIOB->BSRR = (1U << 8);  // mới
```

### Cơ chế
Compiler tự tính offset của field trong struct — miễn thứ tự và kiểu field khớp với bố cục thanh ghi trong datasheet, `GPIOB->BSRR` sẽ được dịch thành `*(uint32_t*)(0x40020400 + 0x18)`. Chính xác cùng địa chỉ như bản `#define`.

Build + flash + debug:
```bash
make && make flash
make debug
```

## Ý nghĩa

Đây là bước quan trọng chuyển từ "địa chỉ raw" sang "peripheral block". Lợi ích:

- **Scale được** — mỗi peripheral có 10-20 register, viết `#define` từng thanh ghi cho GPIO A/B/C/D/E/F/G/H/I là hàng trăm dòng. Struct chỉ cần 1 định nghĩa, dùng lại cho mọi port.
- **Tự động khớp offset** — nếu ST đổi vị trí thanh ghi, chỉ cần sửa struct 1 chỗ.
- **Tương thích chuẩn CMSIS** — các file `stm32l1xx.h` chính chủ ST cũng dùng chính xác pattern này. Bài `04-cmsis-device-c` sẽ chỉ việc thay struct tự viết bằng struct của ST.

Các bài sau (`03-cmsis-core-c`, `04-cmsis-device-c`, `05-hal-c`) sẽ tiếp tục leo bậc thang trừu tượng, mỗi lần bỏ bớt một phần mình tự viết.
