# RF DETECTOR - POWER SUPPLY SCHEMATIC
## 12V Input → Dual Rail (5V @ 3A + 3.3V via ESP32 LDO)

═══════════════════════════════════════════════════════════════════════════
POWER ARCHITECTURE OVERVIEW
═══════════════════════════════════════════════════════════════════════════

Input: 12V DC (9-16V acceptable range)
Primary Rail: 5V @ 3A (LM2596 buck converter)
Secondary Rail: 3.3V @ 500mA (ESP32-S3 onboard LDO)

Power Distribution Tree:
────────────────────────────────────────────────────────────────────────────

                    12V Input (Battery/Wall Adapter)
                         │
                         │ Reverse Polarity Protection
                         ▼
                    ┌─────────┐
                    │ D1: SS34 │  Schottky Diode (3A, 40V)
                    └─────────┘
                         │
                    ┌────┴────┐
                    │   C1    │  1000µF 25V Electrolytic (input filter)
                    └────┬────┘
                         │
              ┌──────────┴──────────┐
              │                     │
              │  LM2596 Buck        │  Switching Regulator Module
              │  Converter          │
              │  Vin: 12V           │
              │  Vout: 5.0V (adj)   │
              │  Iout: 3A max       │
              │                     │
              └──────────┬──────────┘
                         │ 5V Rail
                    ┌────┴────┐
                    │   C2    │  1000µF 16V Electrolytic (output filter)
                    └────┬────┘
                         │
              ┌──────────┼──────────────────┐
              │          │                  │
              │          │                  │
         ┌────▼────┐  ┌──▼──────────┐  ┌───▼──────┐
         │ BL-M88  │  │  ESP32-S3   │  │ Optional │
         │ 5GHz    │  │  Main MCU   │  │  Relays  │
         │ Module  │  │             │  │  5V      │
         │         │  │  Onboard    │  └──────────┘
         │ 5V @    │  │  3.3V LDO   │
         │ 2.5A    │  │     │       │
         └─────────┘  └─────┼───────┘
                            │ 3.3V Rail
                       ┌────┴────┐
                       │   C3    │  100µF 6.3V Tantalum (3.3V filter)
                       └────┬────┘
                            │
              ┌─────────────┼─────────────┬─────────┬────────┐
              │             │             │         │        │
         ┌────▼────┐   ┌────▼────┐  ┌────▼───┐ ┌──▼──┐ ┌───▼──┐
         │ SX1262  │   │  OLED   │  │  GPS   │ │ SD  │ │ LED/ │
         │ 30mA    │   │  20mA   │  │  50mA  │ │100mA│ │ Buzz │
         └─────────┘   └─────────┘  └────────┘ └─────┘ └──────┘

