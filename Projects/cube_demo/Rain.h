/*
 * Rain.h
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#ifndef RAIN_H_
#define RAIN_H_

#include "Demo.h"
#include "Arduino.h"
#include <FastLED.h>

class Rain: public Demo {
private:
  class Drop {
    public:
      Drop();
      void init();
      void tick();
      void draw();
      void reset();

      byte x, y;
      float z, dz;
      CRGB color;
  };
  const static int numDrops = 16;
  Drop drops[numDrops];
public:
  Rain();
  void initialize();
  void tick();
};

#endif /* RAIN_H_ */
