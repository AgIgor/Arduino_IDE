int freq = 4050, time = 126, dly = 230;

void setup() {
    Serial.begin(115200);
    pinMode(7, OUTPUT);

    for(int i=0;i<5;i++){
      tone(7, freq, time);
      delay(dly);
      noTone(7);
    }

/*
126
126
126
126
126 
*/

}

void loop() {
  if (Serial.available() > 0) { // Verifica se há dados disponíveis no monitor serial
    String input = Serial.readStringUntil('\n'); // Lê a string enviada pelo monitor serial
    input.trim(); // Remove espaços em branco do início e do fim da string
    
    int index = input.indexOf("=");
    
    if (input.startsWith("f")) {
      freq = input.substring(index + 1).toInt();
    }
    else if (input.startsWith("t")) {
      time = input.substring(index + 1).toFloat();
    }
    else if(input.startsWith("d")){
      dly = input.substring(index + 1).toFloat();
    }

    Serial.print(freq);
    Serial.print(" | ");
    Serial.print(time);
    Serial.print(" | ");
    Serial.println(dly);

    tone(7, freq, time);
    delay(dly);
    noTone(7);
      
  }//end if serial available
  /* else{
    
    Serial.print(freq);
    Serial.print(" | ");
    Serial.print(time);
    Serial.print(" | ");
    Serial.println(dly);

    // tone(7, freq, time);
    // delay(dly);
    // noTone(7);
    
  } */
	delay(50);
}
