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
void drawSphereSolid(float, float, float, float, CRGB);
byte mapBrightness(float);
CRGB scaleColor(CRGB, byte);
byte getX(int);
byte getY(int);
byte getZ(int);

CRGB leds[512];

// end cube stuff

byte demoMode = 8; // proj, rain, color sphere, wander, function, trimmed sphere, bouncing in rgb space, color snake, color pyramid, floating points, spheres spinning
const unsigned int demoTimes[] = {0, 20000, 30000, 20000, 30000, 20000, 15000, 21000, 10000, 15000, 20000};
bool advanceDemo = false; // toggle to stay on one demo forever
//const unsigned int demoTimes[] = {100, 100, 100, 100, 100, 600000, 100, 100, 100};
const byte numDemos = 11;

// projection
//byte rgb[3*512];
//byte brightness = 64;

// demos

const byte tickMillis = 20;
unsigned long lastTickTime = 0;

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
#define PI2 6.28308530718
float sqr(float x) {
  return x*x;
}
const float inv255 = 1.0/255;
const float invPi = 1.0/PI;
const float invTau_256 = 128.0/PI;
float cosf(float theta) {
  while (theta >= PI2) theta -= PI2;
  while (theta < 0) theta += PI2;
  byte b = int(theta * invTau_256) % 256;

  byte out = cubicwave8(128+b); // cubicwave is a sinewave

  return float(out) * 2.0 * inv255 - 1;
}
float sinf(float theta) {
  while (theta >= PI2) theta -= PI2;
  while (theta < 0) theta += PI2;
  byte b = int(theta * invTau_256) % 256;

  byte out = cubicwave8(64+b); // cubicwave is a sinewave

  return float(out) * 2.0 * inv255 - 1;
}
float randomf() {
  return float( random(256) ) * inv255;
}

// colors
byte hue = 0;
byte hue2 = 0;
const byte value = 192; // brightness of hue


//data snake
int snake_frame = 0;


//bouncing in rgb space
int cubeScale = 8;
float bounceX = 0, bounceY = 0, bounceZ = 0;
float bounceDX = 0.2, bounceDY = 0.3, bounceDZ = 0.4;


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

const float momentum = 0.93;

void beginWander() {
  for (byte i = 0; i < numWanderers; i++) {
    wanderers[i].init();
  }
}

// function plot
float w = 0;
float w_increment = -0.001;

// spinning spheres
float sphereAngle = 0.0;
float sphereAngleIncremement = 0.05;

// floating points ...
typedef struct {
  float x, y, z;
} point;

const int numPoints = 32;
point points[numPoints];
CRGB floatColors[numPoints];
point floatMotion = {0, 0, 0};
point floatDMotion= {0, 0, 0};
float floatRotZ = 0.0;
float floatDRotZ = 0.0;
void beginFloatingPoints() {
  floatMotion = {0, 0, 0};
  floatDMotion = {0, 0, 0};
  floatRotZ = 0;
  floatDRotZ = 0;

  for (int i = 0; i < numPoints; i++) {
    points[i] = {randomf() * 10.0f - 1.0f, randomf() * 10.0f - 1.0f, randomf() * 10.0f - 1.0f};
    floatColors[i] = CHSV(random(256), 255, 255);
  }
}

void setup() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);

  Serial.begin(115200);

  clear();
  FastLED.setCorrection( Typical8mmPixel );
  FastLED.show();
}

void loop() {
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
      case 9: beginFloatingPoints(); break;
    }
  }

  if (millis() >= lastTickTime + tickMillis) {
    lastTickTime = millis();

    const unsigned long computeStartTime = micros();

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
            float dist = sqrt( sqr(dx) + sqr(dy) + sqr(dz) );
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
//        unsigned long startTime = micros();
        for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
            float dx = float(i) - 3.5;
            float dy = float(j) - 3.5;
            float dist = sqrt( sqr(dx) + sqr(dy) );

            float z = 2.0 * cosf((dist + w) * 1.5) + 4.5; // 3.5 + 1
//            drawSmoothedPixel(i, j, z, CRGB(255 - i*32, 31 + i*32, j*32));
            float r = 1.5 * cosf(PI + (dist + w) * 3.0) + 2.0;
            r = pow(r, 0.15);
            float rInv = 1.0 / r;

            CRGB col = CRGB(255 - i*32, 31 + i*32, j*32);
//            for (int k = int(z-r); k <= int(z+r); k++) {
            for (int k = 0; k < 8; k++) {
//              if (k >= 0 && k < 8) {
          float distToCentroid = float(k) - z + r;
          distToCentroid *= (distToCentroid < 0 ? -1 : 1);
                byte val = (max(0.0, double(r - distToCentroid)) * rInv) * 255;
          leds[ getIndex(i, j, k) ] = CRGB(scale8(col.r, val), scale8(col.g, val), scale8(col.b, val));
//              }
            }

            w += w_increment;
            if (w < -PI2 * 0.6666666)
              w += PI2 * 0.6666666;
          }
        }
