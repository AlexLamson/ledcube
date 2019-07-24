/*
 * BallsBouncing.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#include "BallsBouncing.h"
#include <FastLED.h>
#include "cube.h"

BallsBouncing::BallsBouncing() {
  duration = 30000;
}

void BallsBouncing::initialize() {
  // Stagger the initial spawn - they shouldn't all show up at once
  for (int i = 0; i < numBalls; i++) {
    startTimes[i] = 2 + 50 * i;
  }
}

void BallsBouncing::tick() {
  FastLED.clear();

  for (byte i = 0; i < numBalls; i++) {
    if (startTimes[i] > 0) {
      balls[i].init();
      startTimes[i] -= 1;
    }
    else {
      balls[i].tick();
    }
  }
}


BallsBouncing::Ball::Ball() {
  init();
}

void BallsBouncing::Ball::init() {
  radius = 0.5f + randomf() * 1.0f;

  position = {
      10.0f + radius,
      randomf() * 6.0f + 1.0f,
      5.0f + 3.0f * randomf()
  };

  speed = {
      -0.06f - randomf() * 0.03f,
      0.0,
      -(position.z - 6.5f) * 0.01f
  };

  color = CHSV(random(256), 255, 255);
}

void BallsBouncing::Ball::tick() {
  speed.z -= gravity;

  position.x += speed.x;
  position.y += speed.y;
  position.z += speed.z;

  if (position.z < radius) {
    position.z = radius + abs(position.z - radius);
    speed.z = -speed.z * 0.8f;
  }

  if (position.x < - radius - 2.0) {
    init();
  }

  drawSphereSolid(
      position.x,
      position.y,
      position.z,
      radius,
      color);
}
