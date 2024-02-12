#include <Adafruit_AHT10.h>
Adafruit_AHT10 aht;

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#include "fonts.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

#define SPEED 80
#define PAUSE 10000//20000

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Global variables
char msg[] = "19:33";

const uint8_t degC[] = { 6, 3, 3, 56, 68, 68, 68 };
const uint8_t heart[] = {0x00,0x66,0xE7,0xFF,0x7E,0x3C,0x18,0x00,};
const uint8_t percent[] = {0xc2, 0xe7, 0x72, 0x38, 0x1c, 0x4e, 0xe7, 0x43};


// Sprite Definition
const uint8_t F_ROCKET = 2;
const uint8_t W_ROCKET = 11;
const uint8_t PROGMEM rocket[F_ROCKET * W_ROCKET] = {
  0x18, 0x24, 0x42, 0x81, 0x99, 0x18, 0x99, 0x18, 0xa5, 0x5a, 0x81,
  0x18, 0x24, 0x42, 0x81, 0x18, 0x99, 0x18, 0x99, 0x24, 0x42, 0x99,
};

const uint8_t F_GHOST = 2;
const uint8_t W_GHOST = 10;
const uint8_t PROGMEM ghost[F_GHOST * W_GHOST] = {      
  0,248, 157, 54, 94, 94, 54, 157, 252,0,
  0,248, 157, 118, 158, 158, 118, 157, 252,0,  
};

const uint8_t F_PMAN1 = 6;
const uint8_t W_PMAN1 = 8;
static const uint8_t PROGMEM pacman1[F_PMAN1 * W_PMAN1] = {
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
};
 
const uint8_t F_PMAN2 = 6;
const uint8_t W_PMAN2 = 18;
static const uint8_t PROGMEM pacman2[F_PMAN2 * W_PMAN2] = {
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
};

// char  szTimeL[6];    // mm:ss\0
// char  szTimeH[6];

String tmp, hum;
int h, m, s, dia, mes, ano;
String date;

int val = 0;
bool t = true;
const char *saida;
String saidaSTR;

void setup(void){
  P.begin();
  P.setIntensity(1);
  P.addChar('$', degC);
  //P.setFont(0);
  P.displayClear();
  
  Serial.begin(115200);
  if (! aht.begin()) {
    Serial.println("Could not find AHT10? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 found");
  getTemp();
  getTime();
}//end setup

void getTime(){
  tmElements_t tm;
  if (RTC.read(tm)) {
    h = tm.Hour;
    m = tm.Minute;
    s = tm.Second;
    dia = tm.Day;
    mes = tm.Month;
    ano = tmYearToCalendar(tm.Year)-2000;

    date =  String(dia < 10 ? "0" + String(dia) : String(dia))  + "/" + String(mes < 10 ? "0" + String(mes) : String(mes));// + "/" + //String(ano);
    saidaSTR = String(h < 10 ? "0" + String(h) : String(h))  + ":" + String(m < 10 ? "0" + String(m) : String(m));

  }
}//end getTime

void getTemp(){
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  tmp = String(int(temp.temperature)) + "$";
  hum = String(int(humidity.relative_humidity)) + "%";

}//end getTemp

void loop(){
  
  if((millis()/1000)%2){
    if(t){
      getTemp();
      getTime();
      t = false;
      //val++;
      //saida = saidaSTR.c_str();
      //Serial.println(saidaSTR);
    }

  }else t = true;

  switch(val){
    case 0:
      while (P.displayAnimate())  {
        P.setFont(0);
        P.setSpriteData(rocket, W_ROCKET, F_ROCKET, rocket, W_ROCKET, F_ROCKET);
        P.displayText(saidaSTR.c_str(), PA_CENTER, SPEED, PAUSE, PA_SPRITE, PA_SPRITE);
        P.displayReset();
        val++;
      }
      break;

    case 1:
      while (P.displayAnimate())  {
        P.setFont(0);
        P.setSpriteData(ghost, W_GHOST, F_GHOST, ghost, W_GHOST, F_GHOST);
        P.displayText(tmp.c_str(), PA_CENTER, SPEED, PAUSE, PA_SPRITE, PA_SPRITE);
        P.displayReset();
        val++;
      }
      break;

    case 2:
      while (P.displayAnimate())  {
        P.setFont(0);
        P.setSpriteData(pacman1, W_PMAN1, F_PMAN1, pacman1, W_PMAN1, F_PMAN1);
        P.displayText(hum.c_str(), PA_CENTER, SPEED, PAUSE, PA_SPRITE, PA_SPRITE);
        P.displayReset();
        val++;
      }
      break;

    case 3:
      while (P.displayAnimate())  {
        P.setFont(0);
        P.setSpriteData(pacman2, W_PMAN2, F_PMAN2, pacman2, W_PMAN2, F_PMAN2);
        P.displayText(date.c_str(), PA_CENTER, SPEED, PAUSE, PA_SPRITE, PA_SPRITE);
        P.displayReset();
        val++;
      }
      break;

    // case 4:
    //   while (P.displayAnimate())  {
    //     //P.setFont(dig6x8);
    //     P.setSpriteData(pacman2, W_PMAN2, F_PMAN2, pacman2, W_PMAN2, F_PMAN2);
    //     P.displayText(0, PA_CENTER, SPEED, PAUSE, PA_SPRITE, PA_SPRITE);
    //     P.displayReset();
    //     val++;
    //   }
    //   break;

    default:
      val = 0;
      break;
  }



  
  

  
}//end loop