//        Serial.println(micros() - startTime);
        break;
      }

      case 5: // trimmed sphere
      hue += 1;
      hue2 += 3;
//        setColor( CHSV( hue, 255, value ) );
      for (byte i = 0; i < 8; i++) {
        for (byte j = 0; j < 8; j++) {
          for (byte k = 0; k < 8; k++) {
            float dx = float(i) - 3.5;
            float dy = float(j) - 3.5;
            float dz = float(k) - 3.5;
            float dist = sqrt( sqr(dx) + sqr(dy) + sqr(dz) );
            float offset = dist * 0.1649572197684645; // divide by distance from center to corner
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

      case 6: // bouncing in rgb space
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

      if (snake_frame < 512)
        leds[snake_frame] = CHSV( int(1.0*snake_frame/512*255), 255, value);
      else
      leds[snake_frame%512] = 0x0; // clear

      snake_frame = (snake_frame+1) % 1024;
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
              leds[ getIndex( i, j, k ) ] = CRGB( 1, 1, 1);
            } else {
              double angle = atan2(dy,dx)/PI;
//                int temp_value = int(1.0*(7-k)/8*value);
              leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(angle * 128), 255, value);
            }
          }
        }
      }
      break;

      case 9: // spinning spheres
      clear ();
      {
        sphereAngle += sphereAngleIncremement;
        const float sphereRadius = 1.0; //, sphereDialation = 0.7;
        const float interRadius = 2.0, cosSA = cosf(sphereAngle), sinSA = sinf(sphereAngle);
        const float cosUA = cosf(sphereAngle*0.4), sinUA = sinf(sphereAngle*0.4);
        drawSphereSolid(3.5 + interRadius*cosSA * cosUA,
                  3.5       + interRadius*cosSA * sinUA,
                  3.5 + interRadius*sinSA, //       * sinSA,
                  sphereRadius, CRGB(0, 0, 255));

//        const float cosSA1 = cosf(sphereAngle*sphereDialation), sinSA1 = sinf(sphereAngle*sphereDialation);
        const float cosSA1 = cosf(sphereAngle + PI2 * 0.33333333), sinSA1 = sinf(sphereAngle + PI2 * 0.33333333);
        drawSphereSolid(3.5 + interRadius*cosSA1  * cosUA,
                  3.5       + interRadius*cosSA1  * sinUA,
                  3.5 + interRadius*sinSA1, //        * sinSA,
                  sphereRadius, CRGB(0, 255, 0));

//        const float cosSA2 = cosf(sphereAngle*sqr(sphereDialation)), sinSA2 = sinf(sphereAngle*sqr(sphereDialation));
        const float cosSA2 = cosf(sphereAngle + PI2 * 0.66666666), sinSA2 = sinf(sphereAngle + PI2 * 0.66666666);
        drawSphereSolid(3.5 + interRadius*cosSA2  * cosUA,
                  3.5       + interRadius*cosSA2  * sinUA,
                  3.5 + interRadius*sinSA2, //        * sinSA,
                  sphereRadius, CRGB(255, 0, 0));
      }
      break;

      case 10: // floating points
      clear();
      const float dMotionMax = 0.005;
      floatDMotion.x += (randomf() - 0.5) * 0.001;
      floatDMotion.y += (randomf() - 0.5) * 0.001;
      floatDMotion.z += (randomf() - 0.5) * 0.001;
      if (abs(floatDMotion.x) > dMotionMax)
        floatDMotion.x *= 0.9;
      if (abs(floatDMotion.y) > dMotionMax)
        floatDMotion.y *= 0.9;
      if (abs(floatDMotion.z) > dMotionMax)
        floatDMotion.z *= 0.9;

      floatMotion.x += floatDMotion.x;
      floatMotion.y += floatDMotion.y;
      floatMotion.z += floatDMotion.z;

      const float motionCurb = 0.9; // smoothly approach a top speed
      floatMotion.x *= motionCurb;
      floatMotion.y *= motionCurb;
      floatMotion.z *= motionCurb;

      // TODO rotation figuring

      // apply the motion to points and draw
      for (int i = 0; i < numPoints; i++) {
        points[i].x += floatMotion.x;
        points[i].y += floatMotion.y;
        points[i].z += floatMotion.z;

        // if the point is out of bounds replace it on the edge (don't care if it gets replaced next tick)
        // TODO make uniform with respect to the direction you're moving
        if (points[i].x < -1 || points[i].x > 9 ||
            points[i].y < -1 || points[i].y > 9 ||
            points[i].z < -1 || points[i].z > 9) {

          float side = 10 * random(2) - 1; // pick one edge or the other
          float randu= randomf() * 10 - 1;
          float randv= randomf() * 10 - 1;

          switch (random(3)) {
          case 0: // x
            points[i].x = side; points[i].y = randu; points[i].z = randv;
            break;
          case 1: // y
            points[i].y = side; points[i].x = randu; points[i].z = randv;
            break;
          case 2: // z
            points[i].z = side; points[i].x = randu; points[i].y = randv;
            break;
          }

          floatColors[i] = CHSV(random(256), 255, 255);
        } else {
          drawSmoothedPixel(points[i].x, points[i].y, points[i].z, floatColors[i]);
        }
      }

      break;


    } //end of switch(demo)

    unsigned long computeTime = micros() - computeStartTime;

    Serial.println(computeTime);

    FastLED.show();
  }
}

