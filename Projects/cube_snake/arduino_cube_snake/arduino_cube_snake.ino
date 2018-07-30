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

void drawSmoothedPixel(float, float, float, CRGB);
byte mapBrightness(float);

CRGB leds[512];

// end cube stuff

#define ENV_WIDTH 8
#define ENV_HEIGHT 8
#define ENV_DEPTH 8

uint32_t snake_color = 0x0000FF;
uint32_t food_color = 0xFF0000;

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

const int tickMillis = 200;
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

void snakeInit() {
  lastTickMillis = millis();
  
  byte start_x = 2;
  byte start_y = 2;
  byte start_z = 2;
  byte len = 3;

  food_x = 5;
  food_y = 5;
  food_z = 5;

  dir = 0;
  newDir = 0;
 
  head = new struct node();
  head->x = start_x;
  head->y = start_y;
  head->z = start_z;
  leds[getIndex(start_x, start_y, start_z)] = snake_color;
  
  struct node* lastNode = head;
  byte count = 1;
  while (count++ < len) {
    struct node* newNode = new struct node();
    newNode->x = start_x + 1 - count;
    newNode->y = start_y;
    newNode->z = start_z;
    newNode->next = lastNode;
    
    lastNode = newNode;
    
    leds[getIndex(lastNode->x, lastNode->y, lastNode->z)] = snake_color;
  }
  tail = lastNode;
  
  leds[getIndex(food_x, food_y, food_z)] = food_color;

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
        food_x = random(ENV_WIDTH);
        food_y = random(ENV_HEIGHT);
        food_z = random(ENV_DEPTH);
      } while (isInsideSnake(food_x, food_y, food_z, true));
      
      leds[getIndex(food_x, food_y, food_z)] = food_color;
    } else {
      leds[getIndex(tail->x, tail->y, tail->z)] = 0x000000;
      tail = tail->next;
    }
    
    leds[getIndex(head->x, head->y, head->z)] = snake_color;

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

    switch (keyCode[0]) {
      case 37: // left
      newDir = 0;
      break;

      case 39: // right
      newDir = 3;
      break;

      case 38: // up
      newDir = 1;
      break;

      case 40: // down
      newDir = 4;
      break;

      case 65: // a (left)
      newDir = 0;
      break;

      case 68: // d (right)
      newDir = 3;
      break;

      case 87: // w (in)
      newDir = 2;
      break;

      case 83: // s (out)
      newDir = 5;
      break;
    }

    // prevent going backwards
    if (newDir % 3 == dir % 3) {
      newDir = dir;
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

// should be easy to make this work for a linear image
void drawSmoothedPixel(float x, float y, float z, CRGB color) {

  for (int i = int(x - 1); i <= int(x + 1); i++) {
    for (int j = int(y - 1); j <= int(y + 1); j++) {
      for (int k = int(z - 1); k <= int(z + 1); k++) {
        
        if (!(i < 0 || j < 0 || k < 0 || i >= 8 || j >= 8 || k >= 8)) {
          float compX = x - i, compY  = y - j, compZ = z - k;
          float dist = sqrt( pow(compX, 2) + pow(compY, 2) + pow(compZ, 2) );
          
          float normBrightness = max(0, 1.0-dist);
          byte brightness = mapBrightness(normBrightness);

          byte cx = i, cy = j, cz = k;
  
          leds[ getIndex(cx, cy, cz) ] += CRGB( scale8(color.r, brightness), scale8(color.g, brightness), scale8(color.b, brightness) );
        }
        
      }
    }
  }
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