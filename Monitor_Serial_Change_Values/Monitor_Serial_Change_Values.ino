#include <EEPROM.h>


int v1 = 0;
float v2 = 0.0;
int led;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  led = EEPROM.read(3);
}

void loop() {
  
  if (Serial.available() > 0) { // Verifica se há dados disponíveis no monitor serial
    String input = Serial.readStringUntil('\n'); // Lê a string enviada pelo monitor serial
    input.trim(); // Remove espaços em branco do início e do fim da string
    
    int index = input.indexOf("=");
    
    if (input.startsWith("v1")) {
      v1 = input.substring(index + 1).toInt();
      EEPROM.write(v1, 1);
    }
    else if (input.startsWith("v2")) {
      v2 = input.substring(index + 1).toFloat();
      EEPROM.write(v2, 2);
    }
    else if(input.startsWith("led")){
      led = input.substring(index + 1).toInt();
      EEPROM.write(led, 3);
    }
      
  }//end if serial available
  else{
    
    Serial.print(v1);
    Serial.print(" | ");
    Serial.print(v2);
    Serial.print(" | ");
    Serial.println(led);
    
    digitalWrite(LED_BUILTIN, led);
  }
	delay(50);
}
