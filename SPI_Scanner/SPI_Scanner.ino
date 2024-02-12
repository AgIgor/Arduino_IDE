#include <SPI.h>

void setup() {
  // Inicializa a comunicação serial para depuração
  Serial.begin(115200);
  
  // Inicializa a comunicação SPI
  SPI.begin();
  
  // Define a ordem dos bits (MSB ou LSB primeiro)
  SPI.setBitOrder(MSBFIRST);
  
  // Define a divisão de frequência do clock SPI
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  
  // Define o modo de operação do SPI
  SPI.setDataMode(SPI_MODE0);
}

void loop() {
  for (int i = 0; i < 256; i++) {
    // Seleciona o dispositivo escravo
    digitalWrite(SS, LOW);
    
    // Envia um byte de endereço
    //SPI.transfer(i);
    
    // Lê um byte de dados do dispositivo escravo
    byte data = SPI.transfer(i);
    
    // Deseleciona o dispositivo escravo
    digitalWrite(SS, HIGH);
    
    // Exibe os dados lidos via serial
    Serial.print("Endereço 0x");
    Serial.print(i, HEX);
    Serial.print(": ");
    Serial.println(data);
    
    // Aguarda um breve intervalo antes de continuar para o próximo endereço
    delay(10);
  }
}
