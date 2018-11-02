/*
 * SpheresSpinning.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#include "SpheresSpinning.h"
#include <FastLED.h>
#include "cube.h"

SpheresSpinning::SpheresSpinning() {
  duration = 15000;
}

void SpheresSpinning::initialize() {
  angle = 0;
}

void SpheresSpinning::tick() {
  FastLED.clear();

  angle += angleIncremement;
  const float sphereRadius = 1.0;
  const float interRadius = 2.0, cosSA = cosf(angle), sinSA = sinf(angle);
  const float cosUA = cosf(angle*0.4), sinUA = sinf(angle*0.4);
  drawSphereSolid(3.5 + interRadius*cosSA * cosUA,
            3.5       + interRadius*cosSA * sinUA,
            3.5 + interRadius*sinSA,
            sphereRadius, CRGB(0, 0, 255));

  const float cosSA1 = cosf(angle + PI2 * 0.33333333), sinSA1 = sinf(angle + PI2 * 0.33333333);
  drawSphereSolid(3.5 + interRadius*cosSA1  * cosUA,
            3.5       + interRadius*cosSA1  * sinUA,
            3.5 + interRadius*sinSA1,
            sphereRadius, CRGB(0, 255, 0));

  const float cosSA2 = cosf(angle + PI2 * 0.66666666), sinSA2 = sinf(angle + PI2 * 0.66666666);
  drawSphereSolid(3.5 + interRadius*cosSA2  * cosUA,
            3.5       + interRadius*cosSA2  * sinUA,
            3.5 + interRadius*sinSA2,
            sphereRadius, CRGB(255, 0, 0));
}
