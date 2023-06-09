#include <MD_Parola.h>
#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW         // see video number one about this you have a choice of 4 hardware types to select
#define MAX_DEVICES 4                             // Change this number as required
#define CLK_PIN   13                              // SPI pins on the D1 mini
#define DATA_PIN  11
#define CS_PIN    10

#define MAX_ZONES 4

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);   // see video 1 why we have called this "P"

struct animations{
  textEffect_t   effecIn;       // Animation type In
  textEffect_t   effecOut;      // Animation type Out - In this example there are the same, but you can mix them.
  const char *   pText;         // Text to display
  uint16_t       speed;         // Animation speed (multiplier for library default) larger number = SLOWER
  uint16_t       pause;         // pause (multiplier for library default)  larger number = SLOWER
  textPosition_t align;         // PA_LEFT, PA_CENTER, PA_RIGHT
};

const uint8_t degC[] = { 6, 3, 3, 56, 68, 68, 68 }; // Deg C

const char msg[] = "25$";     // dont worry another video will be out soon about this

const uint8_t F_GHOST = 2;                          // Frames
const uint8_t W_GHOST = 10;                         // Width
const uint8_t PROGMEM ghost[F_GHOST * W_GHOST] = {      
  0,248, 157, 54, 94, 94, 54, 157, 252,0,
  0,248, 157, 118, 158, 158, 118, 157, 252,0,  
};

