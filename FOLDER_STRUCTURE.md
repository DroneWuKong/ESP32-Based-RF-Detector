# Project Organization

```
rf-detector-esp32/
├── README.md                      # Project overview (START HERE!)
├── LICENSE                        # MIT License
├── .gitignore                     # Git ignore rules
│
├── firmware/                      # Arduino firmware
│   ├── README.md                  # Firmware documentation
│   └── rf_detector_ESP32S3.ino    # Main code (750 lines)
│
├── hardware/                      # Wiring & components
│   ├── README.md                  # Hardware documentation
│   ├── VISUAL_WIRING_DIAGRAM.md   # ⭐ Pin-by-pin wiring guide
│   ├── BOM.csv                    # Bill of Materials (~$109)
│   ├── SHOPPING_LIST.md           # Where to buy components
│   └── Board_0_Structured_Data.txt # PCB specifications
│
├── docs/                          # Build guides
│   ├── README.md                  # Documentation index
│   ├── ASSEMBLY_GUIDE.md          # ⭐ Step-by-step assembly
│   ├── STACKED_DESIGN.md          # Architecture details
│   ├── POWER_SUPPLY_SCHEMATIC.md  # Power system design
│   ├── UPWORK_JOB_POSTING.md      # Hire PCB designer ($75)
│   └── UPWORK_QUICK_POST.md       # Quick posting template
│
└── case-3d/                       # 3D printable case
    ├── README.md                  # Print instructions
    ├── Case_Top_Stacked_FIXED.scad    # Top shell (OpenSCAD)
    └── Case_Bottom_Stacked.scad       # Bottom shell (OpenSCAD)
```

## Quick Navigation

**Want to build?**  
→ Start with `/docs/ASSEMBLY_GUIDE.md`

**Need wiring help?**  
→ See `/hardware/VISUAL_WIRING_DIAGRAM.md`

**Buying components?**  
→ Check `/hardware/SHOPPING_LIST.md`

**Want to customize?**  
→ Edit `/firmware/rf_detector_ESP32S3.ino`

**Need a case?**  
→ Print from `/case-3d/*.scad` files

**Want custom PCBs?**  
→ Use `/docs/UPWORK_JOB_POSTING.md`

## File Sizes

- Total repository: ~300 KB
- Firmware: ~37 KB
- Documentation: ~250 KB
- 3D files: ~12 KB

All text files - no binaries!
