/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(ASSISTED_TRAMMING)

#include "../gcode.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../feature/bedlevel/bedlevel.h"

#if HAS_MULTI_HOTEND
  #include "../../module/tool_change.h"
#endif

#if ENABLED(BLTOUCH)
  #include "../../feature/bltouch.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

//
// Define tramming point names.
//

#include "../../feature/tramming.h"

#include <iostream>
#include <string>

/**
 * G35: Read bed corners to help adjust bed screws
 *
 *   S<screw_thread>
 *
 * Screw thread: 30 - Clockwise M3
 *               31 - Counter-Clockwise M3
 *               40 - Clockwise M4
 *               41 - Counter-Clockwise M4
 *               50 - Clockwise M5
 *               51 - Counter-Clockwise M5
 **/
void GcodeSuite::G35() {
  int x = 0 ;
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  // SERIAL_ECHOPGM("p0.pic=122");
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  // SERIAL_ECHOPGM("p1.pic=122");
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  // SERIAL_ECHOPGM("p2.pic=123");
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  // SERIAL_ECHOPGM("p3.pic=123");
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  // SERIAL_ECHOPGM("t40.txt=\"deneme yazisi t41 a yazilacak\"");
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");

  // SERIAL_ECHOPGM("\xFF\xFF\xFF");
  // SERIAL_ECHOPGM("t41.txt=\"","deneme","a","yaz\"");
  // SERIAL_ECHOPGM("\xFF\xFF\xFF");




  DEBUG_SECTION(log_G35, "G35", DEBUGGING(LEVELING));

  if (DEBUGGING(LEVELING)) log_machine_info();

  float z_measured[G35_PROBE_COUNT] = { 0 };

  const uint8_t screw_thread = parser.byteval('S', TRAMMING_SCREW_THREAD);
  if (!WITHIN(screw_thread, 30, 51) || screw_thread % 10 > 1) {
    SERIAL_ECHOLNPGM("?(S)crew thread must be 30, 31, 40, 41, 50, or 51.");
    return;
  }

  // Wait for planner moves to finish!
  planner.synchronize();

  // Disable the leveling matrix before auto-aligning
  #if HAS_LEVELING
    #if ENABLED(RESTORE_LEVELING_AFTER_G35)
      const bool leveling_was_active = planner.leveling_active;
    #endif
    set_bed_leveling_enabled(false);
  #endif

  #if ENABLED(CNC_WORKSPACE_PLANES)
    workspace_plane = PLANE_XY;
  #endif

  // Always home with tool 0 active
  #if HAS_MULTI_HOTEND
    const uint8_t old_tool_index = active_extruder;
    tool_change(0, true);
  #endif

  // Disable duplication mode on homing
  TERN_(HAS_DUPLICATION_MODE, set_duplication_enabled(false));

  // Home only Z axis when X and Y is trusted, otherwise all axes, if needed before this procedure
  if (!all_axes_trusted()) process_subcommands_now(F("G28Z"));

  bool err_break = false;

  // Probe all positions
  LOOP_L_N(i, G35_PROBE_COUNT) {

    // In BLTOUCH HS mode, the probe travels in a deployed state.
    // Users of G35 might have a badly misaligned bed, so raise Z by the
    // length of the deployed pin (BLTOUCH stroke < 7mm)
    do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES + TERN0(BLTOUCH, bltouch.z_extra_clearance()));
    const float z_probed_height = probe.probe_at_point(tramming_points[i], PROBE_PT_RAISE, 0, true);

    if (isnan(z_probed_height)) {
      SERIAL_ECHOPGM("G35 failed at point ", i + 1, " (");
      SERIAL_ECHOPGM_P((char *)pgm_read_ptr(&tramming_point_name[i]));
      SERIAL_CHAR(')');
      SERIAL_ECHOLNPGM_P(SP_X_STR, tramming_points[i].x, SP_Y_STR, tramming_points[i].y);
      err_break = true;
      break;
    }

    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOPGM("Probing point ", i + 1, " (");
      DEBUG_ECHOF(FPSTR(pgm_read_ptr(&tramming_point_name[i])));
      DEBUG_CHAR(')');
      DEBUG_ECHOLNPGM_P(SP_X_STR, tramming_points[i].x, SP_Y_STR, tramming_points[i].y, SP_Z_STR, z_probed_height);
    }

    z_measured[i] = z_probed_height;
  }

  if (!err_break) {
    const float threads_factor[] = { 0.5, 0.7, 0.8 };

    // Calculate adjusts
    LOOP_S_L_N(i, 1, G35_PROBE_COUNT) {
      const float diff = z_measured[0] - z_measured[i],
                  adjust = ABS(diff) < 0.001f ? 0 : diff / threads_factor[(screw_thread - 30) / 10];

      const int full_turns = trunc(adjust);
      const float decimal_part = adjust - float(full_turns);
      const int minutes = trunc(decimal_part * 360.0f);
         
      
      if (x == 1){
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        SERIAL_ECHOPGM("t4.txt=\"","Turn ","",(char *)pgm_read_ptr(&tramming_point_name[2]), (screw_thread & 1) == (adjust > 0) ? "CCW" : "CW","\n\"") ;
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        SERIAL_ECHOPGM("t40.txt=\"",ABS(full_turns) , " Turns"," and ", "", ABS(minutes), " Degree\n\"");
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        if ( (screw_thread & 1) == (adjust > 0)){
          SERIAL_ECHOPGM("\xFF\xFF\xFF");
          SERIAL_ECHOPGM("p1.pic=171");
          SERIAL_ECHOPGM("\xFF\xFF\xFF");
          if (ABS(minutes)<=22.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=172");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=45){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=173");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=67.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=174");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=90){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=175");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=112.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=176");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=135){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=177");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=157.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=178");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=180){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=179");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=202.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=180");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=225){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=181");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=247.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=182");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=270){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=183");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=292.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=184");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=315){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=185");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=337.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=186");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=360){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=187");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
        }
        else{
          SERIAL_ECHOPGM("\xFF\xFF\xFF");
          SERIAL_ECHOPGM("p1.pic=207");
          SERIAL_ECHOPGM("\xFF\xFF\xFF");
          if (ABS(minutes)<=22.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=188");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=45){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=189");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=67.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=190");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=90){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=191");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=112.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=192");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=135){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=193");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=157.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=194");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=180){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=195");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=202.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=196");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=225){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=197");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=247.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=198");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=270){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=199");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=292.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=200");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=315){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=201");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=337.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=202");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=360){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p3.pic=203");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
        }
      } 
      if (x == 0){
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        SERIAL_ECHOPGM("t41.txt=\"","Turn ","",(char *)pgm_read_ptr(&tramming_point_name[1]), (screw_thread & 1) == (adjust > 0) ? "CCW" : "CW", " by ", "", ABS(full_turns) , " turns"," and ", "", ABS(minutes), " degree\n\"");
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        if ( (screw_thread & 1) == (adjust > 0)){
          SERIAL_ECHOPGM("\xFF\xFF\xFF");
          SERIAL_ECHOPGM("p0.pic=171");
          SERIAL_ECHOPGM("\xFF\xFF\xFF");
          if (ABS(minutes)<=22.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=172");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=45){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=173");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=67.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=174");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=90){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=175");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=112.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=176");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=135){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=177");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=157.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=178");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=180){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=179");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=202.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=180");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=225){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=181");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=247.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=182");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=270){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=183");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=292.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=184");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=315){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=185");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=337.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=186");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=360){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=187");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          
        }
        else{
          SERIAL_ECHOPGM("\xFF\xFF\xFF");
          SERIAL_ECHOPGM("p0.pic=207");
          SERIAL_ECHOPGM("\xFF\xFF\xFF");
          if (ABS(minutes)<=22.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=188");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=45){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=189");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=67.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=190");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=90){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=191");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=112.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=192");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=135){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=193");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=157.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=194");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=180){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=195");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=202.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=196");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=225){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=197");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=247.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=198");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=270){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=199");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=292.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=200");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=315){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=201");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=337.5){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=202");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }
          else if (ABS(minutes)<=360){
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
            SERIAL_ECHOPGM("p2.pic=203");
            SERIAL_ECHOPGM("\xFF\xFF\xFF");
          }


          
        }
       
        //SERIAL_EOL();
        x=+1;

      }
      if (ENABLED(REPORT_TRAMMING_MM)) SERIAL_ECHOPGM(" (", -diff, "mm)");
      
    }
  
  }
  else
    SERIAL_ECHOLNPGM("G35 aborted.");

  // Restore the active tool after homing
  #if HAS_MULTI_HOTEND
    tool_change(old_tool_index, DISABLED(PARKING_EXTRUDER)); // Fetch previous toolhead if not PARKING_EXTRUDER
  #endif

  #if BOTH(HAS_LEVELING, RESTORE_LEVELING_AFTER_G35)
    set_bed_leveling_enabled(leveling_was_active);
  #endif

  // Stow the probe, as the last call to probe.probe_at_point(...) left
  // the probe deployed if it was successful.
  probe.stow();

  move_to_tramming_wait_pos();

  // After this operation the Z position needs correction
  set_axis_never_homed(Z_AXIS);
}

#endif // ASSISTED_TRAMMING
