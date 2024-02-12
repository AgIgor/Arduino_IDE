#include <Arduino.h>
#include <U8g2lib.h>

U8G2_MAX7219_32X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);

// 'matrix_display_counter', 32x8px
const unsigned char epd_bitmap_matrix_display_counter [] = {
  0x7e, 0x67, 0x75, 0x77, 0xef, 0x21, 0x15, 0x44, 0xe7, 0x21, 0x15, 0x44, 0xe3, 0x27, 0x77, 0x77, 
  0xe7, 0x24, 0x11, 0x15, 0xef, 0x24, 0x11, 0x15, 0x7e, 0x77, 0x71, 0x77, 0x00, 0x00, 0x00, 0x00
}; 

const unsigned char youtube_logo_bitmap [] = { // youtube bitmap logo, 8x7 px
  B01111110,
  B11101111,
  B11100111,
  B11100011,
  B11100111,
  B11101111,
  B01111110
}; 

byte digits[6] = {0, 0, 0, 0, 0, 0}; // individual digits to be displayed on the matrix display
byte digits_offset_perc[6] = {0, 0, 0, 0, 0, 0}; // y offset for the individual digits - percentage 0-100%
char digit_char[2]; // helper array for storing C-style string
char digit_char_next[2]; // helper array for storing C-style string

float y_offset; // y pixel offset for digit

void setup(void) {
  u8g2.begin(); // begin function is required for u8g2 library
  u8g2.setContrast(100); // set display contrast 0-255

  pinMode(A0, INPUT_PULLUP);
}

void loop(void) {

  if (digitalRead(A0) == LOW) { // button is pressed
    if (digits_offset_perc[5] == 0) { // no animation is currently playing for the last digit
      digits_offset_perc[5] = 2; // in that case, animate the last digit (increment the last digit)
    }
  }

  u8g2.clearBuffer(); // clear the internal u8g2 memory
  u8g2.setFont(u8g2_font_minuteconsole_tr); // choose a suitable font with digits 3px wide
  
  u8g2.drawBitmap(0, 0, 8/8, 7, youtube_logo_bitmap); // draw youtube logo on top left corner

  for (int i=5; i>=0; i--) { // go from the last digit to the first digit

    if (digits_offset_perc[i] > 0) { // animate the digit
      digits_offset_perc[i] = digits_offset_perc[i] + 2; // increase the percentage offset

       if ((digits[i] == 9) && (digits_offset_perc[i-1] == 0) && (digits_offset_perc[i] > 20)) {
         digits_offset_perc[i-1] = 2; // digit is 9 turning to 0 = increase digit on the left side
       }

      if (digits_offset_perc[i] >= 100) { // animation is complete, switch to the next digit
        digits_offset_perc[i] = 0; // stop the animation
        digits[i] = digits[i] + 1; // switch to the next digit

        if (digits[i] == 10) { // if the digit goes over 9, go back to 0
          digits[i] = 0;
        }
      }
    }

    // linear movement of digits - looks boring
    //y_offset = round((digits_offset_perc[i] / 100.0) * 8.0); // calculate the Y pixel offset
    // easing using the power function - looks strange for continuous animation
    //y_offset = round(pow((digits_offset_perc[i] / 100.0), 0.4) * 8.0);
    // easing using the cosine function - looks great
    y_offset = round((1-((cos(digits_offset_perc[i] / 100.0 * 3.141592654) / 2.0)+0.5)) * 8.0);

    itoa(digits[i], digit_char, 10); // convert digit number to helper C-style string array
    itoa((digits[i]+1) % 10, digit_char_next, 10); // convert next digit number to helper C-style string array

    u8g2.drawStr(9 + i*4, 7 - y_offset, digit_char);  // draw the current character to the display
    u8g2.drawStr(9 + i*4, 7 - y_offset + 8, digit_char_next); // draw the next character to the display
  }


  u8g2.sendBuffer(); // transfer internal memory to the display
}
