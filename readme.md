# ESP32 with PCM1808 ADC

## Overview

This is a simplified, modular PlatformIO project for capturing audio data from the PCM1808 ADC using ESP32's I2S peripheral. The project demonstrates a clean, object-oriented approach to interfacing with the PCM1808 ADC.

## Features

- **Modular Design**: Organized library structure following PlatformIO best practices
- **PCM1808 Support**: Dedicated class for PCM1808 ADC operation
- **I2S Configuration**: Separate configuration module for easy customization
- **ADC-Only Mode**: Simplified implementation focused on audio input

## Project Structure

```
├── lib/
│   ├── PCM1808/          # PCM1808 ADC driver module
│   │   ├── PCM1808.h
│   │   └── PCM1808.cpp
│   └── I2SConfig/        # I2S pin and audio configuration
│       └── I2SConfig.h
├── src/
│   └── main.cpp          # Main application code
└── platformio.ini        # PlatformIO configuration
```

## Hardware Configuration

### Pinout (ESP32)

| Function | ESP32 Pin | PCM1808 Pin |
| -------- | ---------:| -----------:|
| SCK/MCLK | GPIO 0    | SCK         |
| BCLK     | GPIO 2    | BCK         |
| WS/LRCK  | GPIO 15   | LRCK        |
| DIN      | GPIO 16   | DOUT        |

### Pinout (ESP32-S2 Alternative)

Uncomment the ESP32-S2 configuration in `lib/I2SConfig/I2SConfig.h` to use:

| Function | ESP32-S2 Pin | PCM1808 Pin |
| -------- | ------------:| -----------:|
| SCK/MCLK | GPIO 7       | SCK         |
| BCLK     | GPIO 2       | BCK         |
| WS/LRCK  | GPIO 3       | LRCK        |
| DIN      | GPIO 5       | DOUT        |

## Audio Configuration

- **Sample Rate**: 48 kHz
- **Bit Depth**: 32-bit (24-bit audio data in 32-bit frames)
- **Mode**: Stereo
- **Buffer Size**: 8192 bytes

## Building and Uploading

### Using PlatformIO

1. Install PlatformIO IDE or CLI
2. Connect your ESP32 board
3. Build and upload:
   ```bash
   pio run --target upload
   ```
4. Monitor serial output:
   ```bash
   pio device monitor
   ```

### Select Your Board

The project supports multiple boards. Edit `platformio.ini` or specify the environment:

- ESP32: `pio run -e geekworm-esp32`
- ESP32-S2 Mini: `pio run -e lolin_s2_mini`

## Usage

The main application demonstrates basic ADC functionality:

1. Initializes the PCM1808 ADC
2. Enables the I2S receive channel
3. Continuously reads audio samples
4. Prints sample values to serial monitor

## Extending the Project

The modular structure makes it easy to extend functionality:

- Add audio processing in `pcm1808_read_task()`
- Create additional modules in the `lib/` directory
- Modify I2S parameters in `I2SConfig.h`
- Add DSP, filters, or data streaming capabilities

## License

Open source - feel free to use and modify for your projects.