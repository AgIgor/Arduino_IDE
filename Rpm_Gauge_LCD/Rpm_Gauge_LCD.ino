#include <LiquidCrystal_I2C.h>
// #include <Encoder.h>

LiquidCrystal_I2C lcd(0x27,16,2);
// Encoder myEnc(5,6);
// long oldPosition  = -999;

#define maxRPM 5000
#define mutplRPM 10
#define suavizacao 0.1

#define timeLcd 300
#define timeHallRead 200

#define HallPin 2
#define modePin 7

volatile unsigned int pulseCount = 0;  // Variável para armazenar a contagem de pulsos
unsigned int rpm = 0;  // Variável para armazenar as RPM calculadas
unsigned long lastTime = 0;  // Variável para armazenar o tempo da última leitura
unsigned int rpmFinal = 0;
unsigned long lastTimePrint = 0;
/*
  // int ontime,
  //     offtime,
  //     duty, 
  //     freq,
  //     period,
  //     rpm;
*/
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


void setup(){
  Serial.begin(9600);
  pinMode(HallPin, INPUT_PULLUP);
  pinMode(modePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HallPin), countPulse, RISING);

  bool mode = !digitalRead(modePin);

  if(mode)
  Serial.println("mode 1");
  else
  Serial.println("mode 2");


  lcd.init();                       // initialize the 16x2 lcd module
  lcd.createChar(7, gauge_empty);   // middle empty gauge
  lcd.createChar(1, gauge_fill_1);  // filled gauge - 1 column
  lcd.createChar(2, gauge_fill_2);  // filled gauge - 2 columns
  lcd.createChar(3, gauge_fill_3);  // filled gauge - 3 columns
  lcd.createChar(4, gauge_fill_4);  // filled gauge - 4 columns  
  lcd.createChar(0, warning_icon); // warning icon - just because we have one more custom character that we could use  
  lcd.backlight();                  // enable backlight for the LCD module
}//end setup

void loop(){
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= timeHallRead) {
    detachInterrupt(digitalPinToInterrupt(HallPin));  // Desabilita interrupções para cálculos
    
    rpm = (pulseCount * 60000) / (currentTime - lastTime);  // Cálculo das RPM
    Serial.println(pulseCount);
    pulseCount = 0;  // Zera a contagem de pulsos
    
    lastTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(HallPin), countPulse, RISING);  // Habilita interrupções novamente
  }

  rpmFinal = (suavizacao * rpm) + ((1.0 - suavizacao) * rpmFinal);
  printLcd();
  /*
    // ontime = pulseInLong(HallPin,HIGH);
    // offtime = pulseInLong(HallPin,LOW);
    // period = ontime + offtime;
    // freq = 1000000.0/period;
    // duty = (ontime/period)*100;
    // if(period==0) freq=0;
    // rpm = freq * 60;
    // Serial.print(freq);
    // Serial.print(" Hz ");
    // Serial.print(rpm);
    // Serial.println(" Rpm ");
  */
}//end loop

void countPulse() {
  pulseCount++;
}//countPulse

void printLcd(){
  char *multiplicador = "    ";
  // cpu_gauge = rpmFinal;
  if(rpmFinal < maxRPM) cpu_gauge = rpmFinal;
  else {
    cpu_gauge = rpmFinal / 10;
    multiplicador = " x10 "; 
  } 

  if (cpu_gauge < 0) cpu_gauge = 0;
  if (cpu_gauge > maxRPM) cpu_gauge = maxRPM;
  /*
    //long newPosition = rpm;
    // long newPosition = myEnc.read() * mutplRPM;
    //if (newPosition != oldPosition) {
      //oldPosition = newPosition;
      // Serial.println(cpu_gauge);
    //}
  */
  if(cpu_gauge > 0){
    static unsigned long tempDelay;
    if(millis() - tempDelay > 300){
      tempDelay = millis();
      if(idx < sizeof(rot)-1) idx++;
      else      idx = 0;
      lcd.setCursor(15,0);
      lcd.write(rot[idx]);
    }
  }else{
    lcd.setCursor(15,0);
    lcd.write(0);
  }

  float units_per_pixel = (gauge_size_chars*5.0)/maxRPM;        //  every character is 5px wide, we want to count from 0-100
  int value_in_pixels = round(cpu_gauge * units_per_pixel);    // cpu_gauge value converted to pixel width

  int tip_position = 0;      // 0= not set, 1=tip in first char, 2=tip in middle, 3=tip in last char

  if (value_in_pixels < 5) {tip_position = 1;}                            // tip is inside the first character
  else if (value_in_pixels > gauge_size_chars*5.0-5) {tip_position = 3;}  // tip is inside the last character
  else {tip_position = 2;}                                                // tip is somewhere in the middle

  move_offset = 4 - ((value_in_pixels-1) % 5);      // value for offseting the pixels for the smooth filling

  for (int i=0; i<8; i++) {   // dynamically create left part of the gauge
     if (tip_position == 1) {gauge_left_dynamic[i] = (gauge_fill_5[i] << move_offset) | gauge_left[i];}  // tip on the first character
     else {gauge_left_dynamic[i] = gauge_fill_5[i];}                                                     // tip not on the first character
 
     gauge_left_dynamic[i] = gauge_left_dynamic[i] & gauge_mask_left[i];                                 // apply mask for rounded corners
  }

  for (int i=0; i<8; i++) {   // dynamically create right part of the gauge
     if (tip_position == 3) {gauge_right_dynamic[i] = (gauge_fill_5[i] << move_offset) | gauge_right[i];}  // tip on the last character
     else {gauge_right_dynamic[i] = gauge_right[i];}                                                       // tip not on the last character

     gauge_right_dynamic[i] = gauge_right_dynamic[i] & gauge_mask_right[i];                                // apply mask for rounded corners
  }  

  lcd.createChar(5, gauge_left_dynamic);     // create custom character for the left part of the gauge
  lcd.createChar(6, gauge_right_dynamic);    // create custom character for the right part of the gauge

  for (int i=0; i<gauge_size_chars; i++) {  // set all the characters for the gauge
      if (i==0) {gauge_string[i] = byte(5);}                        // first character = custom left piece
      else if (i==gauge_size_chars-1) {gauge_string[i] = byte(6);}  // last character = custom right piece
      else {                                                        // character in the middle, could be empty, tip or fill
         if (value_in_pixels <= i*5) {gauge_string[i] = byte(7);}   // empty character
         else if (value_in_pixels > i*5 && value_in_pixels < (i+1)*5) {gauge_string[i] = byte(5-move_offset);} // tip
         else {gauge_string[i] = byte(255);}                        // filled character
      }
  }

  if(millis() - lastTimePrint > timeLcd){
    lastTimePrint = millis();
    // gauge drawing
    lcd.setCursor(0,0);                         // move cursor to top left
    sprintf(buffer, "Rpm:%3d% ", cpu_gauge);    // set a string as CPU: XX%, with the number always taking at least 3 character
    lcd.print(buffer);                          // print the string on the display
    lcd.print(multiplicador);
    //lcd.write(byte(0));                         // print warning character  
  }
  
  lcd.setCursor(0,1);              // move the cursor to the next line
  lcd.print(gauge_string);         // display the gauge

  // increase the CPU value, set between 0-100
  //   cpu_gauge = cpu_gauge +1;
  //   if (cpu_gauge > 100) {cpu_gauge = 0;}

  //delay(1);  // wait for a while - 100ms = update the screen 10x in a second
}//end print lcd