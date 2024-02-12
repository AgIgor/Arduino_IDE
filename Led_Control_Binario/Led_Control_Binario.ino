#include "LedControl.h"
#include "digitos.h"

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
#define MAX_DEVICES 4

LedControl lc=LedControl(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

int Hora, Min, Seg, Dia, Mes, Ano;
int dezenaH, unidadeH, dezenaM, unidadeM;

void setup() {
  Serial.begin(115200);

  for(int i=0;i<MAX_DEVICES;i++){
    lc.shutdown(i,false);
    lc.setIntensity(i,8);
    lc.clearDisplay(i);
  }

}//end setup

void getTime(){
  tmElements_t tm;
  RTC.read(tm);
  Hora = tm.Hour;
  Min = tm.Minute;
  Seg = tm.Second;
  Dia = tm.Day;
  Mes = tm.Month;
  Ano = tmYearToCalendar(tm.Year);
}//end get time

void writeArduinoOnMatrix(byte addr, byte digi) {

  for(int i=0;i<8;i++){
    lc.setRow(addr,i,NUMBERS[digi][i]);
  }

}//end writeArduinoOnMatrix

bool blinkDots(){
  bool blink = (millis()/1000)%2;

  //lc.setLed(2, 1, 7, blink);//lc.setLed(1, 1, 0, blink);
  lc.setLed(2, 2, 7, blink);//lc.setLed(1, 2, 0, blink);

  lc.setLed(2, 5, 7, blink);//lc.setLed(1, 5, 0, blink);
  //lc.setLed(2, 6, 7, blink);//lc.setLed(1, 6, 0, blink);
  return blink;

}//end blink dots

void trataDigitos(){
  // Hora = (millis()/10000)%23;
  // Minuto = (millis()/1000)%60;

  getTime();

  dezenaH = Hora;
  unidadeH = dezenaH;
  dezenaH = dezenaH/10;
  unidadeH = unidadeH % 10;

  dezenaM = Min;
  unidadeM = dezenaM;
  dezenaM = dezenaM/10;
  unidadeM = unidadeM % 10;

  // Serial.print(dezenaH);
  // Serial.print(unidadeH);
  // Serial.print(":");
  // Serial.print(dezenaM);
  // Serial.println(unidadeM);
  // Serial.println();

}//end trata digitos


void loop() {  

  if(!blinkDots()){
    trataDigitos();
    writeArduinoOnMatrix(0,unidadeM);
    writeArduinoOnMatrix(1,dezenaM);
    writeArduinoOnMatrix(2,unidadeH);
    writeArduinoOnMatrix(3,dezenaH);
  }

  //lc.setLed(int addr, int row, int col, boolean state)


  delay(1);
  

}//end loop