═══════════════════════════════════════════════════════════════════════════
COMPLETE POWER SUPPLY CIRCUIT SCHEMATIC
═══════════════════════════════════════════════════════════════════════════

                +12V IN (Battery/Wall Adapter)
                  │
                  │  (Red Wire - Positive)
                  │
             ┌────┴────┐
             │    F1   │   Polyfuse 3A (Resettable Fuse)
             │   3A    │
             └────┬────┘
                  │
             ┌────▼────┐
      ┌──────│   D1    │──────┐  SS34 Schottky Diode
      │      │  SS34   │      │  (Reverse Polarity Protection)
      │      │  3A 40V │      │  Forward Voltage: 0.5V
      │      └─────────┘      │  Anode ← Cathode →
      │                       │
   ───┴───                 ───┴───
    ═══ C1                  ═══ C4      Bulk Capacitors
   1000µF                  100nF       Input Filtering
    25V                    50V         (Ceramic + Electrolytic)
   ───┬───                 ───┬───
      │                       │
      └───────────┬───────────┘
                  │
                  │ 12V (Filtered)
                  │
         ┌────────┴────────┐
         │                 │
         │   LM2596 BUCK   │
         │   CONVERTER     │
         │   (Module)      │
         │                 │
         │   IN+ ●─────────┤ ← 12V Input
         │   IN- ●─────────┤ ← GND
         │  OUT+ ●─────────┤ → 5V Output (adjustable via pot)
         │  OUT- ●─────────┤ → GND
         │                 │
         │  Efficiency:    │
         │  ~92% @ 3A      │
         │  Fsw: 150 kHz   │
         │                 │
         └────────┬────────┘
                  │
                  │ 5V Rail (3A capable)
                  │
             ┌────▼────┐
      ┌──────│   C2    │──────┐  1000µF 16V Electrolytic
      │      │ 1000µF  │      │  (5V Rail Filtering)
      │      │   16V   │      │
      │      └─────────┘      │
      │                       │
   ───┴───                 ───┴───
    ═══                     ═══ C5    Ceramic 100nF 50V
   1000µF                  100nF     (High frequency filtering)
    16V                    50V
   ───┬───                 ───┬───
      │                       │
      └───────────┬───────────┘
                  │
         ┌────────┼────────┬─────────┐
         │        │        │         │
         │        │        │         │
    5V to BL-  5V to   5V to    5V to
    M8812EU2   ESP32   Relays  Spare
    Module     VIN     (Opt)   Header
         │        │        │         │
         │        │        │         │
         ▼        ▼        ▼         ▼
    ┌────────┐ ┌──────────────┐  ┌──────┐
    │BL-M88  │ │  ESP32-S3    │  │Relay │
    │5GHz    │ │              │  │Module│
    │Module  │ │  VIN    3V3  │  │ 5V   │
    │        │ │   │      │   │  └──────┘
    │5V @2.5A│ │   │   AMS1117│
    │        │ │   │   3.3V LDO
    │USB D+  │ │   │      │   │
    │USB D-  │ │   ▼      ▼   │
    └───┬┬───┘ │  ┌────────┐  │
        ││     │  │  C3    │  │
        ││     │  │ 100µF  │  │  3.3V Rail Filter
        ││     │  │ 6.3V   │  │  (Tantalum or
        ││     │  │Tantalum│  │   Low-ESR Electrolytic)
        ││     │  └───┬────┘  │
        ││     │      │ 3.3V  │
        ││     │      │       │
        ││     └──────┼───────┘
        ││            │
        ││      ┌─────┼──────┬──────┬──────┬──────┐
        ││      │     │      │      │      │      │
        ││   ┌──▼─┐ ┌─▼──┐ ┌─▼──┐ ┌─▼─┐ ┌──▼──┐ ┌▼──┐
        ││   │SX  │ │OLED│ │GPS │ │SD │ │LED  │ │Buz│
        ││   │1262│ │    │ │    │ │   │ │220Ω │ │   │
        ││   └────┘ └────┘ └────┘ └───┘ └─────┘ └───┘
        ││      │     │      │      │      │      │
        ││      └─────┴──────┴──────┴──────┴──────┘
        ││            │
        ││            │ GND (Common Ground)
        ││            │
        │└────────────┼──── USB D+
        └─────────────┼──── USB D-
                      │
                ┌─────▼─────┐
                │           │
                │  COMMON   │
                │  GROUND   │
                │  (Star    │
                │  Topology)│
                │           │
                └───────────┘
                      │
                      │ (Black Wire)
                      │
                 GND Return


═══════════════════════════════════════════════════════════════════════════
COMPONENT SPECIFICATIONS & PART NUMBERS
═══════════════════════════════════════════════════════════════════════════

Critical Components:
────────────────────────────────────────────────────────────────────────────

F1: Polyfuse (Resettable Fuse)
   Part Number: Bourns MF-R300
   Rating: 3A hold, 6A trip
   Reset Time: <60 seconds @ 25°C
   Purpose: Overcurrent protection
   Cost: $0.50

D1: Schottky Diode (Reverse Polarity Protection)
   Part Number: SS34 or 1N5822
   Voltage: 40V
   Current: 3A continuous
   Forward Voltage: 0.5V @ 3A
   Purpose: Prevents damage if 12V polarity is reversed
   Cost: $0.20

C1: Input Bulk Capacitor
   Value: 1000µF
   Voltage: 25V (rated for 12V input with margin)
   Type: Electrolytic (Low-ESR preferred)
   Part Number: Panasonic EEU-FR1E102
   Purpose: Input voltage smoothing, absorbs switching noise
   ESR: <100mΩ @ 100kHz
   Cost: $0.80

C4: Input Ceramic Capacitor
   Value: 100nF (0.1µF)
   Voltage: 50V
   Type: Ceramic X7R or C0G
   Part Number: Murata GRM31CR71H104KA01L
   Purpose: High-frequency noise filtering (switch-mode spikes)
   Cost: $0.10

