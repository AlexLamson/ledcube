#include "Arduino.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

// pins for wemos d1
// #define DATA0 13 // D7
// #define DATA1 12 // D6
// #define DATA2 14 // D5
// #define DATA3 4  // D4

// pins for wemos d1 mini
#ifndef DATA_
#define DATA_

//#define DATA0 5  // D1
//#define DATA1 4  // D2
//#define DATA2 0  // D3
//#define DATA3 2  // D4
#define DATA0 16 // D1
#define DATA1 17  // D2
#define DATA2 21  // D3
#define DATA3 22  // D4

#endif // DATA_

// pins for arduino uno
// #define DATA0 7
// #define DATA1 6
// #define DATA2 5
// #define DATA3 4

#include "cube.h"

#include "Demo.h"
#include "ColorSphere.h"
#include "Wander.h"
#include "FunctionRipple.h"
#include "ColorSphereTrimmed.h"
#include "RGBSpaceBounce.h"
#include "DataSnake.h"
#include "ColorCone.h"
#include "SpheresSpinning.h"
#include "PointField.h"
#include "SpheresIntersecting.h"
#include "SpheresIntersectingBigger.h"
#include "Rain.h"
#include "BallsBouncing.h"
#include "CubeWireframe.h"
#include "Helix.h"
#include "Snow.h"
#include "MetaBalls.h"
#include "Fireplace.h"


const byte tickMillis = 20;
unsigned long lastTickTime = 0;
unsigned long lastDemoTime = 0;

byte demoMode = 0;
bool advanceDemo = true;
const byte numDemos = 15;
Demo* demos[ numDemos ] = {
    new ColorSphere(),
    new Wander(),
    new FunctionRipple(),
    new ColorSphereTrimmed(),
    new RGBSpaceBounce(),
    new DataSnake(),
    new MetaBalls(),
    new ColorCone(),
    new SpheresSpinning(),
//    new PointField(),
    new SpheresIntersectingBigger(),
    new Helix(),
    new SpheresIntersecting(),
    new Rain(),
    new BallsBouncing(),
    new CubeWireframe(),
    new Fireplace(),
    new Snow()
};

void setup()
{
  Serial.begin(115200);

  cubeBegin();

  FastLED.clear();
  FastLED.setCorrection( Typical8mmPixel );
  FastLED.show();

  demos[demoMode]->initialize();
}

void loop()
{
  if (millis() >= lastDemoTime + demos[demoMode]->duration && advanceDemo) {
    lastDemoTime = millis();

    // advance demo
    demoMode++;
    demoMode = demoMode % numDemos;

    demos[demoMode]->initialize();
  }

  if (millis() >= lastTickTime + tickMillis) {
      lastTickTime = millis();

      const unsigned long computeStartTime = micros();

      demos[demoMode]->tick();

      unsigned long computeTime = micros() - computeStartTime;
      Serial.println(computeTime);


      const unsigned long drawStartTime = micros();
      FastLED.show();
      unsigned long drawTime = micros() - drawStartTime;
      Serial.println(drawTime);
  }
}
