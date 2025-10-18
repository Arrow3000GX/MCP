# Custom ESP32 Firmware Build Guide

## 🛠️ **Prerequisites**

### **1. Install ESP-IDF (Required)**
ESP-IDF is the official development framework for ESP32. You need version 5.4 or later.

#### **Installation Steps:**

```bash
# Clone ESP-IDF
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf

# Install ESP-IDF
./install.sh

# Set up environment
. ./export.sh
```

#### **Add to your shell profile:**
```bash
# Add to ~/.zshrc or ~/.bashrc
alias get_idf='. $HOME/esp/esp-idf/export.sh'
```

### **2. Install Python Dependencies**
```bash
pip3 install esptool
pip3 install pyyaml
pip3 install pyelftools
```

---

## 🔧 **Build Custom Firmware**

### **Step 1: Navigate to Project**
```bash
cd /Users/yliu3y/Desktop/WRO/xiaozhi-esp32-main
```

### **Step 2: Set Target Chip**
Choose your ESP32 variant:

```bash
# For ESP32-S3 (most common)
idf.py set-target esp32s3

# For ESP32-C3
idf.py set-target esp32c3

# For ESP32-P4
idf.py set-target esp32p4

# For ESP32 (original)
idf.py set-target esp32
```

### **Step 3: Configure Project**
```bash
idf.py menuconfig
```

#### **Key Configuration Options:**

**1. Board Selection:**
- Go to `Component config` → `XiaoZhi ESP32` → `Board Configuration`
- Select your specific board (e.g., ESP32-S3-BOX3, M5Stack CoreS3, etc.)

**2. WiFi Configuration:**
- Go to `Component config` → `XiaoZhi ESP32` → `WiFi Configuration`
- Set default WiFi SSID and password
- Configure WiFi timeout settings

**3. Audio Configuration:**
- Go to `Component config` → `XiaoZhi ESP32` → `Audio Configuration`
- Select microphone type (I2S, PDM, etc.)
- Configure speaker settings
- Set audio codec (OPUS, MP3, etc.)

**4. Display Configuration:**
- Go to `Component config` → `XiaoZhi ESP32` → `Display Configuration`
- Select display type (OLED, LCD, TFT)
- Configure display resolution and interface

**5. MCP Server Configuration:**
- Go to `Component config` → `XiaoZhi ESP32` → `MCP Server Configuration`
- Enable/disable MCP server
- Configure MCP server settings

**6. Partition Table:**
- Go to `Partition Table`
- Select appropriate partition table based on flash size:
  - `partitions/v2/4m.csv` (4MB flash)
  - `partitions/v2/8m.csv` (8MB flash)
  - `partitions/v2/16m.csv` (16MB flash)
  - `partitions/v2/32m.csv` (32MB flash)

### **Step 4: Build Firmware**
```bash
idf.py build
```

This will:
- Compile all source code
- Generate binary files
- Create firmware image

### **Step 5: Flash Firmware**
```bash
# Flash to your ESP32
idf.py -p /dev/tty.usbmodem1101 flash

# Flash and monitor output
idf.py -p /dev/tty.usbmodem1101 flash monitor
```

---

## 🎛️ **Custom Configuration Options**

### **1. Custom Wake Words**
Edit `main/audio/wake_words/custom_wake_word.cc`:
```cpp
// Add your custom wake word
const char* custom_wake_word = "Hello Assistant";
```

### **2. Custom Board Configuration**
Create new board file in `main/boards/`:
```cpp
// my_board.h
#ifndef MY_BOARD_H
#define MY_BOARD_H

#include "board.h"

// Define your board's GPIO pins
#define MIC_I2S_WS_PIN    4
#define MIC_I2S_BCK_PIN   5
#define MIC_I2S_DATA_PIN  6
#define SPEAKER_I2S_WS_PIN 10
#define SPEAKER_I2S_BCK_PIN 11
#define SPEAKER_I2S_DATA_PIN 12

// Display pins
#define DISPLAY_SDA_PIN   21
#define DISPLAY_SCL_PIN   22

#endif
```

