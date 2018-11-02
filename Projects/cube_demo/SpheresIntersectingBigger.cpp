/*
 * IntersectingSpheresBigger.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: raffc
 */

#include "SpheresIntersectingBigger.h"

#include <FastLED.h>
#include "cube.h"

SpheresIntersectingBigger::SpheresIntersectingBigger() {
  duration = 15000;
}

void SpheresIntersectingBigger::initialize() {
  angle = 0;
}

void SpheresIntersectingBigger::tick() {
  FastLED.clear();
  {
    angle += angleIncremement;

    const float radiusCenter = 5.0, radiusAmplitude = 3.0;
    const float sphereRadius0 = radiusCenter + radiusAmplitude * cosf(angle * 0.4),
               sphereRadius1 = radiusCenter + radiusAmplitude * cosf(angle * 0.4 + PI2 * 0.33333333),
               sphereRadius2 = radiusCenter + radiusAmplitude * cosf(angle * 0.4 + PI2 * 0.66666666);
    const float interRadius = 4.0, cosSA = cosf(angle), sinSA = sinf(angle);
    drawSphereSolid(
      3.5 + interRadius*cosSA,
      3.5 + interRadius*sinSA,
      3.5,
      sphereRadius0,
      CRGB(0, 0, 255));

    const float cosSA1 = cosf(angle + PI2 * 0.33333333), sinSA1 = sinf(angle + PI2 * 0.33333333);
    drawSphereSolid(
      3.5 + interRadius*cosSA1,
      3.5 + interRadius*sinSA1,
      3.5,
      sphereRadius1,
      CRGB(0, 255, 0));

    const float cosSA2 = cosf(angle + PI2 * 0.66666666), sinSA2 = sinf(angle + PI2 * 0.66666666);
    drawSphereSolid(
      3.5 + interRadius*cosSA2,
      3.5 + interRadius*sinSA2,
      3.5,
      sphereRadius2,
      CRGB(255, 0, 0));
  }

}
