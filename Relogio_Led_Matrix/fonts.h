const uint8_t dig3x8[] PROGMEM = { 4,
0x03, 0xFF, 0x81, 0xFF,
0x02, 0x02, 0xFF, 0x00,
0x03, 0xF9, 0x89, 0x8F,
0x03, 0x81, 0x89, 0xFF,
0x03, 0x1F, 0x10, 0xFC,
0x03, 0x8F, 0x89, 0xF9,
0x03, 0xFF, 0x89, 0xF9,
0x03, 0x01, 0xF1, 0x0F,
0x03, 0xFF, 0x89, 0xFF,
0x03, 0x9F, 0x91, 0xFF,
};

const uint8_t dig6x8[] PROGMEM = { 7,
0x06, 0x7E, 0xFF, 0x81, 0x81, 0xFF, 0x7E,
0x06, 0x00, 0x82, 0xFF, 0xFF, 0x80, 0x00,
0x06, 0xC2, 0xE3, 0xB1, 0x99, 0x8F, 0x86,
0x06, 0x42, 0xC3, 0x89, 0x89, 0xFF, 0x76,
0x06, 0x38, 0x3C, 0x26, 0x23, 0xFF, 0xFF,
0x06, 0x4F, 0xCF, 0x89, 0x89, 0xF9, 0x71,
0x06, 0x7E, 0xFF, 0x89, 0x89, 0xFB, 0x72,
0x06, 0x01, 0x01, 0xF1, 0xF9, 0x0F, 0x07,
0x06, 0x76, 0xFF, 0x89, 0x89, 0xFF, 0x76,
0x06, 0x4E, 0xDF, 0x91, 0x91, 0xFF, 0x7E,
};

const uint8_t dig4x8[] PROGMEM = { 5,
0x04, 0xFF, 0x81, 0x81, 0xFF,
0x04, 0x04, 0x02, 0xFF, 0x00,
0x04, 0xF9, 0x89, 0x89, 0x8F,
0x04, 0x81, 0x89, 0x89, 0xFF,
0x04, 0x1F, 0x10, 0x10, 0xFE,
0x04, 0x8F, 0x89, 0x89, 0xF9,
0x04, 0xFF, 0x89, 0x89, 0xF8,
0x04, 0x01, 0xC1, 0x31, 0x0F,
0x04, 0xFF, 0x89, 0x89, 0xFF,
0x04, 0x1F, 0x91, 0x91, 0xFF,
};

const uint8_t dig3x7[] PROGMEM = { 4,
0x03, 0xFE, 0x82, 0xFE, 
0x03, 0x08, 0x04, 0xFE, 
0x03, 0xF2, 0x92, 0x9E, 
0x03, 0x82, 0x92, 0xFE, 
0x03, 0x3E, 0x20, 0xFC, 
0x03, 0x9E, 0x92, 0xF2, 
0x03, 0xFE, 0x92, 0xF2, 
0x03, 0x02, 0xE2, 0x1E, 
0x03, 0xFE, 0x92, 0xFE, 
0x03, 0x9E, 0x92, 0xFE, 
};
                                  
const uint8_t dig3x6[] PROGMEM = { 4,
0x03, 0xFC, 0x84, 0xFC, 
0x03, 0x10, 0x08, 0xFC, 
0x03, 0xF4, 0x94, 0x9C, 
0x03, 0x84, 0x94, 0xFC, 
0x03, 0x3C, 0x20, 0xF8, 
0x03, 0x9C, 0x94, 0xF4, 
0x03, 0xFC, 0x94, 0xF4, 
0x03, 0x04, 0xE4, 0x1C, 
0x03, 0xFC, 0x94, 0xFC, 
0x03, 0xBC, 0xA4, 0xFC, 
};

