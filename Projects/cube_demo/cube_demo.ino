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
byte getX(int);
byte getY(int);
byte getZ(int);

CRGB leds[512];

// end cube stuff

byte demoMode = 5; // proj, rain, color sphere, wander, function, trimmed sphere, bouncing in rgb space, color snake, color pyramid
const unsigned int demoTimes[] = {5000, 5000, 30000, 5000, 10000, 20000, 20000, 15000, 10000};
bool advanceDemo = true; // toggle to stay on one demo forever
//const unsigned int demoTimes[] = {100, 100, 100, 100, 100, 600000, 100, 100, 100};
const byte numDemos = 9;

// projection
byte rgb[3*512];
byte brightness = 64;

// demos

const byte tickMillis = 20;
unsigned long lastTickTime = 0;

//const int demoTimeout = 30000;
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

//math
#define PI 3.14159265359

// colors
byte hue = 0;
byte hue2 = 0;
const byte value = 128; // brightness of hue


//data snake
int snake_frame = 0;


//bouncing in rgb space
int cubeScale = 8;
float bounceX = 0, bounceY = 0, bounceZ = 0;
float bounceDX = 0.1, bounceDY = 0.2, bounceDZ = 0.3;


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

// function plot
float w = 0;
float w_increment = 0.001;

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
    
    const byte divisor = 256 / brightness;

    for (int i = 0; i < 512; i++) {
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

    if (demoMode == 0 || (millis() >= lastDemoTime + demoTimes[demoMode] && advanceDemo)) {
      lastDemoTime = millis();
      // advance demo
      demoMode++;
      demoMode = demoMode % numDemos;
      if (demoMode == 0) demoMode = 1;
      switch (demoMode) {
        case 1: beginRain(); break;
        case 3: beginWander(); break;
        case 4: w = 0; break;
        case 6: FastLED.clear(); break;
        case 7: snake_frame = 0; break;
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
        
        case 2: // color sphere
        hue += 2;
//        setColor( CHSV( hue, 255, value ) );
        for (byte i = 0; i < 8; i++) {
          for (byte j = 0; j < 8; j++) {
            for (byte k = 0; k < 8; k++) {
              float dx = float(i) - 3.5;
              float dy = float(j) - 3.5;
              float dz = float(k) - 3.5;
              float dist = sqrt( pow(dx, 2) + pow(dy, 2) + pow(dz, 2) );
              float offset = dist * 0.25;
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
        break;

        case 4: // function plotter
        FastLED.clear();
        {
//          unsigned long startTime = millis();
          for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
              float dx = float(i) - 3.5;
              float dy = float(j) - 3.5;
              float dist = sqrt( pow(dx, 2) + pow(dy, 2) );
              
              float z = 2.0 * cos((dist + w) * 1.5) + 3.5;
              drawSmoothedPixel(i, j, z, CRGB(255 - i*32, 31 + i*32, j*32));

              w += w_increment;
            }
          }
//          Serial.println(millis() - startTime);
          break;
        }

        case 5: // sphere with the leds off outside the sphere
        hue += 1;
        hue2 += 3;
//        setColor( CHSV( hue, 255, value ) );
        for (byte i = 0; i < 8; i++) {
          for (byte j = 0; j < 8; j++) {
            for (byte k = 0; k < 8; k++) {
              float dx = float(i) - 3.5;
              float dy = float(j) - 3.5;
              float dz = float(k) - 3.5;
              float dist = sqrt( pow(dx, 2) + pow(dy, 2) + pow(dz, 2) );
              float offset = dist * 0.1649572197684645;
              if(dist > 4) {
                leds[ getIndex( i, j, k ) ] = CRGB( 0, 0, 0 );
              } else {
//                leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 255), 255, byte(offset * 255) - hue );
                leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 255), 255, cubicwave8(hue2 - byte(offset * 255)) );
//                leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 255), 255, hue - byte(offset * 255) );
//                leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 255), 255, hue - byte((1.0-((offset-0.5)*2)) * 255) );
              }
              
            }
          }
        }
        break;

        case 6: // another dumb color thing
        FastLED.clear();

        bounceX += bounceDX;
        if(bounceX <= 0 || bounceX >= value-8*cubeScale) {
          bounceDX *= -1;
        }
        bounceY += bounceDY;
        if(bounceY <= 0 || bounceY >= value-8*cubeScale) {
          bounceDY *= -1;
        }
        bounceZ += bounceDZ;
        if(bounceZ <= 0 || bounceZ >= value-8*cubeScale) {
          bounceDZ *= -1;
        }

        for (byte i = 0; i < 8; i++) {
          for (byte j = 0; j < 8; j++) {
            for (byte k = 0; k < 8; k++) {
              leds[ getIndex( i, j, k ) ] = CRGB( i*cubeScale+(int)bounceX, j*cubeScale+(int)bounceY, k*cubeScale+(int)bounceZ);
            }
          }
        }
        
        break;

        case 7: // color snake
        if(snake_frame == 0) {
          FastLED.clear();
        }
        for (int i = 0; i < snake_frame; i++) {
          leds[i] = CHSV( int(1.0*i/512*255), 255, value);
        }
        snake_frame = (snake_frame+1) % 512;
        break;

        case 8: // color pyramid
        hue += 4;
        for (byte i = 0; i < 8; i++) {
          for (byte j = 0; j < 8; j++) {
            for (byte k = 0; k < 8; k++) {
              float dx = float(i) - 3.5;
              float dy = float(j) - 3.5;
              float dist = sqrt(dx*dx + dy*dy);
              if(dist >= 4-(k/2)) {
                leds[ getIndex( i, j, k ) ] = CRGB( 0, 0, 0);
              } else {
                double angle = atan2(dy,dx)/PI;
//                int temp_value = int(1.0*(7-k)/8*value);
                leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(angle * 128), 255, value);
              }
            }
          }
        }
        break;

      } //end of switch(demo)

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
          
          float normBrightness = max(0.0, 1.0-dist);
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

byte getX(int i) {
  return byte(i /64);
}

byte getY(int i) {
  return byte((i/8) % 8);
}

byte getZ(int i) {
  return byte(i % 8);
}

void setColor(CRGB color) {
  fill_solid( leds, 512, color );
}

void clear() {
  FastLED.clear();
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
