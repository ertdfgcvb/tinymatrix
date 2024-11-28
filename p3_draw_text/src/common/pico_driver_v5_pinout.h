#ifndef MATRIX_HARDWARE_H
#define MATRIX_HARDWARE_H

// formula used is 80000000L/(cfg->clkspeed_hz + 1), must result in >=2.  Acceptable values 26.67MHz, 20MHz, 16MHz, 13.34MHz...
#define ESP32_I2S_CLOCK_SPEED (10000000UL)
// #define ESP32_I2S_CLOCK_SPEED (10000000UL)

#define ESP32_FORUM_PINOUT 0
#define ESP32_FORUM_PINOUT_WITH_LATCH 1
#define SMARTLED_SHIELD_V0_PINOUT 2
#define ESP32_JC_RIBBON_PINOUT 3
#define HUB75_ADAPTER_PINOUT 4
#define HUB75_ADAPTER_LATCH_BREADBOARD_PINOUT 5
#define HUB75_ADAPTER_V0_THT_PINOUT 6
#define HUB75_ADAPTER_V0_SMT_PINOUT 7
#define ESP32_JC_RIBBON_PINOUT_WEMOS 8
#define HUB75_ADAPTER_LITE_V0_PINOUT 9
#define ESP32_RGB64x32MatrixPanel_I2S_DMA_DEFAULT 10

#ifndef GPIOPINOUT
#define GPIOPINOUT ESP32_FORUM_PINOUT
#endif
// #define GPIOPINOUT ESP32_FORUM_PINOUT_WITH_LATCH // note this mode is untested as of 2018-05-17 - not being used anymore now that SmartMatrix Shield is available
// #define GPIOPINOUT SMARTLED_SHIELD_V0_PINOUT

// Upper half RGB
#define BIT_R1 (1 << 0)
#define BIT_G1 (1 << 1)
#define BIT_B1 (1 << 2)
// Lower half RGB
#define BIT_R2 (1 << 3)
#define BIT_G2 (1 << 4)
#define BIT_B2 (1 << 5)

// Control Signals
#define BIT_LAT (1 << 6)
#define BIT_OE (1 << 7)

#define BIT_A (1 << 8)
#define BIT_B (1 << 9)
#define BIT_C (1 << 10)
#define BIT_D (1 << 11)
#define BIT_E (1 << 12)

// ADDX is output directly using GPIO
#define CLKS_DURING_LATCH 0
#define MATRIX_I2S_MODE I2S_PARALLEL_BITS_16
#define MATRIX_DATA_STORAGE_TYPE uint16_t

#define R1_PIN GPIO_NUM_26
#define G1_PIN GPIO_NUM_27
#define B1_PIN GPIO_NUM_14
#define R2_PIN GPIO_NUM_12
#define G2_PIN GPIO_NUM_13
#define B2_PIN GPIO_NUM_15

#define A_PIN GPIO_NUM_4
#define B_PIN GPIO_NUM_19
#define C_PIN GPIO_NUM_22
#define D_PIN GPIO_NUM_21
#define E_PIN GPIO_NUM_18 // v2 -> GPIO 2

#define LAT_PIN GPIO_NUM_25
#define OE_PIN  GPIO_NUM_33
#define CLK_PIN GPIO_NUM_32

#define PICO_LED_PIN GPIO_NUM_0


#else
#pragma GCC error "Multiple MatrixHardware*.h files included"
#endif
