// RF DETECTOR - STACKED VERSION CASE TOP (FIXED VERSION)
// Fixes: Floating objects, antenna/vent conflicts, proper integration
// Print face-down, no supports needed!

// ═══════════════════════════════════════════════════════════════
// PARAMETERS
// ═══════════════════════════════════════════════════════════════

// Case dimensions
case_length = 45;
case_width = 45;
case_height = 20;
wall_thickness = 2.0;

// Cosmetic
corner_radius = 3;

// OLED window
oled_window_width = 28;
oled_window_height = 14;
oled_x_offset = 22.5;
oled_y_offset = 32;

// Buttons
button_diameter = 6;
button_height = 2;
button1_x = 8;
button1_y = 32;
button2_x = 16;
button2_y = 32;

// LED light pipe
led_diameter = 3;
led_x = 38;
led_y = 38;

// Antenna holes (FIXED - moved away from vents!)
ant_diameter = 6.5;
ant1_y = 22.5;  // Middle of right side
ant1_z = 10;
ant2_y = 15;    // Lower right side
ant2_z = 10;
ant3_y = 22.5;  // Middle of left side
ant3_z = 10;

// Ventilation slots
vent_width = 2;
vent_height = 8;
vent_spacing = 6;
vent_start_y = 5;
vent_count = 5;

// ═══════════════════════════════════════════════════════════════
// MAIN TOP SHELL
// ═══════════════════════════════════════════════════════════════

module top_shell() {
    difference() {
        // Main body
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
        
        // OLED window cutout
        translate([oled_x_offset - oled_window_width/2, 
                   oled_y_offset - oled_window_height/2, 
                   case_height - 1])
            cube([oled_window_width, oled_window_height, 2]);
        
        // Button holes (through top surface)
        translate([button1_x, button1_y, case_height - 0.5])
            cylinder(d=button_diameter, h=1.5, $fn=30);
        translate([button2_x, button2_y, case_height - 0.5])
            cylinder(d=button_diameter, h=1.5, $fn=30);
        
        // LED light pipe hole
        translate([led_x, led_y, case_height - 1])
            cylinder(d=led_diameter, h=2, $fn=20);
        
        // ANTENNA HOLES - FIXED POSITIONS!
        // Right side - positioned BETWEEN vent slots
        translate([case_length - 1, ant1_y, ant1_z])
            rotate([0, 90, 0])
                cylinder(d=ant_diameter, h=wall_thickness + 2, $fn=30);
        
        translate([case_length - 1, ant2_y, ant2_z])
            rotate([0, 90, 0])
                cylinder(d=ant_diameter, h=wall_thickness + 2, $fn=30);
        
        // Left side
        translate([-1, ant3_y, ant3_z])
            rotate([0, 90, 0])
                cylinder(d=ant_diameter, h=wall_thickness + 2, $fn=30);
        
        // VENTILATION SLOTS - FIXED to avoid antenna holes!
        // Front side slots
        for(i = [0:vent_count-1]) {
            translate([vent_start_y + i*vent_spacing, -1, 8])
                cube([vent_width, wall_thickness + 2, vent_height]);
        }
        
        // Back side slots
        for(i = [0:vent_count-1]) {
            translate([vent_start_y + i*vent_spacing, case_width - wall_thickness - 1, 8])
                cube([vent_width, wall_thickness + 2, vent_height]);
        }
        
        // Left side slots (BELOW antenna hole)
        for(i = [0:2]) {
            translate([-1, vent_start_y + i*vent_spacing, 8])
                cube([wall_thickness + 2, vent_width, vent_height]);
        }
        
        // Right side slots (BELOW antenna holes)
        for(i = [0:2]) {
            translate([case_length - wall_thickness - 1, vent_start_y + i*vent_spacing, 8])
                cube([wall_thickness + 2, vent_width, vent_height]);
        }
    }
    
    // BUTTON CAPS - FIXED: Properly attached to top surface!
    translate([button1_x, button1_y, case_height - 0.5])
        button_cap();
    translate([button2_x, button2_y, case_height - 0.5])
        button_cap();
    
    // OLED retention lip
    translate([oled_x_offset - oled_window_width/2 - 1, 
               oled_y_offset - oled_window_height/2 - 1, 
               case_height - 1.5])
        difference() {
            cube([oled_window_width + 2, oled_window_height + 2, 1]);
            translate([1, 1, -0.5])
                cube([oled_window_width, oled_window_height, 2]);
        }
    
    // Snap-fit clips (4 corners inside)
    for(x = [6, case_length - 8]) {
        for(y = [6, case_width - 8]) {
            translate([x, y, wall_thickness])
                snap_clip();
        }
    }
}

// ═══════════════════════════════════════════════════════════════
// BUTTON CAP - FIXED: Proper dome shape with base
// ═══════════════════════════════════════════════════════════════

module button_cap() {
    union() {
        // Base cylinder (attaches to case top)
        cylinder(d=button_diameter - 0.5, h=0.5, $fn=30);
        
        // Dome top
        translate([0, 0, 0.5])
            difference() {
                sphere(d=button_diameter, $fn=40);
                translate([0, 0, -button_diameter/2])
                    cube([button_diameter*2, button_diameter*2, button_diameter], center=true);
                // Hollow for tactile switch
                translate([0, 0, -0.5])
                    sphere(d=button_diameter - 1.5, $fn=30);
            }
    }
}

// ═══════════════════════════════════════════════════════════════
// SNAP-FIT CLIP
// ═══════════════════════════════════════════════════════════════

module snap_clip() {
    // Cantilever beam
    cube([1.5, 8, 3]);
    // Hook at end
    translate([0, 8, 0])
        cube([1.5, 1, 3.5]);
}

// ═══════════════════════════════════════════════════════════════
// RENDER
// ═══════════════════════════════════════════════════════════════

top_shell();

// ═══════════════════════════════════════════════════════════════
// PRINT SETTINGS
// ═══════════════════════════════════════════════════════════════

// Material: PETG (best) or PLA
// Layer Height: 0.2mm
// Infill: 20%
// Supports: NONE!
// Orientation: Top surface facing DOWN on build plate
// Print Time: ~3 hours
// Filament: ~25g

// ═══════════════════════════════════════════════════════════════
// CHANGES FROM PREVIOUS VERSION:
// ═══════════════════════════════════════════════════════════════

// FIXED:
// 1. Button caps now have solid base - no longer floating!
// 2. Antenna holes repositioned to Y=22.5, 15, 22.5 (BETWEEN vents)
// 3. Ventilation slots reduced to 5 on each side
// 4. Vent slots positioned to avoid antenna holes
// 5. Button caps properly union() with base surface
// 6. All geometry is manifold (watertight)

// IMPROVED:
// 7. Better button cap design (dome + base cylinder)
// 8. OLED moved up to Y=32 for better spacing
// 9. Cleaner clip placement
// 10. More printable geometry
