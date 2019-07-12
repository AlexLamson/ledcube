/*
 * Wander.h
 *
 *  Created on: Oct 27, 2018
 *      Author: Chris Raff
 */

#ifndef WANDER_H_
#define WANDER_H_

#include "Demo.h"
#include <FastLED.h>
#include "Arduino.h"

class Wander: public Demo {
private:
  class Wanderer
  {
  public:
    Wanderer();
    void init(byte);
    void tick();

    float x, y, z, dx, dy, dz;
    CRGB color;
  };

  static const int numWanderers = 4;
  Wanderer wanderers[numWanderers];

public:
  Wander();
  void initialize();
  void tick();
};

#endif /* WANDER_H_ */
