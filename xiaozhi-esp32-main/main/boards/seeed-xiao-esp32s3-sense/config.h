#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

// Seeed XIAO ESP32S3 Sense Board configuration (Simplified)
// Based on working atoms3-echo-base configuration

#include <driver/gpio.h>

#define AUDIO_INPUT_REFERENCE    true
#define AUDIO_INPUT_SAMPLE_RATE  24000
#define AUDIO_OUTPUT_SAMPLE_RATE 24000

// Simple I2S audio configuration (no external codec)
#define AUDIO_I2S_GPIO_MCLK GPIO_NUM_NC
#define AUDIO_I2S_GPIO_WS GPIO_NUM_3
#define AUDIO_I2S_GPIO_BCLK GPIO_NUM_4
#define AUDIO_I2S_GPIO_DIN  GPIO_NUM_NC
#define AUDIO_I2S_GPIO_DOUT GPIO_NUM_6

// No external audio codec
#define AUDIO_CODEC_I2C_SDA_PIN  GPIO_NUM_NC
#define AUDIO_CODEC_I2C_SCL_PIN  GPIO_NUM_NC
#define AUDIO_CODEC_ES8311_ADDR  ES8311_CODEC_DEFAULT_ADDR
#define AUDIO_CODEC_GPIO_PA GPIO_NUM_NC

// Simple button configuration
#define BUILTIN_LED_GPIO        GPIO_NUM_21
#define BOOT_BUTTON_GPIO        GPIO_NUM_0
#define VOLUME_UP_BUTTON_GPIO   GPIO_NUM_NC
#define VOLUME_DOWN_BUTTON_GPIO GPIO_NUM_NC

// No display
#define DISPLAY_SDA_PIN GPIO_NUM_NC
#define DISPLAY_SCL_PIN GPIO_NUM_NC
#define DISPLAY_WIDTH   128
#define DISPLAY_HEIGHT  128
#define DISPLAY_MIRROR_X false
#define DISPLAY_MIRROR_Y false
#define DISPLAY_SWAP_XY false

#define DISPLAY_OFFSET_X  0
#define DISPLAY_OFFSET_Y  0

#define DISPLAY_BACKLIGHT_PIN GPIO_NUM_NC
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT false

#endif // _BOARD_CONFIG_H_