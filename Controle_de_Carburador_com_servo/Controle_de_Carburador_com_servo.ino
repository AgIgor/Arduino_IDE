#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const int potentiometerPin = A0;  // Pino do potenciômetro
const int hallSensorPin = 2;      // Pino do sensor Hall
const int servoPin = 9;           // Pino do servo motor

// Constantes do PID
const float Kp = 1.0;  // Ganho proporcional
const float Ki = 0.2;  // Ganho integral
const float Kd = 0.1;  // Ganho derivativo

// Variáveis do PID
float setpoint = 0.0;    // Valor setado pelo potenciômetro (ângulo desejado)
float input = 0.0;       // Valor lido do sensor Hall (RPM)
float output = 0.0;      // Valor de saída (ângulo do servo motor)
float previousError = 0; // Erro anterior
float integral = 0.0;    // Termo integral

Servo servo;

byte rot[] = {124, 47, 176};
byte idx;
// define custom characters/arrays - every character is 5x8 "pixels"
byte gauge_empty[8] =  {B11111, B00000, B00000, B00000, B00000, B00000, B00000, B11111};    // empty middle piece
byte gauge_fill_1[8] = {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111};    // filled gauge - 1 column
byte gauge_fill_2[8] = {B11111, B11000, B11000, B11000, B11000, B11000, B11000, B11111};    // filled gauge - 2 columns
byte gauge_fill_3[8] = {B11111, B11100, B11100, B11100, B11100, B11100, B11100, B11111};    // filled gauge - 3 columns
byte gauge_fill_4[8] = {B11111, B11110, B11110, B11110, B11110, B11110, B11110, B11111};    // filled gauge - 4 columns
byte gauge_fill_5[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};    // filled gauge - 5 columns
byte gauge_left[8] =   {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111};    // left part of gauge - empty
byte gauge_right[8] =  {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B11111};    // right part of gauge - empty

byte gauge_mask_left[8] = {B01111, B11111, B11111, B11111, B11111, B11111, B11111, B01111};  // mask for rounded corners for leftmost character
byte gauge_mask_right[8] = {B11110, B11111, B11111, B11111, B11111, B11111, B11111, B11110}; // mask for rounded corners for rightmost character

byte warning_icon[8] = {B00100, B00100, B01110, B01010, B11011, B11111, B11011, B11111};     // warning icon - just because we still have one custom character left

byte gauge_left_dynamic[8];   // left part of gauge dynamic - will be set in the loop function
byte gauge_right_dynamic[8];  // right part of gauge dynamic - will be set in the loop function

int cpu_gauge = 0;       // value for the CPU gauge
char buffer[10];         // helper buffer to store C-style strings (generated with sprintf function)
int move_offset = 0;     // used to shift bits for the custom characters

const int gauge_size_chars = 16;       // width of the gauge in number of characters
char gauge_string[gauge_size_chars+1]; // string that will include all the gauge character to be printed


void setup() {
  servo.attach(servoPin);
  pinMode(hallSensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(hallSensorPin), countPulse, RISING);
  Serial.begin(9600);
}

unsigned int rpm = 0;
unsigned long lastTime = 0;
volatile unsigned int pulseCount = 0;

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 200) {
    detachInterrupt(digitalPinToInterrupt(hallSensorPin));
    rpm = (pulseCount * 60000) / (currentTime - lastTime);
    Serial.println(pulseCount);
    pulseCount = 0;
    lastTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(hallSensorPin), countPulse, RISING);
  }

  setpoint = map(analogRead(potentiometerPin), 0, 1023, 0, 180);

  float error = setpoint - input;
  float proportional = Kp * error;
  integral += Ki * error;
  float derivative = Kd * (error - previousError);
  output = proportional + integral + derivative;
  output = constrain(output, 0, 180);
  servo.write(output);
  Serial.print("Setpoint: ");
  Serial.print(setpoint);
  Serial.print(" | Input: ");
  Serial.print(input);
  Serial.print(" | Output: ");
  Serial.println(output);
  previousError = error;
  delay(1);
}//end loop

void countPulse() {
  pulseCount++;
}//countPulse
