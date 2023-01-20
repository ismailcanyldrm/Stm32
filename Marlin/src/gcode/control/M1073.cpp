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

int PDD1= PD1;
int PDD2= PD0;
int PDD3= PD14;

void GcodeSuite::M1073()
{
    pinMode(PDD1, INPUT_PULLDOWN);
    if (digitalRead(PDD1)== HIGH){
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        SERIAL_ECHOPGM("pdd1.val=1");
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
    }
    if(digitalRead(PDD1)== LOW){
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        SERIAL_ECHOPGM("pdd1.val=0");
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
    }
    pinMode(PDD2, INPUT_PULLDOWN);
    if (digitalRead(PDD2)== HIGH){
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        SERIAL_ECHOPGM("pdd2.val=1");
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
    }
    if(digitalRead(PDD2)== LOW){
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        SERIAL_ECHOPGM("pdd2.val=0");
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
    }
    pinMode(PDD3, INPUT_PULLDOWN);
    if (digitalRead(PDD3)== HIGH){
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        SERIAL_ECHOPGM("pdd3.val=1");
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
    }
    if(digitalRead(PDD3)== LOW){
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
        SERIAL_ECHOPGM("pdd3.val=0");
        SERIAL_ECHOPGM("\xFF\xFF\xFF");
    }


}