LM2596 Buck Converter Module:
   Input Voltage: 4-35V DC
   Output Voltage: 1.25-30V (adjustable via potentiometer)
   Output Current: 3A maximum (with heatsink)
   Efficiency: 92% typical @ 12V→5V, 3A
   Switching Frequency: 150 kHz
   Features: Built-in overcurrent, thermal shutdown
   Part Number: Generic LM2596 module or LM2596S-ADJ chip
   Heatsink: Required for >2A loads
   Cost: $2-3 (module) or $1.50 (chip only)
   
   Alternative: XL4015 (5A capable, $3-4)
   Alternative: MP1584 (3A, smaller, $1.50)

C2: 5V Output Bulk Capacitor
   Value: 1000µF
   Voltage: 16V (rated for 5V output with margin)
   Type: Electrolytic Low-ESR
   Part Number: Panasonic EEU-FR1C102
   Purpose: 5V rail smoothing, load transient response
   ESR: <50mΩ @ 100kHz
   Cost: $0.80

C5: 5V Output Ceramic Capacitor
   Value: 100nF (0.1µF)
   Voltage: 50V
   Type: Ceramic X7R
   Part Number: Murata GRM31CR71H104KA01L
   Purpose: High-frequency decoupling on 5V rail
   Cost: $0.10

C3: 3.3V Rail Capacitor (on ESP32-S3 board)
   Value: 100µF
   Voltage: 6.3V or 10V
   Type: Tantalum or Low-ESR Electrolytic
   Part Number: AVX TAJD107K006RNJ (Tantalum)
   Purpose: 3.3V rail filtering from ESP32 onboard LDO
   ESR: <100mΩ
   Cost: $0.50 (Tantalum) or $0.30 (Electrolytic)

Additional Bypass Capacitors (per module):
   Value: 10µF + 100nF per module
   Placement: As close as possible to VCC pin
   Purpose: Local decoupling, transient response
   Cost: $0.20 each

═══════════════════════════════════════════════════════════════════════════
LM2596 BUCK CONVERTER ADJUSTMENT PROCEDURE
═══════════════════════════════════════════════════════════════════════════

Tools Required:
├─ Multimeter (accurate to 0.01V)
├─ Small Phillips screwdriver (for potentiometer)
├─ 12V power supply or battery
└─ Test load (resistor or light bulb, optional)

Step-by-Step Adjustment:
────────────────────────────────────────────────────────────────────────────

1. INITIAL SETUP (No Load):
   ├─ Connect 12V to LM2596 input (IN+/IN-)
   ├─ DO NOT connect any load yet!
   ├─ Connect multimeter to output (OUT+/OUT-)
   └─ Turn on 12V power supply

2. ADJUST OUTPUT VOLTAGE:
   ├─ Observe multimeter reading (may be 1.25V-30V initially)
   ├─ Locate blue potentiometer on LM2596 module
   ├─ Turn clockwise to increase voltage
   ├─ Turn counter-clockwise to decrease voltage
   ├─ Adjust until multimeter reads EXACTLY 5.00V
   └─ Allow 10 seconds to stabilize, re-check

3. VERIFY UNDER LOAD:
   ├─ Turn off power
   ├─ Connect ESP32-S3 to output (light load ~200mA)
   ├─ Turn on power, verify voltage is still 5.00V ±0.05V
   ├─ If voltage drops significantly, readjust slightly higher
   └─ Final setting: 5.00V - 5.05V under load

4. MARK THE SETTING:
   ├─ Use permanent marker to mark potentiometer position
   ├─ Take photo of setting for future reference
   └─ Apply small dab of hot glue to prevent accidental rotation

IMPORTANT: Output voltage MUST be 5.0V ±5%
├─ Too low (<4.75V): ESP32 may not boot, brownouts
├─ Too high (>5.25V): Risk of damaging 5V-tolerant components
└─ Ideal: 5.00V - 5.05V (accounts for voltage drop under load)

═══════════════════════════════════════════════════════════════════════════
POWER BUDGET CALCULATION
═══════════════════════════════════════════════════════════════════════════

Current Draw Analysis (Worst Case):
────────────────────────────────────────────────────────────────────────────

