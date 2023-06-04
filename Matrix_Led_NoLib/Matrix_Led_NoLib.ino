#include "max7219.h"

// byte scr[NUM_MAX*8 + 8];


const uint8_t teste[10][8] = {
  {
    0b00000000,
    0b00011000,
    0b00011000,
    0b00111000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b01111110
  },{
    0b00000000,
    0b00111100,
    0b01100110,
    0b00000110,
    0b00001100,
    0b00110000,
    0b01100000,
    0b01111110
  },{
    0b00000000,
    0b00111100,
    0b01100110,
    0b00000110,
    0b00011100,
    0b00000110,
    0b01100110,
    0b00111100
  },{
    0b00000000,
    0b00001100,
    0b00011100,
    0b00101100,
    0b01001100,
    0b01111110,
    0b00001100,
    0b00001100
  },{
    0b00000000,
    0b01111110,
    0b01100000,
    0b01111100,
    0b00000110,
    0b00000110,
    0b01100110,
    0b00111100
  },{
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100000,
    0b01111100,
    0b01100110,
    0b01100110,
    0b00111100
  },{
    0b00000000,
    0b01111110,
    0b01100110,
    0b00001100,
    0b00001100,
    0b00011000,
    0b00011000,
    0b00011000
  },{
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100110,
    0b00111100,
    0b01100110,
    0b01100110,
    0b00111100
  },{
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100110,
    0b00111110,
    0b00000110,
    0b01100110,
    0b00111100
  },{
    0b00000000,
    0b00111100,
    0b01100110,
    0b01101110,
    0b01110110,
    0b01100110,
    0b01100110,
    0b00111100
  }
};
const int teste_len = sizeof(teste)/8;
/* 
  // void clear(){
  //   for (uint8_t i=0; i<NUM_MAX; i++)
  //     sendCmdAll(CMD_DIGIT0 + i, 0);
  // }//end clar

  // void sendCmdAll(byte cmd, byte data){
  //   digitalWrite(CS_PIN, LOW);
  //   for (int i = NUM_MAX-1; i>=0; i--) {
  //     shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, cmd);
  //     shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, data);
  //   }
  //   digitalWrite(CS_PIN, HIGH);
  // }//end send cmd

  // void refreshAllRot90(){
  //   byte mask = 0x80;
  //   for (int c = 0; c < 8; c++) {
  //     digitalWrite(CS_PIN, LOW);
  //     for(int i=NUM_MAX-1; i>=0; i--) {
  //       byte bt = 0;
  //       for(int b=0; b<8; b++) {
  //         bt>>=1;
  //         if(scr[i * 8 + b] & mask) bt|=0x80;
  //       }
  //       shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, CMD_DIGIT0 + c);
  //       shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, bt);
  //     }
  //     digitalWrite(CS_PIN, HIGH);
  //     mask>>=1;
  //   }
  // }//end refreshAllRot90
 */
void setup() {
  //configura os pinos usados no loop principal como saídas
  // pinMode(CS_PIN, OUTPUT);
  // pinMode(CLK_PIN, OUTPUT);
  // pinMode(DIN_PIN, OUTPUT);
  // digitalWrite(CS_PIN, HIGH);
  initMAX7219();
  Serial.begin(115200);
  for(int i=0;i<50;i++){Serial.println();}


  Serial.println(teste_len);

  for(int iten=0;iten<10;iten++){
    for(int linha=0;linha<8;linha++){
      for(int bit=8;bit>0;bit--){

        Serial.print(bitRead(teste[iten][linha],bit));


        //src[linha] = bitRead(teste[iten][linha],bit);
        delay(10);
      }
      Serial.println(", ");
    }
  }








  //while (1){
    // for(int bit=0;bit<5;bit++){
    //     Serial.print(", B");

    //   for(int linha=5; linha>0; linha--){
    //     Serial.print(bitRead(teste[bit], linha));

    //     sendCmdAll(linha, bitRead(teste[bit], linha));
    //     delay(10);
    //   }

    // }
  //}



  // for(int i=0;i<sizeof(teste);i++){
  //   //sendCmd(0, i, 1);
  //   Serial.println(bitRead(teste[i],0));


  // }

  //while(1){
    // for(int d=sizeof(teste); d>0; d--){
    //   digitalWrite(CS_PIN, LOW);
    //   for(int i=NUM_MAX-1; i>=0; i--) {
    //     shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, B00001111);
    //   }
    //   // for(int i=1;i<8;i++){
    //   //   //sendCmd(1, i, 1);
    //   // }
    //   digitalWrite(CS_PIN, HIGH);
    //   delay(1);
    // }

    // for(int c=0;c<8;c++){
    //   //digitalWrite(CS_PIN, LOW);
    //   for(int i=NUM_MAX-1; i>=0; i--) {
    //     //shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, teste[c]);
    //   }
    //   //digitalWrite(CS_PIN, HIGH);
    // }

    delay(1);
  //}



}//end setup

void loop() {
  
  delay(2000);
}//end loop