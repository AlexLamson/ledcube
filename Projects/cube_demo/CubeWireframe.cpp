/*
 * LineSpinning.cpp
 *
 *  Created on: Nov 7, 2018
 *      Author: Chris Raff
 *
 *  This is more of a demonstration of implementation than a real demo that should be included
 */

#include "LineSpinning.h"
#include <FastLED.h>
#include "cube.h"

LineSpinning::LineSpinning() {
  duration = 15000;
  angle = 0;
}

void LineSpinning::initialize() {
  angle = 0;
}

void LineSpinning::tick() {
  FastLED.clear();

  angle += 0.01;

  float cosa = cos(angle), sina = sin(angle);

  drawLine(
      3.5 - 3.0 * cosa,
      3.5 - 3.0 * sina,
      3.5,
      3.5 + 3.0 * cosa,
      3.5 + 3.0 * sina,
      3.5,
      CRGB(0, 255, 255));
}
