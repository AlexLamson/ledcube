/*
 * FunctionRipple.cpp
 *
 *  Created on: Oct 28, 2018
 *      Author: Chris Raff
 */

#include "FunctionRipple.h"
#include <FastLED.h>
#include "cube.h"

FunctionRipple::FunctionRipple() {
  duration = 30000;
}

void FunctionRipple::initialize() {
  w = 0;
}

void FunctionRipple::tick() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      float dx = float(i) - 3.5;
      float dy = float(j) - 3.5;
      float dist = sqrt( sqr(dx) + sqr(dy) );

      float z = 2.0 * cosf((dist + w) * 1.5) + 4.5; // 3.5 + 1

      // distance to eq
//            for (int k = 0; k < 8; k++) {
//              byte h = byte( abs(float(k) - z-1) * 28 );
//              leds[ getIndex(i, j, k) ] = CHSV(h, 255, value);
//            }

      // draw just the graph
      float r = 1.5 * cosf(PI + (dist + w) * 3.0) + 2.0;
      r = pow(r, 0.15);
      float rInv = 1.0 / r;

      CRGB col = CRGB(255 - i*32, 31 + i*32, j*32);
//            for (int k = int(z-r); k <= int(z+r); k++) {
      for (int k = 0; k < 8; k++) {
//              if (k >= 0 && k < 8) {
        float distToCentroid = float(k) - z + r;
        distToCentroid *= (distToCentroid < 0 ? -1 : 1);
        byte val = (max(0.0, double(r - distToCentroid)) * rInv) * 255;

        leds[ getIndex(i, j, k) ] = CRGB(scale8(col.r, val), scale8(col.g, val), scale8(col.b, val));
//              }
      }

      w += w_increment;
      if (w < -PI2 * 0.6666666)
        w += PI2 * 0.6666666;
    }
  }
}
