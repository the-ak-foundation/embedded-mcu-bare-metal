#ifndef __LED_BLINK_H__
#define __LED_BLINK_H__

#include <stdint.h>

// STM32 peripherals
#define RCC_AHBENR (*(volatile uint32_t*)0x4002381C)
#define GPIOB_MODER (*(volatile uint32_t*)0x40020400)
#define GPIOB_BSRR (*(volatile uint32_t*)0x40020418)

// Cortex-M3 system registers
#define SYSTICK_CTRL (*(volatile uint32_t*)0xE000E010)
#define SYSTICK_LOAD (*(volatile uint32_t*)0xE000E014)
#define SYSTICK_VAL (*(volatile uint32_t*)0xE000E018)

// LED pin configuration
#define LED_PIN 8

// System clock and tick configuration
#define SYSCLK_HZ 2097000U
#define TICK_HZ 1000U

#endif // __LED_BLINK_H__