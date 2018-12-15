/*
 * Snow.h
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#ifndef SNOW_H_
#define SNOW_H_

#include "Demo.h"
#include "Arduino.h"
#include <FastLED.h>

class Snow: public Demo {
private:
  class Drop {
    public:
      Drop();
      void init();
      void tick();
      void draw();

      byte x, y;
      float z, dz;
  };
  const static int numDrops = 16;
  float heights[64];
  Drop drops[numDrops];
public:
  Snow();
  void initialize();
  void tick();
};

#endif /* SNOW_H_ */
