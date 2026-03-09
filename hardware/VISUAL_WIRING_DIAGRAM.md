# RF DETECTOR - COMPLETE VISUAL WIRING DIAGRAM
## Alternative to KiCad - Use This for Hand-Wiring!

═══════════════════════════════════════════════════════════════════════════
IF KICAD ISN'T WORKING - USE THIS INSTEAD!
═══════════════════════════════════════════════════════════════════════════

This is a COMPLETE, DETAILED wiring guide you can follow without KiCad.
Perfect for breadboard or perfboard construction!

═══════════════════════════════════════════════════════════════════════════
COMPLETE SYSTEM WIRING DIAGRAM (ASCII ART)
═══════════════════════════════════════════════════════════════════════════

```
                         12V DC INPUT
                              │
                              ├─→ [F1: 3A Polyfuse]
                              │
                              ├─→ [D1: SS34 Diode] ────→ [C1: 1000µF 25V]
                              │
                         ┌────┴────┐
                         │ LM2596  │  Buck Converter (12V → 5V @ 3A)
                         │  Buck   │
                         │Convert  │
                         └────┬────┘
                              │ 5V Output
                              ├─→ [C2: 1000µF 16V]
                              │
              ┌───────────────┼───────────────┐
              │               │               │
              │               │               │
         ┌────▼────┐     ┌────▼──────────┐   │
         │ BL-M88  │     │   ESP32-S3    │   │
         │ 5GHz    │     │   N16R8       │   │
         │ Module  │     │               │   │
         │         │     │  VIN  ◄───────┤   │ 5V Input
         │ VDD ◄───┤     │   │           │   │
         │ 5V @3A! │     │   └─→ 3.3V LDO│   │
         │         │     │        │      │   │
         │ USB_DP  │◄───►│GPIO20  │      │   │
         │ USB_DM  │◄───►│GPIO19  │      │   │
         │         │     │        │      │   │
         │ GND ────┼─────┤GND     │      │   │
         └─────────┘     └────────┼──────┘   │
                                  │ 3.3V     │
                    ┌─────────────┼──────────┼───────────┐
                    │             │          │           │
               ┌────▼────┐   ┌────▼───┐ ┌───▼──┐   ┌────▼────┐
               │Wio-SX   │   │ OLED   │ │ GPS  │   │   SD    │
               │1262     │   │SSD1306 │ │Module│   │  Card   │
               │         │   │        │ │      │   │         │
               │VCC ◄────┤   │VCC ◄───┤ │VCC ◄─┤   │VCC ◄────┤
               │         │   │        │ │      │   │         │
               │NSS  ◄───┼───┤        │ │      │   │         │
               │MOSI ◄───┼───┤        │ │      │   │MOSI ◄───┤
               │MISO ─►──┼───┤        │ │      │   │MISO ─►──┤
               │SCK  ◄───┼───┤        │ │      │   │SCK  ◄───┤
               │DIO1 ◄───┤   │        │ │      │   │CS   ◄───┤
               │NRST ◄───┤   │SCL ◄───┤ │TX ─►─┤   │         │
               │BUSY ─►──┤   │SDA ◄───┤ │RX ◄──┤   │         │
               │         │   │        │ │      │   │         │
               │ANT  ──→ │   │        │ │      │   │         │
               │(868/915)│   │        │ │      │   │         │
               │         │   │        │ │      │   │         │
               │GND ─────┼───┤GND ────┤ │GND ──┤   │GND ─────┤
               └─────────┘   └────────┘ └──────┘   └─────────┘
                                  │
                    ┌─────────────┴──────────┬──────┬──────┐
                    │                        │      │      │
               ┌────▼────┐              ┌────▼┐ ┌──▼──┐ ┌─▼──┐
               │  LED    │              │ BZ1 │ │BTN1 │ │BTN2│
               │  [220Ω] │              │Buzz │ │     │ │    │
               └────┬────┘              └──┬──┘ └──┬──┘ └─┬──┘
                    │                      │       │      │
                    └──────────────────────┴───────┴──────┘
                                          │
                                        [GND]
```

═══════════════════════════════════════════════════════════════════════════
CONNECTION TABLE - COMPLETE PIN-BY-PIN WIRING
═══════════════════════════════════════════════════════════════════════════

