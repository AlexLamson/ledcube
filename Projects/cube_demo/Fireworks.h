/*
 * Fireworks.h
 *
 *  Created on: Dec 7, 2019
 *      Author: Alex Lamson
 */

#ifndef Fireworks_H_
#define Fireworks_H_

#include "Demo.h"
#include <FastLED.h>
#include "cube.h"

class Fireworks: public Demo {
private:
  class Dot {
  private:
    float3 position, speed;
    CRGB color;
    byte hue;
    byte brightness;

    constexpr static float gravity = 0.006f;

  public:
    Dot();
    void init();
    void tick();
  };

  const static int numDots = 15;
  Dot dots[ numDots ];
  int startTimes[ numDots ];

public:
  Fireworks();
  void initialize();
  void tick();
};

#endif /* Fireworks_H_ */
