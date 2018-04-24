/*
Project an image or a feed from the webcam onto the LED cube, 
such that it is visible when the cube is viewed from a certain angle
*/


boolean usePort = false; //set this to false if you aren't displaying it on the physical cube
boolean useImage = true; //true = show image, false = show webcam
//to change the image, change the start of the setup function

int ledsPerEdge = 8; //number of LEDs per edge
float cmBetweenLEDs = 3.5;
float cmCameraToCube = 100;
boolean spinCube = true;
float cubeSpinSpeed = 10;//1=slow, 10=fast


//Serial
import processing.serial.*;
Serial port;

//Reading from the camera
import processing.video.*;
Capture cam;

//Display image
PImage img;

// list of LEDs (sorry this is poorly named, these are the LEDs)
LED[] leds = new LED[ledsPerEdge*ledsPerEdge*ledsPerEdge];

/*
neopixel gamma correction

NOTE: THIS IS THE LAST THING I ADDED SINCE THE LEDS STARTED OCCASIONALLY FLASHING RANDOM COLORS

*/
int gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void setup() {
  if(useImage) {
    //img = loadImage("debug4.png");
    //img = loadImage("black.png");
    //img = loadImage("heart1.png");
    //img = loadImage("heart2.png");
    //img = loadImage("heart3.jpg");
    img = loadImage("A.png");
    //img = loadImage("plus.png");
    //img = loadImage("huewheel.jpg");
  }
  else {
    cam = new Capture(this, 640, 480, 30);  // 30 fps
    cam.start();
  }

  //open a port so we can use our serial connection to communicate to the physical cube
  if(usePort) {
    port = new Serial(this, Serial.list()[0], 115200); // open port
  }


  //frameRate(6.5); //predicted max framerate given 115200 baud rate
  size(640, 480, P3D);


  //initialize the cube
  int i = 0;
  for(int xi = 0; xi < 8; xi++) {
    for(int yi = 0; yi < 8; yi++) {
      for(int zi = 0; zi < 8; zi++) {
        leds[i] = new LED((xi-4)*cmBetweenLEDs, (yi-4)*cmBetweenLEDs, (zi-4)*cmBetweenLEDs);
        leds[i].ledColor = color(xi*(256/8), yi*(256/8), zi*(256/8)); //map each axis to a color channel
        i++;
      }
    }
  } //end cube initialization
} //end setup()


void draw() {
  background(30);
  lights();

  // Center cube in display window
  translate(width/2, height/2, cmCameraToCube);
  float fov = 10*DEG_TO_RAD;
  
  //compute optimal fov
  float edgeLength = (ledsPerEdge-1) * cmBetweenLEDs;
  float centerToCornerDist = dist(0,0,0,edgeLength/2,edgeLength/2,edgeLength/2);
  fov = tan(centerToCornerDist/cmCameraToCube) / 1.7; //1.7 is just a number I happened to pick
  
  perspective(fov, width/(float)height, 0.1, 1000);


  //angle at which the cube should be viewed (currently on the right side of the data input wires)
  float mX = 0.1453125; //map(mouseX,0,width,0,1);
  float mY = 0.37222221; //map(mouseY,0,height,0,1);
  rotateX(mY*PI);
  rotateZ(mX*PI);

  // Slowly spin the cube
  if(spinCube) {
    rotateX(frameCount * 0.001 * cubeSpinSpeed);
    rotateY(frameCount * 0.002 * cubeSpinSpeed);
    rotateZ(frameCount * 0.001 * cubeSpinSpeed);
  }


  //find the screen bounds of the cube
  float minCubeX = width;
  float maxCubeX = 0;
  float minCubeY = height;
  float maxCubeY = 0;
  for(LED led : leds) {
    float x = screenX(led.position.x, led.position.y, led.position.z);
    float y = screenY(led.position.x, led.position.y, led.position.z);

    if(x < minCubeX)
      minCubeX = x;
    if(x > maxCubeX)
      maxCubeX = x;
    if(y < minCubeY)
      minCubeY = y;
    if(y > maxCubeY)
      maxCubeY = y;
  }

  //read a frame from the webcam
  if(!useImage && cam.available()) {
    cam.read();
  }

  byte allRGB[] = new byte[3*512]; //stores frame of all colors to send to arduino
  int i = 0;
  for(LED led : leds) {
    float xImagePos = screenX(led.position.x, led.position.y, led.position.z);
    float yImagePos = screenY(led.position.x, led.position.y, led.position.z);
    
    //figure out what color the LED should be
    color pixelColor = color(255);
    if(useImage) {
      //project the image onto the cube
      int x = (int)map(xImagePos, minCubeX, maxCubeX, 0, img.width);
      int y = (int)map(yImagePos, minCubeY, maxCubeY, 0, img.height);
      pixelColor = img.get(x, y);
    }
    else if(cam.available()) {
      //project the (flipped) camera onto the cube
      int x = (int)map(xImagePos, minCubeX, maxCubeX, 0, cam.width);
      int y = (int)map(yImagePos, minCubeY, maxCubeY, 0, cam.height);
      pixelColor = cam.get(cam.width-x, y);
    }
    
    //make each LED slightly transparent
    color transparentPixelColor = color(red(pixelColor), green(pixelColor), blue(pixelColor), 128);
    led.ledColor = transparentPixelColor;

    //gamma adjust the colors so they look more natural, then save them to the cube frame buffer
    allRGB[i*3+0] = (byte)gamma[(int)red(pixelColor)];
    allRGB[i*3+1] = (byte)gamma[(int)green(pixelColor)];
    allRGB[i*3+2] = (byte)gamma[(int)blue(pixelColor)];
    i++;
  }

  //send the frame to the arduino
  if(usePort) {
    port.write(allRGB);
  }


  // Move and rotate leds
  for (LED led : leds) {
    led.update();
    led.display();
  }
}


// Represents color and position of a single LED
class LED {
  PVector position;
  color ledColor = color(255);

  LED(float x, float y, float z) {
    position = new PVector(x, y, z);
  }

  // Update function for potential future use
  void update() {
  }

  // Aproximate the shape of the LED with a sphere
  void display() {
    pushMatrix();
    translate(position.x, position.y, position.z);
    noStroke();
    fill(ledColor);
    sphere(1.27/2);
    popMatrix();
  }
}