#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"

int POOAM= PD10;
int POABM= PE12;

void GcodeSuite::M1994()
{
pinMode(POOAM, OUTPUT);
pinMode(POABM, OUTPUT);
digitalWrite(POOAM, LOW);
digitalWrite(POABM, HIGH);
}
