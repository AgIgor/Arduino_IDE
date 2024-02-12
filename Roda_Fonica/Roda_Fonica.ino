#define SAIDA 13
#define pinopot A1
#define DENTES 60
#define FALHAS 2

int velo;

void setup() {
  Serial.begin(115200);
  pinMode(SAIDA, OUTPUT);
  pinMode(pinopot, INPUT);
}

void loop() {
  velo = map(analogRead(pinopot), 0, 1023, 6000, 0);
  for(int i=0; i<(DENTES-FALHAS)*2; i++){
    digitalWrite(SAIDA, !digitalRead(SAIDA));
    delayMicroseconds(velo);                  
  } 
  digitalWrite(SAIDA, LOW);
  delayMicroseconds(velo*2);                 
}
