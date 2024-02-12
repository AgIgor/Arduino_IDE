#define ch_a 2
#define ch_b 3

int opt = 0, MAX_OPT = 5;
bool key = false;

void setup() {
  pinMode(ch_a, INPUT_PULLUP);
  pinMode(ch_b, INPUT_PULLUP);

  Serial.begin(115200);

}//end setup

byte codeKey(){
  static byte a, b;
  byte seq[5]={0, 1, 11, 10, 0};
  a = b;
  b = !digitalRead(ch_a)*10 + !digitalRead(ch_b);

  if(a != b){
    for(int i=1; i<5; i++){
      if(a==seq[i-1]&&seq[i]==b) return 1;
    }
    return 2;
  }
  return 0;
}//end code key

void loop() {
  byte encode = codeKey();

  if(encode == 1){
   if(++opt >= MAX_OPT) opt=0;
  }
  if(encode == 2){
    if(--opt < 0) opt=MAX_OPT;
  }
  key = true;
  for(int i=0; i<3; i++) if(!digitalRead(i)) key = false;
  delay(90);
  

  Serial.println(opt);
  switch(opt){
    case 0:
    Serial.println("case 0");
    break;
    case 1:
    Serial.println("case 1");
    break;
    case 2:
    Serial.println("case 2");
    break;
    case 3:
    Serial.println("case 3");
    break;
    case 4:
    Serial.println("case 4");
    break;
    case 5:
    Serial.println("reset");
    //opt = 0;
    break;
  }
  //delay(10);

}//end loop
