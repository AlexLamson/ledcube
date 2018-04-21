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

CRGB getPixel(int, int, int);
int getIndex(int, int, int);
void clear();
void setColor(CRGB);

void drawSmoothedPixel(float, float, float, CRGB);
byte mapBrightness(float);

CRGB leds[512];

// end cube stuff

class Wanderer {
  public:
    Wanderer() { };
    Wanderer(CRGB);
    Wanderer(float, float, float, CRGB);
    void tick();

    float x, y, z, dx, dy, dz;
    CRGB color;
};

//const int numWanderers = 1;
//Wanderer wanderers[numWanderers] = {Wanderer( CRGB(0, 64, 255) )};
Wanderer wanderer = Wanderer( CRGB(0, 64, 255) );
Wanderer wanderer1= Wanderer( CRGB(255, 0, 255) );

const float momentum = 0.95;

const byte tickMillis = 20;

unsigned long lastTickTime = 0;

void setup() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);

  Serial.begin(115200);

  clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() >= lastTickTime + tickMillis) {
    lastTickTime = millis();
    
    clear();

//    for (int i = 0; i < numWanderers; i++) {
//      Wanderer wanderer = (wanderers[i]);
      
      wanderer.tick();
      wanderer1.tick();
      
      Serial.print(wanderer.x);
      Serial.print(" ");
      Serial.print(wanderer.y);
      Serial.print(" ");
      Serial.print(wanderer.z);
      Serial.print(" ");
  
      drawSmoothedPixel(wanderer.x, wanderer.y, wanderer.z, wanderer.color);
      drawSmoothedPixel(wanderer1.x,wanderer1.y,wanderer1.z, wanderer1.color);
//    }
    
    FastLED.show();

    Serial.print(0);
    Serial.print(" ");
    Serial.println(8);
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

Wanderer::Wanderer(float sx, float sy, float sz, CRGB color) {
  x = sx;
  y = sy;
  z = sz;

  dx = 0;
  dy = 0;
  dz = 0;

  this->color = color;
}

Wanderer::Wanderer(CRGB color) {
  x = 3.5;
  y = 3.5;
  z = 3.5;

  dx = 0;
  dy = 0;
  dz = 0;

  this->color = color;
}


void Wanderer::tick() {
  // ought to be done as a random vector but that's effort for a later date
  dx += (float(random(512)) / 512.0 - 0.5) * 0.075;
  dy += (float(random(512)) / 512.0 - 0.5) * 0.075;
  dz += (float(random(512)) / 512.0 - 0.5) * 0.075;

  dx *= momentum;
  dy *= momentum;
  dz *= momentum;

  x += dx;
  y += dy;
  z += dz;

  // bounce
  if (x < 0) { x = 0; dx = -dx; }
  if (y < 0) { y = 0; dy = -dy; }
  if (z < 0) { z = 0; dz = -dz; }
  
  if (x >= 7) { x = 7; dx = -dx; }
  if (y >= 7) { y = 7; dy = -dy; }
  if (z >= 7) { z = 7; dz = -dz; }
}

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
