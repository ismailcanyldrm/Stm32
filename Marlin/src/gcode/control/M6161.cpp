#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"
int A= PC6; // neckog kart için yapıldı.
int C1= PD11;
int M1= PD10;
//-----------------------
int B= PE15;
int C2= PE13;
int M2= PE12;
//-----------------------
int MK= PE11; //karıştırma için
//-----------------------
int H= PE8;
//------------------
//int rr= PD13;
//--------------------
//int rol= PC5;

              // A-B KOMPONENT - START KONUMU

void GcodeSuite::M6161()
{
  Serial.begin(250000);
  pinMode(A, OUTPUT);
  pinMode(C1, OUTPUT);
  pinMode(M1, OUTPUT);
  //-----------------------
  pinMode(B, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(M2, OUTPUT);
  //-----------------------
  pinMode(MK, OUTPUT);
  //-----------------------
  pinMode(H, OUTPUT);
  //pinMode(rr, OUTPUT);
  //pinMode(rol, OUTPUT);
  //-----------------------
digitalWrite(A, HIGH);
digitalWrite(C1, LOW);
digitalWrite(M1, HIGH);
//-----------------------
digitalWrite(B, HIGH);
digitalWrite(C2, LOW);
digitalWrite(M2, HIGH);
//-----------------------
digitalWrite(MK, LOW);
//-----------------------
digitalWrite(H, HIGH);
//digitalWrite(rr, HIGH);
//-----------------------
//digitalWrite(rol, HIGH);
//-----------------------
Serial.println("Press button to START");
Serial.println("Motors");
}