5V Rail Components:
Component           Voltage  Current   Power    Notes
─────────────────────────────────────────────────────────────────────────
ESP32-S3 (active)   5V       300mA     1.5W     VIN to onboard LDO
BL-M8812EU2 (RX)    5V       500mA     2.5W     Idle/receive mode
BL-M8812EU2 (TX)    5V       2500mA    12.5W    Transmit/scan mode ⚠️
Relays (if used)    5V       400mA     2.0W     5x 80mA coils
─────────────────────────────────────────────────────────────────────────
TOTAL 5V Rail:               3200mA    16.0W    Worst case (all active)

3.3V Rail Components (from ESP32 LDO):
Component           Voltage  Current   Power    Notes
─────────────────────────────────────────────────────────────────────────
ESP32-S3 Core       3.3V     250mA     0.83W    WiFi + BT active
Wio-SX1262 (RX)     3.3V     30mA      0.10W    Receive mode
OLED Display        3.3V     20mA      0.07W    50% brightness
GPS Module          3.3V     50mA      0.17W    Active tracking
SD Card (write)     3.3V     100mA     0.33W    During logging
LED + Buzzer        3.3V     50mA      0.17W    Simultaneous use
─────────────────────────────────────────────────────────────────────────
TOTAL 3.3V Rail:             500mA     1.67W    ESP32 LDO output

Input Power Calculation:
────────────────────────────────────────────────────────────────────────────

Total System Power: 16.0W + 1.67W = 17.67W
LM2596 Efficiency: ~92% @ 3A load
Input Power Required: 17.67W / 0.92 = 19.2W
Input Current @ 12V: 19.2W / 12V = 1.6A

Safety Margin: Use 3A supply (2x headroom)

Battery Life Estimation:
────────────────────────────────────────────────────────────────────────────

Example: 3S LiPo 5000mAh @ 11.1V
────────────────────────────────────────────────────────────────────────────
Battery Capacity: 5000mAh × 11.1V = 55.5 Wh
System Draw (avg): 10W (mix of scan + idle)
Runtime: 55.5Wh / 10W = 5.5 hours theoretical
Actual Runtime: ~4-5 hours (accounting for efficiency losses)

Conservative Estimates:
├─ Idle mode (1A @ 12V): ~12W → 4.6 hours
├─ Scan mode (1.5A @ 12V): ~18W → 3.1 hours  
└─ Full active (2A @ 12V): ~24W → 2.3 hours

Recommended Batteries:
────────────────────────────────────────────────────────────────────────────
Option 1: 3S LiPo 5000mAh (11.1V)    → 4-5 hours runtime
Option 2: 12V 7Ah SLA Battery        → 6-8 hours runtime
Option 3: 18650 3S4P (12V 10Ah)      → 8-10 hours runtime
Option 4: DC Power Brick (12V 3A)    → Unlimited (mains powered)

═══════════════════════════════════════════════════════════════════════════
GROUNDING STRATEGY (Star Topology)
═══════════════════════════════════════════════════════════════════════════

Proper Grounding Prevents:
├─ Ground loops (noise coupling between modules)
├─ Voltage drops (causing resets/brownouts)
├─ RF interference (improves radio performance)
└─ EMI/EMC issues (FCC compliance)

Star Ground Implementation:
────────────────────────────────────────────────────────────────────────────

                      Main GND Point
                   (near power input)
                          │
          ┌───────────────┼───────────────┬───────────────┐
          │               │               │               │
      12V Input      LM2596 GND      ESP32 GND      BL-M88 GND
      GND Return     (OUT-)          Pin            Module GND
          │               │               │               │
          └───────────────┴───────────────┴───────────────┘
                          │
                    All grounds meet
                    at SINGLE point

PCB Layout Rules:
────────────────────────────────────────────────────────────────────────────
1. Bottom layer = solid GND plane (copper pour)
2. All module GND pins connect to plane via short traces
3. Power input GND connects to plane at ONE point (star center)
4. High current paths (12V, 5V) use thick traces (>1.5mm)
5. Keep digital GND and analog GND separate until star point
6. Use vias to connect top-layer GND pads to bottom GND plane

Trace Width Guidelines (2oz copper):
────────────────────────────────────────────────────────────────────────────
12V Input (2A max):      2.0mm minimum
5V Rail (3A):            1.5mm minimum  
3.3V Rail (500mA):       0.8mm minimum
Signal traces:           0.3mm minimum
GND traces:              As wide as possible (or use plane)

