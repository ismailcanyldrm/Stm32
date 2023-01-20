#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"

int POAM= PD10;
int PABM= PE12;
int PEEM= PD13;

void GcodeSuite::M1998()
{
pinMode(POAM, OUTPUT);
pinMode(PABM, OUTPUT);
pinMode(PEEM, OUTPUT);
digitalWrite(POAM, HIGH);
digitalWrite(PABM, LOW);
digitalWrite(PEEM, HIGH);
}
