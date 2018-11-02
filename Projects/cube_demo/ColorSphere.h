/*
 * ColorSphere.h
 *
 *  Created on: Oct 27, 2018
 *      Author: Chris Raff
 */

#ifndef COLORSPHERE_H_
#define COLORSPHERE_H_

#include "Arduino.h"
#include "Demo.h"

class ColorSphere: public Demo {
private:
  byte hue = 0;
public:
  ColorSphere();
  void initialize();
  void tick();
};



#endif /* COLORSPHERE_H_ */