ESP32-S3 Connections:
────────────────────────────────────────────────────────────────────────────
Pin         Connect To              Description             Wire Color
────────────────────────────────────────────────────────────────────────────
VIN         LM2596 OUT+ (5V)        Power input             RED
GND         Common GND              Ground                  BLACK
3V3         (See below)             3.3V output to modules  ORANGE

GPIO 10     SX1262 NSS              SPI Chip Select         ORANGE
GPIO 11     SX1262 MOSI + DIO1      SPI Data Out (shared!)  BLUE
GPIO 12     SX1262 SCK + NRST       SPI Clock (shared!)     GRAY
GPIO 13     SX1262 MISO + BUSY      SPI Data In (shared!)   PURPLE
GPIO 5      SD Card CS              SD Chip Select          YELLOW

GPIO 19     BL-M8812EU2 USB_DM      USB Data Negative       WHITE
GPIO 20     BL-M8812EU2 USB_DP      USB Data Positive       GREEN

GPIO 17     OLED SCL                I2C Clock               YELLOW
GPIO 18     OLED SDA                I2C Data                BLUE

GPIO 43     GPS TX (→ ESP RX)       GPS Serial Output       GREEN
GPIO 44     GPS RX (← ESP TX)       GPS Serial Input        WHITE

GPIO 48     LED Anode (+)           Status LED (+ 220Ω)     RED
GPIO 45     Buzzer (+)              Piezo Buzzer            ORANGE
GPIO 0      Button 1                Start/Stop (pullup)     BROWN
GPIO 35     Button 2                Manual DF (pullup)      BROWN

BL-M8812EU2 Module (32x32mm):
────────────────────────────────────────────────────────────────────────────
Pin         Connect To              Description             Wire Color
────────────────────────────────────────────────────────────────────────────
Pin 1       LM2596 OUT+ (5V @ 3A!)  5V Power (DEDICATED!)   RED (thick!)
Pin 2       ESP32 GPIO 20           USB D+                  GREEN
Pin 3       ESP32 GPIO 19           USB D-                  WHITE
Pin 4-18    Common GND              Ground (all pins)       BLACK
J0 (IPEX)   5.8 GHz Antenna #1      Primary antenna         Coax
J1 (IPEX)   5.8 GHz Antenna #2      Diversity antenna       Coax

⚠️ CRITICAL: BL-M8812EU2 draws 2-3A! Use separate 5V supply from LM2596!

Wio-SX1262 Module (25x25mm):
────────────────────────────────────────────────────────────────────────────
Pin         Connect To              Description             Wire Color
────────────────────────────────────────────────────────────────────────────
VCC         ESP32 3.3V              Power (3.3V only!)      ORANGE
GND         Common GND              Ground                  BLACK
NSS         ESP32 GPIO 10           SPI Chip Select         ORANGE
MOSI        ESP32 GPIO 11           SPI Data Out (shared)   BLUE
MISO        ESP32 GPIO 13           SPI Data In (shared)    PURPLE
SCK         ESP32 GPIO 12           SPI Clock (shared)      GRAY
DIO1        ESP32 GPIO 11 (shared!) Interrupt (multiplex)   BLUE
NRST        ESP32 GPIO 12 (shared!) Reset (multiplex)       GRAY
BUSY        ESP32 GPIO 13 (shared!) Busy status (multiplex) PURPLE
ANT         SMA Connector           868/915 MHz antenna     Coax

Note: DIO1, NRST, BUSY share GPIO with SPI - THIS IS INTENTIONAL!

LM2596 Buck Converter Module:
────────────────────────────────────────────────────────────────────────────
Terminal    Connect To              Description             Wire Color
────────────────────────────────────────────────────────────────────────────
IN+         12V DC Input (+)        12V positive            RED (thick)
IN-         12V DC Input (-)        12V negative/GND        BLACK (thick)
OUT+        5V Rail (see below)     5V output @ 3A          RED (thick)
OUT-        Common GND              Ground                  BLACK (thick)

⚠️ Adjust potentiometer to EXACTLY 5.00V before connecting anything!

5V Distribution (from LM2596 OUT+):
────────────────────────────────────────────────────────────────────────────
1. BL-M8812EU2 Pin 1 (5V @ 3A) ──→ Main power consumer
2. ESP32-S3 VIN (5V)           ──→ Powers ESP32 + onboard 3.3V LDO
3. [Optional] Relay module     ──→ If using relays

