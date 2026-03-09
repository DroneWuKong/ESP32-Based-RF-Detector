# RF DETECTOR STACKED - SHOPPING CHECKLIST
## Everything You Need to Buy - Organized by Supplier

═══════════════════════════════════════════════════════════════════════════
QUICK SUMMARY
═══════════════════════════════════════════════════════════════════════════

Total Cost: ~$109 per complete unit
Build Time: 3-4 hours
Skill Level: Intermediate

3 custom PCBs (order from JLCPCB)
ESP32-S3 dev board (off-the-shelf)
BL-M8812EU2 + Wio-SX1262 modules
Display, GPS, accessories
Stacking hardware
3D printed case

═══════════════════════════════════════════════════════════════════════════
ORDER #1: JLCPCB (PCBs) - $25 for 5 complete sets
═══════════════════════════════════════════════════════════════════════════

Upload 3 separate Gerber files:

□ Main_Board_40x40mm.zip
  ├─ Layers: 4
  ├─ Size: 40x40mm
  ├─ Thickness: 1.6mm
  ├─ Surface: HASL (or ENIG)
  ├─ Quantity: 5
  └─ Cost: ~$15

□ RF_Daughter_40x40mm.zip
  ├─ Layers: 2  
  ├─ Size: 40x40mm
  ├─ Thickness: 1.6mm
  ├─ Surface: HASL
  ├─ Quantity: 5
  └─ Cost: ~$5

□ Display_Board_40x30mm.zip
  ├─ Layers: 2
  ├─ Size: 40x30mm
  ├─ Thickness: 1.6mm
  ├─ Surface: HASL
  ├─ Quantity: 5
  └─ Cost: ~$5

TOTAL JLCPCB: $25 (5 complete PCB sets)
Delivery: 5-10 days

═══════════════════════════════════════════════════════════════════════════
ORDER #2: AMAZON (Fast Shipping) - $46
═══════════════════════════════════════════════════════════════════════════

Main Components:
────────────────────────────────────────────────────────────────────────────
□ ESP32-S3-DevKitC-1-N16R8                      $12
  Search: "ESP32-S3 N16R8 development board"
  Verify: 16MB Flash, 8MB PSRAM, USB-C
  
□ 12V 3A Power Adapter (barrel jack)            $8
  Search: "12V 3A power supply"
  
□ 868/915 MHz SMA Antenna (2-5dBi)              $6
  Search: "915MHz LoRa antenna SMA"
  
□ 5.8 GHz SMA Antenna (2x)                      $10
  Search: "5.8GHz WiFi antenna SMA" (buy 2)
  
□ Soldering Kit (if needed)                     $10
  Search: "soldering iron kit temperature"
  Includes: Iron, solder, flux pen, desoldering braid

AMAZON SUBTOTAL: $46
Prime Delivery: 1-2 days

═══════════════════════════════════════════════════════════════════════════
ORDER #3: ALIEXPRESS (Cheap, Slow) - $55
═══════════════════════════════════════════════════════════════════════════

RF Modules (Most Expensive):
────────────────────────────────────────────────────────────────────────────
□ BL-M8812EU2 WiFi Module                       $12.50
  Search: "BL-M8812EU2 RTL8812EU"
  Verify: 32x32mm, 5GHz, high power
  Seller: LB-LINK official or resellers
  
□ Wio-SX1262 LoRa Module                        $17.50
  Search: "SX1262 LoRa module 915MHz"
  Alternative: Buy from Seeed Studio (faster)
  Verify: 25x25mm, SMA connector

Peripherals:
────────────────────────────────────────────────────────────────────────────
□ OLED Display 0.96" I2C 128x64                 $4
  Search: "SSD1306 OLED 0.96 I2C"
  Color: White or Blue/Yellow
  
□ GPS Module NEO-6M or ATGM336H                 $10
  Search: "NEO-6M GPS module"
  Verify: 3.3V compatible, UART
  
□ MicroSD Card Module                           $2.50
  Search: "MicroSD card module SPI"

Electronics Components:
────────────────────────────────────────────────────────────────────────────
□ LM2596 SMD IC or Module                       $2.50
  Search: "LM2596 buck converter module"
  Verify: Adjustable output, 3A rating

