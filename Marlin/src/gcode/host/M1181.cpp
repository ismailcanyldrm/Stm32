#include "../gcode.h"
#include "../../core/serial.h"

/**
 * M1181: Display a message in the host console.
 *
 *  A1  Prepend '// ' for an action command, as in OctoPrint
 *  E1  Have the host 'echo:' the text
 *  Pn  Redirect to another serial port
 *        0 : Announce to all ports
 *      1-9 : Serial ports 1 to 9
 */
void GcodeSuite::M1181() {
  bool hasE = false, hasA = false;
  #if HAS_MULTI_SERIAL
    int8_t port = -1; // Assume no redirect
  #endif
  char *p = parser.string_arg;
  for (uint8_t i = 3; i--;) {
    // A1, E1, and Pn are always parsed out
    if (!( ((p[0] == 'A' || p[0] == 'E') && p[1] == '1') || (p[0] == 'P' && NUMERIC(p[1])) )) break;
    switch (p[0]) {
      case 'A': hasA = true; break;
      case 'E': hasE = true; break;
      #if HAS_MULTI_SERIAL
        case 'P': port = p[1] - '0'; break;
      #endif
    }
    p += 2;
    while (*p == ' ') ++p;
  }

  PORT_REDIRECT(WITHIN(port, 0, NUM_SERIAL) ? (port ? SERIAL_PORTMASK(port - 1) : SerialMask::All) : multiSerial.portMask);

  if (hasE) SERIAL_ECHO_START();
  if (hasA) SERIAL_ECHOPGM("//");
  LCD_SERIAL.print(p);

}