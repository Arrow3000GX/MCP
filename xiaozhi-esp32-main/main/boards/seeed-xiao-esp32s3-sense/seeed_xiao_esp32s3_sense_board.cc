#include "wifi_board.h"
#include "codecs/no_audio_codec.h"
#include "application.h"
#include "button.h"
#include "config.h"
#include "assets/lang_config.h"

#include <esp_log.h>

#define TAG "SeeedXiaoEsp32s3Sense"

class SeeedXiaoEsp32s3SenseBoard : public WifiBoard {
private:
    Button boot_button_;
    NoAudioCodec audio_codec_;

public:
    // Constructor
    SeeedXiaoEsp32s3SenseBoard() : boot_button_(BOOT_BUTTON_GPIO) {
        ESP_LOGI(TAG, "Initializing Seeed XIAO ESP32S3 Sense Board (Simplified)");
        
        // Initialize buttons
        InitializeButtons();
        
        ESP_LOGI(TAG, "Board initialization complete");
    }

    // Initialize buttons
    void InitializeButtons() {
        ESP_LOGI(TAG, "Initializing buttons");
        
        // Initialize boot button
        if (BOOT_BUTTON_GPIO != GPIO_NUM_NC) {
            boot_button_.Init();
            ESP_LOGI(TAG, "Boot button initialized on GPIO %d", BOOT_BUTTON_GPIO);
        }
    }

    // Override GetBoardType to return our board name
    virtual std::string GetBoardType() override {
        return "seeed-xiao-esp32s3-sense";
    }

    // Get audio codec (no external codec)
    virtual AudioCodec* GetAudioCodec() override {
        return &audio_codec_;
    }

    // Get display (no display)
    virtual Display* GetDisplay() override {
        return nullptr;
    }
    
    // Get backlight (no display)
    virtual Backlight* GetBacklight() override {
        return nullptr;
    }

    // Get camera (no camera for now)
    virtual Camera* GetCamera() override {
        return nullptr;
    }

    // Get RGB LED (no RGB LED for now)
    virtual RgbLed* GetRgbLed() override {
        return nullptr;
    }

    // Get status LED (no status LED for now)
    virtual StatusLed* GetStatusLed() override {
        return nullptr;
    }

    // Get battery monitor (no battery monitor for now)
    virtual BatteryMonitor* GetBatteryMonitor() override {
        return nullptr;
    }

    // Get buttons
    virtual Button* GetBootButton() override {
        return &boot_button_;
    }

    virtual Button* GetVolumeUpButton() override {
        return nullptr;
    }

    virtual Button* GetVolumeDownButton() override {
        return nullptr;
    }

    // Initialize MCP tools (minimal set)
    virtual void InitializeTools() override {
        ESP_LOGI(TAG, "Initializing MCP tools");
        
        // Add basic tools only
        AddTool("speaker", "Speaker control tool");
        AddTool("button", "Button control tool");
        
        ESP_LOGI(TAG, "MCP tools initialized");
    }

private:
    void AddTool(const std::string& name, const std::string& description) {
        ESP_LOGI(TAG, "Adding MCP tool: %s - %s", name.c_str(), description.c_str());
        // Tool registration would go here
    }
};

// CRITICAL: This macro registers the board with the system
DECLARE_BOARD(SeeedXiaoEsp32s3SenseBoard);