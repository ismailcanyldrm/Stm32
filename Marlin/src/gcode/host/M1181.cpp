#include "../gcode.h"
#include "../../core/serial.h"

int Esp = PE9;
void GcodeSuite::M1181() {
  char *p = parser.string_arg;

  pinMode(Esp, OUTPUT);

  if (p[0] == 'E'){
    digitalWrite(Esp,HIGH);
  }
  else if (p[0] == 'N'){
    digitalWrite(Esp,LOW);
  }
  else
  {
    LCD_SERIAL.print(p);
  }
}
