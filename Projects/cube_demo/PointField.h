/*
 * PointField.h
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#ifndef POINTFIELD_H_
#define POINTFIELD_H_

#include "Demo.h"
#include <FastLED.h>
#include "cube.h"

class PointField: public Demo {
private:
  typedef struct {
    float x, y, z;
  } point;

  static const int numPoints = 32;
  point points[numPoints];
  CRGB colors[numPoints];
  point motion = {0, 0, 0};
  point dMotion= {0, 0, 0};
  float rotZ = 0.0;
  float dRotZ = 0.0;
public:
  PointField();
  void initialize();
  void tick();
};

#endif /* POINTFIELD_H_ */
