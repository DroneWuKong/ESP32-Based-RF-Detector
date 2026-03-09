# Documentation

Complete build guides and project documentation.

## Build Guides

- **ASSEMBLY_GUIDE.md** - Step-by-step assembly instructions (⭐ START HERE)
- **STACKED_DESIGN.md** - Complete architecture and design rationale
- **POWER_SUPPLY_SCHEMATIC.md** - Power system design details

## PCB Design Options

- **UPWORK_JOB_POSTING.md** - Hire a PCB designer ($75, detailed guide)
- **UPWORK_QUICK_POST.md** - Quick job posting template

## Build Options

### Option 1: Perfboard ($85, 1 weekend)
Follow `/hardware/VISUAL_WIRING_DIAGRAM.md`

### Option 2: Custom PCB ($180, 3 weeks)
1. Post Upwork job using templates here
2. Designer creates Gerbers from specs
3. Order from JLCPCB
4. Assemble per `ASSEMBLY_GUIDE.md`

### Option 3: Breadboard ($60, 2 days)
Buy pre-made modules, wire per diagram

## Assembly Time Estimates

- **Perfboard:** 2-3 hours soldering
- **Custom PCB:** 1-2 hours assembly
- **Breadboard:** 30 minutes wiring

## Testing Procedure

See `ASSEMBLY_GUIDE.md` for complete testing steps:
1. Power supply only
2. Add ESP32
3. Add modules one-by-one
4. Full system test

## Troubleshooting

Common issues and solutions in `ASSEMBLY_GUIDE.md`:
- Power supply issues
- Module initialization failures
- Communication problems
- RF detection issues
