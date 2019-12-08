/*
 * ChristmasTree.cpp
 *
 *  Created on: Nov 7, 2018
 *      Author: Michelle Wellman
 *
 */

#include "ChristmasTree.h"

#include <FastLED.h>
#include "cube.h"

ChristmasTree::ChristmasTree() {
  duration = 25000;
}

void ChristmasTree::initialize() {
}

void ChristmasTree::tick() {
  FastLED.clear();

  CRGB branchColor = CRGB(0, 255, 0);
  CRGB trunkColor = CRGB(50, 40, 0);
  CRGB starColor = CRGB(255, 255, 0);


  // Draw the tree
  for(byte x = 0; x < 8; x++) {
    for(byte y = 0; y < 8; y++) {
      for(byte z = 1; z < 8; z++) {
        if( z >= 1 && z-7 <= -(6/3.5)*abs(y - 3.5) && z-7 <= -(6/3.5)*abs(x - 3.5)) {
          float twinkle = randomf();
          if(twinkle < 0.15) {
            leds[getIndex(x, y, z)] = CRGB(100, 0, 0);
          }
          else if (twinkle < 0.15 + 0.05) {
            leds[getIndex(x, y, z)] = CRGB(0, 0, 100);
          }
          else {
            leds[getIndex(x, y, z)] = branchColor;
          }
        }
      }
    }
  }

    // Draw the trunk
  leds[getIndex(3, 3, 0)] = trunkColor;
  leds[getIndex(3, 4, 0)] = trunkColor;
  leds[getIndex(4, 4, 0)] = trunkColor;
  leds[getIndex(4, 3, 0)] = trunkColor;
  leds[getIndex(3, 3, 1)] = trunkColor;
  leds[getIndex(3, 4, 1)] = trunkColor;
  leds[getIndex(4, 4, 1)] = trunkColor;
  leds[getIndex(4, 3, 1)] = trunkColor;

  // Draw the star
  leds[getIndex(3, 3, 7)] = starColor;
  leds[getIndex(3, 4, 7)] = starColor;
  leds[getIndex(4, 4, 7)] = starColor;
  leds[getIndex(4, 3, 7)] = starColor;

  delay(500);
}
