/*
 * IntersectingSpheresBigger.h
 *
 *  Created on: Nov 2, 2018
 *      Author: raffc
 */

#ifndef SPHERESINTERSECTINGBIGGER_H_
#define SPHERESINTERSECTINGBIGGER_H_

#include "Demo.h"

class SpheresIntersectingBigger: public Demo {
private:
  float angle = 0;
  const float angleIncremement = 0.05;
public:
  SpheresIntersectingBigger();
  void initialize();
  void tick();
};

#endif /* SPHERESINTERSECTINGBIGGER_H_ */
