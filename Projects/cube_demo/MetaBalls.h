/*
 * BallsBouncing.h
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#ifndef METABALLS_H_
#define METABALLS_H_

#include "Demo.h"
#include <FastLED.h>
#include "cube.h"

class MetaBalls: public Demo {
private:
  class Ball {
  public:
    Ball();
    void init();
    void tick();

    float3 position, speed;
    float radius;
  };

  const static int numBalls = 3;
  Ball balls[ numBalls ];

public:
  MetaBalls();
  void initialize();
  void tick();
};

#endif /* METABALLS_H_ */