animations animList[] = {
  { PA_NO_EFFECT,         PA_NO_EFFECT ,        "Parola",                           3, 3, PA_CENTER },
  { PA_PRINT,             PA_PRINT,             "PRINT",                            3, 2, PA_CENTER },
  { PA_SCROLL_UP,         PA_SCROLL_UP,         "SCR.UP",                           3, 2, PA_CENTER },
  { PA_SCROLL_DOWN,       PA_SCROLL_DOWN,       "SCR.DN",                           3, 2, PA_CENTER },
  { PA_SCROLL_LEFT,       PA_SCROLL_LEFT,       "SCR.L",                            3, 2, PA_CENTER },
  { PA_SCROLL_RIGHT,      PA_SCROLL_RIGHT,      "SCR.R",                            3, 2, PA_CENTER },
  { PA_SPRITE,            PA_SPRITE,             msg,                               4, 1, PA_CENTER },
  { PA_SLICE,             PA_SLICE,             "SLICE",                            1, 0, PA_CENTER },
  { PA_MESH,              PA_MESH,              "MESH",                             15, 2, PA_CENTER },
  { PA_FADE,              PA_FADE,              "FADE",                             20, 3, PA_CENTER },
  { PA_DISSOLVE,          PA_DISSOLVE,          "DISS",                             10, 2, PA_CENTER },
  { PA_BLINDS,            PA_BLINDS,            "BLIND",                            6, 3, PA_CENTER },
  { PA_RANDOM,            PA_RANDOM,            "Random",                           3, 2, PA_CENTER },
  { PA_WIPE,              PA_WIPE,              "WIPE",                             5, 2, PA_CENTER },
  { PA_WIPE_CURSOR,       PA_WIPE_CURSOR,       "WIPE.C",                           5, 2, PA_CENTER },
  { PA_SCAN_HORIZ,        PA_SCAN_HORIZ,        "SCN.H",                            3, 2, PA_CENTER },
  { PA_SCAN_HORIZX,       PA_SCAN_HORIZX,       "SCN.HX",                           3, 2, PA_CENTER },
  { PA_SCAN_VERT,         PA_SCAN_VERT,         "SCN.V",                            5, 2, PA_CENTER },
  { PA_SCAN_VERTX,        PA_SCAN_VERTX,        "SCN.VX",                           5, 2, PA_CENTER },
  { PA_OPENING,           PA_OPENING,           "OPEN",                             3, 2, PA_CENTER },
  { PA_OPENING_CURSOR,    PA_OPENING_CURSOR,    "OPEN.C",                           3, 2, PA_CENTER },
  { PA_CLOSING,           PA_CLOSING,           "CLOSE",                            3, 2, PA_CENTER },
  { PA_CLOSING_CURSOR,    PA_CLOSING_CURSOR,    "CLOS.C",                           3, 2, PA_CENTER },
  { PA_SCROLL_UP_LEFT,    PA_SCROLL_UP_LEFT,    "SC.U.L",                           3, 2, PA_CENTER },
  { PA_SCROLL_UP_RIGHT,   PA_SCROLL_UP_RIGHT,   "SC.U.R",                           3, 2, PA_CENTER },
  { PA_SCROLL_DOWN_LEFT,  PA_SCROLL_DOWN_LEFT,  "SC.D.L",                           3, 2, PA_CENTER },
  { PA_SCROLL_DOWN_RIGHT, PA_SCROLL_DOWN_RIGHT, "SC.D.R",                           3, 2, PA_CENTER },
  { PA_GROW_UP,           PA_GROW_UP,           "GROW.U",                           4, 2, PA_CENTER },
  { PA_GROW_DOWN,         PA_GROW_DOWN,         "GROW.D",                           4, 2, PA_CENTER },
  { PA_SCROLL_LEFT,       PA_FADE,              "Thanks for Watching",              3, 3, PA_CENTER },
  { PA_GROW_DOWN,         PA_WIPE,              "Like",                             3, 2, PA_CENTER },
  { PA_FADE,              PA_SCROLL_DOWN,       "Share",                            3, 2, PA_CENTER },
  { PA_SCROLL_DOWN_LEFT,  PA_SCROLL_UP_LEFT,    "Sub",                              3, 2, PA_CENTER },
  { PA_PRINT,             PA_PRINT,             "Sub",                              0, 1, PA_CENTER },
  { PA_PRINT,             PA_PRINT,             "Sub",                              0, 1, PA_CENTER },
  { PA_PRINT,             PA_PRINT,             "Sub",                              0, 1, PA_CENTER },
  { PA_SCROLL_LEFT,       PA_SCROLL_UP_LEFT,    "Lots more Max7219 matrix videos to come, so stay tuned, please comment below",                       3, 5, PA_LEFT },
  { PA_SCROLL_LEFT,       PA_SCROLL_DOWN_RIGHT, "We will make custom FONTS & SPRITES. Connect and dispaly sensor data",                               3, 5, PA_LEFT },
  { PA_SCROLL_LEFT,       PA_SCROLL_UP_LEFT,    "Make a World Time Clock, Custom Enclosure, And change our ROUND pixels to SQUARE, now thats magic",  3, 5, PA_LEFT },
  { PA_SCROLL_LEFT,       PA_SCROLL_DOWN_RIGHT, "Make the Matrix BlueTooth Enabled so you can change the Text displayed from your phone",             3, 5, PA_LEFT },
  { PA_SCROLL_LEFT,       PA_SCROLL_UP_LEFT,    "Use a WiFi Manager so you can connect it to another Wifi without the use of a computer",             3, 5, PA_LEFT },
  { PA_SCROLL_LEFT,       PA_SCROLL_DOWN_RIGHT, "Make sure to Subscribe so not to miss any of the action",                                            3, 5, PA_LEFT },

};

void setup() {
  P.begin();
  P.addChar('$', degC);
  P.displayClear();     // clear the display
  P.setIntensity(1);    // set the intensity 0 to 15
  P.setInvert(false);   // sets invert for all zones OK, we will cover zones in another video

  P.setZone(0, 0,0);
  P.setZone(1, 1,3);
  P.setFont(newFont);
  
  P.setSpriteData(ghost, W_GHOST, F_GHOST, ghost, W_GHOST, F_GHOST);      // for the Sprite

  for (uint8_t i = 0; i < ARRAY_SIZE(animList); i++) {animList[i].speed *= P.getSpeed(); animList[i].pause *= 500;}

}


void loop() {
  // static uint8_t i = 6;                                                   // text effect index

  if (P.displayAnimate()){
    for(uint8_t i = 0; i<MAX_ZONES; i++){
      if(P.getZoneStatus(0)){

      }
    }

  //   if (i == ARRAY_SIZE(animList))i = 0;                                  // reset loop index

  //     P.displayText(animList[i].pText, 
  //       animList[i].align, 
  //         animList[i].speed, 
  //           animList[i].pause,//30000,
  //             animList[i].effecIn, 
  //               animList[i].effecOut);

                  
  //   delay(1000);
  //   i++;                                                                  // then set up for next text effect
  }
}