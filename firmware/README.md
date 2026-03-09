# Firmware

This folder contains the main firmware for the RF Detector.

## Main Files

- **rf_detector_ESP32S3.ino** - Main firmware (750 lines, ready to upload)

## Arduino IDE Setup

**Board Settings:**
```
Board: ESP32S3 Dev Module
Flash Size: 16MB (128Mb)
PSRAM: OPI PSRAM
Partition Scheme: 16M Flash (3MB APP/9.9MB FATFS)
USB CDC On Boot: Enabled
CPU Frequency: 240MHz (WiFi)
Upload Speed: 115200
```

## Required Libraries

Install via Arduino Library Manager:
- RadioLib (for SX1262)
- Adafruit GFX Library
- Adafruit SSD1306
- TinyGPSPlus
- SD (built-in)

## Upload Instructions

1. Connect ESP32-S3 via USB-C
2. Open `rf_detector_ESP32S3.ino`
3. Select correct board settings (above)
4. Click Upload
5. Wait for completion
6. Open Serial Monitor (115200 baud)

## First Boot

After upload, the device will:
1. Start WiFi AP: `RF_Detector_ULTIMATE`
2. Password: `rf123456`
3. IP address: `192.168.4.1`
4. Initialize all modules
5. Display status on OLED

## Troubleshooting

**SX1262 Init Failed:**
- Check antenna is connected
- Verify SPI connections
- Check 3.3V power

**BL-M8812EU2 Not Detected:**
- Needs 5V @ 3A power
- Check USB D+/D- connections (GPIO 19/20)

**GPS No Fix:**
- Needs clear sky view
- Takes 30-60 seconds for first fix
- Check UART connections (GPIO 43/44)
