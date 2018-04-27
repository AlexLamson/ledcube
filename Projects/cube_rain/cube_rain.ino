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

void drawSmoothedPixel(float, float, float, CRGB);
byte mapBrightness(float);
CRGB scaleColor(CRGB, byte);

CRGB leds[512];

// end cube stuff

class Drop {
  public:
    Drop();
    void init();
    void tick();
    void draw();
    void reset();

    byte x, y;
    float z, dz;
    CRGB color;
};

const int numDrops = 16;
Drop drops[numDrops];

const byte tickMillis = 20;

unsigned long lastTickTime = 0;

void setup() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);

  Serial.begin(115200);

  for (byte i = 0; i < numDrops; i++) {
    drops[i].z = 8 + random(8);
  }

  clear();
}

void loop() {
  if (millis() >= lastTickTime + tickMillis) {
    lastTickTime = millis();
    
    clear();

    for (int i = 0; i < numDrops; i++) {
      
      drops[i].tick();
      
      drops[i].draw();
    }
    
    FastLED.show();
  }
}

// ----

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
  
          leds[ getIndex(cx, cy, cz) ] += CRGB( scale8(color.r, brightness), scale8(color.g, brightness), scale8(color.b, brightness) );
        }
        
      }
    }
  }
}

byte mapBrightness(float input) {
  return 255*(0.5-cos( PI * pow(input, 1.5) )/2); // power of 1.5 seems to preserve brightness pretty well.
}

CRGB scaleColor(CRGB color, byte brightness) {
  return CRGB( scale8(color.r, brightness), scale8(color.g, brightness), scale8(color.b, brightness) );
}

Drop::Drop() {
  init();
}

void Drop::init() {
  x = random(8);
  y = random(8);
  z = 8;

  dz = 0.1 + float(random(10)) / 100;

  color = CHSV( random(256), 255, 255 );
}


void Drop::tick() {
  z -= dz;

  // reset if too low
  if (z < -4) {
    init();
  }
}
void Drop::draw() {
  float currZ = z;
  const byte length = 1;
  while (currZ - z < length) {
    drawSmoothedPixel(x, y, currZ, scaleColor(color, 255 - (currZ - z)*(256/length)));
    currZ += 1;
  }
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
