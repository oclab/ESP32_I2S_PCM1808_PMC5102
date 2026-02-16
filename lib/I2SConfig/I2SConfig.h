#ifndef I2S_CONFIG_H
#define I2S_CONFIG_H

#include "driver/gpio.h"

// I2S Pin Configuration
/* For ESP32 */
#define I2S_MCLK_PIN        GPIO_NUM_0      // I2S master clock io number
#define I2S_BCLK_PIN        GPIO_NUM_2      // I2S bit clock io number
#define I2S_WS_PIN          GPIO_NUM_15     // I2S word select io number
#define I2S_DIN_PIN         GPIO_NUM_16     // I2S data in io number (ADC)

/* For ESP32-S2 (alternative configuration)
#define I2S_MCLK_PIN        GPIO_NUM_7      // I2S master clock io number
#define I2S_BCLK_PIN        GPIO_NUM_2      // I2S bit clock io number
#define I2S_WS_PIN          GPIO_NUM_3      // I2S word select io number
#define I2S_DIN_PIN         GPIO_NUM_5      // I2S data in io number
*/

// Audio Configuration
#define SAMPLE_RATE         48000           // Sample rate in Hz
#define AUDIO_BUFF_SIZE     4096*2          // Audio buffer size

#endif // I2S_CONFIG_H
