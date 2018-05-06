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

byte rgb[3*512];

boolean flipper = false;

void setup() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);

//  Serial.begin(115200);
  Serial.begin(921600);

  FastLED.setBrightness(64);
  clear();
  FastLED.show();
}

void loop() {
  if ( Serial.available() ) {

    Serial.readBytes(rgb, 3*512);

    for (int i = 0; i < 512; i++) {
      //read in the color channels for each pixel
      byte r = rgb[i*3+0];
      byte g = rgb[i*3+1];
      byte b = rgb[i*3+2];

      //compute the xyz position from the index
      byte x = byte(i /64);
      byte y = byte((i/8) % 8);
      byte z = byte(i % 8);

      //DEBUG: if x or y or z is out of bounds
//      if(x < 0 || x > 7 || y < 0 || y > 7 || z < 0 || z > 7) {
//        leds[ getIndex( 0, 0, 7 ) ] = CRGB( 255, 0, 0 );
//      }
//      //DEBUG: if r or g or b is out of bounds
//      else if(r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
//        leds[ getIndex( 0, 0, 4 ) ] = CRGB( 255, 0, 0 );
//      }
//      else {
//        leds[ getIndex( x, y, z ) ] = CRGB( r, g, b );
//      }

      leds[ getIndex( x, y, z ) ] = CRGB( r, g, b );
    }


//    if(flipper) {
//      leds[ getIndex( 0, 0, 0 ) ] = CRGB( 0, 255, 0 );
//    } else {
//      leds[ getIndex( 0, 0, 0 ) ] = CRGB( 0, 0, 0 );
//    }
//    flipper = !flipper;
    
    FastLED.show();
  }
}

// ----


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
