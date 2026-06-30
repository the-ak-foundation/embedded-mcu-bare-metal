#ifndef __BM_LED_BLINK_H__
#define __BM_LED_BLINK_H__

#include <stdint.h>

typedef struct
{
	volatile uint32_t RCC_AHBENR;
	volatile uint32_t GPIOB_MODER;
	volatile uint32_t GPIOB_BSRR;
} bm_led_blink_gpio_t;

#define RCC_AHBENR (*(volatile uint32_t*)0x4002381C)
#define GPIOB_MODER (*(volatile uint32_t*)0x40020400)
#define GPIOB_BSRR (*(volatile uint32_t*)0x40020418)

#define GPIOB ((bm_led_blink_t*)0x40020400)

#define LED_PIN 8

#endif // __BM_LED_BLINK_H__