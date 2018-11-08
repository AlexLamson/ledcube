/*
 * LineSpinning.h
 *
 *  Created on: Nov 7, 2018
 *      Author: Chris Raff
 */

#ifndef CUBEWIREFRAME_H_
#define CUBEWIREFRAME_H_

#include "Demo.h"

class CubeWireframe: public Demo {
private:
  float angle;
public:
  CubeWireframe();
  void initialize();
  void tick();
};

#endif /* CUBEWIREFRAME_H_ */
