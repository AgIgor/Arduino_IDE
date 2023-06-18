int binaryNumber = 0b10101010;

#define numLeds 8
#define pinLedInit 2

byte leds[] = {
  0b00000011, //0
  0b10011111, //1
  0b00100101, //2
  0b00001101, //3
  0b10011001, //4
  0b01001001, //5
  0b01000001, //6
  0b00011111, //7
  0b00000001, //8
  0b00001001, //9
  
//   0b10000001,
//   0b01000010,
//   0b00100100,
//   0b00011000,
//   0b00011000,
//   0b00100100,
//   0b01000010,
//   0b10000001,

//   0b10000000,
//   0b11000000,
//   0b11100000,
//   0b11110000,
//   0b11111000,
//   0b11111100,
//   0b11111110,
//   0b11111111,

//   0b00000000,
//   0b00000011,
//   0b00000111,
//   0b00001111,
//   0b00011111,
//   0b00111111,
//   0b01111111,
//   0b11111111,

//   0b11111111,
//   0b11111110,
//   0b11111100,
//   0b11111000,
//   0b11110000,
//   0b11100000,
//   0b11000000,
//   0b00000000,
};

void setup() {
  //Serial.begin(115200);
  for(byte i = pinLedInit; i < numLeds+pinLedInit ;i++){
    pinMode(i, OUTPUT);
    digitalWrite(i,LOW);
  }
}//end setup

void loop() {

  for(byte bin = 0; bin < sizeof(leds); bin++){
    for(byte pin = 0; pin < 8; pin++){
      digitalWrite(pin+2, bitRead(leds[bin],pin));
      Serial.print(bitRead(leds[bin],pin));
      Serial.print("-");
    }
    delay(350);
    Serial.println();
    Serial.println();
  }
  delay(1);




}
