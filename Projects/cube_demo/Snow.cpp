/*
 * Rain.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#include "Snow.h"
#include <FastLED.h>
#include "cube.h"
#include <Streaming.h>

Snow::Snow() {
  duration = 20000;
}

void Snow::initialize() {
  for (byte i = 0; i < numDrops; i++) {
    drops[i].init();
    drops[i].z = 8 + random(8);
  }

  memset(heights, 0.0, 64);
}

void Snow::tick() {
  FastLED.clear();

  for (int i = 0; i < numDrops; i++) {

    if (drops[i].z < heights[drops[i].x + drops[i].y*8] - 1.0) {
      drops[i].init();
      heights[drops[i].x + drops[i].y*8] += 0.5;
    }

    drops[i].tick();

    drops[i].draw();
  }

  for (int i = 0; i < 64; i++) {
    byte x = i/8, y = i%8;
    heights[i] = max (0, heights[i] - 0.001);
    drawLine(x + 0.01, y + 0.01, 0, x, y, heights[i], 0xFFFFFF);
  }
}


Snow::Drop::Drop() {
  init();
}
void Snow::Drop::init() {
  x = random(8);
  y = random(8);
  z = 8;

  dz = 0.1 + float(random(10)) / 100;
}
void Snow::Drop::tick() {
  z -= dz;

  // reset if too low
  if (z < -4) {
    init();
  }
}
void Snow::Drop::draw() {
  float currZ = z;
  const byte length = 1;
  while (currZ - z < length) {
    drawSmoothedPixel(x, y, currZ, 0xFFFFFF);
    currZ += 1;
  }
}
