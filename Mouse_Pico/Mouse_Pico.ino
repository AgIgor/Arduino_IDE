#include "PluggableUSBHID.h"
#include "USBMouse.h"
#include <SPI.h>

int SCLK = 5;
int SDIO = 6;
int NCS  = 7;

USBMouse Mouse;

void setup() {
  Serial.begin(115200);
  pinMode(SCLK, OUTPUT);
  pinMode(SDIO, OUTPUT);
  pinMode(NCS, OUTPUT);
  
  mouse_reset();
  delay(10);
  Serial.println("ok");

}

void mouse_reset(){
  // Initiate chip reset
  digitalWrite(NCS, LOW);
  pushbyte(0x3a);
  pushbyte(0x5a);
  digitalWrite(NCS, HIGH);
  delay(10);
  // Set 1000cpi resolution
  digitalWrite(NCS, LOW);
  pushbyte(0x0d);
  pushbyte(0x01);
  digitalWrite(NCS, HIGH);
}

unsigned int readLoc(uint8_t addr){
  unsigned int ret=0;
  digitalWrite(NCS, LOW);
  pushbyte(addr);
  ret=pullbyte();
  digitalWrite(NCS, HIGH);
  return(ret);
}

void pushbyte(uint8_t c){
  pinMode(SDIO, OUTPUT);
  for(unsigned int i=0x80;i;i=i>>1){
    digitalWrite(SCLK, LOW);
    digitalWrite(SDIO, c & i);
    digitalWrite(SCLK, HIGH);
  }
}

unsigned int pullbyte(){
  unsigned int ret=0;
  pinMode(SDIO, INPUT);
  for(unsigned int i=0x80; i>0; i>>=1) {
    digitalWrite(SCLK, LOW);
    ret |= i*digitalRead(SDIO);
    digitalWrite(SCLK, HIGH);
  }
  pinMode(SDIO, OUTPUT);
  return(ret);
}

void loop() {
  unsigned int x = readLoc(0x03);
  unsigned int y = readLoc(0x04);
  unsigned int sql = readLoc(0x05);

  unsigned int mb = readLoc(0x63);
  unsigned int maxp = readLoc(0x08);
  unsigned int minp = readLoc(0x0a);

  unsigned int motion = readLoc(0x02);
  if(motion > 1){
    Mouse.move(x,y);
    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);
  }
  delay(20);
}
