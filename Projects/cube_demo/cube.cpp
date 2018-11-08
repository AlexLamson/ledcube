/*
 * cube.cpp
 *
 *  Created on: Oct 27, 2018
 *      Author: Chris Raff
 */

#include "cube.h"
#include <FastLED.h>

#ifndef DATA0
#define DATA0 5  // wemos D1 mini D1
#endif
#ifndef DATA1
#define DATA1 4  // wemos D1 mini D2
#endif
#ifndef DATA2
#define DATA2 0  // wemos D1 mini D3
#endif
#ifndef DATA3
#define DATA3 2  // wemos D1 mini D4
#endif

CRGB leds[512];

void cubeBegin() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);
}

// math
// #define PI 3.14159265359
// #define PI2 6.28308530718
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

// convenience functions
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

// cube drawing functions
void drawSmoothedPixel(float x, float y, float z, CRGB color) {

  for (int i = int(x); i <= int(x + 1); i++) {
    for (int j = int(y); j <= int(y + 1); j++) {
      for (int k = int(z); k <= int(z + 1); k++) {

        if (!(i < 0 || j < 0 || k < 0 || i >= 8 || j >= 8 || k >= 8)) {
          float compX = x - i, compY  = y - j, compZ = z - k;
          float dist = sqrt( sqr(compX) + sqr(compY) + sqr(compZ) );

          // float normBrightness = max(0.0, 1.0-dist);
          // byte brightness = mapBrightness(normBrightness);
          byte brightness = byte(255.0*max(0.0, 1.0 - dist));

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

// takes the cube center, the dimensions of the cube, the rotation axis, and rotation angle
// rotation axis must have unit length
void drawCubeSolid(
    float cx, float cy, float cz,
    float sx, float sy, float sz,
    float rx, float ry, float rz,
    float rAngle,
    CRGB color)
{
  const float outsideCutoff = 1.0;
  const float outsideCutoffInv = 1.0;

  // float cosAngle = cosf(rAngle);
  // float sinAngle = sinf(rAngle);
  float cosAngle = cos(rAngle); // jittery if using the optimized cos, sin
  float sinAngle = sin(rAngle);


  // rotated x norm
  float nxx = cosAngle + 0 + (1 - cosAngle) * rx * rx;
  float nxy = 0 + sinAngle * rz + (1 - cosAngle) * rx * ry;
  float nxz = 0 + sinAngle * (-ry) + (1 - cosAngle) * rx * rz;

  // rotated y norm
  float nyx = 0 + sinAngle * (-rz) + (1 - cosAngle) * ry * rx;
  float nyy = cosAngle + 0 + (1 - cosAngle) * ry * ry;
  float nyz = 0 + sinAngle * rx + (1 - cosAngle) * ry * rz;

  // rotated z norm
  float nzx = 0 + sinAngle * ry + (1 - cosAngle) * rz * rx;
  float nzy = 0 + sinAngle * (-rx) + (1 - cosAngle) * rz * ry;
  float nzz = cosAngle + 0 + (1 - cosAngle) * rz * rz;


  // half cube dims
  float sx2 = sx / 2;
  float sy2 = sy / 2;
  float sz2 = sz / 2;

  // only for debug color
  float sx_ = 1.0/sx;
  float sy_ = 1.0/sy;
  float sz_ = 1.0/sz;

  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      for (byte k = 0; k < 8; k++) {
        float x = i, y = j, z = k;
        float dx = x - cx, dy = y - cy, dz = z - cz;

        float xdist_ = dx*nxx + dy*nxy + dz*nxz;
        float xdista = (xdist_ < 0 ? -xdist_ : xdist_); // abs was returning an int?
        float xdist = xdista - sx2;
        if (xdist > 1)
          continue;

        float ydist_ = dx*nyx + dy*nyy + dz*nyz;
        float ydista = (ydist_ < 0 ? -ydist_ : ydist_);
        float ydist = ydista - sy2;
        if (ydist > 1)
          continue;

        float zdist_ = dx*nzx + dy*nzy + dz*nzz;
        float zdista = (zdist_ < 0 ? -zdist_ : zdist_);
        float zdist = zdista - sz2;
        if (zdist > 1)
          continue;

        float distSqr = sqr(max(xdist, 0.0f)) + sqr(max(ydist, 0.0f)) + sqr(max(zdist, 0.0f));
        // float distSqr = max(xdist, 0.0f) + max(ydist, 0.0f) + max(zdist, 0.0f);


        // debug color
        // color = CRGB(
        //   byte(150.0 * (xdist_ + sx2) * sx_) + 40,
        //   byte(150.0 * (ydist_ + sy2) * sy_) + 40,
        //   byte(150.0 * (zdist_ + sz2) * sz_) + 40
        // );

        if (distSqr <= 0) { // fully inside cube
          leds[ getIndex(i, j, k) ] = color;
        }
        else if (distSqr < outsideCutoff*outsideCutoff) { // within unit distance of cube
          byte brightness = byte( 256.0 - 255.0 * (sqrt(distSqr)) * outsideCutoffInv );
          leds[ getIndex(i, j, k) ] = CRGB( scale8(color.r, brightness), scale8(color.g, brightness), scale8(color.b, brightness) );
        }
      }
    }
  }
}

void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, CRGB color) {
  byte xSmallest = int(min(x1, x2)), ySmallest = int(min(y1, y2)), zSmallest = int(min(z1, z2));
  byte xLargest = 1+int(max(x1, x2)), yLargest = 1+int(max(y1, y2)), zLargest = 1+int(max(z1, z2));

  float x12 = x2 - x1, y12 = y2 - y1, z12 = z2 - z1;
  float lDistSqr = sqr(x12) + sqr(y12) + sqr(z12);
  float lDist = sqrt(lDistSqr);
  float lDist_inv = 1.0 / lDist;
  float lDistSqr_inv = 1.0 / lDistSqr;

  for (byte i = xSmallest; i <= xLargest; i++) {
    for (byte j = ySmallest; j <= yLargest; j++) {
      for (byte k = zSmallest; k <= zLargest; k++) {
        float x1p = x1 - i, y1p = y1 - j, z1p = z1 - k;

        float t = -(x1p*x12 + y1p*y12 + z1p*z12) * lDist_inv;

        // if the projection onto the line is beyond the endpoints
        if (t <= -1.0 || t-lDist >= 1.0)
          continue;

        // compute distance to unbounded line
        float xCross = y12 * z1p - z12 * y1p;
        float yCross = x12 * z1p - z12 * x1p;
        float zCross = x12 * y1p - y12 * x1p;

        float distSqr = (sqr(xCross) + sqr(yCross) + sqr(zCross)) * lDistSqr_inv;

        if (distSqr >= 1.0)
          continue;
        // The pixel is near the line

        if (t < 0.0) {
          distSqr = sqr(x1p) + sqr(y1p) + sqr(z1p);
          if (distSqr >= 1.0)
            continue;
        }
        else if (t-lDist > 0.0) {
          distSqr = sqr(x2 - i) + sqr(y2 - j) + sqr(z2 - k);
          if (distSqr >= 1.0)
            continue;
        }

        byte brightness = byte( 256.0 - 255.0 * sqrt(distSqr) );
        leds[ getIndex(i, j, k) ] += CRGB( scale8(color.r, brightness), scale8(color.g, brightness), scale8(color.b, brightness) );
      }
    }
  }
}


// unused after gamma correction - also is pretty inefficient
// byte mapBrightness(float input) {
//   return 255*(0.5-cosf( PI * pow(input, 1.5) )*0.5); // power of 1.5 seems to preserve brightness pretty well.
// }

// only used in Rain demo, does it work?
CRGB scaleColor(CRGB color, byte brightness) {
  return CRGB( scale8(color.r, brightness), scale8(color.g, brightness), scale8(color.b, brightness) );
}
