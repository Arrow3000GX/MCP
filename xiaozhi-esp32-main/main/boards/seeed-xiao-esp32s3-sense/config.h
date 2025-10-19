#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <driver/gpio.h>

// Audio Configuration - Based on working bread-compact-wifi
#define AUDIO_INPUT_SAMPLE_RATE  16000
#define AUDIO_OUTPUT_SAMPLE_RATE 16000

#define AUDIO_INPUT_REFERENCE    false

// I2S Audio Pins - Using XIAO ESP32S3 Sense pins
#define AUDIO_I2S_GPIO_MCLK      GPIO_NUM_NC
#define AUDIO_I2S_GPIO_WS        GPIO_NUM_6  // LRC (Left/Right Clock)
#define AUDIO_I2S_GPIO_BCLK      GPIO_NUM_8  // BCLK (Bit Clock)
#define AUDIO_I2S_GPIO_DIN       GPIO_NUM_7  // DIN (Data In)
#define AUDIO_I2S_GPIO_DOUT      GPIO_NUM_5  // DOUT (Data Out)

// Button Configuration
#define BOOT_BUTTON_GPIO         GPIO_NUM_1  // BOOT button on XIAO ESP32S3
#define BUILTIN_LED_GPIO         GPIO_NUM_21 // Built-in LED

// Display Configuration - Disabled
#define DISPLAY_SDA_PIN          GPIO_NUM_NC
#define DISPLAY_SCL_PIN          GPIO_NUM_NC
#define DISPLAY_WIDTH            128
#define DISPLAY_HEIGHT           128
#define DISPLAY_MIRROR_X         false
#define DISPLAY_MIRROR_Y         false
#define DISPLAY_SWAP_XY          false
#define DISPLAY_OFFSET_X         0
#define DISPLAY_OFFSET_Y         0

// Camera Configuration - Disabled
#define CAMERA_PIN_PWDN          GPIO_NUM_NC
#define CAMERA_PIN_RESET         GPIO_NUM_NC
#define CAMERA_PIN_XCLK          GPIO_NUM_NC
#define CAMERA_PIN_SIOD          GPIO_NUM_NC
#define CAMERA_PIN_SIOC          GPIO_NUM_NC
#define CAMERA_PIN_D7            GPIO_NUM_NC
#define CAMERA_PIN_D6            GPIO_NUM_NC
#define CAMERA_PIN_D5            GPIO_NUM_NC
#define CAMERA_PIN_D4            GPIO_NUM_NC
#define CAMERA_PIN_D3            GPIO_NUM_NC
#define CAMERA_PIN_D2            GPIO_NUM_NC
#define CAMERA_PIN_D1            GPIO_NUM_NC
#define CAMERA_PIN_D0            GPIO_NUM_NC
#define CAMERA_PIN_VSYNC         GPIO_NUM_NC
#define CAMERA_PIN_HREF          GPIO_NUM_NC
#define CAMERA_PIN_PCLK          GPIO_NUM_NC

// MCP Tools - Disabled for minimal config
#define MCP_TOOL_SPEAKER         false
#define MCP_TOOL_LED             false
#define MCP_TOOL_BATTERY         false
#define MCP_TOOL_CAMERA          false

// Hardware Features - Disabled for minimal config
#define BOARD_HAS_CAMERA         false
#define BOARD_HAS_RGB_LED        false
#define BOARD_HAS_STATUS_LED     false
#define BOARD_HAS_BUILTIN_MIC    false
#define BOARD_HAS_EXTERNAL_AMP   false

#define AUDIO_HAS_AEC            false
#define AUDIO_HAS_NS             false
#define AUDIO_HAS_AGC            false

#endif // _BOARD_CONFIG_H_