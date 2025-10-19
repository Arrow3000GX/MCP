#include "wifi_board.h"
#include "codecs/no_audio_codec.h"
#include "display/display.h"
#include "system_reset.h"
#include "application.h"
#include "button.h"
#include "config.h"
#include "led/single_led.h"
#include "assets/lang_config.h"

#include <wifi_station.h>
#include <esp_log.h>

#define TAG "SeeedXiaoEsp32s3Sense"

class SeeedXiaoEsp32s3SenseBoard : public WifiBoard {
private:
    Button boot_button_;

    void InitializeButtons() {
        ESP_LOGI(TAG, "Initializing buttons");
        
        boot_button_.OnClick([this]() {
            auto& app = Application::GetInstance();
            if (app.GetDeviceState() == kDeviceStateStarting && !WifiStation::GetInstance().IsConnected()) {
                ResetWifiConfiguration();
            }
            app.ToggleChatState();
        });

        // Initialize boot button
        if (BOOT_BUTTON_GPIO != GPIO_NUM_NC) {
            boot_button_.Init();
            ESP_LOGI(TAG, "Boot button initialized on GPIO %d", BOOT_BUTTON_GPIO);
        }
    }

public:
    // Constructor
    SeeedXiaoEsp32s3SenseBoard() : boot_button_(BOOT_BUTTON_GPIO) {
        ESP_LOGI(TAG, "Initializing Seeed XIAO ESP32S3 Sense Board");
        
        // Initialize buttons
        InitializeButtons();
        
        ESP_LOGI(TAG, "Board initialization complete");
    }

    // Override GetBoardType to return our board name
    virtual std::string GetBoardType() override {
        return "seeed-xiao-esp32s3-sense";
    }

    virtual Led* GetLed() override {
        static SingleLed led(BUILTIN_LED_GPIO);
        return &led;
    }

    virtual AudioCodec* GetAudioCodec() override {
        static NoAudioCodecDuplex audio_codec(AUDIO_INPUT_SAMPLE_RATE, AUDIO_OUTPUT_SAMPLE_RATE,
            AUDIO_I2S_GPIO_BCLK, AUDIO_I2S_GPIO_WS, AUDIO_I2S_GPIO_DOUT, AUDIO_I2S_GPIO_DIN);
        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        static NoDisplay display;
        return &display;
    }
};

// CRITICAL: This macro registers the board with the system
DECLARE_BOARD(SeeedXiaoEsp32s3SenseBoard);