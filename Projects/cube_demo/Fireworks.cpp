/*
 * Fireworks.cpp
 *
 *  Created on: Dec 7, 2019
 *      Author: Alex Lamson
 */

#include "Fireworks.h"
#include <FastLED.h>
#include "cube.h"

Fireworks::Fireworks() {
  duration = 30000;
}

void Fireworks::initialize() {
  // Stagger the initial spawn - they shouldn't all show up at once
  for (int i = 0; i < numDots; i++) {
    startTimes[i] = 2 + 50 * i;
  }
}

void Fireworks::tick() {
  FastLED.clear();

  for (byte i = 0; i < numDots; i++) {
    if (startTimes[i] > 0) {
      dots[i].init();
      startTimes[i] -= 1;
    }
    else {
      dots[i].tick();
    }
  }
}


Fireworks::Dot::Dot() {
  init();
}

void Fireworks::Dot::init() {
  position = {
      3.5f,
      3.5f,
      3.5f
  };

  // byte angle = random(255);
  // speed = {
  //     (cos8(angle)-128)/255.0 * 0.5f,
  //     (sin8(angle)-128)/255.0 * 0.5f,
  //     (randomf()-0.5f) * 0.5f
  // };
  float theta = 2.0f * PI * randomf();
  float phi = acos(2.0f * randomf() - 1.0f);
  speed = {
      0.2f * sin(theta) * cos(phi),
      0.2f * sin(theta) * sin(phi),
      0.2f * cos(theta)
  };

  brightness = 255;
  hue = random(256);
  color = CHSV(hue, 255, brightness);
}

void Fireworks::Dot::tick() {
  speed.z -= gravity*0.1f;

  position.x += speed.x;
  position.y += speed.y;
  position.z += speed.z;

  speed.x += -speed.x * 0.07f;
  speed.y += -speed.y * 0.07f;
  speed.z += -speed.z * 0.07f;

  brightness = max(brightness-1, 0);
  color = CHSV(hue, 255, brightness);

  if (brightness == 0 || !inCubeBounds(position.x, position.y, position.z)) {
    init();
  }

  drawSmoothedPixel(
      position.x,
      position.y,
      position.z,
      color);

  // leds[ getIndex(position.x, position.y, position.z) ] = color;
}
