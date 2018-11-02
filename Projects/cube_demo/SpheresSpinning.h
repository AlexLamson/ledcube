/*
 * SpheresSpinning.h
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#ifndef SPHERESSPINNING_H_
#define SPHERESSPINNING_H_

#include "Demo.h"

class SpheresSpinning: public Demo {
private:
  float angle = 0;
  const float angleIncremement = 0.05;
public:
  SpheresSpinning();
  void initialize();
  void tick();
};

#endif /* SPHERESSPINNING_H_ */
