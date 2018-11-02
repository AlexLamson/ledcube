/*
 * Rain.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#include "Rain.h"
#include <FastLED.h>
#include "cube.h"

Rain::Rain() {
  duration = 20000;
}

void Rain::initialize() {
  for (byte i = 0; i < numDrops; i++) {
    drops[i].init();
    drops[i].z = 8 + random(8);
  }
}

void Rain::tick() {
  FastLED.clear();

  for (int i = 0; i < numDrops; i++) {

    drops[i].tick();

    drops[i].draw();
  }
}


Rain::Drop::Drop() {
  init();
}
void Rain::Drop::init() {
  x = random(8);
  y = random(8);
  z = 8;

  dz = 0.1 + float(random(10)) / 100;

  color = CHSV( random(256), 255, 255 );
}
void Rain::Drop::tick() {
  z -= dz;

  // reset if too low
  if (z < -4) {
    init();
  }
}
void Rain::Drop::draw() {
  float currZ = z;
  const byte length = 1;
  while (currZ - z < length) {
    drawSmoothedPixel(x, y, currZ, scaleColor(color, 255 - (currZ - z)*(256/length)));
    currZ += 1;
  }
}
