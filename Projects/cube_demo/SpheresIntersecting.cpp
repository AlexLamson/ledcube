/*
 * SpheresIntersecting.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: raffc
 */

#include "SpheresIntersecting.h"
#include <FastLED.h>
#include "cube.h"

SpheresIntersecting::SpheresIntersecting() {
  duration = 15000;
}

void SpheresIntersecting::initialize() {
  angle = 0;
}

void SpheresIntersecting::tick() {
  FastLED.clear();
  angle += angleIncremement;
  const float sphereRadius = 1.0, interRadius = 2.0;

  const float cosSA = cosf(angle*0.5), cosSA1 = cosf(angle*1.0), cosSA2 = cosf(angle*1.5);
  drawSphereSolid(3.5 + interRadius*cosSA,
            3.5,
            3.5,
            sphereRadius, CRGB(0, 0, 255));

  drawSphereSolid(3.5,
            3.5 + interRadius*cosSA1,
            3.5,
            sphereRadius, CRGB(0, 255, 0));

  drawSphereSolid(3.5,
            3.5,
            3.5       + interRadius*cosSA2,
            sphereRadius, CRGB(255, 0, 0));
}