const uint8_t dig3x5[] PROGMEM = { 4,
0x03, 0xF8, 0x88, 0xF8, 
0x02, 0x10, 0xF8, 0x00, 
0x03, 0xE8, 0xA8, 0xB8, 
0x03, 0x88, 0xA8, 0xF8, 
0x03, 0x38, 0x20, 0xF8, 
0x03, 0xB8, 0xA8, 0xE8, 
0x03, 0xF8, 0xA8, 0xE8, 
0x03, 0x08, 0x08, 0xF8, 
0x03, 0xF8, 0xA8, 0xF8, 
0x03, 0xB8, 0xA8, 0xF8, 
};
const uint8_t dig5x8rn[] PROGMEM = { 6,
0x05, 0x7E, 0x81, 0x81, 0xFF, 0x7E, 
0x05, 0x04, 0x02, 0xFF, 0xFF, 0x00, 
0x05, 0xF1, 0x89, 0x89, 0x8F, 0x86, 
0x05, 0x81, 0x89, 0x89, 0xFF, 0x76, 
0x05, 0x1F, 0x10, 0x10, 0xFE, 0xFE, 
0x05, 0x8F, 0x89, 0x89, 0xF9, 0x71, 
0x05, 0x7E, 0x89, 0x89, 0xF9, 0x70, 
0x05, 0x01, 0xC1, 0xF1, 0x3F, 0x0F, 
0x05, 0x76, 0x89, 0x89, 0xFF, 0x76, 
0x05, 0x0E, 0x91, 0x91, 0xFF, 0x7E, 
};

const uint8_t dig5x8sq[] PROGMEM = { 6,
0x05, 0xFF, 0x81, 0x81, 0xFF, 0xFF, 
0x04, 0x04, 0x02, 0xFF, 0xFF, 0x00, 
0x05, 0xF9, 0x89, 0x89, 0x8F, 0x8F, 
0x05, 0x81, 0x89, 0x89, 0xFF, 0xFF, 
0x05, 0x1F, 0x10, 0x10, 0xFE, 0xFE, 
0x05, 0x8F, 0x89, 0x89, 0xF9, 0xF9, 
0x05, 0xFF, 0x89, 0x89, 0xF9, 0xF9, 
0x05, 0x01, 0x01, 0x01, 0xFF, 0xFF, 
0x05, 0xFF, 0x89, 0x89, 0xFF, 0xFF, 
0x05, 0x9F, 0x91, 0x91, 0xFF, 0xFF, 
};

const uint8_t dweek_pl[] PROGMEM = { 11,
0x0A, 0xFC, 0x08, 0x10, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x94, 0x84, 
0x09, 0xFC, 0x24, 0x24, 0x3C, 0x00, 0xFC, 0x84, 0x84, 0xFC, 0x00,  
0x09, 0xFC, 0x80, 0xF0, 0x80, 0xFC, 0x00, 0x04, 0xFC, 0x04, 0x00, 
0x09, 0xDC, 0x94, 0x96, 0xF5, 0x00, 0xFC, 0x24, 0x64, 0xBC, 0x00,
0x09, 0xFC, 0x84, 0x84, 0xCC, 0x00, 0xC4, 0xA4, 0x94, 0x8C, 0x00,
0x0A, 0xFC, 0x24, 0x24, 0x3C, 0x00, 0xFC, 0x00, 0xFC, 0x24, 0xFC,  
0x09, 0xDC, 0x94, 0x94, 0xF4, 0x00, 0xFC, 0x84, 0x84, 0xFC, 0x00,  
0x09, 0x07, 0x05, 0x07, 0x00, 0xFF, 0xFF, 0x81, 0x81, 0x81, 0x00,  //degC
};

const uint8_t dweek_en[] PROGMEM = { 11,
0x09, 0x9C, 0x94, 0x94, 0xF4, 0x00, 0xFC, 0x80, 0x80, 0xFC, 0x00,
0x0A, 0xFC, 0x04, 0x3C, 0x04, 0xFC, 0x00, 0xFC, 0x84, 0x84, 0xFC,
0x0A, 0x04, 0x04, 0xFC, 0x04, 0x04, 0x00, 0xFC, 0x80, 0x80, 0xFC,
0x0A, 0xFC, 0x80, 0xF0, 0x80, 0xFC, 0x00, 0xFC, 0x94, 0x94, 0x84,
0x0A, 0x04, 0x04, 0xFC, 0x04, 0x04, 0x00, 0xFC, 0x10, 0x10, 0xFC,
0x09, 0xFC, 0x24, 0x24, 0x04, 0x00, 0xFC, 0x24, 0x64, 0xBC, 0x00,
0x09, 0x9C, 0x94, 0x94, 0xF4, 0x00, 0xFC, 0x24, 0x24, 0xFC, 0x00,
0x09, 0x07, 0x05, 0x07, 0x00, 0xFF, 0xFF, 0x81, 0x81, 0x81, 0x00,
};

