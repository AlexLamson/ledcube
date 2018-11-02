/*
 * ColorCone.h
 *
 *  Created on: Nov 2, 2018
 *      Author: raffc
 */

#ifndef COLORCONE_H_
#define COLORCONE_H_

#include "Demo.h"
#include "Arduino.h"

class ColorCone: public Demo {
private:
  byte hue = 0;
public:
  ColorCone();
  void initialize();
  void tick();
};

#endif /* COLORCONE_H_ */
