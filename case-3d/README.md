# 3D Printable Case

OpenSCAD files for 3D printing the detector case.

## Files

- **Case_Top_Stacked_FIXED.scad** - Top shell (FIXED - no floating parts!)
- **Case_Bottom_Stacked.scad** - Bottom shell with mounting bosses

## Specifications

- **Size:** 45x45x20mm per shell (90x40mm total assembled)
- **Mounting:** M2.5 screws or heat-set inserts
- **Features:**
  - OLED display window
  - Button caps (integrated)
  - LED light pipe
  - Antenna holes (side-mounted SMA)
  - Ventilation slots
  - Snap-fit clips

## How to Generate STL Files

### Using OpenSCAD (Free)

1. Download OpenSCAD: https://openscad.org/downloads.html
2. Open `Case_Top_Stacked_FIXED.scad`
3. Press F5 (Preview)
4. Press F6 (Render - wait 30 seconds)
5. File → Export → Export as STL
6. Save as `RF_Detector_Top.stl`
7. Repeat for bottom shell

**Time:** 5 minutes total

### Using Online OpenSCAD

If OpenSCAD won't install:
1. Go to: https://openscad.cloud
2. Paste file contents
3. Click Render
4. Download STL

## Print Settings

**Material:** PETG (best) or PLA  
**Layer Height:** 0.2mm  
**Infill:** 20% (gyroid)  
**Supports:** NONE needed!  
**Orientation:** Top/bottom surface DOWN on build plate

**Print Time:** ~3 hours per shell (6 hours total)  
**Filament:** ~50g total (~$1-2)

## Post-Processing

**Top Shell:**
- Test button caps (should click smoothly)
- Insert clear PLA into LED hole (optional light pipe)
- Sand OLED window edges if needed

**Bottom Shell:**
- Install M2.5 heat-set inserts in mounting bosses
  (Heat to 200°C, press insert into boss)
- Apply 4x rubber feet in corner recesses
- Clean up any stringing around ports

## Assembly

1. Install heat-set inserts in bottom shell
2. Mount PCB stack using M2.5x6mm screws
3. Connect all modules
4. Test before closing case
5. Snap top shell onto bottom
6. Secure with screws if needed (or just snap-fit)

## Customization

Both files are parametric - you can edit:
- Case dimensions
- Corner radius
- Button sizes
- Antenna hole positions
- Ventilation pattern

Just modify the parameters at the top of each .scad file!