□ SMD Component Kit (resistors, caps)           $5
  Search: "0805 SMD kit resistors capacitors"
  Should include: 220Ω, 100nF, 10µF
  
□ SS34 Schottky Diode (pack of 10)              $1
  Search: "SS34 SMD diode"

Connectors & Hardware:
────────────────────────────────────────────────────────────────────────────
□ IPEX to SMA Pigtail Cable 50mm (2x)           $4
  Search: "IPEX to SMA pigtail 50mm"
  Verify: U.FL connector, 50mm length
  
□ Pin Header Kit (male + female)                $3
  Search: "2.54mm pin headers assortment"
  
□ Stacking Headers 1x19 (2x)                    $4
  Search: "Arduino stacking headers"
  OR buy female + male separately
  
□ M2.5 Standoff Kit (brass, various lengths)    $5
  Search: "M2.5 standoff kit brass"
  Need: 6mm, 8mm, 10mm lengths
  
□ Screw Terminal 2-pin 5.08mm                   $0.50
  Search: "KF301-2P screw terminal"

Small Parts:
────────────────────────────────────────────────────────────────────────────
□ Tactile Switch 6x6mm SMD (pack of 10)         $1
  Search: "6x6mm tactile button SMD"
  
□ Piezo Buzzer 12mm SMD                         $1
  Search: "12mm SMD buzzer"
  
□ Rubber Feet Adhesive 8mm (pack of 20)         $1
  Search: "rubber feet adhesive 8mm"

ALIEXPRESS SUBTOTAL: $55
Shipping: 2-4 weeks (choose ePacket or AliExpress Standard)

═══════════════════════════════════════════════════════════════════════════
ORDER #4: MOUSER/DIGIKEY (Quality Components) - Optional $15
═══════════════════════════════════════════════════════════════════════════

For Better Quality (Optional - AliExpress parts work fine!):

□ Panasonic Electrolytic Caps (C1, C2)          $2.40
  Part: EEE-FK1E102P (1000µF 25V)
  Part: EEE-FK1C102P (1000µF 16V)
  
□ Kemet Tantalum Cap (C3)                       $0.50
  Part: T491D107K006AT (100µF 6.3V)
  
□ Bourns Polyfuse (F1)                          $0.50
  Part: MF-R300 (3A resettable)
  
□ Vishay Schottky Diode (D1)                    $0.20
  Part: SS34 (3A 40V)
  
□ Samtec Stacking Headers (J11)                 $4
  Part: SSW-119-02-G-D
  
□ Amphenol SMA Connector (J2)                   $1.50
  Part: 132289 (edge mount)

MOUSER SUBTOTAL: $9.10
Shipping: $8 (unless free shipping threshold met)

═══════════════════════════════════════════════════════════════════════════
ORDER #5: LOCAL 3D PRINTING - $2
═══════════════════════════════════════════════════════════════════════════

If you have 3D printer:
────────────────────────────────────────────────────────────────────────────
□ PETG Filament (already have?)                 $0
  Print: Case_Top_Stacked.stl
  Print: Case_Bottom_Stacked.stl
  Settings: 20% infill, 0.2mm layers
  Time: ~6 hours total
  Material: ~50g PETG ($1.50 value)

If you don't have 3D printer:
────────────────────────────────────────────────────────────────────────────
□ Order from 3D Hubs / local makerspace          $10-15
  Upload: STL files
  Material: PETG (preferred) or PLA
  Color: Black, gray, or your choice

CASE COST: $1.50-15 depending on method

═══════════════════════════════════════════════════════════════════════════
OPTIONAL EXTRAS
═══════════════════════════════════════════════════════════════════════════

Nice to Have (Add Later):
────────────────────────────────────────────────────────────────────────────
□ Better antennas (high-gain)                   $20-40
  915MHz 5-9dBi directional
  5.8GHz 8-14dBi patch antenna
  
□ GPS Active Antenna (better reception)         $12
  Search: "GPS active antenna SMA"
  
□ MicroSD Card (8-32GB)                         $8
  For logging detections
  