Thermal Relief:
────────────────────────────────────────────────────────────────────────────
For LM2596 heatsink connection to GND plane:
├─ Use thermal vias (4x 0.8mm vias under heatsink pad)
├─ Connect heatsink tab to GND plane for heat dissipation
└─ Ensure electrical isolation if heatsink tab is connected to Vin!

═══════════════════════════════════════════════════════════════════════════
OVERCURRENT & THERMAL PROTECTION
═══════════════════════════════════════════════════════════════════════════

Built-in Protection Features:
────────────────────────────────────────────────────────────────────────────

F1 Polyfuse (Input):
├─ Hold Current: 3A continuous
├─ Trip Current: 6A (overcurrent event)
├─ Reset: Automatic when fault clears
└─ Response Time: <1 second @ 2x rated current

D1 Schottky Diode:
├─ Reverse Polarity: Blocks reverse voltage
├─ Forward Drop: 0.5V (acceptable loss)
└─ If reverse connected: Polyfuse trips, no damage

LM2596 Internal Protection:
├─ Overcurrent Limit: ~3.5A typical
├─ Thermal Shutdown: 150°C junction temp
├─ Automatic Restart: When temperature drops below 125°C
└─ Short Circuit: Hiccup mode (pulsed operation until fault clears)

ESP32-S3 Onboard LDO:
├─ Current Limit: 500mA (thermal foldback)
├─ Thermal Shutdown: 150°C
└─ Reverse Voltage: Protected by input diode

Additional Protection Recommendations:
────────────────────────────────────────────────────────────────────────────

TVS Diode (Optional, for automotive use):
   Part Number: SMAJ13CA (13V bidirectional)
   Location: Across 12V input (after F1, before D1)
   Purpose: Protects against voltage spikes (alternator load dump)
   Cost: $0.40

Thermal Monitoring:
   NTC Thermistor on LM2596 heatsink
   Alert if temperature >80°C
   Shutdown if temperature >90°C

Visual Indicators:
   Green LED on 5V rail (power good)
   Red LED on fault condition (optional)

═══════════════════════════════════════════════════════════════════════════
TESTING & VERIFICATION PROCEDURE
═══════════════════════════════════════════════════════════════════════════

Pre-Power Checks (Multimeter in Continuity Mode):
────────────────────────────────────────────────────────────────────────────
□ Verify NO short between 12V input and GND (should be open)
□ Verify NO short between 5V rail and GND (should be open)
□ Verify NO short between 3.3V rail and GND (should be open)
□ Check polarity of all electrolytic capacitors (+ to positive rail)
□ Verify diode D1 orientation (band = cathode, points to 12V+)
□ Check all solder joints for bridges or cold joints

Power-On Sequence (With Multimeter in DC Voltage Mode):
────────────────────────────────────────────────────────────────────────────
Step 1: Connect 12V input, NO load
   └─ Measure 12V at LM2596 input: Should read 11.5-12.0V (after D1 drop)

Step 2: Measure 5V output, NO load
   └─ Adjust LM2596 potentiometer to exactly 5.00V

Step 3: Connect ESP32-S3 only (light load ~200mA)
   └─ Measure 5V rail: Should read 4.95-5.05V
   └─ Measure 3.3V rail: Should read 3.25-3.35V

Step 4: Connect all modules except BL-M8812EU2
   └─ Measure 5V rail: Should read 4.90-5.05V
   └─ Measure 3.3V rail: Should read 3.20-3.35V
   └─ Check current draw: Should be <500mA @ 12V

Step 5: Connect BL-M8812EU2 module
   └─ Measure 5V rail under load: Should read 4.85-5.05V
   └─ Check current draw during scan: Should be 1.5-2.5A @ 12V
   └─ Feel LM2596 heatsink: Should be warm, not burning hot (<80°C)

Load Testing:
────────────────────────────────────────────────────────────────────────────
□ Run full system for 10 minutes, monitor temperatures
□ Trigger BL-M8812EU2 active scan, verify no voltage sag
□ Check ripple voltage with oscilloscope: <100mV pk-pk on 5V rail
□ Verify no audible noise (coil whine) from LM2596
□ Test with low battery voltage (10V input): System should still operate

Fault Testing:
────────────────────────────────────────────────────────────────────────────
□ Reverse polarity test: Connect 12V backwards, verify polyfuse trips
□ Overcurrent test: Short 5V to GND briefly, verify LM2596 protection
□ Thermal test: Run at max load, verify thermal shutdown if >150°C

