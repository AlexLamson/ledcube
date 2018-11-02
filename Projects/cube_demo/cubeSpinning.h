/*
 * cubeSpinning.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Chris Raff
 */

#ifndef CUBESPINNING_H_
#define CUBESPINNING_H_

#include "Demo.h"

class cubeSpinning: public Demo {
private:
  float angle;
  const float angleIncrement = 0.05;
public:
  cubeSpinning();
  void initialize();
  void tick();
};

#endif /* CUBESPINNING_H_ */
