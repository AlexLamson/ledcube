/*
 * MetaBalls.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#include "MetaBalls.h"
#include <FastLED.h>
#include "cube.h"
#include <Streaming.h>

MetaBalls::MetaBalls() {
  duration = 30000;
}

void MetaBalls::initialize() {
  for (int i = 0; i < numBalls; i++) {
    balls[i].init();
  }
}

void MetaBalls::tick() {
  FastLED.clear();

  for (byte i = 0; i < numBalls; i++) {
    balls[i].tick();
  }

//  for (int i = 0; i < 512; i++) {
  for (byte x = 0; x < 8; x++) {
    for (byte y = 0; y < 8; y++) {
      for (byte z = 0; z < 8; z++) {

        int i = getIndex(x, y, z);

        float dist = 0;
        float min_ = 10000000, max_ = 0;
        for (byte b = 0; b < numBalls; b++) {
          float dx = balls[b].position.x - x;
          float dy = balls[b].position.y - y;
          float dz = balls[b].position.z - z;

          dist += balls[b].radius / (0.75f + sqrt(sqr(dx) + sqr(dy) + sqr(dz)));
          if (dist > max_) {
            max_ = dist;
          }
          if (dist < min_) {
            min_ = dist;
          }
        }

        leds[i] = CHSV(byte(dist), 255, 128);
      }
    }
  }

//    Serial << "min " << min_ << " max " << max_<< endl;
}


MetaBalls::Ball::Ball() {
  init();
}

void MetaBalls::Ball::init() {
  radius = 300.0f ;//+ randomf() * 1500.0f;

  position = {
      randomf() * 8.0f,
      randomf() * 8.0f,
      randomf() * 8.0f
  };

  speed = {
      randomf() * 0.04f - 0.064f,
      randomf() * 0.04f - 0.064f,
      randomf() * 0.04f - 0.064f
  };
}

void MetaBalls::Ball::tick() {
  position.x += speed.x;
  position.y += speed.y;
  position.z += speed.z;

  if (position.x < 0 || position.x > 8) {
    speed.x *= -1;
    position.x += speed.x;
  }
  if (position.y < 0 || position.y > 8) {
    speed.y *= -1;
    position.y += speed.y;
  }
  if (position.z < 0 || position.z > 8) {
    speed.z *= -1;
    position.z += speed.z;
  }
}
