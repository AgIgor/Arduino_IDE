const byte pino1 = 2; // Pino 2 do PORTD
const byte pino2 = 3; // Pino 3 do PORTD
const byte pino3 = 4; // Pino 4 do PORTD

void setup() {
  // Configuração dos pinos como saídas usando o registrador DDRD
  //DDRD |= (1 << pino1) | (1 << pino2) | (1 << pino3);
  

  // Define os estados iniciais dos pinos usando o registrador PORTD
  //PORTD &= ~((1 << pino1) | (1 << pino2) | (1 << pino3));

  DDRD |= B11111100;
  DDRB |= B00000011;



}

void loop() {
  PORTD = (B10101000);
  PORTB = (B00000010);
  delay(1000);
  PORTD = (B01010100);
  PORTB = (B00000001);
  delay(1000);
  PORTD = (B10101000);
  PORTB = (B00000010);
  delay(1000);
  PORTD = (B01010100);
  PORTB = (B00000001);
  delay(1000);
  PORTD = (B10101000);
  PORTB = (B00000010);
  delay(1000);
  PORTD = (B01010100);
  PORTB = (B00000001);
  delay(1000);
}
