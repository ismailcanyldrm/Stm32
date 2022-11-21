#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F
#include "../queue.h"
#include "../../feature/power.h"
#include "../../inc/MarlinConfig.h"
// neckog kart için yapıldı.
int AZ= PC6;
int C1Z= PD11;
int M1Z= PD10;
//-----------------------
int BZ= PE15;
int C2Z= PE13;
int M2Z= PE12;
//-----------------------
int MKZ= PE11; //karıştırma için
//-----------------------
int HZ= PE8;
//------------------
int rrZ= PD13;



              // A-B KOMPONENT - START KONUMU

void GcodeSuite::M6100()
{
  Serial.begin(250000);
  pinMode(AZ, OUTPUT);
  pinMode(C1Z, OUTPUT);
  pinMode(M1Z, OUTPUT);
  //-----------------------
  pinMode(BZ, OUTPUT);
  pinMode(C2Z, OUTPUT);
  pinMode(M2Z, OUTPUT);
  //-----------------------
  pinMode(MKZ, OUTPUT);
  //-----------------------
  pinMode(HZ, OUTPUT);
  pinMode(rrZ, OUTPUT);

  //-----------------------
digitalWrite(AZ, HIGH);
digitalWrite(C1Z, HIGH);
digitalWrite(M1Z, HIGH);
//-----------------------
digitalWrite(BZ, HIGH);
digitalWrite(C2Z, HIGH);
digitalWrite(M2Z, HIGH);
//-----------------------
digitalWrite(MKZ, HIGH);
//-----------------------
digitalWrite(HZ, HIGH);
digitalWrite(rrZ, HIGH);
//-----------------------
Serial.println("Hepsi açık");
}
