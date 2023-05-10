// Define o pino do tacômetro
const int pinTach = 2;

// Variáveis para o cálculo da velocidade do motor
volatile unsigned int pulses = 0; // Contagem de pulsos do tacômetro
unsigned long lastTime = 0;       // Hora da última leitura do tacômetro
unsigned int rpm = 0;             // Velocidade do motor em RPM

void setup() {
  // Configura o pino do tacômetro como entrada
  pinMode(pinTach, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Habilita as interrupções externas para o pino do tacômetro
  attachInterrupt(digitalPinToInterrupt(pinTach), countPulses, RISING);
  
  // Inicializa a porta serial para exibição de informações
  Serial.begin(115200);
}

void loop() {
  digitalWrite(LED_BUILTIN, digitalRead(pinTach));
  // Lê a velocidade do motor a cada 1000ms
  if (millis() - lastTime >= 1000) {

    // Calcula a velocidade do motor em RPM
    // rpm = (pulses / 2) * 60 / 12;
    rpm = pulses * 60;
    
    // Exibe a velocidade do motor na porta serial
    Serial.print("RPM: ");
    Serial.println(rpm);
    
    // Reinicia a contagem de pulsos
    pulses = 0;
    
    // Atualiza a hora da última leitura
    lastTime = millis();
  }
}

void countPulses() {
  // Incrementa a contagem de pulsos
  pulses++;

}
