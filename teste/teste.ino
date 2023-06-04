#include "Arduino.h"
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Adafruit_AHT10.h>

Adafruit_AHT10 aht;

#define NUM_MAX 4
#define LINE_WIDTH 8
#define ROTATE  90

#define DIN_PIN 11
#define CS_PIN  10
#define CLK_PIN 13

#include "max7219.h"
#include "fonts.h"


void setup() {
  Serial.begin(115200);
  
  Serial.print("Last Update: ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);

  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN,1);
  sendCmdAll(CMD_INTENSITY,0); // Adjust the brightness between 0 and 15
  //printStringWithShift("Connecting ",16);
  aht.begin();
}//end setup

#define MAX_DIGITS 4
byte dig[MAX_DIGITS]={0};
byte digold[MAX_DIGITS]={0};
byte digtrans[MAX_DIGITS]={0};
String date;
int updCnt = 0;
int dots = 0;
long dotTime = 0;
long clkTime = 0;
int dx=0;
int dy=0;
byte del=0;
int h, m, s, dia, mes, ano;
String tmp, humi;
// long localEpoc = 0;
// long localMillisAtUpdate = 0;

void loop(){
  if(updCnt<=0) { // every 10 scrolls, ~450s=7.5m
    updCnt = 10;
    //Serial.println("Getting data ...");
    //printStringWithShift("  Getting data",20);
   
    getTime();
    //Serial.println("Data loaded");
    clkTime = millis();
  }
  if(millis()-clkTime > 20000 && !del && dots) { // clock for 15s, then scrolls for about 30s 20000
    getTemp();
    printStringWithShift(tmp.c_str(),40);
    delay(7000);//7000
    printStringWithShift(humi.c_str(),40);
    delay(7000);//7000
    printStringWithShift(date.c_str(),40);
    delay(7000);//7000
    printStringWithShift(date.c_str(),40);
    delay(7000);//7000
    updCnt--;
    clkTime = millis();
  }
  if(millis()-dotTime > 500) {
    dotTime = millis();
    dots = !dots;
  }
  updateTime();
  showAnimClock();
  //showSimpleClock();
    
  // Adjusting LED intensity.
  // 12am to 6am, lowest intensity 0
  //if ( (h == 0) || ((h >= 1) && (h <= 6)) ) sendCmdAll(CMD_INTENSITY, 0);
  // 6pm to 11pm, intensity = 2
  //else if ( (h >=18) && (h <= 23) ) sendCmdAll(CMD_INTENSITY, 2);
  // max brightness during bright daylight
  //else sendCmdAll(CMD_INTENSITY, 10);
  //delay(500);
}//end loop

void getTemp(){
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  tmp = "       " + String(int(temp.temperature)) + "'C";
  humi = "      " + String(int(humidity.relative_humidity)) + "%";
}//end getTemp

void showSimpleClock(){
  dx=dy=0;
  clr();
  showDigit(h/10,  4, dig3x8);
  showDigit(h%10,  12, dig3x8);
  showDigit(m/10, 21, dig3x8);
  showDigit(m%10, 29, dig3x8);
  showDigit(s/10, 38, dig3x8);
  showDigit(s%10, 46, dig3x8);
  setCol(19,dots ? B00100100 : 0);
  setCol(36,dots ? B00100100 : 0);
  refreshAll();
}//end showSimpleClock

// int fontID = 0;
// uint8_t fontSLCT [] = {dig3x8};


void showAnimClock(){
  byte digPos[4]={1,8,17,25};
  int digHt = 12;
  int num = 4; 
  int i;
  if(del==0) {
    del = digHt;
    for(i=0; i<num; i++) digold[i] = dig[i];
    dig[0] = h/10 ? h/10 : 10;
    dig[1] = h%10;
    dig[2] = m/10;
    dig[3] = m%10;
    for(i=0; i<num; i++)  digtrans[i] = (dig[i]==digold[i]) ? 0 : digHt;
  } else
    del--;
  
  clr();
  for(i=0; i<num; i++) {
    if(digtrans[i]==0) {
      dy=0;
      showDigit(dig[i], digPos[i], dig6x8);
    } else {
      dy = digHt-digtrans[i];
      showDigit(digold[i], digPos[i], dig6x8);
      dy = -digtrans[i];
      showDigit(dig[i], digPos[i], dig6x8);
      digtrans[i]--;
    }
  }
  dy=0;
  setCol(15,dots ? B00100100 : 0);
  refreshAll();
 delay(30);
}//end showAnimClock

void showDigit(char ch, int col, const uint8_t *data){
  if(dy<-8 | dy>8) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for (int i = 0; i < w; i++)
    if(col+i>=0 && col+i<8*NUM_MAX) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy>0 ? v>>dy : v<<-dy;
    }
}//end showDigit

void setCol(int col, byte v){
  if(dy<-8 | dy>8) return;
  col += dx;
  if(col>=0 && col<8*NUM_MAX)
    if(!dy) scr[col] = v; else scr[col] |= dy>0 ? v>>dy : v<<-dy;
}//end setCol

int showChar(char ch, const uint8_t *data){
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[NUM_MAX*8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[NUM_MAX*8 + i] = 0;
  return w;
}//end showChar

void printCharWithShift(unsigned char c, int shiftDelay){
  
  if (c < ' ' || c > '~'+25) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i=0; i<w+1; i++) {
    delay(shiftDelay);
    scrollLeft();
    refreshAll();
  }
}//end printCharWithShift

void printStringWithShift(const char* s, int shiftDelay){
  while (*s) {
    printCharWithShift(*s, shiftDelay);
    s++;
  }
}//end printStringWithShift

void getTime(){
  // date = "31/05/2023";
  // h = 12;
  // m = 34;
  // s = 56;

  tmElements_t tm;
  if (RTC.read(tm)) {
    h = tm.Hour;
    m = tm.Minute;
    s = tm.Second;
    dia = tm.Day;
    mes = tm.Month;
    ano = tmYearToCalendar(tm.Year)-2000;

    date =  "    " + String(dia < 10 ? "0" + String(dia) : String(dia))  + "/" + 
            String(mes < 10 ? "0" + String(mes) : String(mes)) + " ";// + "/" + 
            //String(ano);
  }
}//end getTime

void updateTime(){
  // h = h;//random(0, 24);
  // m = m;//random(0, 60);
  // s = s;//random(0, 60);
  getTime();
}//end updateTime