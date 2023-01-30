#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"
int AAA= PC6;
int CAA1= PD11;
int MAA1= PD10;
//-----------------------
int BAA= PE15;
int CAA2= PE13;
int MAA2= PE12;
//-----------------------
int MK2= PE11;
//-----------------------
int HHH= PE8;
//-----------------------
//int RRR= PD13;

              // B KOMPONENT - START KONUMU

void GcodeSuite::M6191()
{
  Serial.begin(250000);
  pinMode(AAA, OUTPUT);
  pinMode(CAA1, OUTPUT);
  pinMode(MAA1, OUTPUT);
  //-----------------------
  pinMode(BAA, OUTPUT);
  pinMode(CAA2, OUTPUT);
  pinMode(MAA2, OUTPUT);
  //--------------------
  pinMode(MK2, OUTPUT);
  //-----------------------
  pinMode(HHH, OUTPUT);
  //-----------------------
  //pinMode(RRR, OUTPUT);
  //-----------------------
digitalWrite(AAA, LOW);
digitalWrite(CAA1, LOW);
digitalWrite(MAA1, LOW);
//-----------------------
digitalWrite(BAA, HIGH);
digitalWrite(CAA2, LOW);
digitalWrite(MAA2, LOW);
//------------------------
digitalWrite(MK2, LOW);
//-----------------------
digitalWrite(HHH, HIGH);
//-----------------------
//digitalWrite(RRR, LOW);
}
