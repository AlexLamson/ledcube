/*
 * PointField.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Chris Raff
 */

#include "PointField.h"
#include <FastLED.h>
#include "cube.h"

PointField::PointField() {
  duration = 15000;
}

void PointField::initialize() {
  motion = {0, 0, 0};
  dMotion = {0, 0, 0};
  rotZ = 0;
  dRotZ = 0;

  for (int i = 0; i < numPoints; i++) {
    points[i] = {randomf() * 10.0f - 1.0f, randomf() * 10.0f - 1.0f, randomf() * 10.0f - 1.0f};
    colors[i] = CHSV(random(256), 255, 255);
  }
}

void PointField::tick() {
  FastLED.clear();
  const float dMotionMax = 0.005;
  dMotion.x += (randomf() - 0.5) * 0.001;
  dMotion.y += (randomf() - 0.5) * 0.001;
  dMotion.z += (randomf() - 0.5) * 0.001;
  if (abs(dMotion.x) > dMotionMax)
    dMotion.x *= 0.9;
  if (abs(dMotion.y) > dMotionMax)
    dMotion.y *= 0.9;
  if (abs(dMotion.z) > dMotionMax)
    dMotion.z *= 0.9;

  motion.x += dMotion.x;
  motion.y += dMotion.y;
  motion.z += dMotion.z;

  const float motionCurb = 0.9; // smoothly approach a top speed
  motion.x *= motionCurb;
  motion.y *= motionCurb;
  motion.z *= motionCurb;

  // TODO rotation figuring

  // apply the motion to points and draw
  for (int i = 0; i < numPoints; i++) {
    points[i].x += motion.x;
    points[i].y += motion.y;
    points[i].z += motion.z;

    // if the point is out of bounds replace it on the edge (don't care if it gets replaced next tick)
    // TODO make uniform with respect to the direction you're moving
    if (points[i].x < -1 || points[i].x > 9 ||
        points[i].y < -1 || points[i].y > 9 ||
        points[i].z < -1 || points[i].z > 9) {

      float side = 10 * random(2) - 1; // pick one edge or the other
      float randu= randomf() * 10 - 1;
      float randv= randomf() * 10 - 1;

      switch (random(3)) {
      case 0: // x
        points[i].x = side; points[i].y = randu; points[i].z = randv;
        break;
      case 1: // y
        points[i].y = side; points[i].x = randu; points[i].z = randv;
        break;
      case 2: // z
        points[i].z = side; points[i].x = randu; points[i].y = randv;
        break;
      }

      colors[i] = CHSV(random(256), 255, 255);
    } else {
      drawSmoothedPixel(points[i].x, points[i].y, points[i].z, colors[i]);
    }
  }

}
