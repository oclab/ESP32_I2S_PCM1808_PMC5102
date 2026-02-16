#ifndef PCM1808_H
#define PCM1808_H

#include <Arduino.h>
#include "driver/i2s_std.h"
#include "I2SConfig.h"

class PCM1808 {
public:
    // Constructor
    PCM1808();
    
    // Destructor
    ~PCM1808();
    
    // Initialize the PCM1808 ADC
    bool begin();
    
    // Read audio data from PCM1808
    // Returns number of bytes read, 0 on failure
    size_t read(uint8_t* buffer, size_t bufferSize, uint32_t timeout_ms = 1000);
    
    // Enable/disable the I2S channel
    bool enable();
    bool disable();
    
    // Check if initialized
    bool isInitialized() const { return initialized; }
    
private:
    i2s_chan_handle_t rx_handle;
    bool initialized;
    
    // Setup I2S in RX-only mode
    bool setupI2S();
};

#endif // PCM1808_H
