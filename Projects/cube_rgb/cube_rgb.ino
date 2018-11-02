#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

// pins for wemos d1
// #define DATA0 13 // D7
// #define DATA1 12 // D6
// #define DATA2 14 // D5
// #define DATA3 4  // D4

// pins for wemos d1 mini
#define DATA0 5  // D1
#define DATA1 4  // D2
#define DATA2 0  // D3
#define DATA3 2  // D4

// pins for arduino uno
// #define DATA0 7
// #define DATA1 6
// #define DATA2 5
// #define DATA3 4

CRGB getPixel(int, int, int);
int getIndex(int, int, int);
void clear();
void setColor(CRGB);

CRGB leds[512];

void setup() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);

  Serial.begin(115200);

  for (byte x = 0; x < 8; x++) {
    for (byte y = 0; y < 8; y++) {
      for (byte z = 0; z < 8; z++) {
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.print(", ");
        Serial.print(z);
        Serial.print(": ");
        Serial.println(getIndex(x, y, z));
        Serial.print( (7 - z) * 32 );
        Serial.print(" ");
        Serial.print( (7 - y) * 32 );
        Serial.print(" ");
        Serial.println( (7 - x) * 32 );

        leds[ getIndex(x, y, z) ].setRGB( (7 - z) * 32, (7 - y) * 32, (7 - x) * 32 );
      }
    }
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  FastLED.show();
  delay(2);
}

// ----

CRGB getPixel(int x, int y, int z) {
  return leds[ getIndex(x, y, z) ];
}

int getIndex(int x, int y, int z) {
  // consider the last installed slice to be the front (y = 0), with left having x=0, and bottom z=0
  
  return ( (z + y%2)%2 == 0 ? x : 7 - x ) +
    ( y%2 == 0 ? z*8 : (7 - z) * 8 ) + 
    y * 64;
}


void setColor(CRGB color) {
  fill_solid( leds, 512, color );
}

void clear() {
  setColor(0);
}
