# ESP32 Firmware Flashing Guide

## 🚀 **Quick Start (Pre-built Firmware)**

### **Step 1: Download Pre-built Firmware**

For the xiaozhi ESP32 project, you can download pre-built firmware from the official releases:

1. **Go to GitHub Releases**: https://github.com/78/xiaozhi-esp32/releases
2. **Download the latest firmware** for your ESP32 board type:
   - `xiaozhi-esp32s3-v2.x.x.bin` (for ESP32-S3)
   - `xiaozhi-esp32c3-v2.x.x.bin` (for ESP32-C3)
   - `xiaozhi-esp32p4-v2.x.x.bin` (for ESP32-P4)

### **Step 2: Install Flashing Tools**

#### **Option A: ESP Flash Tool (Recommended for beginners)**
- Download ESP Flash Tool from: https://www.espressif.com/en/support/download/other-tools
- This provides a GUI interface for flashing

#### **Option B: Command Line (Advanced users)**
```bash
# Install esptool
pip install esptool
```

### **Step 3: Connect ESP32**

1. **Connect ESP32 to computer** via USB cable
2. **Find the COM port**:
   - **Windows**: Device Manager → Ports (COM & LPT) → USB Serial Device (COMx)
   - **macOS**: `/dev/tty.usbserial-*` or `/dev/tty.SLAB_USBtoUART`
   - **Linux**: `/dev/ttyUSB0` or `/dev/ttyACM0`

### **Step 4: Flash Firmware**

#### **Using ESP Flash Tool (GUI)**
1. Open ESP Flash Tool
2. Select your COM port
3. Click "Choose" and select your firmware file
4. Set flash address to `0x00000000`
5. Click "START"
6. Press and hold BOOT button on ESP32, then press RESET, then release BOOT

#### **Using Command Line**
```bash
# For ESP32-S3
esptool.py --chip esp32s3 --port /dev/ttyUSB0 --baud 460800 write_flash 0x00000000 xiaozhi-esp32s3-v2.x.x.bin

# For ESP32-C3  
esptool.py --chip esp32c3 --port /dev/ttyUSB0 --baud 460800 write_flash 0x00000000 xiaozhi-esp32c3-v2.x.x.bin

# For ESP32-P4
esptool.py --chip esp32p4 --port /dev/ttyUSB0 --baud 460800 write_flash 0x00000000 xiaozhi-esp32p4-v2.x.x.bin
```

**Note**: Replace `/dev/ttyUSB0` with your actual COM port (e.g., `COM3` on Windows)

---

## 🔧 **Build from Source (Advanced)**

### **Prerequisites**

1. **Install ESP-IDF**:
   ```bash
   # Install ESP-IDF v5.4 or later
   git clone --recursive https://github.com/espressif/esp-idf.git
   cd esp-idf
   ./install.sh
   . ./export.sh
   ```

2. **Install dependencies**:
   ```bash
   pip install esptool
   ```

### **Build Process**

1. **Navigate to project**:
   ```bash
   cd /Users/yliu3y/Desktop/WRO/xiaozhi-esp32-main
   ```

2. **Configure for your board**:
   ```bash
   # For ESP32-S3
   idf.py set-target esp32s3
   
   # For ESP32-C3
   idf.py set-target esp32c3
   
   # For ESP32-P4
   idf.py set-target esp32p4
   ```

3. **Configure project**:
   ```bash
   idf.py menuconfig
   ```
   - Configure WiFi settings
   - Select your board configuration
   - Set partition table

4. **Build firmware**:
   ```bash
   idf.py build
   ```

5. **Flash firmware**:
   ```bash
   idf.py -p /dev/ttyUSB0 flash
   ```

6. **Monitor output**:
   ```bash
   idf.py -p /dev/ttyUSB0 monitor
   ```

---

## 📱 **Board-Specific Instructions**

### **ESP32-S3 Boards**
- **Supported boards**: ESP32-S3-BOX3, M5Stack CoreS3, LILYGO T-Circle-S3
- **Flash size**: 16MB recommended
- **Partition table**: Use `partitions/v2/16m.csv`

### **ESP32-C3 Boards**  
- **Supported boards**: 虾哥 Mini C3, ESP32-C3-DevKitM-1
- **Flash size**: 4MB minimum, 16MB recommended
- **Partition table**: Use `partitions/v2/16m_c3.csv`

### **ESP32-P4 Boards**
- **Supported boards**: ESP32-P4-DevKitM-1
- **Flash size**: 16MB recommended
- **Partition table**: Use `partitions/v2/16m.csv`

---

## 🛠️ **Troubleshooting**

### **Common Issues**

1. **"Failed to connect to ESP32"**:
   - Press and hold BOOT button
   - Press and release RESET button
   - Release BOOT button
   - Try flashing again

2. **"Permission denied" on Linux/macOS**:
   ```bash
   sudo chmod 666 /dev/ttyUSB0
   ```

3. **"No module named 'esptool'"**:
   ```bash
   pip install esptool
   ```

4. **Wrong chip detected**:
   - Specify chip type: `esptool.py --chip esp32s3 ...`
   - Check your board documentation

### **Manual Boot Mode**
If automatic boot mode doesn't work:
1. Hold BOOT button
2. Press RESET button
3. Release RESET button
4. Release BOOT button
5. ESP32 should now be in bootloader mode

---

## 📋 **Firmware Configuration**

After flashing, configure the device:

1. **WiFi Setup**:
   - Connect to device's WiFi hotspot
   - Open web interface (usually 192.168.4.1)
   - Configure WiFi credentials

2. **Server Configuration**:
   - Default: xiaozhi.me official server
   - Or configure your own server

3. **Audio Configuration**:
   - Test microphone and speaker
   - Adjust audio levels

---

## 🔗 **Useful Links**

- **Official Project**: https://github.com/78/xiaozhi-esp32
- **Documentation**: https://ccnphfhqs21z.feishu.cn/wiki/F5krwD16viZoF0kKkvDcrZNYnhb
- **ESP-IDF**: https://docs.espressif.com/projects/esp-idf/en/latest/
- **esptool**: https://github.com/espressif/esptool

---

## 📞 **Support**

- **QQ Group**: 1011329060
- **GitHub Issues**: https://github.com/78/xiaozhi-esp32/issues
- **Documentation**: https://ccnphfhqs21z.feishu.cn/wiki/

---

*This guide is for the xiaozhi ESP32 AI chatbot project. For other ESP32 projects, adapt the commands accordingly.*
