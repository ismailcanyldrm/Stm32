#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"
int AS= PC6;
int CS1= PD11;
int MS1= PD10;
//-----------------------
int BS= PE15;
int CS2= PE13;
int MS2= PE12;
//-----------------------
int SM= PE11;
//-----------------------
int SH= PE8;
//---------------
//int rrr= PD13;
//---------------
int m1= PA5;
int m2= PA6;

              // A-B KOMPONENT - STOP KONUMU

void GcodeSuite::M2828()
{
  Serial.begin(250000);
  pinMode(AS, OUTPUT);
  pinMode(CS1, OUTPUT);
  pinMode(MS1, OUTPUT);
  //-----------------------
  pinMode(BS, OUTPUT);
  pinMode(CS2, OUTPUT);
  pinMode(MS2, OUTPUT);
  //-----------------------
  pinMode(SM, OUTPUT);
  //-----------------------
  pinMode(SH, OUTPUT);
  //pinMode(rrr, OUTPUT);
  //-----------------------
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  //-----------------------
digitalWrite(AS, LOW);
digitalWrite(CS1, LOW);
digitalWrite(MS1, LOW);
//-----------------------
digitalWrite(BS, LOW);
digitalWrite(CS2, LOW);
digitalWrite(MS2, LOW);
//-----------------------
digitalWrite(SM, LOW);
//-----------------------
digitalWrite(SH, LOW);
//digitalWrite(rrr, LOW);
//-----------------------
digitalWrite(m1, LOW);
digitalWrite(m2, LOW);

}
