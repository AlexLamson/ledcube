/*
 * LineSpinning.cpp
 *
 *  Created on: Nov 7, 2018
 *      Author: Chris Raff
 *
 */

#include "Helix.h"

#include <FastLED.h>
#include "cube.h"
//#include "geometry/AxisAngleRotation.h"

Helix::Helix() {
  duration = 25000;
  angle = 0;
  hue = 0;

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

void Helix::initialize() {
  angle = 0;
}

void Helix::tick() {
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

  angle += 0.04;
  hue++;

//  rot.setAngle(angle);

  CRGB color = CRGB(0, 128, 255);
  float size = 3.0;

//  float cosa = cos(angle), sina = sin(angle);
  const float angleDelta = 0.5;

  for (int z = 0; z < 8; z++) {
    float cosa = cos(angle + angleDelta * z), sina = sin(angle + angleDelta * z);
    drawLine(
        3.5 + size * cosa,
        3.5 + size * sina,
        z,
        3.5 - size * cosa,
        3.5 - size * sina,
        z,
        CHSV(hue + 15*z, 255, 255)
    );
  }
}
