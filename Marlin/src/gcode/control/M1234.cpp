#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"
#include "../../lcd/extui/nextion/nextion_tft.h"
#include "../../lcd/extui/nextion/nextion_tft_defs.h"
#include "../../lcd/extui/nextion/FileNavigator.h"

int ON= PE10;
int paga= PA4;

void GcodeSuite::M1234()
{
pinMode(ON, OUTPUT); // RÖLE KAPATIYOR. KART KAPALI
digitalWrite(ON, LOW);
Serial.println("MACHINE CLOSE");
pinMode(paga, OUTPUT); // RÖLE KAPATIYOR. KART KAPALI
digitalWrite(paga, LOW);

//LCD_SERIAL.print("1");
// SERIAL_ECHOPGM("\xFF\xFF\xFF");
// SERIAL_ECHOPGM(" t41.txt=\"deneme yazısı bu yazı t41 a yazılacak\"");
// SERIAL_ECHOPGM("\xFF\xFF\xFF");
}