Acceptance Criteria:
────────────────────────────────────────────────────────────────────────────
✅ 5V rail: 4.90-5.10V under all load conditions
✅ 3.3V rail: 3.20-3.40V under all load conditions
✅ Voltage ripple: <100mV pk-pk
✅ No excessive heat: All components <80°C
✅ Current draw: <3A @ 12V worst case
✅ Efficiency: >85% (measured: Pout/Pin)

═══════════════════════════════════════════════════════════════════════════
BOM (Bill of Materials) - POWER SUPPLY ONLY
═══════════════════════════════════════════════════════════════════════════

Component    Part Number           Quantity  Unit Price  Total
─────────────────────────────────────────────────────────────────────────
F1           Bourns MF-R300        1         $0.50       $0.50
D1           SS34 Schottky         1         $0.20       $0.20
C1           1000µF 25V Elec       1         $0.80       $0.80
C2           1000µF 16V Elec       1         $0.80       $0.80
C3           100µF 6.3V Tant       1         $0.50       $0.50
C4, C5       100nF 50V Ceramic     2         $0.10       $0.20
LM2596       Buck Module           1         $2.50       $2.50
Heatsink     TO-220 Heatsink       1         $0.50       $0.50
Wire         18 AWG silicone       1m        $1.00/m     $1.00
Connectors   DC barrel jack        1         $0.50       $0.50
Terminal     Screw terminal 2-pin  1         $0.30       $0.30
─────────────────────────────────────────────────────────────────────────
TOTAL POWER SUPPLY COST:                               $7.80

Optional Components:
─────────────────────────────────────────────────────────────────────────
TVS Diode    SMAJ13CA              1         $0.40       $0.40
Power LED    Green LED 3mm         1         $0.10       $0.10
Resistor     1kΩ LED resistor      1         $0.05       $0.05
Switch       SPST Rocker           1         $1.00       $1.00
Voltmeter    Digital 0.28" display 1         $3.00       $3.00
─────────────────────────────────────────────────────────────────────────
OPTIONAL TOTAL:                                        $4.55

GRAND TOTAL (with optionals): $12.35

═══════════════════════════════════════════════════════════════════════════
ALTERNATIVE POWER SOLUTIONS
═══════════════════════════════════════════════════════════════════════════

Option A: USB-C Power Delivery (Compact/Modern)
────────────────────────────────────────────────────────────────────────────
Components:
├─ USB-C PD Trigger Board (requests 20V from PD charger)
├─ LM2596 or XL4015 (20V → 5V @ 3A)
└─ USB-C PD charger (20V 3A, 60W)

Advantages:
✅ Single cable for power
✅ Widely available USB-C PD chargers
✅ Compact (no external 12V brick)

Disadvantages:
❌ Requires PD trigger board ($5-8)
❌ Not all USB-C chargers support 20V
❌ More complex troubleshooting

Option B: Dual USB-A Ports (Budget/Simple)
────────────────────────────────────────────────────────────────────────────
Components:
├─ USB-A cable #1 to ESP32-S3 (5V 2A) → Powers ESP32 + peripherals
├─ USB-A cable #2 to BL-M8812EU2 (5V 3A) → Dedicated 5GHz module power
└─ Dual USB wall charger (5V 5A total, two ports)

Advantages:
✅ No buck converter needed
✅ Simple wiring
✅ Cheap USB chargers readily available

Disadvantages:
❌ Two USB cables required
❌ Must ensure USB #2 port can supply 3A
❌ Less elegant cable management

Option C: Battery with BMS (Portable Operation)
────────────────────────────────────────────────────────────────────────────
Components:
├─ 3S LiPo with BMS (11.1V, 5000mAh, XT60 connector)
├─ LM2596 Buck (11.1V → 5V @ 3A)
├─ Battery voltage monitor/alarm (low voltage warning)
└─ Charging: External balance charger or USB-C charging module

Advantages:
✅ Portable operation (4-5 hours runtime)
✅ No wires during operation
✅ Rechargeable

Disadvantages:
❌ LiPo requires careful handling (fire risk if damaged)
❌ BMS adds cost ($10-15)
❌ Needs separate charger

Recommended: Option A (USB-C PD) for portability, 12V input for best cost/performance

═══════════════════════════════════════════════════════════════════════════
END OF POWER SUPPLY SCHEMATIC
═══════════════════════════════════════════════════════════════════════════
