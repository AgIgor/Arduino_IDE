#define ENC_A 2
#define ENC_B 3

void setup() {
  Serial.begin(115200);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
}//end setup

byte getEncoder(){
  static byte a, b;
  byte seq[5]={0,1,11,10,0};
  a = b;
  b = !digitalRead(ENC_A)*10 + !digitalRead(ENC_B);
  if(a != b){
    for(byte i=1;i<5;i++){
      if(a == seq[i-1] && seq[i] == b) return 1;
    }
    return 2;
  }
  return 0;
}//byte get encoder

int val;
byte oldEnc, newEnc; 
void loop() {
  newEnc = getEncoder();

  if(oldEnc != newEnc){
    oldEnc = newEnc;
    if(newEnc == 1){
      if(val <100) val++;
    }
    if(newEnc == 2){
      if(val > 0) val--;
    }
    Serial.print(val);
    Serial.println();
  }
  delay(10);
}//end loop