// ----

void drawSmoothedPixel(float x, float y, float z, CRGB color) {

//  for (int i = int(x - 1); i <= int(x + 1); i++) {
//    for (int j = int(y - 1); j <= int(y + 1); j++) {
//      for (int k = int(z - 1); k <= int(z + 1); k++) {
  for (int i = int(x); i <= int(x + 1); i++) {
    for (int j = int(y); j <= int(y + 1); j++) {
      for (int k = int(z); k <= int(z + 1); k++) {

        if (!(i < 0 || j < 0 || k < 0 || i >= 8 || j >= 8 || k >= 8)) {
          float compX = x - i, compY  = y - j, compZ = z - k;
          float dist = sqrt( sqr(compX) + sqr(compY) + sqr(compZ) );

          float normBrightness = max(0.0, 1.0-dist);
          byte brightness = mapBrightness(normBrightness);

          byte cx = i, cy = j, cz = k;

          leds[ getIndex(cx, cy, cz) ] += CRGB( scale8(color.r, brightness), scale8(color.g, brightness), scale8(color.b, brightness) );
        }

      }
    }
  }
}

void drawSphereSolid(float x, float y, float z, float r, CRGB color) {
  for (int i = int(x-r); i <= int(x+r + 1); i++) {
    for (int j = int(y-r); j <= int(y+r + 1); j++) {
      for (int k = int(z-r); k <= int(z+r + 1); k++) {

        if (i < 0 || j < 0 || k < 0 || i >= 8 || j >= 8 || k >= 8)
          continue;

        float compX = x - i, compY  = y - j, compZ = z - k;
        float distSqr = sqr(compX) + sqr(compY) + sqr(compZ);

        // avoid computations on space outside the sphere
        if (distSqr > sqr(r + 1))
          continue;

        byte brightness = 255;
        if (distSqr > sqr(r)) {
          // this pixel is outside the sphere but within a unit of it
          brightness = byte( 256.0 - 255.0 * (sqrt(distSqr) - r) );
        }

        byte cx = i, cy = j, cz = k;

        leds[ getIndex(cx, cy, cz) ] += CRGB( scale8(color.r, brightness), scale8(color.g, brightness), scale8(color.b, brightness) );

      }
    }
  }
}

byte mapBrightness(float input) {
  return 255*(0.5-cosf( PI * pow(input, 1.5) )*0.5); // power of 1.5 seems to preserve brightness pretty well.
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
  return byte(i>>6);
}

byte getY(int i) {
  return byte((i>>3) % 8);
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
  dx += (randomf() - 0.5) * 0.075;
  dy += (randomf() - 0.5) * 0.075;
  dz += (randomf() - 0.5) * 0.075;

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

