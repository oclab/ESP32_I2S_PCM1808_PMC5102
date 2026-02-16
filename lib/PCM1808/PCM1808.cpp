#include "PCM1808.h"

PCM1808::PCM1808() : rx_handle(nullptr), initialized(false) {
}

PCM1808::~PCM1808() {
    if (rx_handle != nullptr) {
        i2s_channel_disable(rx_handle);
        i2s_del_channel(rx_handle);
    }
}

bool PCM1808::begin() {
    if (initialized) {
        Serial.println("PCM1808: Already initialized");
        return true;
    }
    
    if (!setupI2S()) {
        Serial.println("PCM1808: Failed to setup I2S");
        return false;
    }
    
    initialized = true;
    Serial.println("PCM1808: Initialization successful");
    return true;
}

bool PCM1808::setupI2S() {
    // Allocate I2S RX channel only
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    
    esp_err_t ret = i2s_new_channel(&chan_cfg, nullptr, &rx_handle);
    if (ret != ESP_OK) {
        Serial.printf("PCM1808: Failed to create I2S channel: %x\n", ret);
        return false;
    }
    Serial.printf("PCM1808: I2S Channel created: %x\n", ret);
    
    // Configure I2S in standard mode for RX
    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_MCLK_PIN,
            .bclk = I2S_BCLK_PIN,
            .ws = I2S_WS_PIN,
            .dout = I2S_GPIO_UNUSED,  // No output for ADC-only mode
            .din = I2S_DIN_PIN,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };
    
    // Set MCLK multiple
    std_cfg.clk_cfg.mclk_multiple = I2S_MCLK_MULTIPLE_256;
    
    ret = i2s_channel_init_std_mode(rx_handle, &std_cfg);
    if (ret != ESP_OK) {
        Serial.printf("PCM1808: Failed to initialize I2S RX: %x\n", ret);
        return false;
    }
    Serial.printf("PCM1808: I2S RX initialized: %x\n", ret);
    
    return true;
}

bool PCM1808::enable() {
    if (!initialized) {
        Serial.println("PCM1808: Not initialized, call begin() first");
        return false;
    }
    
    esp_err_t ret = i2s_channel_enable(rx_handle);
    if (ret != ESP_OK) {
        Serial.printf("PCM1808: Failed to enable I2S RX: %x\n", ret);
        return false;
    }
    
    Serial.printf("PCM1808: I2S RX enabled: %x\n", ret);
    return true;
}

bool PCM1808::disable() {
    if (!initialized) {
        return false;
    }
    
    esp_err_t ret = i2s_channel_disable(rx_handle);
    return (ret == ESP_OK);
}

size_t PCM1808::read(uint8_t* buffer, size_t bufferSize, uint32_t timeout_ms) {
    if (!initialized) {
        Serial.println("PCM1808: Not initialized");
        return 0;
    }
    
    size_t bytes_read = 0;
    esp_err_t ret = i2s_channel_read(rx_handle, buffer, bufferSize, &bytes_read, timeout_ms);
    
    if (ret != ESP_OK) {
        Serial.printf("PCM1808: Read failed with code: %d\n", ret);
        return 0;
    }
    
    return bytes_read;
}
