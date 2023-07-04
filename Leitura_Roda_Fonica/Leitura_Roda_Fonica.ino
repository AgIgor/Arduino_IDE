const int ledPin = 13;
const int pinoSensor = 2; // Pino digital usado para ler o sinal da roda fônica
const int numDentes = 60; // Número total de dentes na roda fônica
const int numFalhas = 2; // Número de falhas na roda fônica

volatile int contagemPulsos = 0; // Contagem de pulsos
long int currentInt;
int dly, maxDly;

void setup() {
  Serial.begin(9600);
  //pinMode(pinoSensor, INPUT_PULLUP);
  // pinMode(ledPin, OUTPUT);

  DDRB |= (1 << 5); //pino 13 como saida
  PORTB &= ~(1 << 5); //pino 13 em LOW

  DDRB |= ~(1 << 4);  //pino 2 como entrada
  PORTB &= (1 << 4);  //pino 2 em HIGH


  //attachInterrupt(digitalPinToInterrupt(pinoSensor), detectarPulso, FALLING);

  while(1){
    // PORTD = (PIND & (1<<2)) ?  contagemPulsos++ : 0;//(PORTB &= ~(1<<5)) : (PORTB |= (1<<5));

    if (PINB & (1<<4)){
      //contagemPulsos++;
      // Serial.print("RPM: ");
     
      // dly = (millis() - currentInt);
      // Serial.print(dly);
      // Serial.println();

      // if(contagemPulsos >= (numDentes - numFalhas)){
      //   contagemPulsos = 0;
      //   // PORTB |= (1 << 5);
      //   // delayMicroseconds(500);
      //   // PORTB &= ~(1 << 5);
      // }

    }else{
      currentInt = millis();
    }
    Serial.println(millis() - currentInt);

  }
}

// void loop() {


//   // if(contagemPulsos >= numDentes){
//   // }
// }

// void detectarPulso() {
//   contagemPulsos++;
//   Serial.print("RPM: ");
//   Serial.println(contagemPulsos);
// }
// void ignit(){
//   digitalWrite(ledPin, HIGH);
//   delayMicroseconds(5);
//   digitalWrite(ledPin, LOW);
// }
