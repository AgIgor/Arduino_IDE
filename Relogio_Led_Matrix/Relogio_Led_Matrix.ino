#include <MD_Parola.h>
#include <Adafruit_AHT10.h>
#include <TimeLib.h>
#include <DS1307RTC.h>  //0x50 0x68 
#include <Wire.h>

Adafruit_AHT10 aht; //0x38

// #include "fonts.h"
 
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
 
int i = 4;
int Animation;
String RandAn;
 
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
 
char daysOfTheWeek[7][12] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
char monthOfTheYear[12][12] = {"Jan", "Fev", "Març", "Abrl", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};

// const uint8_t degr[] PROGMEM = {11,
//   0x0A, 0xFC, 0x08, 0x10, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x94, 0x84, 
//   0x09, 0xFC, 0x24, 0x24, 0x3C, 0x00, 0xFC, 0x84, 0x84, 0xFC, 0x00,  
//   0x09, 0xFC, 0x80, 0xF0, 0x80, 0xFC, 0x00, 0x04, 0xFC, 0x04, 0x00, 
//   0x09, 0xDC, 0x94, 0x96, 0xF5, 0x00, 0xFC, 0x24, 0x64, 0xBC, 0x00,
//   0x09, 0xFC, 0x84, 0x84, 0xCC, 0x00, 0xC4, 0xA4, 0x94, 0x8C, 0x00,
//   0x0A, 0xFC, 0x24, 0x24, 0x3C, 0x00, 0xFC, 0x00, 0xFC, 0x24, 0xFC,  
//   0x09, 0xDC, 0x94, 0x94, 0xF4, 0x00, 0xFC, 0x84, 0x84, 0xFC, 0x00,  
//   0x09, 0x07, 0x05, 0x07, 0x00, 0xFF, 0xFF, 0x81, 0x81, 0x81, 0x00,  //degC
// };

const uint8_t degr[] = { 6, 3, 3, 56, 68, 68, 68 };
 
struct sCatalog{
  textEffect_t  effect;   
  const char *  psz;      
  uint16_t      speed;    
  uint16_t      pause;    
};
 
sCatalog catalog[] = {
  { PA_PRINT,"",80,3000 },
  { PA_SCROLL_UP,"",80,3000 },
  { PA_SCROLL_DOWN,"",80,3000 },
  { PA_SCROLL_LEFT,"",80,3000 },
  { PA_SCROLL_RIGHT,"",80,3000 },
  { PA_SPRITE,"",80,3000  },
  { PA_SLICE,"",15,3000  },
  { PA_MESH,"",150,3000  },
  { PA_FADE,"",250,3000  },
  { PA_DISSOLVE,"",500,3000  },
  { PA_BLINDS,"",120,3000  },
  { PA_RANDOM,"",50,3000  },
  { PA_WIPE,"",80,3000  },
  { PA_WIPE_CURSOR,"",80,3000  },
  { PA_SCAN_HORIZ,"",80,3000  },
  { PA_SCAN_HORIZX,"",80,3000  },
  { PA_SCAN_VERT,"",80,3000  },
  { PA_SCAN_VERTX,"",80,3000  },
  { PA_OPENING,"",80,3000  },
  { PA_OPENING_CURSOR,"",80,3000  },
  { PA_CLOSING,"",80,3000  },
  { PA_CLOSING_CURSOR,"",80,3000  },
  { PA_SCROLL_UP_LEFT,"",80,3000  },
  { PA_SCROLL_UP_RIGHT,"",80,3000   },
  { PA_SCROLL_DOWN_LEFT,"",80,3000  },
  { PA_SCROLL_DOWN_RIGHT,"",80,3000  },
  { PA_GROW_UP,"",80,3000  },
  { PA_GROW_DOWN,"",80,3000  },
};
 
const uint8_t F_PMAN1 = 6;
const uint8_t W_PMAN1 = 8;
static const uint8_t PROGMEM pacman1[F_PMAN1 * W_PMAN1] = {
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
};
 
