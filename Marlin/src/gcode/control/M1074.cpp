#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../inc/MarlinConfig.h"
#include "../../core/serial.h"

int dd13= PD13;

void GcodeSuite::M1074()
{
    char *p = parser.string_arg;
    if (p[0] =='A'){
        pinMode(dd13, OUTPUT);
        digitalWrite(dd13, HIGH);
        Serial.println("high");
    }
    if(p[0]=='B'){
        pinMode(dd13, OUTPUT);
        digitalWrite(dd13, LOW);
        Serial.println("low");
    }
}