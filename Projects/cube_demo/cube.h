/*
 * cube.cpp
 *
 *  Created on: Oct 27, 2018
 *      Author: Chris Raff
 */

#include <FastLED.h>

extern CRGB *leds;

void cubeBegin();

// math
#define PI 3.14159265359
#define PI2 6.28308530718
float sqr(float);
extern const float inv255;
extern const float invPi;
extern const float invTau_256;
extern float cosf(float);
float sinf(float);
float randomf();

#ifndef CONVENIENCE_STRUCTS_
#define CONVENIENCE_STRUCTS_

// convenience structs
typedef struct {
  float x, y, z;
} float3;

#endif /* CONVENIENCE_STRUCTS_*/

// convenience functions
int getIndex(int x, int y, int z);

byte getX(int);

byte getY(int);

byte getZ(int);

boolean inCubeBounds(int, int, int);

void setColor(CRGB);

int max(int, int);
int min(int, int);

CRGB scaleColor(CRGB color, byte brightness);

// cube drawing functions
void drawSmoothedPixel(float x, float y, float z, CRGB color);

void drawSphereSolid(float x, float y, float z, float r, CRGB color);

void drawCubeSolid(float cx, float cy, float cz, float sx, float sy, float sz, float rx, float ry, float rz, float rAngle, CRGB color);

void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, CRGB color);