const uint8_t F_PMAN2 = 6;
const uint8_t W_PMAN2 = 18;
static const uint8_t PROGMEM pacman2[F_PMAN2 * W_PMAN2] = {
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
};

void getTimeRTC(){
  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    Serial.print(tm.Hour);
    Serial.write(':');
    Serial.print(tm.Minute);
    Serial.write(':');
    Serial.print(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  }

}

void getTemp(){
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");

}
void setup(){
  Serial.begin(115200);
  P.begin();
  P.setInvert(false); 
  P.setIntensity(5);
  P.addChar('$', degr);
  Wire.begin();

  //P.setSpriteData(pacman1, W_PMAN1, F_PMAN1, pacman2, W_PMAN2, F_PMAN2);

  // P.displayText("Hello" , PA_CENTER, 100, 10, PA_SCROLL_LEFT, PA_SCROLL_LEFT); 

  while (!P.displayAnimate());

  if (! aht.begin(0x38)) {
    Serial.println("Could not find AHT10? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 found");

}

String TIMP=" ";
String HORA=" ";
String MINUT=" ";
 
String Day;
String Month;
String Year;

String HumSTR;
String TempSTR;
 
void loop(){
 
  String DayOfWeek = daysOfTheWeek[6];//
  String MonthOfYear = monthOfTheYear[4];//

  const char *DayOfWeekC = DayOfWeek.c_str();
  const char *MonthOfYearC = MonthOfYear.c_str();
 
  HORA = ((random(0,23))%24);//

  if (HORA.length() < 2){ HORA = "0"+ HORA;}
 
  MINUT = ((millis()/1000)%60);//

  if (MINUT.length() < 2){ MINUT = "0"+ MINUT;}
 
  TIMP = HORA + ":" + MINUT;
 
  // const char *Timp = TIMP.c_str();
 
  Day = 27;//
  Month = 5;//
  Year = 2023;//

  HumSTR = random(20,80);//
  TempSTR = random(10, 32);//

  HumSTR += "%";
  TempSTR += "$";
 
  const char *DIA = Day.c_str();
  const char *MES = Month.c_str();
  const char *ANO = Year.c_str();
 
  String Date = Day + "/" + Month;

  const char *Data = Date.c_str();
  const char *HumC = HumSTR.c_str();
  const char *TempC = TempSTR.c_str();

  static uint32_t	lastTime = 0;
  static bool	flasher = false;
 
  
   if (i == 1){
        Animation = random(1, 29);
        P.displayText(DayOfWeekC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
  }

  if (i == 2){
        Animation = random(1, 29);
        P.displayText(DIA, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
  }

  if (i == 3){
      Animation = random(1, 29);
      P.displayText(MonthOfYearC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
        while (!P.displayAnimate());
  }

  if (i == 4){
        Animation = random(1, 29);
        P.displayText(ANO, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
  }
 
  if (i == 5){
        Animation = random(1, 29);
        P.displayText("23$", PA_CENTER, catalog[Animation].speed, 10000, catalog[Animation].effect, catalog[Animation].effect); 
        while (!P.displayAnimate());
  }

  if (i == 6){
        Animation = random(1, 29);
        P.displayText(Data, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
  }

  if (i == 7){
        Animation = random(1, 29);
        P.displayText(HumC, PA_CENTER, catalog[Animation].speed, 10000, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
  }

  if (i == 8){
        Animation = random(1, 29);
        P.displayText(TempC, PA_CENTER, catalog[Animation].speed, 10000, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
  }

  //Serial.println(Animation);
  // delay(1000);

  if (millis() - lastTime >= 1000){
    lastTime = millis();
    i++;
    i > 8 ? i = 0 : i=i;
    //flasher = !flasher; 
    //flasher ? TIMP.replace(":"," ") : TIMP.replace(" ",":");
    //*Ti = TIMP.c_str();
    //Serial.println(TIMP);
    getTimeRTC(); 
    getTemp();
  }
 
}
 