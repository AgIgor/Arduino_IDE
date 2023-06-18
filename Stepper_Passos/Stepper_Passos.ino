#define pinM1 8 
#define pinM2 9 
#define pinM3 10 
#define pinM4 11

#define steps 100
#define microSecs 700
int val;

void stepperCC(int pos){
  byte stepsSimplesCC[]={
    0b00101,
    0b01001,
    0b01011,
    0b01010,
    0b11010,
    0b10010,
    0b10110,
    0b10100,
    0b10101,
  };
  while (val < pos) {
    val++;
    for(byte bin = 0; bin < sizeof(stepsSimplesCC); bin++){
      for(byte pin = 0; pin < 5; pin++){
        digitalWrite(pin+8, bitRead(stepsSimplesCC[bin],pin));
      }
      delayMicroseconds(microSecs);
    }
    Serial.println("CC");
  }
}//stepperCC

void stepperC(int pos){
  byte stepsSimples[]={
    0b01101,
    0b01001,
    0b01011,
    0b01010,
    0b11010,
    0b10010,
    0b10110,
    0b10100,
    0b10101,
    0b00101,
  };
  while (val > pos) {
    val--;
    for(byte bin = 0; bin < sizeof(stepsSimples); bin++){
      for(byte pin = 0; pin < 5; pin++){
        digitalWrite(pin+8, bitRead(stepsSimples[bin],pin));
      }
      delayMicroseconds(microSecs);
    }
    Serial.println("C");
  }
}//stepperC

void setup() {
  pinMode(pinM1, OUTPUT);
  pinMode(pinM2, OUTPUT);
  pinMode(pinM3, OUTPUT);
  pinMode(pinM4, OUTPUT);

  Serial.begin(115200);

}//setup

void loop() {
  // stepperCC(100);
  // delay(5);
  // stepperCC(200);
  // delay(5);
  // stepperCC(300);
  // delay(5);
  stepperCC(400);
  delay(5);

  stepperC(0);
  delay(5000);

}//loop
