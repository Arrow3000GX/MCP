# Seeed XIAO ESP32S3 Sense Board Configuration

## 硬件规格 (Hardware Specifications)

### 主控芯片 (Main Controller)
- **芯片型号**: ESP32-S3
- **CPU**: 双核Xtensa LX7 @ 240MHz
- **内存**: 512KB SRAM + 16MB PSRAM
- **存储**: 16MB Flash
- **连接**: WiFi + Bluetooth 5.0

### 音频系统 (Audio System)
- **内置麦克风**: I2S接口，支持AEC/NS/AGC
- **编解码器**: ES8311 (内置)
- **外接放大器**: MAX98357A I2S数字功放
- **扬声器**: 4Ω 3W 40mm扬声器
- **采样率**: 输入16kHz，输出24kHz

### 视觉系统 (Vision System)
- **摄像头**: OV2640 (内置)
- **分辨率**: 支持多种分辨率，默认HVGA(480×320)
- **格式**: RGB565，支持JPEG编码
- **接口**: DVP并行接口

### 其他功能 (Other Features)
- **RGB LED**: 内置可编程RGB LED
- **状态LED**: 内置状态指示LED
- **按键**: BOOT按键
- **电池监控**: 支持电池电压检测
- **显示屏**: 可选外接SPI显示屏

## 引脚配置 (Pin Configuration)

### 音频引脚 (Audio Pins)
```
I2S_WS   -> GPIO4  (LRC - Left/Right Clock)
I2S_BCLK -> GPIO5  (Bit Clock)
I2S_DIN  -> GPIO6  (Data In - 麦克风)
I2S_DOUT -> GPIO6  (Data Out - 扬声器，与DIN共用)
```

### 摄像头引脚 (Camera Pins)
```
PWDN     -> GPIO15 (Power Down)
RESET    -> GPIO16 (Reset)
XCLK     -> GPIO14 (External Clock)
SIOD     -> GPIO17 (I2C Data)
SIOC     -> GPIO18 (I2C Clock)
D7-D0    -> GPIO19-46 (Data Bus)
VSYNC    -> GPIO47 (Vertical Sync)
HREF     -> GPIO38 (Horizontal Reference)
PCLK     -> GPIO37 (Pixel Clock)
```

### 控制引脚 (Control Pins)
```
BOOT_BUTTON -> GPIO1  (Boot Button)
RGB_LED     -> GPIO48 (RGB LED)
STATUS_LED  -> GPIO21 (Status LED)
BATTERY_ADC -> GPIO44 (Battery Voltage)
```

### MAX98357A连接 (MAX98357A Connections)
```
BCLK -> GPIO5  (Bit Clock)
LRC  -> GPIO4  (Left/Right Clock)
DIN  -> GPIO6  (Data Input)
SD   -> GPIO7  (Shutdown - 连接到VCC启用)
VCC  -> 3.3V
GND  -> GND
```

## 编译配置 (Build Configuration)

### 目标芯片 (Target Chip)
```bash
idf.py set-target esp32s3
```

### 分区表 (Partition Table)
- 使用16MB分区表: `partitions/v2/16m.csv`
- 支持OTA升级
- 预留足够的存储空间

### 音频配置 (Audio Configuration)
- 输入采样率: 16kHz (麦克风)
- 输出采样率: 24kHz (扬声器)
- 支持AEC回声消除
- 支持NS噪声抑制
- 支持AGC自动增益控制

## 使用方法 (Usage)

### 1. 环境准备 (Environment Setup)
```bash
# 安装ESP-IDF
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh
. ./export.sh

# 进入项目目录
cd /path/to/xiaozhi-esp32-main
```

### 2. 配置项目 (Configure Project)
```bash
# 设置目标芯片
idf.py set-target esp32s3

# 进入配置菜单
idf.py menuconfig
```

在配置菜单中:
- 选择 `Xiaozhi Assistant` -> `Board Type` -> `Seeed XIAO ESP32S3 Sense`
- 配置WiFi设置
- 配置音频参数
- 配置摄像头参数

### 3. 编译固件 (Build Firmware)
```bash
# 编译
idf.py build

# 烧录
idf.py -p /dev/tty.usbmodem1101 flash monitor
```

### 4. 使用release脚本 (Using Release Script)
```bash
# 自动编译和打包
python scripts/release.py seeed-xiao-esp32s3-sense
```

## MCP工具 (MCP Tools)

### 可用的MCP工具 (Available MCP Tools)

#### 1. 扬声器控制 (Speaker Control)
```json
{
  "tool": "speaker",
  "action": "set_volume",
  "volume": 80
}
```

#### 2. LED控制 (LED Control)
```json
{
  "tool": "led",
  "r": 255,
  "g": 0,
  "b": 0
}
```

#### 3. 电池状态 (Battery Status)
```json
{
  "tool": "battery"
}
```

#### 4. 摄像头控制 (Camera Control)
```json
{
  "tool": "camera",
  "action": "capture"
}
```

## 硬件连接图 (Hardware Connection Diagram)

```
XIAO ESP32S3 Sense
┌─────────────────┐
│  GPIO4  ──────┐ │
│  GPIO5  ──────┤ │  MAX98357A
│  GPIO6  ──────┤ │  ┌─────────┐
│  GPIO7  ──────┘ │  │ BCLK    │
│                 │  │ LRC     │
│  GPIO1  ────────┼─→│ DIN     │
│  GPIO48 ────────┼─→│ SD      │
│  GPIO21 ────────┼─→│ VCC     │
│  GPIO44 ────────┼─→│ GND     │
└─────────────────┘  └─────────┘
                           │
                           ▼
                    4Ω 3W Speaker
```

## 故障排除 (Troubleshooting)

### 音频问题 (Audio Issues)
1. **无声音输出**: 检查MAX98357A的SD引脚是否连接到VCC
2. **音量过小**: 检查扬声器阻抗是否为4Ω
3. **音质差**: 检查I2S引脚连接和采样率配置

### 摄像头问题 (Camera Issues)
1. **无法初始化**: 检查摄像头引脚连接
2. **图像异常**: 检查电源和时钟信号
3. **颜色异常**: 检查RGB565字节序配置

### 网络问题 (Network Issues)
1. **无法连接WiFi**: 检查WiFi凭据和信号强度
2. **连接不稳定**: 检查天线连接

## 技术参数 (Technical Parameters)

| 参数 | 数值 | 说明 |
|------|------|------|
| 工作电压 | 3.3V | 主控电压 |
| 音频输入 | 16kHz | 麦克风采样率 |
| 音频输出 | 24kHz | 扬声器采样率 |
| 摄像头分辨率 | 480×320 | 默认分辨率 |
| 内存 | 512KB + 16MB | SRAM + PSRAM |
| 存储 | 16MB | Flash存储 |
| 功耗 | <1W | 典型工作功耗 |

## 更新日志 (Changelog)

### v1.0.0 (2024-10-18)
- 初始版本发布
- 支持XIAO ESP32S3 Sense基本功能
- 支持MAX98357A放大器
- 支持OV2640摄像头
- 支持RGB LED和电池监控
- 集成MCP工具支持

## 许可证 (License)

本项目遵循MIT许可证。详见LICENSE文件。

## 联系方式 (Contact)

如有问题或建议，请通过以下方式联系:
- GitHub Issues: [项目地址]
- 邮箱: [联系邮箱]
