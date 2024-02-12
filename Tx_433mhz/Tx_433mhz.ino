//==== Inclusão de bibliotecas ====//
#include <VirtualWire.h>

//==== Mapemamento de Hardware e constantes =====//
#define Size 2
#define pinButton 3
#define pinPot A0

//===== Variáveis globais ========//
byte TX_buffer[2];


void setup(){
  //Entradas/Saídas digitais
  pinMode(pinButton, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  vw_set_tx_pin(2); //Seleciona o pino de comunicação do TX 
  vw_setup(3000); // Velocidade de comunicação em BPS
}//endSetup

long msDelay;
void loop(){

  if(millis() - msDelay > 500){
    msDelay = millis();

    TX_buffer[0] = millis()/1000;
    TX_buffer[1] = !TX_buffer[1];
    digitalWrite(LED_BUILTIN, TX_buffer[1]);

    Serial.print("PWM: ");
    Serial.print(TX_buffer[0]);
    Serial.print("   Button: ");
    Serial.println(TX_buffer[1]);

    vw_send(TX_buffer, Size);
    vw_wait_tx();

  }
}

/* void loop(){
  while ((millis()/1000)%6 < 5) {

    TX_buffer[0] = millis()/1000;//map(analogRead(pinPot), 0, 1023, 0, 255);

    if ((millis()/100)%2) {
      TX_buffer[1] = 1;
      digitalWrite(LED_BUILTIN, TX_buffer[1]);
    }
    else {
      TX_buffer[1] = 0;
      digitalWrite(LED_BUILTIN, TX_buffer[1]);
    }

    vw_send(TX_buffer, Size);
    vw_wait_tx();

    }//end while
  Serial.println("End While");

  delay(200);
}//endLoop */
