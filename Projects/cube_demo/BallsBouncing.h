/*
 * BallsBouncing.h
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#ifndef BALLSBOUNCING_H_
#define BALLSBOUNCING_H_

#include "Demo.h"
#include <FastLED.h>
#include "cube.h"

class BallsBouncing: public Demo {
private:
  class Ball {
  private:
    float3 position, speed;
    float radius;
    CRGB color;

    constexpr static float gravity = 0.006f;

  public:
    Ball();
    void init();
    void tick();
  };

  const static int numBalls = 4;
  Ball balls[ numBalls ];
  int startTimes[ numBalls ];

public:
  BallsBouncing();
  void initialize();
  void tick();
};

#endif /* BALLSBOUNCING_H_ */
