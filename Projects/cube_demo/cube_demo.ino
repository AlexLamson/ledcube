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

byte demoMode = 1; // proj, rain, colors, wander
const byte numDemos = 4;

// projection
byte rgb[3*512];
byte brightness = 64;

// demos

const byte tickMillis = 20;
unsigned long lastTickTime = 0;

const int demoTimeout = 30000;
const int serialTimeout = 5000;
unsigned long lastDemoTime = 0;

// rain
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

void beginRain() {
  for (byte i = 0; i < numDrops; i++) {
    drops[i].init();
    drops[i].z = 8 + random(8);
  }
}

// colors
byte hue = 0;
const byte value = 128; // brightness of hue

// wander
class Wanderer {
  public:
    Wanderer();
    void init();
    void tick();

    float x, y, z, dx, dy, dz;
    CRGB color;
};

const int numWanderers = 8;
Wanderer wanderers[numWanderers];

const float momentum = 0.95;

void beginWander() {
  for (byte i = 0; i < numWanderers; i++) {
    wanderers[i].init();
  }
}

void setup() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);

//  Serial.begin(115200);
  Serial.begin(921600);

//  FastLED.setBrightness(64);
  clear();
  FastLED.show();
}

void loop() {
  
  if ( Serial.available() ) {
    lastTickTime = millis();
    demoMode = 0;

    Serial.readBytes(rgb, 3*512);

    for (int i = 0; i < 512; i++) {
      const byte divisor = 256 / brightness;
      //read in the color channels for each pixel
      byte r = rgb[i*3+0];// / divisor;
      byte g = rgb[i*3+1];// / divisor;
      byte b = rgb[i*3+2];// / divisor;

      //compute the xyz position from the index
      byte x = byte(i /64);
      byte y = byte((i/8) % 8);
      byte z = byte(i % 8);
    
      leds[ getIndex( x, y, z ) ] = CRGB( r, g, b );
    }
    
    FastLED.show();
  } else if (demoMode == 0 && millis() < lastTickTime + serialTimeout) {
    // wait for serial to return
  } else {
    // demo

    if (demoMode == 0 || millis() >= lastDemoTime + demoTimeout) {
      lastDemoTime = millis();
      // advance demo
      demoMode++;
      demoMode = demoMode % numDemos;
      if (demoMode == 0) demoMode = 1;
      switch (demoMode) {
        case 1: beginRain(); break;
        case 3: beginWander();
      }
    }
    
    if (millis() >= lastTickTime + tickMillis) {
      lastTickTime = millis();

      switch (demoMode) {
        case 1: // rain
        clear();
    
        for (int i = 0; i < numDrops; i++) {
          
          drops[i].tick();
          
          drops[i].draw();
        }
        break;
        
        case 2: // colors
        hue += 2;
//        setColor( CHSV( hue, 255, value ) );
        for (byte i = 0; i < 8; i++) {
          for (byte j = 0; j < 8; j++) {
            for (byte k = 0; k < 8; k++) {
              float dx = float(i) - 3.5;
              float dy = float(j) - 3.5;
              float dz = float(k) - 3.5;
              float dist = sqrt( pow(dx, 2) + pow(dy, 2) + pow(dz, 2) );
              float offset = dist / 4.0;
              leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 128), 255, value);
            }
          }
        }
        break;
        
        case 3: // wander
        clear(); // looks pretty cool without this to be honest
          
        for (int i = 0; i < numWanderers; i++) {
            
          wanderers[i].tick();
          
          drawSmoothedPixel(wanderers[i].x, wanderers[i].y, wanderers[i].z, wanderers[i].color);
        }
      }

      FastLED.show();
    }
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

// rain
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

// wander
Wanderer::Wanderer() {
  init();
}

void Wanderer::init() {
  x = 3.5;
  y = 3.5;
  z = 3.5;

  dx = 0;
  dy = 0;
  dz = 0;

  color = CHSV( random(256), 255, 255 );
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
