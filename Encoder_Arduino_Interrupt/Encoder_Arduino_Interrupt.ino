const int pinA = 2;
const int pinB = 3;
const int pinBot = 12;
const int pinLed = 13;

volatile int encoderPos = 0;
volatile int lastEncoded = 0;

void setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinBot, INPUT_PULLUP);
  pinMode(pinLed, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pinA), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), updateEncoder, CHANGE);
  
  Serial.begin(115200);
}

void loop() {
  Serial.println(encoderPos);
  digitalWrite(pinLed, !digitalRead(pinBot));
  //updateEncoder();
  delay(1);
}

void updateEncoder() {
  int MSB = digitalRead(pinA);  // Valor do bit mais significativo
  int LSB = digitalRead(pinB);  // Valor do bit menos significativo
  
  int encoded = (MSB << 1) | LSB;  // Combinação dos bits em um número
  //Serial.println(encoded, BIN);
  
  int sum = (lastEncoded << 2) | encoded;  // Combinação do estado anterior com o estado atual
  
  // Atualiza a posição do encoder
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderPos+=5;
  } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderPos-=5;
  }
  
  lastEncoded = encoded;  // Armazena o estado atual para a próxima iteração
}
