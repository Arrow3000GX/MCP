#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <driver/gpio.h>

// ========================================
// 音频配置 (Audio Configuration)
// ========================================

// 音频采样率配置
#define AUDIO_INPUT_SAMPLE_RATE  16000   // 麦克风输入采样率
#define AUDIO_OUTPUT_SAMPLE_RATE 24000   // 扬声器输出采样率 (匹配MAX98357A要求)

// I2S 音频引脚配置 (XIAO ESP32S3 Sense)
#define AUDIO_I2S_GPIO_MCLK GPIO_NUM_0   // MCLK (Master Clock) - 通常不需要
#define AUDIO_I2S_GPIO_WS   GPIO_NUM_4   // LRC (Left/Right Clock) - WS引脚
#define AUDIO_I2S_GPIO_BCLK GPIO_NUM_5   // BCLK (Bit Clock) - BCLK引脚
#define AUDIO_I2S_GPIO_DIN  GPIO_NUM_6   // DIN (Data In) - 麦克风数据输入
#define AUDIO_I2S_GPIO_DOUT GPIO_NUM_6   // DOUT (Data Out) - 扬声器数据输出 (与DIN共用)

// 音频编解码器配置 (XIAO ESP32S3 Sense 内置)
#define AUDIO_CODEC_PA_PIN       GPIO_NUM_NC  // MAX98357A不需要PA控制引脚
#define AUDIO_CODEC_I2C_SDA_PIN  GPIO_NUM_2   // I2C SDA (内置编解码器)
#define AUDIO_CODEC_I2C_SCL_PIN  GPIO_NUM_3   // I2C SCL (内置编解码器)
#define AUDIO_CODEC_ES8311_ADDR  0x18         // ES8311 编解码器地址

// MAX98357A 放大器配置
#define MAX98357A_SD_PIN         GPIO_NUM_7   // SD引脚 (Shutdown) - 连接到VCC启用
#define MAX98357A_GAIN_PIN       GPIO_NUM_NC  // GAIN引脚 (可选)

// ========================================
// 按钮配置 (Button Configuration)
// ========================================

#define BOOT_BUTTON_GPIO        GPIO_NUM_1   // BOOT按钮 (XIAO ESP32S3)

// ========================================
// 显示屏配置 (Display Configuration) - 可选
// ========================================

// 如果使用外接显示屏，配置SPI引脚
#define DISPLAY_SPI_SCK_PIN     GPIO_NUM_8   // SPI时钟
#define DISPLAY_SPI_MOSI_PIN    GPIO_NUM_9   // SPI数据
#define DISPLAY_DC_PIN          GPIO_NUM_10  // 数据/命令选择
#define DISPLAY_SPI_CS_PIN      GPIO_NUM_11  // 片选
#define DISPLAY_RST_PIN         GPIO_NUM_12  // 复位

// 显示屏参数 (以ST7789 240x240为例)
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  240
#define DISPLAY_MIRROR_X false
#define DISPLAY_MIRROR_Y false
#define DISPLAY_SWAP_XY false

#define DISPLAY_OFFSET_X  0
#define DISPLAY_OFFSET_Y  0

#define DISPLAY_BACKLIGHT_PIN GPIO_NUM_13    // 背光控制
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT false

// ========================================
// LED配置 (LED Configuration)
// ========================================

#define STATUS_LED_PIN          GPIO_NUM_21  // 状态LED (XIAO ESP32S3内置)
#define RGB_LED_PIN             GPIO_NUM_48  // RGB LED (XIAO ESP32S3内置)

// ========================================
// 传感器配置 (Sensor Configuration)
// ========================================

// XIAO ESP32S3 Sense 内置传感器
#define CAMERA_PIN_PWDN         GPIO_NUM_15  // 摄像头电源控制
#define CAMERA_PIN_RESET        GPIO_NUM_16  // 摄像头复位
#define CAMERA_PIN_XCLK         GPIO_NUM_14  // 摄像头时钟
#define CAMERA_PIN_SIOD         GPIO_NUM_17  // 摄像头I2C数据
#define CAMERA_PIN_SIOC         GPIO_NUM_18  // 摄像头I2C时钟
#define CAMERA_PIN_D7           GPIO_NUM_19  // 摄像头数据7
#define CAMERA_PIN_D6           GPIO_NUM_20  // 摄像头数据6
#define CAMERA_PIN_D5           GPIO_NUM_39  // 摄像头数据5
#define CAMERA_PIN_D4           GPIO_NUM_40  // 摄像头数据4
#define CAMERA_PIN_D3           GPIO_NUM_41  // 摄像头数据3
#define CAMERA_PIN_D2           GPIO_NUM_42  // 摄像头数据2
#define CAMERA_PIN_D1           GPIO_NUM_45  // 摄像头数据1
#define CAMERA_PIN_D0           GPIO_NUM_46  // 摄像头数据0
#define CAMERA_PIN_VSYNC        GPIO_NUM_47  // 摄像头垂直同步
#define CAMERA_PIN_HREF         GPIO_NUM_38  // 摄像头水平参考
#define CAMERA_PIN_PCLK         GPIO_NUM_37  // 摄像头像素时钟

// ========================================
// 电源管理配置 (Power Management)
// ========================================

#define BATTERY_ADC_PIN         GPIO_NUM_44  // 电池电压检测ADC
#define POWER_EN_PIN            GPIO_NUM_35  // 电源使能引脚

// ========================================
// MCP工具配置 (MCP Tools Configuration)
// ========================================

// 可用的MCP工具
#define MCP_TOOL_SPEAKER        true   // 扬声器控制
#define MCP_TOOL_LED            true   // LED控制
#define MCP_TOOL_BATTERY        true   // 电池状态
#define MCP_TOOL_CAMERA         true   // 摄像头控制 (可选)

// ========================================
// 硬件特性配置 (Hardware Features)
// ========================================

// XIAO ESP32S3 Sense 特性
#define BOARD_HAS_CAMERA        true   // 支持摄像头
#define BOARD_HAS_RGB_LED       true   // 支持RGB LED
#define BOARD_HAS_STATUS_LED    true   // 支持状态LED
#define BOARD_HAS_BUILTIN_MIC   true   // 支持内置麦克风
#define BOARD_HAS_EXTERNAL_AMP  true   // 支持外接放大器

// 音频特性
#define AUDIO_HAS_AEC           true   // 支持回声消除
#define AUDIO_HAS_NS            true   // 支持噪声抑制
#define AUDIO_HAS_AGC           true   // 支持自动增益控制

#endif // _BOARD_CONFIG_H_
