#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"
int AA= PC6;
int CA1= PD11;
int MA1= PD10;
//-----------------------
int BA= PE15;
int CA2= PE13;
int MA2= PE12;
//-----------------------
int MK1= PE11;
//-----------------------
int HH= PE8;
//---------------
//int RR= PD13;

              // A KOMPONENT - START KONUMU

void GcodeSuite::M6189()
{
  Serial.begin(250000);
  pinMode(AA, OUTPUT);
  pinMode(CA1, OUTPUT);
  pinMode(MA1, OUTPUT);
  //-----------------------
  pinMode(BA, OUTPUT);
  pinMode(CA2, OUTPUT);
  pinMode(MA2, OUTPUT);
  //---------------------
  pinMode(MK1, OUTPUT);
  //-----------------------
  pinMode(HH, OUTPUT);
  //-----------------------
  //pinMode(RR, OUTPUT);
  //-----------------------
digitalWrite(AA, HIGH);
digitalWrite(CA1, LOW);
digitalWrite(MA1, LOW);
//-----------------------
digitalWrite(BA, LOW);
digitalWrite(CA2, LOW);
digitalWrite(MA2, LOW);
//------------------------
digitalWrite(MK1, LOW);
//-----------------------
digitalWrite(HH, HIGH);
//-----------------------
//digitalWrite(RR, LOW);
}
