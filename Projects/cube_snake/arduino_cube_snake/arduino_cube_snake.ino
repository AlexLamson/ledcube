#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

// pins for wemos d1
//#define DATA0 13 // D7
//#define DATA1 12 // D6
//#define DATA2 14 // D5
//#define DATA3 4  // D4

// pins for wemos d1 mini
#define DATA0 5  // D1
#define DATA1 4  // D2
#define DATA2 0  // D3
#define DATA3 2  // D4

// pins for arduino uno
//#define DATA0 7
//#define DATA1 6
//#define DATA2 5
//#define DATA3 4

int getIndex(int, int, int);
void clear();
void setColor(CRGB);

byte mapBrightness(float);

CRGB leds[512];

// end cube stuff

#define ENV_WIDTH 8
#define ENV_HEIGHT 8
#define ENV_DEPTH 8

byte snakeHeadHue;
const byte snakeHueDelta = 8;
uint32_t foodColor = 0xFFFFFF;

byte playerOrientation = 0;
byte orientationMap(byte);

struct node {
  byte x;
  byte y;
  byte z;
  struct node* next;
};

struct node* head;
struct node* tail;

byte food_x;
byte food_y;
byte food_z;

int tickMillis = 200;
unsigned long lastTickMillis = 0;

byte dir = 0;
byte newDir = 0;

bool isInsideSnake(byte, byte, byte, bool);
void snakeInit();

void setup() {
  FastLED.addLeds<WS2812B, DATA0>(leds, 0,   128);
  FastLED.addLeds<WS2812B, DATA1>(leds, 128, 128);
  FastLED.addLeds<WS2812B, DATA2>(leds, 256, 128);
  FastLED.addLeds<WS2812B, DATA3>(leds, 384, 128);

  Serial.begin(115200);

  FastLED.clear();

  snakeInit();
}

byte len;

void snakeInit() {
  lastTickMillis = millis();

  byte start_x = 2;
  byte start_y = 2;
  byte start_z = 2;
  len = 3;

  food_x = 5;
  food_y = 5;
  food_z = 5;

  snakeHeadHue = 0;

  dir = 0;
  newDir = 0;

  head = new struct node();
  head->x = start_x;
  head->y = start_y;
  head->z = start_z;
  leds[getIndex(start_x, start_y, start_z)] = CHSV(snakeHeadHue, 255, 255);

  struct node* lastNode = head;
  byte count = 1;
  while (count++ < len) {
    struct node* newNode = new struct node();
    newNode->x = start_x + 1 - count;
    newNode->y = start_y;
    newNode->z = start_z;
    newNode->next = lastNode;

    lastNode = newNode;

    leds[getIndex(lastNode->x, lastNode->y, lastNode->z)] = CHSV(snakeHeadHue - snakeHueDelta * count, 255, 255);
  }
  tail = lastNode;

  snakeHeadHue += snakeHueDelta;

  leds[getIndex(food_x, food_y, food_z)] = foodColor;

  FastLED.show();
}

