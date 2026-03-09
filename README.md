# Ultimate RF Detector - ESP32-S3 Edition

**Compact, multi-frequency RF detector for drone detection and spectrum monitoring**

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Status](https://img.shields.io/badge/status-ready--to--build-green.svg)

## 🎯 Overview

A credit-card sized RF detector capable of detecting drones, WiFi, and other RF signals from **150 MHz to 5.8 GHz**. Features GPS logging, web interface, and OLED display.

**Detection Range:** Up to 20 km (line of sight)  
**Size:** 40x40x35mm (stacked design)  
**Cost:** ~$180 complete build

## ✨ Features

- **Multi-frequency detection:** 150 MHz - 5.8 GHz coverage
- **Three RF modules:** 
  - BL-M8812EU2 (5/5.8 GHz WiFi)
  - Wio-SX1262 (150-960 MHz LoRa/sub-GHz)
  - ESP32-S3 built-in WiFi (2.4 GHz)
- **GPS tracking:** Location logging for detections
- **Web interface:** Control via WiFi (192.168.4.1)
- **OLED display:** Real-time status
- **SD card logging:** Save all detections
- **Compact design:** 4-layer stacked PCB architecture

## 📦 Hardware

**Main Components:**
- ESP32-S3 N16R8 (16MB Flash, 8MB PSRAM)
- BL-M8812EU2 5GHz WiFi module (800mW, RTL8812EU)
- Wio-SX1262 LoRa module (-148 dBm sensitivity)
- NEO-6M GPS module
- SSD1306 OLED 128x64 display
- MicroSD card module
- LM2596 buck converter (12V → 5V @ 3A)

**Power:**
- Input: 12V DC @ 3A
- Battery option: 3S LiPo (11.1V, 4-5 hour runtime)

## 🛠️ Build Options

### Option 1: Custom PCB (Professional)
**Cost:** $180 | **Time:** 3 weeks | **Difficulty:** Intermediate

1. Hire PCB designer using `UPWORK_JOB_POSTING.md` ($75)
2. Order PCBs from JLCPCB ($25 for 5 sets)
3. Order components from `SHOPPING_LIST.md` ($80)
4. Assemble following `Stacked_Version/ASSEMBLY_GUIDE_STACKED.md`

### Option 2: Perfboard Build (Budget)
**Cost:** $85 | **Time:** 1 weekend | **Difficulty:** Beginner

1. Order components from `SHOPPING_LIST.md` ($80)
2. Get 40x40mm perfboard ($5)
3. Follow `VISUAL_WIRING_DIAGRAM.md` pin-by-pin
4. Upload firmware and test

### Option 3: Breadboard Prototype (Fastest)
**Cost:** $60 | **Time:** 2 days | **Difficulty:** Beginner

1. Order pre-made modules (Amazon Prime)
2. Wire on breadboard using wiring diagram
3. Test before committing to PCB

## 📂 Repository Structure

```
├── firmware/
│   └── rf_detector_ESP32S3.ino          # Main firmware (ready to upload)
│
├── hardware/
│   ├── VISUAL_WIRING_DIAGRAM.md         # Complete pin-by-pin wiring
│   ├── BOM_STACKED_COMPLETE.csv         # Full parts list with prices
│   ├── SHOPPING_LIST.md                 # Where to buy components
│   └── Board_0_Structured_Data.txt      # PCB specifications
│
├── stacked-design/
│   ├── STACKED_DESIGN.md                # Complete architecture
│   ├── ASSEMBLY_GUIDE_STACKED.md        # Step-by-step build guide
│   ├── 3D_Case/
│   │   ├── Case_Top_Stacked_FIXED.scad  # 3D printable top shell
│   │   └── Case_Bottom_Stacked.scad     # 3D printable bottom shell
│   └── Gerbers/                         # PCB specifications
│
├── docs/
│   ├── UPWORK_JOB_POSTING.md           # Hire PCB designer guide
│   ├── POWER_SUPPLY_SCHEMATIC.md       # Power system design
│   └── PCB_LAYOUT_GUIDE.md             # PCB design documentation
│
└── README.md                            # This file
```

## 🚀 Quick Start

### 1. Choose Your Build Path
- **Fastest:** Breadboard with pre-made modules (2 days)
- **Cheapest:** Perfboard build ($85, 1 weekend)
- **Professional:** Custom stacked PCBs ($180, 3 weeks)

### 2. Order Components
See `SHOPPING_LIST.md` for complete sourcing guide:
- Amazon: Fast shipping, higher prices
- AliExpress: Cheap, 2-4 week shipping
- JLCPCB: PCB manufacturing

### 3. Build Hardware
Follow one of these guides:
- `VISUAL_WIRING_DIAGRAM.md` - Breadboard/perfboard
- `Stacked_Version/ASSEMBLY_GUIDE_STACKED.md` - Custom PCB

### 4. Upload Firmware
```bash
# Arduino IDE Settings:
Board: ESP32S3 Dev Module
Flash: 16MB (128Mb)
PSRAM: OPI PSRAM
Partition: 16M Flash (3MB APP/9.9MB FATFS)
Upload Speed: 115200

# Upload:
1. Open firmware/rf_detector_ESP32S3.ino
2. Connect ESP32 via USB-C
3. Click Upload
```

### 5. Test & Use
1. Power on detector
2. Connect to WiFi: `RF_Detector_ULTIMATE` (password: `rf123456`)
3. Open browser: `http://192.168.4.1`
4. Start scanning!

## 📊 Pin Assignments

**ESP32-S3 GPIO Map:**
```
Power:
- VIN: 5V from LM2596
- 3V3: 3.3V to peripherals

RF Modules:
- GPIO 10: SX1262 NSS
- GPIO 11: SPI MOSI / SX1262 DIO1 (shared)
- GPIO 12: SPI SCK / SX1262 NRST (shared)
- GPIO 13: SPI MISO / SX1262 BUSY (shared)
- GPIO 19-20: USB D-/D+ to BL-M8812EU2

Peripherals:
- GPIO 17-18: I2C (OLED)
- GPIO 43-44: UART (GPS)
- GPIO 5: SD Card CS
- GPIO 48: Status LED
- GPIO 45: Buzzer
- GPIO 0, 35: Buttons
```

See `VISUAL_WIRING_DIAGRAM.md` for complete connection details.

## 💰 Cost Breakdown

| Component | Cost | Source |
|-----------|------|--------|
| ESP32-S3 N16R8 | $10 | Amazon/AliExpress |
| BL-M8812EU2 | $12.50 | AliExpress/Taobao |
| Wio-SX1262 | $17.50 | Seeed Studio |
| GPS + OLED + SD | $16.50 | AliExpress |
| Power supply components | $12 | AliExpress |
| Passives + connectors | $10 | AliExpress |
| **Components Total** | **$80** | |
| PCB design (Upwork) | $75 | Optional |
| PCB manufacturing | $25 | JLCPCB |
| 3D printed case | $2 | DIY print |
| **Complete Build** | **$180** | |

**Budget option:** $85 (perfboard build, no custom PCB)

## 🔧 Technical Specs

**Frequency Coverage:**
- 150-960 MHz: Wio-SX1262 (LoRa/sub-GHz)
- 2.4 GHz: ESP32-S3 WiFi (built-in)
- 5.0-5.8 GHz: BL-M8812EU2 (high power)

**Sensitivity:**
- Sub-GHz: -148 dBm (Wio-SX1262)
- 2.4 GHz: -95 dBm (ESP32)
- 5.8 GHz: -85 dBm (BL-M8812EU2)

**Detection Range (Line of Sight):**
- 2.4 GHz WiFi: 1-5 km
- 5.8 GHz: 5-15 km
- Sub-GHz: 10-20 km

**Power Consumption:**
- Idle: ~500 mA @ 5V
- Active scanning: 2-3 A @ 5V (BL-M8812EU2 draws heavily!)
- Battery life: 4-5 hours with 5000mAh 3S LiPo

**Physical:**
- PCB stack: 40x40mm (4 boards)
- Case: 45x45x40mm
- Weight: ~85g complete

## 📖 Documentation

- **[VISUAL_WIRING_DIAGRAM.md](VISUAL_WIRING_DIAGRAM.md)** - Complete wiring guide
- **[STACKED_DESIGN.md](Stacked_Version/STACKED_DESIGN.md)** - Architecture overview
- **[ASSEMBLY_GUIDE_STACKED.md](Stacked_Version/ASSEMBLY_GUIDE_STACKED.md)** - Build instructions
- **[SHOPPING_LIST.md](Stacked_Version/SHOPPING_LIST.md)** - Component sourcing
- **[UPWORK_JOB_POSTING.md](UPWORK_JOB_POSTING.md)** - Hire PCB designer

## ⚠️ Important Notes

**Power Requirements:**
- BL-M8812EU2 requires dedicated 5V @ 3A supply
- Do NOT power from ESP32 5V pin
- LM2596 must be rated for 3A continuous

**Pin Multiplexing:**
- GPIO 11, 12, 13 share SPI bus with SX1262 control signals
- This is intentional and handled in firmware
- Do not modify pin assignments

**Legal Notice:**
- Use responsibly and legally
- Check local regulations for RF detection devices
- Do not interfere with licensed spectrum
- Intended for hobbyist/educational use

## 🤝 Contributing

Contributions welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Submit pull request with description

Areas for improvement:
- Additional RF module support
- Enhanced web interface
- Direction finding algorithms
- Power optimization
- Case design variations

## 📝 License

MIT License - see LICENSE file for details

## 🙏 Acknowledgments

- ESP32 community for excellent libraries
- RadioLib for SX1262 support
- Seeed Studio for Wio-SX1262 module
- JLCPCB for affordable PCB manufacturing

## 📧 Support

- **Issues:** Use GitHub Issues for bug reports
- **Discussions:** Use GitHub Discussions for questions
- **Wiki:** Check wiki for additional guides

## 🔗 Links

- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [RadioLib Documentation](https://github.com/jgromes/RadioLib)
- [JLCPCB PCB Service](https://jlcpcb.com)
- [Seeed Wio-SX1262](https://www.seeedstudio.com/Wio-SX1262-LoRa-E5-p-5466.html)

---

**Built with ❤️ for the RF detection community**

**Version:** 2.0 (Stacked Edition)  
**Last Updated:** March 2026
