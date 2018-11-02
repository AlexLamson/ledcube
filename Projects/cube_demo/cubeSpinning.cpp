/*
 * cubeSpinning.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Chris Raff
 */

#include "cubeSpinning.h"
#include <FastLED.h>
#include "cube.h"

cubeSpinning::cubeSpinning() {
  duration = 20000;

  angle = 0;
}

void cubeSpinning::initialize() {
  angle = 0;
}

void cubeSpinning::tick() {
  FastLED.clear();

//  drawCubeSolid(
//      3.5, 3.5, 3.5,
//      20.0, 0.0, 20.0,
//      0,0,1,
//      angle,
//      CRGB(0,0,200));
//  drawCubeSolid(
//        3.5, 3.5, 3.5,
//        20.0, 0.0, 20.0,
//        1,0,0,
//        angle,
//        CRGB(200,0,0));
  drawCubeSolid(
        3.5, 3.5, 3.5 + 3.5 * sinf(angle),
        20.0, 20.0, 0.0,
        0,1,0,
        0,
        CRGB(0,200,0));

  angle += angleIncrement;
}
