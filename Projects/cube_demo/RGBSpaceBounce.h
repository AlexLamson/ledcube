/*
 * RGBSpaceBounce.h
 *
 *  Created on: Oct 28, 2018
 *      Author: raffc
 */

#ifndef RGBSPACEBOUNCE_H_
#define RGBSPACEBOUNCE_H_

#include "Demo.h"

class RGBSpaceBounce: public Demo {
private:
  int cubeScale = 16;
  float bounceX = 0, bounceY = 0, bounceZ = 0;
  float bounceDX = 0.5, bounceDY = 0.7, bounceDZ = 0.9;
public:
  RGBSpaceBounce();
  void initialize();
  void tick();
};

#endif /* RGBSPACEBOUNCE_H_ */
