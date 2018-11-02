/*
 * ColorCone.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: raffc
 */

#include "ColorCone.h"
#include <FastLED.h>
#include "cube.h"

ColorCone::ColorCone() {
  duration = 10000;
  hue = 0;
}

void ColorCone::initialize() { }

void ColorCone::tick() {
  hue += 4;
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      for (byte k = 0; k < 8; k++) {
        float dx = float(i) - 3.5;
        float dy = float(j) - 3.5;
        float dist = sqrt(dx*dx + dy*dy);
        if(dist >= 4-(k/2)) {
          leds[ getIndex( i, j, k ) ] = CRGB( 0, 0, 0);
        } else {
          double angle = atan2(dy,dx)/PI;
          leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(angle * 128), 255, 255);
        }
      }
    }
  }
}
