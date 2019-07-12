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

#define BUTTON 14 // D5
#define BUTTON_LOW 12 // D6, in case you can't easily connect to ground

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

bool lastButtonState = true; // true means not pressed
const byte buttonDebounceTime = 50;
const int buttonHoldTime = 500; // hold the button to go back a demo
unsigned long lastButtonTime = 0;

byte demoMode = 0;
bool advanceDemo = true;
const byte numDemos = 13;
Demo* demos[] = {
    new ColorSphere(), // 10
    new FunctionRipple(), // 9
    new MetaBalls(), // 8
    new SpheresSpinning(), // 7
    new CubeWireframe(), // 7
    new ColorSphereTrimmed(), // 7
    new Helix(), // 6
    new SpheresIntersecting(), // 5
    new DataSnake(), // 4
    new BallsBouncing(), // 7
    new ColorCone(), // 5
    new Wander(), // 3
    new Rain(), // 3
    // new RGBSpaceBounce(),
    // new PointField(),
    // new SpheresIntersectingBigger(),
    // new Fireplace(),
    // new Snow()
};

void nextDemo(int num = 1)
{
    lastDemoTime = millis();

    int tempDemoMode = demoMode; // if the number is negative, demoMode won't wrap around correctly
  
    tempDemoMode += num;
    while (tempDemoMode < 0) {
      tempDemoMode += numDemos;
    }
    demoMode = tempDemoMode % numDemos;

    demos[demoMode]->initialize();
}

void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_LOW, OUTPUT);
  digitalWrite(BUTTON_LOW, LOW);

  cubeBegin();

  FastLED.clear();
  FastLED.setCorrection( Typical8mmPixel );
  FastLED.show();

  demos[demoMode]->initialize();
}

void loop()
{
  // button logic
  boolean buttonState = digitalRead(BUTTON);
  if (buttonState != lastButtonState && millis() >= lastButtonTime + buttonDebounceTime) {
    lastButtonState = buttonState;

    // if the button was released, the demo can be advanced
    if (buttonState) {
      // normal button press - advance the demo
      if (millis() - lastButtonTime < buttonHoldTime) {
        nextDemo();
      }
      // if the button was held, move back 1 demo
      else {
        nextDemo(-1);
      }
    }
    
    lastButtonTime = millis();
  }

  // if the demo has ran long enough, move to the next one
  if (millis() >= lastDemoTime + demos[demoMode]->duration && advanceDemo) {
    nextDemo();
  }
  
  // if enough time has passed, compute and draw the next frame
  if (millis() >= lastTickTime + tickMillis) {
      lastTickTime = millis();

      // have the demo draw the image
      const unsigned long computeStartTime = micros();

      demos[demoMode]->tick();

      unsigned long computeTime = micros() - computeStartTime;
//      Serial.println(computeTime);


      // push image to cube
      const unsigned long drawStartTime = micros();
      FastLED.show();
      unsigned long drawTime = micros() - drawStartTime;
//      Serial.println(drawTime);
  }
}
