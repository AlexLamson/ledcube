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
  }

  memset(heights, 0.0, 64 * sizeof(float));
}

void Snow::tick() {
  FastLED.clear();

  for (int i = 0; i < numDrops; i++) {
    // if a snowflake lands on a snow pile, grow the pile and reset the flake
    if (drops[i].z < heights[drops[i].x + 8*drops[i].y]) {
      heights[drops[i].x + 8*drops[i].y] += 1.0f;//0.5;
      drops[i].init();
    }

    // draw each snowflake
    drops[i].tick();
    drops[i].draw();
  }

  // draw the snow piles
  for (int i = 0; i < 64; i++) {
    byte x = i/8, y = i%8;

    // taller piles melt faster
    if(randomf() < 0.01f*(heights[i]+1)/8.0f) {
      heights[i] -= 0.001f;
    }
    heights[i] = max(heights[i], 0);
    heights[i] = min(heights[i], 8);
    drawLine(x + 0.01, y + 0.01, 0, x, y, heights[i]-1, 0xFFFFFF);
  }
}


Snow::Drop::Drop() {
  init();
}
void Snow::Drop::init() {
  x = random(8);
  y = random(8);
  z = 8 + random(8);
  dz = 0.1 + 0.1f*randomf();
}
void Snow::Drop::tick() {
  z -= dz;

  // reset if too low (unlikely, should hit snow pile first)
  if (z < -2) {
    init();
  }
}
void Snow::Drop::draw() {
  drawSmoothedPixel(x, y, z, 0xFFFFFF);
}
