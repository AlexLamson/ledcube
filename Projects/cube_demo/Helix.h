/*
 * Helix.h
 *
 *  Created on: Nov 7, 2018
 *      Author: Chris Raff
 */

#ifndef HELIX_H_
#define HELIX_H_

#include "Demo.h"
#include "cube.h"
//#include "geometry/TransformStack.h"
//#include "geometry/Translation.h"
//#include "geometry/AxisAngleRotation.h"

class Helix: public Demo {
private:
  float angle;
  byte hue;
//  TransformStack* t;
//  Translation* tTranslate;
//  AxisAngleRotation* tRotate;
//  float3 verts[24];
public:
  Helix();
  void initialize();
  void tick();
};

#endif /* HELIX_H_ */
