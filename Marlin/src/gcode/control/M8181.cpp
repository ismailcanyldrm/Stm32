/**#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"

#include "../../inc/MarlinConfig.h"

int u= PE10;

void GcodeSuite::M8181()
{
pinMode(u, OUTPUT);
digitalWrite(u, HIGH);
//powerManager.power_on();

}
*/
