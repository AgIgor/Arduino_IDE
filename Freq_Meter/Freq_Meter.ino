#define pulse_ip 3

int ontime,offtime,duty;
float freq,period;
   
void setup(){
  pinMode(pulse_ip,INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
}
void loop(){
  ontime = pulseIn(pulse_ip,HIGH);
  offtime = pulseIn(pulse_ip,LOW);
  period = ontime+offtime;
  freq = 1000000.0/period;
  duty = (ontime/period)*100; 

  if(period==0){ 
  freq=0;
  }
  Serial.print(freq);
  Serial.print(" Hz ");
  Serial.print(freq * 60);
  Serial.println(" Rpm ");

  delay(100);
}