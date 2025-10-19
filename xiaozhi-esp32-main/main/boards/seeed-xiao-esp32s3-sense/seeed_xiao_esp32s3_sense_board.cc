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

        // Boot button is initialized in constructor
        if (BOOT_BUTTON_GPIO != GPIO_NUM_NC) {
            ESP_LOGI(TAG, "Boot button initialized on GPIO %d", BOOT_BUTTON_GPIO);
        }
    }

public:
    // Constructor
    SeeedXiaoEsp32s3SenseBoard() : boot_button_(BOOT_BUTTON_GPIO) {
        ESP_LOGI(TAG, "Initializing Seeed XIAO ESP32S3 Sense Board");
        ESP_LOGI(TAG, "Debug: Constructor started");
        
        // Initialize buttons
        InitializeButtons();
        
        ESP_LOGI(TAG, "Debug: Buttons initialized");
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
        ESP_LOGI(TAG, "Debug: GetAudioCodec called");
        static NoAudioCodecDuplex audio_codec(AUDIO_INPUT_SAMPLE_RATE, AUDIO_OUTPUT_SAMPLE_RATE,
            AUDIO_I2S_GPIO_BCLK, AUDIO_I2S_GPIO_WS, AUDIO_I2S_GPIO_DOUT, AUDIO_I2S_GPIO_DIN);
        ESP_LOGI(TAG, "Debug: AudioCodec created and returned");
        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        ESP_LOGI(TAG, "Debug: GetDisplay called");
        static NoDisplay display;
        ESP_LOGI(TAG, "Debug: NoDisplay created and returned");
        return &display;
    }
};

// CRITICAL: This macro registers the board with the system
DECLARE_BOARD(SeeedXiaoEsp32s3SenseBoard);