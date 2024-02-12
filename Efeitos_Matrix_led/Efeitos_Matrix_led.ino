#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

//Hardware SPI Arduino UNO
// CLK Pin  > 13 SCK
// Data Pin > 11 MOSI

//#define CLK_PIN   4
//#define DATA_PIN  2
#define CS_PIN      10

// Hardware SPI connection
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
//MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

int i = 0;

textEffect_t texteffect[] = {
  PA_WIPE,
  PA_WIPE_CURSOR,
  PA_OPENING_CURSOR,
  PA_CLOSING_CURSOR,
  PA_BLINDS,
  PA_MESH,
  PA_OPENING,
  PA_CLOSING,
  PA_SCAN_VERT,
  PA_DISSOLVE,
  PA_RANDOM,
  PA_PRINT,
  PA_SCROLL_RIGHT,
  PA_SCROLL_LEFT,
  PA_GROW_UP,
  PA_GROW_DOWN,
  PA_SCROLL_UP,
  PA_SCROLL_DOWN,
  PA_SCROLL_UP_RIGHT,
  PA_SCROLL_UP_LEFT,
  PA_SCROLL_DOWN_RIGHT,
  PA_SCROLL_DOWN_LEFT,
};
void setup() {
  myDisplay.begin();
  myDisplay.setIntensity(0);
  myDisplay.setTextAlignment(PA_CENTER);
  myDisplay.setSpeed(100);
  myDisplay.setPause(1000);
  myDisplay.displayClear();
}

String t = "";

void loop() {

  if (myDisplay.displayAnimate()) {
    if (i < sizeof(texteffect)) {
      i++;
    }
    else {
      i = 0;
    }
    t = (millis()/1000);
    const char *teste = t.c_str();
    
    myDisplay.displayText(teste, myDisplay.getTextAlignment(), myDisplay.getSpeed(), myDisplay.getPause(), texteffect[i], texteffect[i]);
    myDisplay.displayReset();
  }
  // delay(2000);
}