const uint8_t font[] PROGMEM = {
6,
2, B00000000, B00000000, B00000000, B00000000, B00000000, // space
1, B01011111, B00000000, B00000000, B00000000, B00000000, // !
3, B00000011, B00000000, B00000011, B00000000, B00000000, // "
5, B00010100, B00111110, B00010100, B00111110, B00010100, // #
4, B00100100, B01101010, B00101011, B00010010, B00000000, // $
5, B01100011, B00010011, B00001000, B01100100, B01100011, // %
5, B00110110, B01001001, B01010110, B00100000, B01010000, // &
1, B00000011, B00000000, B00000000, B00000000, B00000000, // '
3, B00011100, B00100010, B01000001, B00000000, B00000000, // (
3, B01000001, B00100010, B00011100, B00000000, B00000000, // )
5, B00101000, B00011000, B00001110, B00011000, B00101000, // *
5, B00001000, B00001000, B00111110, B00001000, B00001000, // +
2, B10110000, B01110000, B00000000, B00000000, B00000000, // ,
4, B00001000, B00001000, B00001000, B00001000, B00000000, // -
2, B01100000, B01100000, B00000000, B00000000, B00000000, // .
4, B01100000, B00011000, B00000110, B00000001, B00000000, // /
4, B00111110, B01000001, B01000001, B00111110, B00000000, // 0
3, B01000010, B01111111, B01000000, B00000000, B00000000, // 1
4, B01100010, B01010001, B01001001, B01000110, B00000000, // 2
4, B00100010, B01000001, B01001001, B00110110, B00000000, // 3
4, B00011000, B00010100, B00010010, B01111111, B00000000, // 4
4, B00100111, B01000101, B01000101, B00111001, B00000000, // 5
4, B00111110, B01001001, B01001001, B00110010, B00000000, // 6
4, B01100001, B00010001, B00001001, B00000111, B00000000, // 7
4, B00110110, B01001001, B01001001, B00110110, B00000000, // 8
4, B00100110, B01001001, B01001001, B00111110, B00000000, // 9
1, B01000100, B00000000, B00000000, B00000000, B00000000, // :
2, B10000000, B01010000, B00000000, B00000000, B00000000, // ;
3, B00010000, B00101000, B01000100, B00000000, B00000000, // <
3, B00010100, B00010100, B00010100, B00000000, B00000000, // =
3, B01000100, B00101000, B00010000, B00000000, B00000000, // >
4, B00000010, B01011001, B00001001, B00000110, B00000000, // ?
5, B00111110, B01001001, B01010101, B01011101, B00001110, // @
4, B01111110, B00010001, B00010001, B01111110, B00000000, // A
4, B01111111, B01001001, B01001001, B00110110, B00000000, // B
4, B00111110, B01000001, B01000001, B00100010, B00000000, // C
4, B01111111, B01000001, B01000001, B00111110, B00000000, // D
4, B01111111, B01001001, B01001001, B01000001, B00000000, // E
4, B01111111, B00001001, B00001001, B00000001, B00000000, // F
4, B00111110, B01000001, B01001001, B01111010, B00000000, // G
4, B01111111, B00001000, B00001000, B01111111, B00000000, // H
3, B01000001, B01111111, B01000001, B00000000, B00000000, // I
4, B00110000, B01000000, B01000001, B00111111, B00000000, // J
4, B01111111, B00001000, B00010100, B01100011, B00000000, // K
4, B01111111, B01000000, B01000000, B01000000, B00000000, // L
5, B01111111, B00000010, B00001100, B00000010, B01111111, // M
5, B01111111, B00000100, B00001000, B00010000, B01111111, // N
4, B00111110, B01000001, B01000001, B00111110, B00000000, // O
4, B01111111, B00001001, B00001001, B00000110, B00000000, // P
4, B00111110, B01000001, B01000001, B10111110, B00000000, // Q
4, B01111111, B00001001, B00001001, B01110110, B00000000, // R
4, B00100110, B01001001, B01001001, B00110010, B00000000, // S
5, B00000001, B00000001, B01111111, B00000001, B00000001, // T
4, B00111111, B01000000, B01000000, B00111111, B00000000, // U
5, B00001111, B00110000, B01000000, B00110000, B00001111, // V
5, B00111111, B01000000, B00111000, B01000000, B00111111, // W
5, B01100011, B00010100, B00001000, B00010100, B01100011, // X
5, B00000111, B00001000, B01110000, B00001000, B00000111, // Y
4, B01100001, B01010001, B01001001, B01000111, B00000000, // Z
2, B01111111, B01000001, B00000000, B00000000, B00000000, // [
4, B00000001, B00000110, B00011000, B01100000, B00000000, // \ backslash
2, B01000001, B01111111, B00000000, B00000000, B00000000, // ]
3, B00000010, B00000001, B00000010, B00000000, B00000000, // hat
4, B01000000, B01000000, B01000000, B01000000, B00000000, // _
2, B00000001, B00000010, B00000000, B00000000, B00000000, // `
4, B00100000, B01010100, B01010100, B01111000, B00000000, // a
4, B01111111, B01000100, B01000100, B00111000, B00000000, // b
4, B00111000, B01000100, B01000100, B00101000, B00000000, // c
4, B00111000, B01000100, B01000100, B01111111, B00000000, // d
4, B00111000, B01010100, B01010100, B00011000, B00000000, // e
3, B00000100, B01111110, B00000101, B00000000, B00000000, // f
4, B10011000, B10100100, B10100100, B01111000, B00000000, // g
4, B01111111, B00000100, B00000100, B01111000, B00000000, // h
3, B01000100, B01111101, B01000000, B00000000, B00000000, // i
4, B01000000, B10000000, B10000100, B01111101, B00000000, // j
4, B01111111, B00010000, B00101000, B01000100, B00000000, // k
3, B01000001, B01111111, B01000000, B00000000, B00000000, // l
5, B01111100, B00000100, B01111100, B00000100, B01111000, // m
4, B01111100, B00000100, B00000100, B01111000, B00000000, // n
4, B00111000, B01000100, B01000100, B00111000, B00000000, // o
4, B11111100, B00100100, B00100100, B00011000, B00000000, // p
4, B00011000, B00100100, B00100100, B11111100, B00000000, // q
4, B01111100, B00001000, B00000100, B00000100, B00000000, // r
4, B01001000, B01010100, B01010100, B00100100, B00000000, // s
3, B00000100, B00111111, B01000100, B00000000, B00000000, // t
4, B00111100, B01000000, B01000000, B01111100, B00000000, // u
5, B00011100, B00100000, B01000000, B00100000, B00011100, // v
5, B00111100, B01000000, B00111100, B01000000, B00111100, // w
5, B01000100, B00101000, B00010000, B00101000, B01000100, // x
4, B10011100, B10100000, B10100000, B01111100, B00000000, // y
3, B01100100, B01010100, B01001100, B00000000, B00000000, // z
3, B00001000, B00110110, B01000001, B00000000, B00000000, // {
1, B01111111, B00000000, B00000000, B00000000, B00000000, // |
3, B01000001, B00110110, B00001000, B00000000, B00000000, // }
4, B00001000, B00000100, B00001000, B00000100, B00000000, // ~

5, B00100000, B01010100, B01010100, B11111000, B10000000, // a
4, B00111000, B01000100, B01000110, B00101001, B00000000, // c
4, B00111000, B01010100, B11010100, B10011000, B00000000, // e
3, B01010001, B01111111, B01000100, B00000000, B00000000, // l
4, B01111100, B00000100, B00000110, B01111001, B00000000, // n
4, B00111000, B01000100, B01000110, B00111001, B00000000, // o
4, B01001000, B01010100, B01010110, B00100101, B00000000, // s
3, B01100100, B01010110, B01001101, B00000000, B00000000, // z
3, B01100100, B01010101, B01001100, B00000000, B00000000, // z
5, B01111110, B00010001, B00010001, B11111110, B10000000, // A
4, B00111100, B01000010, B01000011, B00100101, B00000000, // C
5, B01111111, B01001001, B01001001, B11000001, B10000000, // E
4, B01111111, B01001000, B01000100, B01000000, B00000000, // L
5, B01111110, B00000100, B00001010, B00010001, B01111110, // N
4, B00111100, B01000110, B01000011, B00111100, B00000000, // O
4, B00100100, B01001010, B01001011, B00110000, B00000000, // S
4, B01100010, B01010110, B01001011, B01000110, B00000000, // Z
4, B01101001, B01011001, B01001101, B01001011, B00000000, // Z

5, B00111110, B01010101, B01100001, B01010101, B00111110, // :)
5, B00111110, B01100101, B01010001, B01100101, B00111110, // :(
5, B00111110, B01000101, B01010001, B01000101, B00111110, // :o
5, 0x06, 0x1F, 0x7E, 0x1F, 0x06 // heart
};

