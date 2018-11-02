/*
 * ColorSphereTrimmed.h
 *
 *  Created on: Oct 28, 2018
 *      Author: raffc
 */

#ifndef COLORSPHERETRIMMED_H_
#define COLORSPHERETRIMMED_H_

#include "Demo.h"
#include "Arduino.h"

class ColorSphereTrimmed: public Demo {
private:
  byte hue;
  byte hue2;
public:
  ColorSphereTrimmed();
  void initialize();
  void tick();
};

#endif /* COLORSPHERETRIMMED_H_ */
