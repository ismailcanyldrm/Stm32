#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"
int ATT= PC6;
int CTT1= PD11;
int MTT1= PD10;
//-----------------------
int BTT= PE15;
int CTT2= PE13;
int MTT2= PE12;
//-----------------------
int MK4= PE11;
//-----------------------
int HCC= PE8;
//-----------------------
//int ROO= PD13;

              // A-TEMİZLİK - START KONUMU

void GcodeSuite::M1463()
{
  Serial.begin(250000);
  pinMode(ATT, OUTPUT);
  pinMode(CTT1, OUTPUT);
  pinMode(MTT1, OUTPUT);
  //-----------------------
  pinMode(BTT, OUTPUT);
  pinMode(CTT2, OUTPUT);
  pinMode(MTT2, OUTPUT);
  //-----------------------
  pinMode(MK4, OUTPUT);
  //-----------------------
  pinMode(HCC, OUTPUT);
  //-----------------------
  //pinMode(ROO, OUTPUT);
  //-----------------------
digitalWrite(ATT, LOW);
digitalWrite(CTT1, HIGH);
digitalWrite(MTT1, LOW);
//-----------------------
digitalWrite(BTT, LOW);
digitalWrite(CTT2, LOW);
digitalWrite(MTT2, LOW);
//---------------------
digitalWrite(MK4, LOW);
//-----------------------
digitalWrite(HCC, LOW);
//-----------------------
//digitalWrite(ROO, LOW);
}
