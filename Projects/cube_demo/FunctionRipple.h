/*
 * FunctionRipple.h
 *
 *  Created on: Oct 28, 2018
 *      Author: Chris Raff
 */

#ifndef FUNCTIONRIPPLE_H_
#define FUNCTIONRIPPLE_H_

#include "Demo.h"

class FunctionRipple: public Demo {
private:
  float w = 0;
  float w_increment = -0.001;
public:
  FunctionRipple();
  void initialize();
  void tick();
};

#endif /* FUNCTIONRIPPLE_H_ */
