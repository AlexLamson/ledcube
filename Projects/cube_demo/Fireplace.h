/*
 * Fireplace.h
 *
 *  Created on: Oct 28, 2018
 *      Author: Chris Raff
 */

#ifndef Fireplace_H_
#define Fireplace_H_

#include "Demo.h"

class Fireplace: public Demo {
// private:
//   Drop drops[numDrops];
public:
  Fireplace();
  void initialize();
  void tick();
};

#endif /* Fireplace_H_ */
