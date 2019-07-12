/*
 * Wander.cpp
 *
 *  Created on: Oct 27, 2018
 *      Author: Chris Raff
 */

#include "Wander.h"
#include <FastLED.h>
#include "cube.h"
#include "Arduino.h"

const float momentum = 0.98;
const float fadeFactor = 0.99;
const float movementSpeedMultiplier = 0.05;

// Wanderer particle logic
Wander::Wanderer::Wanderer() {
  init(random(255));
}

void Wander::Wanderer::init(byte hue) {
  x = 3.5;
  y = 3.5;
  z = 3.5;

  dx = 0;
  dy = 0;
  dz = 0;

  color = CHSV( hue, 255, 100 );
}

void Wander::Wanderer::tick() {
  // ought to be done as a random vector but that's effort for a later date
  dx += (randomf() - 0.5) * movementSpeedMultiplier;
  dy += (randomf() - 0.5) * movementSpeedMultiplier;
  dz += (randomf() - 0.5) * movementSpeedMultiplier;

  dx *= momentum;
  dy *= momentum;
  dz *= momentum;

  x += dx;
  y += dy;
  z += dz;

  // bounce
  if (x < 0) { x = 0; dx = -dx; }
  if (y < 0) { y = 0; dy = -dy; }
  if (z < 0) { z = 0; dz = -dz; }

  if (x >= 7) { x = 7; dx = -dx; }
  if (y >= 7) { y = 7; dy = -dy; }
  if (z >= 7) { z = 7; dz = -dz; }
}

// Demo inherited functions
Wander::Wander() {
  duration = 20000;
}

void Wander::initialize() {
  FastLED.clear();

  byte hue = random(255);

  for (byte i = 0; i < numWanderers; i++) {
    wanderers[i].init(hue);
    hue += 256 / numWanderers;
  }
}

void Wander::tick() {
  for (int i = 0; i < 512; i++) {
    leds[i].r *= fadeFactor;
    leds[i].g *= fadeFactor;
    leds[i].b *= fadeFactor;
  }

  for (int i = 0; i < numWanderers; i++) {
    wanderers[i].tick();

    drawSmoothedPixel(wanderers[i].x, wanderers[i].y, wanderers[i].z, wanderers[i].color);
  }
}
