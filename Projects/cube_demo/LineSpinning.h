/*
 * LineSpinning.h
 *
 *  Created on: Nov 7, 2018
 *      Author: Chris Raff
 */

#ifndef LINESPINNING_H_
#define LINESPINNING_H_

#include "Demo.h"

class LineSpinning: public Demo {
private:
  float angle;
public:
  LineSpinning();
  void initialize();
  void tick();
};

#endif /* LINESPINNING_H_ */
