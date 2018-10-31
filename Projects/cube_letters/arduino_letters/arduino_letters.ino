#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

// pins for wemos d1
//#define DATA0 13 // D7
//#define DATA1 12 // D6
//#define DATA2 14 // D5
//#define DATA3 4  // D4

// pins for wemos d1 mini
#define DATA0 5  // D1
#define DATA1 4  // D2
#define DATA2 0  // D3
#define DATA3 2  // D4

// pins for arduino uno
//#define DATA0 7
//#define DATA1 6
//#define DATA2 5
//#define DATA3 4


int getIndex(int, int, int);

CRGB leds[512];
boolean flipper = true;
long lastFlip = 0;

// end cube stuff


void setup() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);

  Serial.begin(921600);

  FastLED.clear();
  FastLED.setCorrection( Typical8mmPixel );
  FastLED.setBrightness(64);
  FastLED.show();
}

void loop() {
  if ( Serial.available() ) {
    Serial.readBytes( (char*)leds, 512 * 3);
    FastLED.show();
  }

//  leds[getIndex( 0, 0, 0 )].r = flipper*255;
//  long currTime = millis();
//  if(currTime - lastFlip > 500) {
//    lastFlip = currTime;
//    flipper = !flipper;
//  }

//  FastLED.show();
}


int getIndex(int x, int y, int z) {
  // consider the last installed slice to be the front (y = 0), with left having x=0, and bottom z=0
  
  return ( (z + y%2)%2 == 0 ? x : 7 - x ) +
    ( y%2 == 0 ? z*8 : (7 - z) * 8 ) + 
    y * 64;
}