3.3V Distribution (from ESP32-S3 3V3 pin):
────────────────────────────────────────────────────────────────────────────
1. Wio-SX1262 VCC
2. OLED VCC
3. GPS Module VCC
4. SD Card Module VCC
5. LED (via 220Ω resistor)
6. Buzzer

OLED Display (SSD1306 128x64 I2C):
────────────────────────────────────────────────────────────────────────────
Pin         Connect To              Description             Wire Color
────────────────────────────────────────────────────────────────────────────
VCC         ESP32 3.3V              Power                   ORANGE
GND         Common GND              Ground                  BLACK
SCL         ESP32 GPIO 17           I2C Clock               YELLOW
SDA         ESP32 GPIO 18           I2C Data                BLUE

GPS Module (NEO-6M or similar):
────────────────────────────────────────────────────────────────────────────
Pin         Connect To              Description             Wire Color
────────────────────────────────────────────────────────────────────────────
VCC         ESP32 3.3V (or 5V)      Power (check module!)   ORANGE/RED
GND         Common GND              Ground                  BLACK
TX          ESP32 GPIO 43 (RX)      GPS output → ESP input  GREEN
RX          ESP32 GPIO 44 (TX)      GPS input ← ESP output  WHITE

SD Card Module:
────────────────────────────────────────────────────────────────────────────
Pin         Connect To              Description             Wire Color
────────────────────────────────────────────────────────────────────────────
VCC         ESP32 3.3V              Power                   ORANGE
GND         Common GND              Ground                  BLACK
CS          ESP32 GPIO 5            SPI Chip Select         YELLOW
MOSI        ESP32 GPIO 11           SPI Data Out (shared)   BLUE
MISO        ESP32 GPIO 13           SPI Data In (shared)    PURPLE
SCK         ESP32 GPIO 12           SPI Clock (shared)      GRAY

Other Components:
────────────────────────────────────────────────────────────────────────────
Component       Connections                 Notes
────────────────────────────────────────────────────────────────────────────
LED + R1 220Ω   GPIO 48 → LED+ → R1 → GND   Status indicator
Buzzer          GPIO 45 → BZ+ → BZ- → GND   Piezo buzzer
Button 1        GPIO 0 → SW → GND           Internal pullup enabled
Button 2        GPIO 35 → SW → GND          Internal pullup enabled
C1 1000µF       12V+ → GND                  Input filter cap
C2 1000µF       5V+ → GND                   Output filter cap
C3 100µF        3.3V → GND                  ESP32 LDO cap
D1 SS34         Between 12V source and LM2596  Reverse polarity protection
F1 Polyfuse     In series with 12V+         Overcurrent protection

═══════════════════════════════════════════════════════════════════════════
GROUND (GND) CONNECTIONS - STAR TOPOLOGY
═══════════════════════════════════════════════════════════════════════════

ALL GND connections meet at ONE POINT (near 12V input):

12V Input GND → Star GND Point ← Connect ALL of these here:
                     │
                     ├─ LM2596 OUT-
                     ├─ LM2596 IN-
                     ├─ ESP32-S3 GND
                     ├─ BL-M8812EU2 GND (all pins)
                     ├─ Wio-SX1262 GND
                     ├─ OLED GND
                     ├─ GPS GND
                     ├─ SD Card GND
                     ├─ LED cathode (via resistor)
                     ├─ Buzzer negative
                     ├─ Button terminals
                     ├─ Capacitor negative terminals
                     └─ All other GND connections

Use thick wire (18 AWG) for star GND point!

═══════════════════════════════════════════════════════════════════════════
BREADBOARD LAYOUT (For Testing Before Soldering)
═══════════════════════════════════════════════════════════════════════════

Power Rails:
────────────────────────────────────────────────────────────────────────────
Top Red Rail:    5V from LM2596
Top Blue Rail:   GND (common)
Bottom Red Rail: 3.3V from ESP32
Bottom Blue Rail: GND (common)

Component Placement on Breadboard:
────────────────────────────────────────────────────────────────────────────

[Top]
LM2596 ──→ (Power Supply section, left side)
     ↓
ESP32-S3 ──→ (Center, main controller)
     ↓
Wio-SX1262 ──→ (Right side, RF section)
     ↓
BL-M8812EU2 ──→ (Far right, needs space!)

