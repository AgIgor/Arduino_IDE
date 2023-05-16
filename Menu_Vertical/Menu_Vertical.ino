#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define led1 13
#define led2 12
#define relay 11
#define speak 10

#define pin_select 5
#define pin_up 6
#define pin_down 7

void setup() {
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.print("Hello World");
  delay(500);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(speak, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(relay, LOW);

  pinMode(pin_select, INPUT_PULLUP);
  pinMode(pin_up, INPUT_PULLUP);
  pinMode(pin_down, INPUT_PULLUP);
}

void blink(){
  digitalWrite(led1, (millis()/1000)%2);
}

void limpa(){
  for(int c=0; c<17; c++){
    for(int l=0; l<2;l++){
      lcd.setCursor(c, l);
      lcd.print(" ");
    }
  }
}

void menu(){
  const int MX_OPT = 5;
  int dy = 0, opt = 0;
  bool key = 0, opSt[MX_OPT] = {0}, blk = 0;

  String msg[2][MX_OPT] = {
    {"L1 On", "L2 On", "Rly On", "Rst", "Blk On"},
    {"L1 Off", "L2 Off", "Rly Off", "Rst", "Blk Off"}
  };

  // lcd.clear();
  limpa();
  while(1){
    lcd.setCursor(0,0);
    lcd.print(opt);
    if(!dy) lcd.print(">");
    else lcd.print(" ");
    lcd.print(msg[opSt[opt]][opt]);

    lcd.setCursor(0, 1);
    lcd.print(opt+1);
    if(dy) lcd.print(">");
    else lcd.print(" ");
    lcd.print(msg[opSt[opt + 1]][opt + 1]);

    //mover opçoes
    if(key && !digitalRead(pin_down)){
      //lcd.clear();
      limpa();
      tone(speak, 150, 10);
      if(!dy) dy = 1;
      else if (++opt >= MX_OPT - 1){
        dy = 0;
        opt = 0;
      }
    }

    if(key && !digitalRead(pin_up)){
      // lcd.clear();
      limpa();
      tone(speak, 160, 10);
      if(dy) dy = 0;
      else if (--opt < 0){
        dy = 1;
        opt = MX_OPT - 2;
      }
    }

    //trava dos botoes
    key = 1;
    // for(int i=5; i<7; i++) if(!digitalRead(i)) key = 0;
    if(!digitalRead(pin_select)) key = 0;
    if(!digitalRead(pin_down)) key = 0;
    if(!digitalRead(pin_up)) key = 0;
    delay(10);

    //controle das portas
    if(!digitalRead(pin_select)){
      // lcd.clear();
      limpa();
      while(!digitalRead(pin_select));
      opSt[opt+dy] = !opSt[opt+dy];
      switch(opt + dy){
        case 0: digitalWrite(led1, opSt[opt+dy]); break;
        case 1: digitalWrite(led2, opSt[opt+dy]); break;
        case 2: digitalWrite(relay, opSt[opt+dy]); break;
        case 3: setup(); return; break;
        case 4: blk = !blk; break;
      }//end switch opt
    }//end if select

    if(blk){
      blink();
    }

  }//end while 1
}//end void menu

void loop() {
  menu();
  //blink();

}
