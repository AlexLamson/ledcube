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
void clear();
void setColor(CRGB);

byte mapBrightness(byte);

CRGB leds[512];

// end cube stuff

byte levels[64];

void setup() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);

  Serial.begin(115200);

  clear();

  FastLED.show();
}

void loop() {
  if ( Serial.available() ) {
    // make sure we're aligned with the data
//    int count = 0;
//    bool valid = true;
//    while (count < 3) {
//      Serial.readBytes(levels, 1);
//
//      if (levels[0] == 254)
//        count++;
//      else
//        valid = false;
//    }

    Serial.readBytes(levels, 64);

    for (int i = 0; i < 64; i++) {
      byte height = levels[i];
      byte x = i % 8, y = i / 8;

      for (int j = 0; j < 8; j++) {
        byte intensity = max(0, min(255, (height - j*32)*8));

        leds[ getIndex( x, y, j ) ] = CRGB(
          scale8( 255 - 32 - x * 32, intensity ),
          scale8( y * 32, intensity),
          scale8( x * 32, intensity) );
      }
    }
    
    FastLED.show();
  }
}

// ----

// should be easy to make this work for a linear image
void drawSmoothedPixel(float x, float y, float z, CRGB color) {

  for (int i = int(x - 1); i <= int(x + 1); i++) {
    for (int j = int(y - 1); j <= int(y + 1); j++) {
      for (int k = int(z - 1); k <= int(z + 1); k++) {
        
        if (!(i < 0 || j < 0 || k < 0 || i >= 8 || j >= 8 || k >= 8)) {
          float compX = x - i, compY  = y - j, compZ = z - k;
          float dist = sqrt( pow(compX, 2) + pow(compY, 2) + pow(compZ, 2) );
          
          float normBrightness = max(0, 1.0-dist);
          byte brightness = mapBrightness(normBrightness);

          byte cx = i, cy = j, cz = k;

//          // wrap
//          while (cx < 0) { cx += 8; }
//          while (cy < 0) { cy += 8; }
//          while (cz < 0) { cz += 8; }
//          
//          while (cx >= 8) { cx -= 8; }
//          while (cy >= 8) { cy -= 8; }
//          while (cz >= 8) { cz -= 8; }
  
          leds[ getIndex(cx, cy, cz) ] += CRGB( scale8(color.r, brightness), scale8(color.g, brightness), scale8(color.b, brightness) );
        }
        
      }
    }
  }
}

byte mapBrightness(float input) {
  return 255*(0.5-cos( PI * pow(input, 1.5) )/2); // power of 1.5 seems to preserve brightness pretty well.
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