□ 3S LiPo Battery 5000mAh                       $25
  For portable operation (4-5 hour runtime)
  
□ Lipo charger                                  $15
  If using battery power
  
□ Weatherproof case (upgrade)                   $20
  Pelican-style small case
  
□ Better soldering iron                         $50-100
  Hakko FX-888D or similar
  
□ Hot air station (for BL-M8812EU2)             $40-80
  Makes SMD soldering much easier!

═══════════════════════════════════════════════════════════════════════════
RECOMMENDED BUYING STRATEGY
═══════════════════════════════════════════════════════════════════════════

FASTEST BUILD (1 week):
────────────────────────────────────────────────────────────────────────────
Week 1:
□ Order PCBs from JLCPCB ($25) - 5-10 days
□ Order everything from Amazon ($46) - 1-2 days
□ Order BL-M8812EU2 + Wio-SX1262 from:
  - Seeed Studio (Wio-SX1262): 3-5 days
  - AliExpress express shipping (BL-M88): 7-10 days
  Cost: +$20 for express shipping

Week 2:
□ Parts arrive, start building!

TOTAL COST (Fast): $130-140

CHEAPEST BUILD (4 weeks):
────────────────────────────────────────────────────────────────────────────
Week 1:
□ Order everything from AliExpress ($55)
□ Order PCBs from JLCPCB ($25)
□ Order ESP32 + power supply from Amazon ($20)

Weeks 2-4:
□ Wait for AliExpress delivery (2-4 weeks)

Week 4-5:
□ Everything arrives, build!

TOTAL COST (Cheap): $100-110

BALANCED BUILD (2 weeks):
────────────────────────────────────────────────────────────────────────────
□ PCBs from JLCPCB ($25) - 7-10 days
□ Main components from Amazon ($46) - 2 days
□ Modules from AliExpress standard ($55) - 2-3 weeks
□ 3D print case while waiting

TOTAL COST (Balanced): $126

═══════════════════════════════════════════════════════════════════════════
TOOLS CHECKLIST (One-Time Purchase)
═══════════════════════════════════════════════════════════════════════════

If you don't have soldering tools:

Essential ($35):
────────────────────────────────────────────────────────────────────────────
□ Soldering iron (temp controlled)              $20-25
  Recommended: Hakko FX-600 or KSGER T12
  
□ Solder (60/40 or lead-free 0.8mm)            $5
  1-2 oz spool sufficient
  
□ Flux pen                                      $3
  Makes SMD soldering MUCH easier!
  
□ Tweezers (fine point ESD-safe)                $3
  Essential for placing SMD parts
  
□ Desoldering braid                             $2
  For fixing mistakes

Nice to Have ($45):
────────────────────────────────────────────────────────────────────────────
□ Helping hands / PCB holder                    $10
  Holds board while soldering
  
□ Magnifying glass with light                   $15
  Essential for inspecting SMD joints
  
□ Multimeter                                    $15
  For testing continuity and voltage
  
□ Wire stripper/cutter                          $5
  For cutting component leads

Advanced ($120):
────────────────────────────────────────────────────────────────────────────
□ Hot air rework station                        $50-80
  Makes BL-M8812EU2 soldering much easier!
  
□ Microscope (USB or optical)                   $30-40
  For inspecting fine-pitch soldering

═══════════════════════════════════════════════════════════════════════════
FINAL CHECKLIST BEFORE ORDERING
═══════════════════════════════════════════════════════════════════════════

□ I have reviewed the BOM
□ I understand the build difficulty (intermediate)
□ I have basic soldering skills (or willing to learn!)
□ I have 3-4 hours for assembly
□ I have ordered/own necessary tools
□ I understand delivery times
□ I have Gerber files ready for JLCPCB
□ I have firmware ready to upload
□ I have budget allocated (~$110 + tools)
□ I'm excited to build! 🚀

═══════════════════════════════════════════════════════════════════════════
READY TO ORDER!
═══════════════════════════════════════════════════════════════════════════

Start with:
1. JLCPCB (PCBs) - $25
2. Amazon (ESP32 + basics) - $46
3. AliExpress (modules) - $55

Total: $126 for complete stacked RF detector!

Happy shopping! 🛒
