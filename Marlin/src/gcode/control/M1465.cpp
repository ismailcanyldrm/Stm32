#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"
int ATTT= PC6;
int CTTT1= PD11;
int MTTT1= PD10;
//-----------------------
int BTTT= PE15;
int CTTT2= PE13;
int MTTT2= PE12;
//-----------------------
int MK5= PE11;
//-----------------------
int HCCC= PE8;
//-----------------------
//int ROOO= PD13;

              // B-TEMİZLİK - START KONUMU

void GcodeSuite::M1465()
{
  Serial.begin(250000);
  pinMode(ATTT, OUTPUT);
  pinMode(CTTT1, OUTPUT);
  pinMode(MTTT1, OUTPUT);
  //-----------------------
  pinMode(BTTT, OUTPUT);
  pinMode(CTTT2, OUTPUT);
  pinMode(MTTT2, OUTPUT);
  //-----------------------
  pinMode(MK5, OUTPUT);
  //-----------------------
  pinMode(HCCC, OUTPUT);
  //-----------------------
  //pinMode(ROOO, OUTPUT);
  //-----------------------
digitalWrite(ATTT, LOW);
digitalWrite(CTTT1, LOW);
digitalWrite(MTTT1, LOW);
//-----------------------
digitalWrite(BTTT, LOW);
digitalWrite(CTTT2, HIGH);
digitalWrite(MTTT2, LOW);
//-----------------------
digitalWrite(MK5, LOW);
//-----------------------
digitalWrite(HCCC, LOW);
//-----------------------
//digitalWrite(ROOO, LOW);
}
