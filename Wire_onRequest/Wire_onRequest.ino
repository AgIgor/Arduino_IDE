#include <Wire.h>

struct Data
{
  int steering;
  int throttle;
  int motor;
  int servo;
};

Data myData;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Wire.begin(8);
  // Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

int x;

void loop()
{
  myData.steering = analogRead(A2);
  myData.throttle = millis()/1000;

  digitalWrite(LED_BUILTIN, HIGH);
  delay(15); // Wait for 1000 millisecond(s)
  digitalWrite(LED_BUILTIN, LOW);
  delay(15); // Wait for 1000 millisecond(s)
  
  Wire.beginTransmission(4); // transmit to device #8
  Wire.write((byte *)&myData, sizeof myData);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  x++;
    
}

// void receiveEvent(int howMany) {
//  while (Wire.available()) {
//     Wire.readBytes((byte*)&myData, sizeof(myData));
//     Serial.print( myData.led1);
//   }
//   Serial.println();
// }

void requestEvent() {
 //Wire.write("111222333444555666777888999");  /*send string on request */
 Wire.write((byte *)&myData, sizeof myData);  /*send string on request */
}