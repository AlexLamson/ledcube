/*
 * ColorSphereTrimmed.cpp
 *
 *  Created on: Oct 28, 2018
 *      Author: raffc
 */

#include "ColorSphereTrimmed.h"
#include <FastLED.h>
#include "cube.h"

ColorSphereTrimmed::ColorSphereTrimmed() {
  duration = 20000;

  hue = 0;
  hue2 = 0;
}

void ColorSphereTrimmed::initialize() {
  hue2 = 0;
}

void ColorSphereTrimmed::tick() {
  hue += 1;
  hue2 += 3;
  // setColor( CHSV( hue, 255, value ) );
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      for (byte k = 0; k < 8; k++) {
        float dx = float(i) - 3.5;
        float dy = float(j) - 3.5;
        float dz = float(k) - 3.5;
        float dist = sqrt( sqr(dx) + sqr(dy) + sqr(dz) );
        float offset = dist * 0.1649572197684645; // divide by distance from center to corner
        if(dist > 4) {
          leds[ getIndex( i, j, k ) ] = CRGB( 0, 0, 0 );
        } else {
          // leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 255), 255, byte(offset * 255) - hue );
          leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 255), 255, cubicwave8(hue2 - byte(offset * 255)) );
          // leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 255), 255, hue - byte(offset * 255) );
          // leds[ getIndex( i, j, k ) ] = CHSV( hue - byte(offset * 255), 255, hue - byte((1.0-((offset-0.5)*2)) * 255) );
        }

      }
    }
  }
}
