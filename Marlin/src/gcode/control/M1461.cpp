#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"
int AT= PC6;
int CT1= PD11;
int MT1= PD10;
//-----------------------
int BT= PE15;
int CT2= PE13;
int MT2= PE12;
//-----------------------
int MK3= PE11;
//-----------------------
int HC= PE8;
//-----------------------
//int RO= PD13;

              // TEMİZLİK - START KONUMU

void GcodeSuite::M1461()
{
  Serial.begin(250000);
  pinMode(AT, OUTPUT);
  pinMode(CT1, OUTPUT);
  pinMode(MT1, OUTPUT);
  //-----------------------
  pinMode(BT, OUTPUT);
  pinMode(CT2, OUTPUT);
  pinMode(MT2, OUTPUT);
  //-----------------------
  pinMode(MK3, OUTPUT);
  //-----------------------
  pinMode(HC, OUTPUT);
  //-----------------------
  //pinMode(RO, OUTPUT);
  //-----------------------
digitalWrite(AT, LOW);
digitalWrite(CT1, HIGH);
digitalWrite(MT1, LOW);
//-----------------------
digitalWrite(BT, LOW);
digitalWrite(CT2, HIGH);
digitalWrite(MT2, LOW);
//-----------------------
digitalWrite(MK3, LOW);
//-----------------------
digitalWrite(HC, LOW);
//-----------------------
//digitalWrite(RO, LOW);
}