### **3. Custom MCP Tools**
Edit `main/mcp_server.cc` to add your custom tools:
```cpp
// Add custom MCP tool
void add_custom_tool() {
    // Your custom tool implementation
}
```

### **4. Custom Audio Processing**
Edit `main/audio/audio_processor.h`:
```cpp
class CustomAudioProcessor : public AudioProcessor {
    // Your custom audio processing logic
};
```

---

## 📱 **Board-Specific Configurations**

### **ESP32-S3 Boards**
- **Flash Size**: 16MB recommended
- **Partition Table**: `partitions/v2/16m.csv`
- **Audio**: I2S microphone and speaker
- **Display**: SPI or I2C OLED/LCD

### **ESP32-C3 Boards**
- **Flash Size**: 4MB minimum, 16MB recommended
- **Partition Table**: `partitions/v2/16m_c3.csv`
- **Audio**: I2S or PDM microphone
- **Display**: I2C OLED

### **ESP32-P4 Boards**
- **Flash Size**: 16MB recommended
- **Partition Table**: `partitions/v2/16m.csv`
- **Audio**: High-quality I2S
- **Display**: High-resolution LCD

---

## 🔍 **Build Output Files**

After building, you'll find these files in `build/`:

- `bootloader.bin` - Bootloader
- `partition-table.bin` - Partition table
- `xiaozhi-esp32.bin` - Main application
- `xiaozhi-esp32.elf` - ELF file for debugging

### **Manual Flash Command:**
```bash
esptool.py --chip esp32s3 --port /dev/tty.usbmodem1101 --baud 460800 write_flash \
  0x1000 build/bootloader/bootloader.bin \
  0x8000 build/partition_table/partition-table.bin \
  0x10000 build/xiaozhi-esp32.bin
```

---

## 🐛 **Debugging and Monitoring**

### **Monitor Serial Output:**
```bash
idf.py -p /dev/tty.usbmodem1101 monitor
```

### **Debug with GDB:**
```bash
idf.py -p /dev/tty.usbmodem1101 debug
```

### **Common Build Issues:**

1. **"No module named 'esptool'"**:
   ```bash
   pip3 install esptool
   ```

2. **"Permission denied"**:
   ```bash
   sudo chmod 666 /dev/tty.usbmodem1101
   ```

3. **"Chip not detected"**:
   - Hold BOOT button, press RESET, release BOOT
   - Check USB cable and drivers

4. **Build errors**:
   - Clean build: `idf.py clean`
   - Rebuild: `idf.py build`

---

## 🚀 **Advanced Customization**

### **1. Custom MCP Server**
Implement your own MCP server in `main/mcp_server.cc`:
```cpp
class CustomMCPServer {
    void handle_custom_tool(const std::string& tool_name, const json& args) {
        // Your custom tool logic
    }
};
```

### **2. Custom Audio Codec**
Add new audio codec in `main/audio/codecs/`:
```cpp
class CustomAudioCodec : public AudioCodec {
    // Your custom codec implementation
};
```

### **3. Custom Display Driver**
Implement custom display in `main/display/`:
```cpp
class CustomDisplay : public Display {
    // Your custom display implementation
};
```

---

## 📋 **Build Scripts**

Create custom build scripts:

### **build_custom.sh**
```bash
#!/bin/bash
cd /Users/yliu3y/Desktop/WRO/xiaozhi-esp32-main

# Set target
idf.py set-target esp32s3

# Configure
idf.py menuconfig

# Build
idf.py build

# Flash
idf.py -p /dev/tty.usbmodem1101 flash monitor
```

### **Make it executable:**
```bash
chmod +x build_custom.sh
./build_custom.sh
```

---

## 📞 **Support and Resources**

- **Official Documentation**: https://docs.espressif.com/projects/esp-idf/en/latest/
- **XiaoZhi Project**: https://github.com/78/xiaozhi-esp32
- **ESP-IDF Examples**: https://github.com/espressif/esp-idf/tree/master/examples
- **Community Forum**: https://esp32.com/

---

*This guide will help you build custom firmware for your xiaozhi ESP32 project with full control over features and configuration.*
