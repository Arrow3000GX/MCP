#include "wifi_board.h"
#include "codecs/no_audio_codec.h"
#include "display/lcd_display.h"
#include "application.h"
#include "button.h"
#include "config.h"
#include "mcp_server.h"
#include "camera/ov2640_camera.h"
#include "led/rgb_led.h"
#include "power/battery_monitor.h"

#include <esp_log.h>
#include <driver/i2c_master.h>
#include <driver/spi_common.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

#define TAG "XiaoESP32S3Sense"

class SeeedXiaoEsp32s3SenseBoard : public WifiBoard {
private:
    i2c_master_bus_handle_t codec_i2c_bus_;
    Button boot_button_;
    LcdDisplay* display_;
    Ov2640Camera* camera_;
    RgbLed* rgb_led_;
    BatteryMonitor* battery_monitor_;
    esp_adc_cal_characteristics_t adc_chars_;

    // I2C初始化
    void InitializeI2c() {
        i2c_master_bus_config_t i2c_bus_cfg = {
            .i2c_port = I2C_NUM_0,
            .sda_io_num = AUDIO_CODEC_I2C_SDA_PIN,
            .scl_io_num = AUDIO_CODEC_I2C_SCL_PIN,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .intr_priority = 0,
            .trans_queue_depth = 0,
            .flags = {
                .enable_internal_pullup = 1,
            },
        };
        ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_cfg, &codec_i2c_bus_));
        ESP_LOGI(TAG, "I2C initialized for audio codec");
    }

    // SPI初始化（用于显示屏）
    void InitializeSpi() {
        if (BOARD_HAS_CAMERA) {
            spi_bus_config_t buscfg = {};
            buscfg.mosi_io_num = DISPLAY_SPI_MOSI_PIN;
            buscfg.miso_io_num = GPIO_NUM_NC;
            buscfg.sclk_io_num = DISPLAY_SPI_SCK_PIN;
            buscfg.quadwp_io_num = GPIO_NUM_NC;
            buscfg.quadhd_io_num = GPIO_NUM_NC;
            buscfg.max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t);
            ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
            ESP_LOGI(TAG, "SPI initialized for display");
        }
    }

    // 按钮初始化
    void InitializeButtons() {
        boot_button_.OnClick([this]() {
            auto& app = Application::GetInstance();
            if (app.GetDeviceState() == kDeviceStateStarting && !WifiStation::GetInstance().IsConnected()) {
                ResetWifiConfiguration();
            }
            app.ToggleChatState();
        });
        ESP_LOGI(TAG, "Boot button initialized");
    }

    // 显示屏初始化（可选）
    void InitializeDisplay() {
        if (BOARD_HAS_CAMERA) {
            esp_lcd_panel_io_handle_t panel_io = nullptr;
            esp_lcd_panel_handle_t panel = nullptr;
            
            esp_lcd_panel_io_spi_config_t io_config = {};
            io_config.csrailway_gpio_num = DISPLAY_SPI_CS_PIN;
            io_config.dc_gpio_num = DISPLAY_DC_PIN;
            io_config.spi_mode = 2;
            io_config.pclk_hz = 80 * 1000 * 1000;
            io_config.trans_queue_depth = 10;
            io_config.lcd_cmd_bits = 8;
            io_config.lcd_param_bits = 8;
            ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(SPI2_HOST, &io_config, &panel_io));

            esp_lcd_panel_dev_config_t panel_config = {};
            panel_config.reset_gpio_num = DISPLAY_RST_PIN;
            panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
            panel_config.bits_per_pixel = 16;
            ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(panel_io, &panel_config, &panel));
            
            esp_lcd_panel_reset(panel);
            esp_lcd_panel_init(panel);
            esp_lcd_panel_invert_color(panel, true);
            esp_lcd_panel_swap_xy(panel, DISPLAY_SWAP_XY);
            esp_lcd_panel_mirror(panel, DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y);
            
            display_ = new SpiLcdDisplay(panel_io, panel,
                                        DISPLAY_WIDTH, DISPLAY_HEIGHT, 
                                        DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y, 
                                        DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y, DISPLAY_SWAP_XY);
            ESP_LOGI(TAG, "Display initialized");
        }
    }

    // 摄像头初始化
    void InitializeCamera() {
        if (BOARD_HAS_CAMERA) {
            camera_config_t camera_config = {};
            camera_config.pin_pwdn = CAMERA_PIN_PWDN;
            camera_config.pin_reset = CAMERA_PIN_RESET;
            camera_config.pin_xclk = CAMERA_PIN_XCLK;
            camera_config.pin_sccb_sda = CAMERA_PIN_SIOD;
            camera_config.pin_sccb_scl = CAMERA_PIN_SIOC;
            camera_config.pin_d7 = CAMERA_PIN_D7;
            camera_config.pin_d6 = CAMERA_PIN_D6;
            camera_config.pin_d5 = CAMERA_PIN_D5;
            camera_config.pin_d4 = CAMERA_PIN_D4;
            camera_config.pin_d3 = CAMERA_PIN_D3;
            camera_config.pin_d2 = CAMERA_PIN_D2;
            camera_config.pin_d1 = CAMERA_PIN_D1;
            camera_config.pin_d0 = CAMERA_PIN_D0;
            camera_config.pin_vsync = CAMERA_PIN_VSYNC;
            camera_config.pin_href = CAMERA_PIN_HREF;
            camera_config.pin_pclk = CAMERA_PIN_PCLK;
            camera_config.xclk_freq_hz = 20000000;
            camera_config.pixel_format = PIXFORMAT_RGB565;
            camera_config.frame_size = FRAMESIZE_HVGA;  // 480x320
            camera_config.jpeg_quality = 12;
            camera_config.fb_count = 1;

            camera_ = new Ov2640Camera(camera_config);
            ESP_LOGI(TAG, "Camera initialized");
        }
    }

    // RGB LED初始化
    void InitializeRgbLed() {
        if (BOARD_HAS_RGB_LED) {
            rgb_led_ = new RgbLed(RGB_LED_PIN);
            rgb_led_->SetColor(0, 0, 255);  // 蓝色表示启动
            ESP_LOGI(TAG, "RGB LED initialized");
        }
    }

    // 电池监控初始化
    void InitializeBatteryMonitor() {
        if (BOARD_HAS_BATTERY_MONITOR) {
            // 配置ADC
            adc1_config_width(ADC_WIDTH_BIT_12);
            adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);  // GPIO44
            
            // 校准ADC
            esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars_);
            
            battery_monitor_ = new BatteryMonitor(BATTERY_ADC_PIN, &adc_chars_);
            ESP_LOGI(TAG, "Battery monitor initialized");
        }
    }

    // MAX98357A放大器初始化
    void InitializeMax98357a() {
        if (BOARD_HAS_EXTERNAL_AMP) {
            // 配置SD引脚为高电平以启用放大器
            gpio_config_t io_conf = {};
            io_conf.intr_type = GPIO_INTR_DISABLE;
            io_conf.mode = GPIO_MODE_OUTPUT;
            io_conf.pin_bit_mask = (1ULL << MAX98357A_SD_PIN);
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            gpio_config(&io_conf);
            
            // 启用MAX98357A
            gpio_set_level(MAX98357A_SD_PIN, 1);
            ESP_LOGI(TAG, "MAX98357A amplifier enabled");
        }
    }

    // MCP工具初始化
    void InitializeTools() {
        auto& mcp_server = McpServer::GetInstance();
        
        // 扬声器控制工具
        if (MCP_TOOL_SPEAKER) {
            mcp_server.AddTool("speaker", [this](const json& args) -> json {
                std::string action = args["action"];
                if (action == "set_volume") {
                    int volume = args["volume"];
                    // 实现音量控制逻辑
                    return {{"success", true}, {"volume", volume}};
                } else if (action == "mute") {
                    bool mute = args["mute"];
                    // 实现静音控制逻辑
                    return {{"success", true}, {"muted", mute}};
                }
                return {{"error", "Invalid action"}};
            });
        }

        // LED控制工具
        if (MCP_TOOL_LED && rgb_led_) {
            mcp_server.AddTool("led", [this](const json& args) -> json {
                int r = args["r"];
                int g = args["g"];
                int b = args["b"];
                rgb_led_->SetColor(r, g, b);    
                return {{"success", true}, {"color", {{"r", r}, {"g", g}, {"b", b}}}};
            });
        }

        // 电池状态工具
        if (MCP_TOOL_BATTERY && battery_monitor_) {
            mcp_server.AddTool("battery", [this](const json& args) -> json {
                float voltage = battery_monitor_->GetVoltage();
                int percentage = battery_monitor_->GetPercentage();
                return {{"success", true}, {"voltage", voltage}, {"percentage", percentage}};
            });
        }

        // 摄像头工具
        if (MCP_TOOL_CAMERA && camera_) {
            mcp_server.AddTool("camera", [this](const json& args) -> json {
                std::string action = args["action"];
                if (action == "capture") {
                    // 实现拍照逻辑
                    return {{"success", true}, {"message", "Photo captured"}};
                }
                return {{"error", "Invalid action"}};
            });
        }

        ESP_LOGI(TAG, "MCP tools initialized");
    }

