/*
 * ColorSphere.cpp
 *
 *  Created on: Oct 27, 2018
 *      Author: raffc
 */

#include "ColorSphere.h"
#include <FastLED.h>
#include "cube.h"

const byte brightness = 192;

ColorSphere::ColorSphere() {
  duration = 30000;
}

void ColorSphere::initialize() {}

void ColorSphere::tick() {
  hue += 2;
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      for (byte k = 0; k < 8; k++) {
        float dx = float(i) - 3.5;
        float dy = float(j) - 3.5;
        float dz = float(k) - 3.5;
        float dist = sqrt( sqr(dx) + sqr(dy) + sqr(dz) );
        float offset = dist * 0.25;
        leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 128), 255, brightness);
      }
    }
  }
}