void loop() {
  if (millis() >= lastTickMillis + tickMillis) {
    lastTickMillis = millis();

    dir = newDir;

    struct node* newHead = new struct node();
    newHead->x = head->x;
    newHead->y = head->y;
    newHead->z = head->z;
    switch (dir) {
      case 0:
      newHead->x += 1;
      break;

      case 1:
      newHead->y += 1;
      break;

      case 2:
      newHead->z += 1;
      break;

      case 3:
      newHead->x -= 1;
      break;

      case 4:
      newHead->y -= 1;
      break;

      case 5:
      newHead->z -= 1;
      break;
    }

    head->next = newHead;
    head = head->next;

    if (head->x == food_x && head->y == food_y && head->z == food_z) {
      do {
        food_x = random(ENV_WIDTH - 2) + 1;
        food_y = random(ENV_HEIGHT - 2) + 1;
        food_z = random(ENV_DEPTH - 2) + 1;
      } while (isInsideSnake(food_x, food_y, food_z, true));

      len++;

      leds[getIndex(food_x, food_y, food_z)] = foodColor;
    } else {
      leds[getIndex(tail->x, tail->y, tail->z)] = 0x000000;
      tail = tail->next;
    }

//    leds[getIndex(head->x, head->y, head->z)] = CHSV(snakeHeadHue, 255, 255);
    node* currNode = tail;
    byte i = len;
    while (currNode != NULL) {
        leds[getIndex(currNode->x, currNode->y, currNode->z)] = CHSV(snakeHeadHue - (--i)*snakeHueDelta, 255, ((len-i)%3==1 ? 128 : 255));
        currNode = currNode->next;
    }

//    snakeHeadHue += snakeHueDelta;

    if (head->x < 0 || head->x >= ENV_WIDTH ||
        head->y < 0 || head->y >= ENV_HEIGHT ||
        head->z < 0 || head->z >= ENV_DEPTH ||
        isInsideSnake(head->x, head->y, head->z, false)) {
      delay(1000);
      FastLED.clear();
      snakeInit();
    }

    FastLED.show();
  }


  if (Serial.available()) {
    byte keyCode[1];
    Serial.readBytes(keyCode, 1);

    byte incomingDir = dir;

    switch (keyCode[0]) {
      case 37: // left
        incomingDir = orientationMap(0);
      break;

      case 39: // right
        incomingDir = orientationMap(3);
      break;

      case 38: // up
        incomingDir = orientationMap(1);
      break;

      case 40: // down
        incomingDir = orientationMap(4);
      break;

      case 65: // a (left)
        incomingDir = orientationMap(0);
      break;

      case 68: // d (right)
        incomingDir = orientationMap(3);
      break;

      case 87: // w (in)
        incomingDir = 2;
      break;

      case 83: // s (out)
        incomingDir = 5;
      break;


      case 61: // + (increase speed)
        tickMillis -= 50; // reduce wait time
      break;

      case 45: // - (decrease speed)
        tickMillis += 50; // increase wait time
      break;


      case 85: // u (rotate controls left)
        playerOrientation = (playerOrientation + 1) % 4;
      break;

      case 73: // i (rotate controls right)
        playerOrientation = (playerOrientation + 3) % 4;
      break;
    }

    // prevent going backwards
    if (incomingDir % 3 != dir % 3) {
      newDir = incomingDir;
    }
  }
}

bool isInsideSnake(byte x, byte y, byte z, bool includeHead) {
  struct node* curr = tail;

  while (curr != NULL && !(!includeHead && (curr == head))) {
    if (x == curr->x && y == curr->y && z == curr->z)
      return true;
    curr = curr->next;
  }

  return false;
}

// ----

byte orientationMap(byte d) {
  if (playerOrientation == 0)
    return d;
  if (playerOrientation == 2) {
    switch (d) {
    case 0: return 3;
    case 3: return 0;
    case 1: return 4;
    case 4: return 1;
    }
  }
  if (playerOrientation == 1) {
    switch (d) {
    case 0: return 4;
    case 3: return 1;
    case 1: return 0;
    case 4: return 3;
    }
  }
  if (playerOrientation == 3) {
    switch (d) {
    case 0: return 4;
    case 3: return 1;
    case 1: return 0;
    case 4: return 3;
    }
  }

  return d;
}

int getIndex(int x, int y, int z) {
  // consider the last installed slice to be the front (y = 0), with left having x=0, and bottom z=0

  return ( (z + y%2)%2 == 0 ? x : 7 - x ) +
    ( y%2 == 0 ? z*8 : (7 - z) * 8 ) +
    y * 64;
}


byte mapBrightness(float input) {
  return 255*(0.5-cos( PI * pow(input, 1.25) )/2); // power of 1.5 seems to preserve brightness pretty well.
}

void setColor(CRGB color) {
  fill_solid( leds, 512, color );
}

void clear() {
  setColor(0);
}
