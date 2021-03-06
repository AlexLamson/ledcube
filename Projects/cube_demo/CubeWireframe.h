/*
 * LineSpinning.h
 *
 *  Created on: Nov 7, 2018
 *      Author: Chris Raff
 */

#ifndef CUBEWIREFRAME_H_
#define CUBEWIREFRAME_H_

#include "Demo.h"
#include "cube.h"
//#include "geometry/TransformStack.h"
//#include "geometry/Translation.h"
//#include "geometry/AxisAngleRotation.h"

class CubeWireframe: public Demo {
private:
  float angle;
//  TransformStack* t;
//  Translation* tTranslate;
//  AxisAngleRotation* tRotate;
//  float3 verts[24];
public:
  CubeWireframe();
  void initialize();
  void tick();
};

#endif /* CUBEWIREFRAME_H_ */
