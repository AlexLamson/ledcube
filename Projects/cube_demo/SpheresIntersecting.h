/*
 * SpheresIntersecting.h
 *
 *  Created on: Nov 2, 2018
 *      Author: raffc
 */

#ifndef SPHERESINTERSECTING_H_
#define SPHERESINTERSECTING_H_

#include "Demo.h"

class SpheresIntersecting: public Demo {
private:
  float angle = 0;
  const float angleIncremement = 0.05;
public:
  SpheresIntersecting();
  void initialize();
  void tick();
};

#endif /* SPHERESINTERSECTING_H_ */