[Bottom]
OLED ──→ (Top center, I2C)
GPS  ──→ (Bottom center, UART)
SD   ──→ (Bottom right, SPI)
LED + Buttons ──→ (Top left, UI)

Jumper Wire Tips:
────────────────────────────────────────────────────────────────────────────
- Use color coding (see table above!)
- Keep power wires short and thick
- Bundle signal wires together (SPI, I2C)
- Label each wire with masking tape if needed
- Double-check EVERY connection before powering on!

═══════════════════════════════════════════════════════════════════════════
TESTING PROCEDURE (Step-by-Step)
═══════════════════════════════════════════════════════════════════════════

TEST 1: Power Supply Only
────────────────────────────────────────────────────────────────────────────
1. Connect ONLY:
   - 12V input to LM2596
   - C1, C2 capacitors
   - D1 diode, F1 polyfuse

2. Power on 12V supply
3. Measure voltage at LM2596 OUT+:
   ✓ Should read 4.9-5.1V
   ✓ Adjust potentiometer if needed to EXACTLY 5.00V
4. Check current draw: <50mA (no load)
5. Power off

TEST 2: Add ESP32-S3
────────────────────────────────────────────────────────────────────────────
1. Connect ESP32 VIN to 5V rail
2. Connect ESP32 GND to GND rail
3. Power on
4. ESP32 should boot (LED blinks)
5. Measure 3.3V at ESP32 3V3 pin: should be 3.25-3.35V
6. Upload blink sketch via USB-C
7. Power off

TEST 3: Add One Module at a Time
────────────────────────────────────────────────────────────────────────────
Add in this order, testing each:

1. OLED Display
   - Connect I2C (SCL, SDA)
   - Upload OLED test sketch
   - Verify text appears

2. GPS Module
   - Connect UART (TX, RX)
   - Upload GPS serial test
   - Verify NMEA sentences

3. SD Card
   - Connect SPI (shared with SX1262)
   - Upload SD test sketch
   - Write/read test file

4. Wio-SX1262
   - Connect SPI + control pins
   - Upload RadioLib test
   - Verify SPI communication

5. BL-M8812EU2 (LAST - High Power!)
   - Connect USB D+/D- to ESP32
   - Connect 5V @ 3A power (separate!)
   - Upload USB Host test
   - Verify enumeration

TEST 4: Full System
────────────────────────────────────────────────────────────────────────────
1. Upload final RF detector firmware
2. Power on
3. Check serial monitor for init messages:
   ✓ ESP32 boot
   ✓ WiFi AP started
   ✓ SX1262 Init OK
   ✓ BL-M8812EU2 detected
   ✓ OLED display active
   ✓ GPS searching

4. Access web interface: http://192.168.4.1
5. Test RF scanning
6. Done!

═══════════════════════════════════════════════════════════════════════════
TROUBLESHOOTING
═══════════════════════════════════════════════════════════════════════════

Problem: ESP32 won't boot
Solution:
- Check 5V at VIN pin
- Check 3.3V at 3V3 pin
- Remove all peripherals, test ESP32 alone
- Ensure GPIO 0 not pulled LOW during boot

Problem: BL-M8812EU2 not detected
Solution:
- Check 5V power (needs 3A!)
- Verify USB D+/D- connections (GPIO 19/20)
- Module gets HOT - normal, ensure ventilation
- Try connecting to PC via USB to verify module works

Problem: SX1262 init fails
Solution:
- Verify 3.3V power
- Check all SPI connections (MOSI, MISO, SCK, NSS)
- Antenna MUST be connected (module won't init without it!)
- Check that multiplexed pins are correct (DIO1, NRST, BUSY)

Problem: OLED blank
Solution:
- Check I2C address (0x3C or 0x3D)
- Verify SCL/SDA connections (GPIO 17/18)
- Try adding 4.7kΩ pullup resistors

Problem: High current / overheating
Solution:
- BL-M8812EU2 draws 2-3A when active (NORMAL!)
- Ensure LM2596 has adequate heatsink
- Check for shorts between power rails
- Add cooling fan if needed

═══════════════════════════════════════════════════════════════════════════
END OF VISUAL WIRING DIAGRAM
═══════════════════════════════════════════════════════════════════════════

USE THIS GUIDE INSTEAD OF KICAD!

Print this out, highlight connections as you wire them, and you'll have
a fully functional RF detector without ever opening KiCad!

Good luck! 🎯
