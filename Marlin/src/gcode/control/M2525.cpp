#include "../gcode.h"
#include "../../module/endstops.h"

/**
 * M2525: Output filament states to serial output and nextion screen
 */
void GcodeSuite::M2525() {

  endstops.filament();
  

}