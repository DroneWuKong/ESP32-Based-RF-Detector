# Hardware

Complete hardware specifications and wiring diagrams.

## Files

- **VISUAL_WIRING_DIAGRAM.md** - Complete pin-by-pin wiring guide (⭐ START HERE)
- **BOM.csv** - Bill of Materials with prices (~$109/unit)
- **SHOPPING_LIST.md** - Where to buy components
- **Board_0_Structured_Data.txt** - PCB specifications for designer

## Quick Start

**For Breadboard/Perfboard Build:**
1. Open `VISUAL_WIRING_DIAGRAM.md`
2. Follow the ASCII art diagram
3. Wire components pin-by-pin
4. Use color-coded wire recommendations

**For Custom PCB:**
1. Use `Board_0_Structured_Data.txt` 
2. Hire designer (see `/docs/UPWORK_JOB_POSTING.md`)
3. Get Gerber files
4. Order from JLCPCB

## Component Summary

**Main ICs:**
- ESP32-S3 N16R8 ($10)
- BL-M8812EU2 WiFi module ($12.50)
- Wio-SX1262 LoRa module ($17.50)
- LM2596 buck converter ($2.50)

**Total Cost:** ~$109 per complete detector

See `BOM.csv` for full parts list.

## Power Requirements

- Input: 12V DC @ 3A
- LM2596 output: 5V @ 3A
- ESP32 onboard LDO: 3.3V for peripherals
- BL-M8812EU2: Needs dedicated 5V @ 3A (CRITICAL!)

## Pin Assignments

See `VISUAL_WIRING_DIAGRAM.md` for complete GPIO mapping.

**Key Connections:**
- GPIO 10-13: SPI bus + SX1262 control
- GPIO 19-20: USB to BL-M8812EU2
- GPIO 17-18: I2C to OLED
- GPIO 43-44: UART to GPS
- GPIO 5: SD Card CS

## Wiring Tips

1. Wire power first, test before adding modules
2. Use color-coded wires (see diagram)
3. Test each module individually
4. Keep USB differential pair (GPIO 19/20) twisted together
5. Star ground topology recommended
