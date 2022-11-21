#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"

int ON= PE10;

void GcodeSuite::M1234()
{
pinMode(ON, OUTPUT); // RÖLE KAPATIYOR. KART KAPALI
digitalWrite(ON, LOW);
Serial.println("MACHINE CLOSE");
}
