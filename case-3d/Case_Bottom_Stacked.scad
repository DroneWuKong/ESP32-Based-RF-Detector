// RF DETECTOR - STACKED VERSION CASE (BOTTOM SHELL)
// 45x45x20mm with power ports, PCB mounts, ventilation
// Print face-down, no supports needed!

// ═══════════════════════════════════════════════════════════════
// PARAMETERS - Match top shell!
// ═══════════════════════════════════════════════════════════════

// Case dimensions
case_length = 45;           // mm
case_width = 45;            // mm
case_height = 20;           // mm (bottom shell height)
wall_thickness = 2.0;       // mm

// Cosmetic
corner_radius = 3;          // mm (rounded corners)

// Power ports (rear panel)
usbc_width = 9;             // mm
usbc_height = 3.5;          // mm
usbc_x = 22.5;              // mm from left (centered)
usbc_y = -1;                // mm (rear edge)
usbc_z = 8;                 // mm height

dc_jack_diameter = 8;       // mm (barrel jack)
dc_jack_x = 12;             // mm from left
dc_jack_y = -1;             // mm (rear edge)
dc_jack_z = 8;              // mm height

// PCB mounting
pcb_mount_hole_dia = 2.7;   // mm (M2.5 clearance)
pcb_mount_boss_dia = 5;     // mm (standoff base)
pcb_mount_height = 3;       // mm (height from bottom)

// Rubber feet recesses
feet_diameter = 8;          // mm
feet_depth = 1.5;           // mm

// ═══════════════════════════════════════════════════════════════
// MAIN BOTTOM SHELL
// ═══════════════════════════════════════════════════════════════

module bottom_shell() {
    difference() {
        // Main body with rounded corners
        hull() {
            translate([corner_radius, corner_radius, 0])
                cylinder(r=corner_radius, h=case_height, $fn=30);
            translate([case_length-corner_radius, corner_radius, 0])
                cylinder(r=corner_radius, h=case_height, $fn=30);
            translate([corner_radius, case_width-corner_radius, 0])
                cylinder(r=corner_radius, h=case_height, $fn=30);
            translate([case_length-corner_radius, case_width-corner_radius, 0])
                cylinder(r=corner_radius, h=case_height, $fn=30);
        }
        
        // Hollow interior
        translate([wall_thickness, wall_thickness, wall_thickness])
            hull() {
                cr = corner_radius - wall_thickness;
                translate([cr, cr, 0])
                    cylinder(r=cr, h=case_height, $fn=30);
                translate([case_length-2*wall_thickness-cr, cr, 0])
                    cylinder(r=cr, h=case_height, $fn=30);
                translate([cr, case_width-2*wall_thickness-cr, 0])
                    cylinder(r=cr, h=case_height, $fn=30);
                translate([case_length-2*wall_thickness-cr, case_width-2*wall_thickness-cr, 0])
                    cylinder(r=cr, h=case_height, $fn=30);
            }
        
        // USB-C port cutout (rear)
        translate([usbc_x - usbc_width/2, usbc_y, usbc_z - usbc_height/2])
            cube([usbc_width, wall_thickness + 2, usbc_height]);
        
        // DC barrel jack cutout (rear)
        translate([dc_jack_x, dc_jack_y, dc_jack_z])
            rotate([-90, 0, 0])
                cylinder(d=dc_jack_diameter, h=wall_thickness + 2, $fn=30);
        
        // Rubber feet recesses (bottom corners)
        translate([corner_radius + 2, corner_radius + 2, -0.5])
            cylinder(d=feet_diameter, h=feet_depth + 0.5, $fn=30);
        translate([case_length - corner_radius - 2, corner_radius + 2, -0.5])
            cylinder(d=feet_diameter, h=feet_depth + 0.5, $fn=30);
        translate([corner_radius + 2, case_width - corner_radius - 2, -0.5])
            cylinder(d=feet_diameter, h=feet_depth + 0.5, $fn=30);
        translate([case_length - corner_radius - 2, case_width - corner_radius - 2, -0.5])
            cylinder(d=feet_diameter, h=feet_depth + 0.5, $fn=30);
        
        // Hexagonal ventilation pattern (bottom)
        for(x = [8:6:case_length-8]) {
            for(y = [8:6:case_width-8]) {
                offset_y = (x % 12 == 0) ? 3 : 0;
                translate([x, y + offset_y, -0.5])
                    cylinder(d=4, h=wall_thickness + 1, $fn=6);
            }
        }
        
        // Snap-fit receptacles (match top shell clips)
        for(x = [5, case_length - 7]) {
            for(y = [5, case_width - 7]) {
                translate([x, y, case_height - 4])
                    cube([1.8, 8.5, 5]);
            }
        }
    }
    
    // PCB mounting bosses (4 corners)
    pcb_mount_positions = [
        [8, 8],
        [case_length - 8, 8],
        [8, case_width - 8],
        [case_length - 8, case_width - 8]
    ];
    
    for(pos = pcb_mount_positions) {
        translate([pos[0], pos[1], wall_thickness])
            pcb_mounting_boss();
    }
    
    // Cable management channels (interior bottom)
    translate([10, 5, wall_thickness])
        cube([25, 1, 2]);  // Power cable channel
    translate([10, case_width - 6, wall_thickness])
        cube([25, 1, 2]);  // Antenna cable channel
    
    // Label area (smooth recess for sticker)
    translate([case_length/2 - 10, case_width/2 - 5, -0.3])
        linear_extrude(0.3)
            square([20, 10]);
}

// ═══════════════════════════════════════════════════════════════
// PCB MOUNTING BOSS (For M2.5 standoffs or direct screws)
// ═══════════════════════════════════════════════════════════════

module pcb_mounting_boss() {
    difference() {
        // Boss cylinder
        cylinder(d=pcb_mount_boss_dia, h=pcb_mount_height, $fn=30);
        
        // Screw hole (for M2.5 screw or heat-set insert)
        translate([0, 0, -0.5])
            cylinder(d=pcb_mount_hole_dia, h=pcb_mount_height + 1, $fn=20);
    }
}

// ═══════════════════════════════════════════════════════════════
// RENDER
// ═══════════════════════════════════════════════════════════════

bottom_shell();

// ═══════════════════════════════════════════════════════════════
// PRINT SETTINGS
// ═══════════════════════════════════════════════════════════════

// Material: PETG (best) or PLA
// Layer Height: 0.2mm
// Infill: 20% (gyroid)
// Supports: NONE! (Print face-down)
// Orientation: Bottom surface facing build plate
// Print Time: ~3 hours
// Filament: ~25g

// ═══════════════════════════════════════════════════════════════
// POST-PROCESSING
// ═══════════════════════════════════════════════════════════════

// 1. Remove from bed
// 2. Install M2.5 heat-set inserts in mounting bosses
//    (Heat to 200°C, press insert into boss)
// 3. Test-fit PCB stack (should sit flat on bosses)
// 4. Apply 4x rubber feet in corner recesses
// 5. Clean up any stringing around ports

// ═══════════════════════════════════════════════════════════════
// ASSEMBLY NOTES
// ═══════════════════════════════════════════════════════════════

// Mounting Options:
// Option A: M2.5 heat-set inserts + M2.5x6mm screws
// Option B: Self-tapping M2.5 screws directly into boss
// Option C: Standoffs (M2.5 female-female) + screws

// Recommended: Heat-set inserts for best results!
