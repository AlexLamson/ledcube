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
  static const int numPoints = 32;
  float3 points[numPoints];
  CRGB colors[numPoints];
  float3 motion = {0, 0, 0};
  float3 dMotion= {0, 0, 0};
  float rotZ = 0.0;
  float dRotZ = 0.0;
public:
  PointField();
  void initialize();
  void tick();
};

#endif /* POINTFIELD_H_ */
