#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2s.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"
#include "soc/gpio_periph.h"

#define TAG "AUDIO_TEST"

// I2S pins for MAX98357A
#define I2S_WS_GPIO    6   // LRC (Left/Right Clock)
#define I2S_BCK_GPIO   8   // BCLK (Bit Clock)  
#define I2S_DO_GPIO    5   // DOUT (Data Out)

// Audio settings
#define SAMPLE_RATE    44100
#define BITS_PER_SAMPLE 16
#define BUFFER_SIZE    1024

// Generate sine wave data
void generate_sine_wave(int16_t* buffer, int length, int frequency, int sample_rate) {
    for (int i = 0; i < length; i++) {
        buffer[i] = (int16_t)(32767 * sin(2 * M_PI * frequency * i / sample_rate));
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting audio test...");
    
    // Configure I2S
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2,
        .dma_buf_count = 4,
        .dma_buf_len = BUFFER_SIZE,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCK_GPIO,
        .ws_io_num = I2S_WS_GPIO,
        .data_out_num = I2S_DO_GPIO,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    // Install I2S driver
    esp_err_t ret = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install I2S driver: %s", esp_err_to_name(ret));
        return;
    }

    ret = i2s_set_pin(I2S_NUM_0, &pin_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set I2S pins: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "I2S driver installed successfully");

    // Generate test tone data
    int16_t audio_buffer[BUFFER_SIZE];
    int frequencies[] = {440, 880, 1320}; // A4, A5, E6
    int freq_count = sizeof(frequencies) / sizeof(frequencies[0]);

    ESP_LOGI(TAG, "Starting audio output test...");

    for (int cycle = 0; cycle < 10; cycle++) {
        for (int f = 0; f < freq_count; f++) {
            int frequency = frequencies[f];
            ESP_LOGI(TAG, "Playing tone: %d Hz", frequency);
            
            // Generate sine wave for this frequency
            generate_sine_wave(audio_buffer, BUFFER_SIZE, frequency, SAMPLE_RATE);
            
            // Play the tone for 2 seconds
            for (int i = 0; i < 100; i++) {
                size_t bytes_written = 0;
                ret = i2s_write(I2S_NUM_0, audio_buffer, sizeof(audio_buffer), &bytes_written, portMAX_DELAY);
                if (ret != ESP_OK) {
                    ESP_LOGE(TAG, "I2S write failed: %s", esp_err_to_name(ret));
                    break;
                }
                vTaskDelay(pdMS_TO_TICKS(20)); // ~20ms delay
            }
            
            // Brief pause between tones
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        
        ESP_LOGI(TAG, "Cycle %d complete", cycle + 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    ESP_LOGI(TAG, "Audio test complete");
    
    // Uninstall I2S driver
    i2s_driver_uninstall(I2S_NUM_0);
    ESP_LOGI(TAG, "I2S driver uninstalled");
}
