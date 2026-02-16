#include <Arduino.h>
#include "PCM1808.h"
#include "I2SConfig.h"

// Create PCM1808 ADC instance
PCM1808 adc;

// Task function declaration
static void pcm1808_read_task(void* args);

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  Serial.println("\n=== PCM1808 ADC Only Demo ===");

  delay(1000);

  // Initialize PCM1808 ADC
  if (!adc.begin()) {
    Serial.println("Failed to initialize PCM1808!");
    while (1) {
      delay(1000);
    }
  }

  // Enable the ADC
  if (!adc.enable()) {
    Serial.println("Failed to enable PCM1808!");
    while (1) {
      delay(1000);
    }
  }

  delay(500);

  // Start audio read task
  xTaskCreate(pcm1808_read_task, "pcm1808_read_task", 4096, NULL, 5, NULL);
  
  Serial.println("PCM1808 ADC initialized and ready!");
}

void loop() {
  delay(1000);
}

// PCM1808 ADC read task
static void pcm1808_read_task(void* args)
{
  uint8_t* buffer = (uint8_t*)calloc(1, AUDIO_BUFF_SIZE);
  if (!buffer) {
    Serial.println("Failed to allocate audio buffer!");
    vTaskDelete(NULL);
    return;
  }

  Serial.println("PCM1808 read task started");

  while (1) {
    // Read audio data from PCM1808
    size_t bytes_read = adc.read(buffer, AUDIO_BUFF_SIZE, 1000);
    
    if (bytes_read > 0) {
      // Use helper method to extract first sample
      int32_t sample = PCM1808::extractSample(buffer, 0);
      Serial.printf("Sample: %d, Bytes read: %d\n", sample, bytes_read);
    }
    
    // Delay to avoid flooding serial output
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  free(buffer);
  vTaskDelete(NULL);
}