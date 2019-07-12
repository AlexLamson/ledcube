/*
 * DataSnake.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: raffc
 */

#include "DataSnake.h"
#include <FastLED.h>
#include "cube.h"

DataSnake::DataSnake() {
  duration = 20450;
  frame = 0;
}

void DataSnake::initialize() {
  frame = 0;
}

void DataSnake::tick() {
  if(frame == 0) {
    FastLED.clear();
  }

  if (frame < 512)
    leds[frame] = CHSV( int(1.0*frame/512*255), 255, 192);
  else
  leds[frame%512] = 0x0; // clear

  frame = (frame+1) % 1024;
}
