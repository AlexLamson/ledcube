/*
 * RGBSpaceBounce.cpp
 *
 *  Created on: Oct 28, 2018
 *      Author: raffc
 */

#include "RGBSpaceBounce.h"
#include <FastLED.h>
#include "cube.h"

const int value = 192; // used to be a byte, does this matter?

RGBSpaceBounce::RGBSpaceBounce() {
  duration = 15000;
}

void RGBSpaceBounce::initialize() {}

void RGBSpaceBounce::tick() {
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
}
