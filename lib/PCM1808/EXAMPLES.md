# PCM1808 Module Examples

This document provides examples of how to use the PCM1808 library module.

## Basic Usage

### Simple Audio Capture

```cpp
#include <Arduino.h>
#include "PCM1808.h"
#include "I2SConfig.h"

PCM1808 adc;

void setup() {
    Serial.begin(115200);
    
    // Initialize and enable ADC
    if (adc.begin() && adc.enable()) {
        Serial.println("PCM1808 ready!");
    }
}

void loop() {
    uint8_t buffer[AUDIO_BUFF_SIZE];
    size_t bytes_read = adc.read(buffer, AUDIO_BUFF_SIZE);
    
    if (bytes_read > 0) {
        // Process audio data here
        Serial.printf("Read %d bytes\n", bytes_read);
    }
    
    delay(10);
}
```

### Audio Processing with FreeRTOS Task

```cpp
#include <Arduino.h>
#include "PCM1808.h"
#include "I2SConfig.h"

PCM1808 adc;

void audioProcessingTask(void* parameter) {
    uint8_t* buffer = (uint8_t*)malloc(AUDIO_BUFF_SIZE);
    
    while (1) {
        size_t bytes_read = adc.read(buffer, AUDIO_BUFF_SIZE);
        
        if (bytes_read > 0) {
            // Apply your audio processing here
            // Example: Calculate RMS, apply filters, etc.
        }
        
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    
    free(buffer);
    vTaskDelete(NULL);
}

void setup() {
    Serial.begin(115200);
    
    adc.begin();
    adc.enable();
    
    xTaskCreate(audioProcessingTask, "audio_task", 4096, NULL, 5, NULL);
}

void loop() {
    delay(1000);
}
```

### Sample Value Extraction

```cpp
// Extract a single sample from buffer using helper method
int32_t sample = PCM1808::extractSample(buffer, 0);  // First sample
int32_t sample2 = PCM1808::extractSample(buffer, 4); // Second sample

// Extract left and right channel samples from stereo buffer
int32_t leftSample, rightSample;
PCM1808::extractStereoSamples(buffer, leftSample, rightSample);

// Manual extraction (if needed)
// PCM1808 outputs 24-bit samples in 32-bit frames
// Format: [L3 L2 L1 L0] [R3 R2 R1 R0]
int32_t manualSample = (buffer[0] << 24) | (buffer[1] << 16) | 
                       (buffer[2] << 8) | buffer[3];
```

## Advanced Usage

### Custom Configuration

You can modify the I2S configuration by editing `lib/I2SConfig/I2SConfig.h`:

```cpp
// Change sample rate
#define SAMPLE_RATE         44100  // or 96000, etc.

// Change buffer size (default is 8192)
#define AUDIO_BUFF_SIZE     4096

// Change pin assignments (ESP32-S2)
#define I2S_MCLK_PIN        GPIO_NUM_7
#define I2S_BCLK_PIN        GPIO_NUM_2
#define I2S_WS_PIN          GPIO_NUM_3
#define I2S_DIN_PIN         GPIO_NUM_5
```

### Resource Management

The PCM1808 class properly manages resources:

```cpp
void setup() {
    PCM1808* adc = new PCM1808();
    
    adc->begin();
    adc->enable();
    
    // Use the ADC...
    
    // Cleanup is automatic when object is destroyed
    delete adc;  // Calls destructor, frees I2S resources
}
```

## Tips and Best Practices

1. **Buffer Size**: Larger buffers reduce overhead but increase latency
2. **Task Priority**: Audio tasks should have higher priority than UI tasks
3. **Stack Size**: FreeRTOS tasks need sufficient stack (4096 bytes recommended)
4. **Delay Management**: Use `vTaskDelay()` in tasks instead of `delay()`
5. **Error Handling**: Always check return values from `begin()` and `enable()`

## Troubleshooting

### No Data Received
- Check physical connections (MCLK, BCLK, LRCK, DOUT)
- Verify PCM1808 power supply (3.3V or 5V depending on module)
- Check if PCM1808 MCLK is receiving clock signal

### Noisy Data
- Add decoupling capacitors near PCM1808
- Use shielded cables for analog input
- Ensure proper grounding

### Build Errors
- Make sure all files are in correct directories
- Verify `platformio.ini` is configured correctly
- Check that ESP-IDF version is compatible
