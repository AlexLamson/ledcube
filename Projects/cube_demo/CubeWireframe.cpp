/*
 * LineSpinning.cpp
 *
 *  Created on: Nov 7, 2018
 *      Author: Chris Raff
 *
 */

#include "CubeWireframe.h"

#include <FastLED.h>
#include "cube.h"
//#include "geometry/AxisAngleRotation.h"

CubeWireframe::CubeWireframe() {
  duration = 25000;
  angle = 0;

//  rot = new AxisAngleRotation();
//  rot->setAxis({0, 0, 1});
//  rot->setAngle(0.02);

//  const float a = 1.0, b = 6.0;
//
//  float3 temp[24] = {
//      {a, a, a},
//      {a, a, b},
//
//      {a, a, b},
//      {a, b, b},
//
//      {a, b, b},
//      {a, b, a},
//
//      {a, b, a},
//      {a, a, a},
//
//
//      {b, a, a},
//      {b, a, b},
//
//      {b, a, b},
//      {b, b, b},
//
//      {b, b, b},
//      {b, b, a},
//
//      {b, b, a},
//      {b, a, a},
//
//
//      {a, a, a},
//      {b, a, a},
//
//      {a, a, b},
//      {b, a, b},
//
//      {a, b, a},
//      {b, b, a},
//
//      {a, b, b},
//      {b, b, b}
//  };
//  for (int i = 0; i < 24; i++) verts[i] = temp[i];
}

void CubeWireframe::initialize() {
  angle = 0;
}

void CubeWireframe::tick() {
  FastLED.clear();

//  for (int i = 0; i < 24; i++) {
//    verts[i].x -= 3.5;
//    verts[i].y -= 3.5;
//    verts[i].z -= 3.5;
//
//    rot->apply(verts[i], &(verts[i]));
//
//    verts[i].x += 3.5;
//    verts[i].y += 3.5;
//    verts[i].z += 3.5;
//  }
//
//  drawLineList(verts, 24, CRGB(0, 128, 255));
//}

  angle += 0.02;

//  rot.setAngle(angle);

  CRGB color = CRGB(0, 128, 255);
  float size_2 = 3.5355339059327;

  float cosa = cos(angle), sina = sin(angle);

  float q1x = + size_2 * cosa;
  float q1y = + size_2 * sina;

  float q2x = + size_2 * sina;
  float q2y = - size_2 * cosa;

  float q3x = - q1x;
  float q3y = - q1y;

  float q4x = - q2x;
  float q4y = - q2y;

  for (float z = 1.0; z <= 6.0; z += 5.0) {
    drawLine(
        3.5 + q1x,
        3.5 + q1y,
        z,
        3.5 + q2x,
        3.5 + q2y,
        z,
        color);
    drawLine(
        3.5 + q2x,
        3.5 + q2y,
        z,
        3.5 + q3x,
        3.5 + q3y,
        z,
        color);
    drawLine(
        3.5 + q3x,
        3.5 + q3y,
        z,
        3.5 + q4x,
        3.5 + q4y,
        z,
        color);
    drawLine(
        3.5 + q4x,
        3.5 + q4y,
        z,
        3.5 + q1x,
        3.5 + q1y,
        z,
        color);
  }

  // the vertical lines go from 2 to 6 so they don't contribute to corner brightness
  drawLine(
      3.5 + q1x,
      3.5 + q1y,
      2.0,
      3.5 + q1x,
      3.5 + q1y,
      6.0,
      color
  );
  drawLine(
      3.5 + q2x,
      3.5 + q2y,
      2.0,
      3.5 + q2x,
      3.5 + q2y,
      6.0,
      color
  );
  drawLine(
      3.5 + q3x,
      3.5 + q3y,
      2.0,
      3.5 + q3x,
      3.5 + q3y,
      6.0,
      color
  );
  drawLine(
      3.5 + q4x,
      3.5 + q4y,
      2.0,
      3.5 + q4x,
      3.5 + q4y,
      6.0,
      color
  );
}
