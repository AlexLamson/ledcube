/*
 * Fireplace.cpp
 *
 *  Created on: Oct 27, 2018
 *      Author: raffc
 */

#include "Fireplace.h"
#include <FastLED.h>
#include "cube.h"

const float sparkProbability = 0.9;
const int sparkHeat = 255;
const byte samplesPerPoint = 8;



Fireplace::Fireplace() {
  duration = 60000;
}

void Fireplace::initialize() {

}

void Fireplace::tick() {
  FastLED.clear();


  for (byte x = 0; x < 8; x++) {
    for (byte y = 0; y < 8; y++) {
      
      if(randomf() < sparkProbability) {
        leds[ getIndex( x, y, 0 ) ] = CRGB( sparkHeat, 0, 0);
      }

    }
  }


//  leds[ getIndex( x-1, y, 0 ) ].g
//
  for (byte x = 0; x < 8; x++) {
    for (byte y = 0; y < 8; y++) {
      for (byte z = 1; z < 8; z++) {

//        leds[ getIndex( x, y, z ) ] -= 128;
//        leds[ getIndex( x, y, z ) ] /= 2;

//        if(inCubeBounds(x, y, z-1)) leds[ getIndex( x, y, z ) ] += leds[ getIndex( x, y, z-1 ) ]/samplesPerPoint;

        if(inCubeBounds(x-1, y, z-1)) leds[ getIndex( x, y, z ) ] += leds[ getIndex( x-1, y, z-1 ) ]/samplesPerPoint;
        if(inCubeBounds(x+1, y, z-1)) leds[ getIndex( x, y, z ) ] += leds[ getIndex( x+1, y, z-1 ) ]/samplesPerPoint;
        if(inCubeBounds(x, y-1, z-1)) leds[ getIndex( x, y, z ) ] += leds[ getIndex( x, y-1, z-1 ) ]/samplesPerPoint;
        if(inCubeBounds(x, y+1, z-1)) leds[ getIndex( x, y, z ) ] += leds[ getIndex( x, y+1, z-1 ) ]/samplesPerPoint;

        if(inCubeBounds(x-1, y, z)) leds[ getIndex( x, y, z ) ] += leds[ getIndex( x-1, y, z ) ]/samplesPerPoint;
        if(inCubeBounds(x+1, y, z)) leds[ getIndex( x, y, z ) ] += leds[ getIndex( x+1, y, z ) ]/samplesPerPoint;
        if(inCubeBounds(x, y-1, z)) leds[ getIndex( x, y, z ) ] += leds[ getIndex( x, y-1, z ) ]/samplesPerPoint;
        if(inCubeBounds(x, y+1, z)) leds[ getIndex( x, y, z ) ] += leds[ getIndex( x, y+1, z ) ]/samplesPerPoint;
      }
    }
  }
}