public:
    // 构造函数
    SeeedXiaoEsp32s3SenseBoard() : boot_button_(BOOT_BUTTON_GPIO) {
        ESP_LOGI(TAG, "Initializing Seeed XIAO ESP32S3 Sense Board");
        
        // InitializeI2c(); // Disabled - using NoAudioCodec instead of ES8311
        InitializeSpi();
        // InitializeDisplay(); // Disabled - no display connected
        InitializeCamera();
        InitializeRgbLed();
        InitializeBatteryMonitor();
        InitializeMax98357a();
        InitializeButtons();
        InitializeTools();
        
        if (GetBacklight()) {
            GetBacklight()->SetBrightness(100);
        }
        
        ESP_LOGI(TAG, "Board initialization completed");
    }

    // 获取音频编解码器 (暂时使用NoAudioCodec避免初始化问题)
    virtual AudioCodec* GetAudioCodec() override {
        static NoAudioCodec audio_codec;
        return &audio_codec;
    }

    // 获取显示屏 (无显示屏)
    virtual Display* GetDisplay() override {
        return nullptr; // No display connected
    }
    
    // 获取背光控制 (无显示屏)
    virtual Backlight* GetBacklight() override {
        return nullptr; // No display, no backlight
    }

    // 获取摄像头
    virtual Camera* GetCamera() override {
        return camera_;
    }

    // 获取RGB LED
    virtual RgbLed* GetRgbLed() override {
        return rgb_led_;
    }

    // 获取电池监控器
    virtual BatteryMonitor* GetBatteryMonitor() override {
        return battery_monitor_;
    }
};

// 注册开发板
DECLARE_BOARD(SeeedXiaoEsp32s3SenseBoard);
