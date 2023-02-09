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

#include "../gcode.h"
#include "../../core/serial.h"

/**
 * M118: Display a message in the host console.
 *
 *  A1  Prepend '// ' for an action command, as in OctoPrint
 *  E1  Have the host 'echo:' the text
 *  Pn  Redirect to another serial port
 *        0 : Announce to all ports
 *      1-9 : Serial ports 1 to 9
 */
int x = 0;
int y = 0;
int z = 0;
void GcodeSuite::M118() {

  char *p = parser.string_arg;

  if (p[0] == 'A'){
    y = 1;
  }
  if ((y == 1)&& !(p[0]=='A')){
    SERIAL_ECHOPGM("\xFF\xFF\xFF");
    SERIAL_ECHOPGM("t0645.txt=\"","",p,"\"");
    SERIAL_ECHOPGM("\xFF\xFF\xFF");
    y=0;
    z=1;
  }


  if (p[0] == 'C'){
    x = 0;
  }
  if(!(p[0]=='A') && !(p[0]=='C') && !(z == 1)){
    SERIAL_ECHOPGM("\xFF\xFF\xFF");
    if (x == 0){SERIAL_ECHOPGM("b100.txt=\"");}
    if (x == 1){SERIAL_ECHOPGM("b101.txt=\"");}
    if (x == 2){SERIAL_ECHOPGM("b102.txt=\"");}
    if (x == 3){SERIAL_ECHOPGM("b103.txt=\"");}
    if (x == 4){SERIAL_ECHOPGM("b104.txt=\"");}
    if (x == 5){SERIAL_ECHOPGM("b105.txt=\"");}
    if (x == 6){SERIAL_ECHOPGM("b106.txt=\"");}
    if (x == 7){SERIAL_ECHOPGM("b107.txt=\"");}
    if (x == 8){SERIAL_ECHOPGM("b108.txt=\"");}
    SERIAL_ECHOPGM(p);
    SERIAL_ECHOPGM("\"\xFF\xFF\xFF");
    x+=1;
  }
}
