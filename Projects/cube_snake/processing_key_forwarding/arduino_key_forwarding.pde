import processing.serial.*;

Serial port;

void setup() {
  size(512, 256);
  
  if (Serial.list().length > 0) {
    port = new Serial(this, Serial.list()[0], 115200);
  }
}

void draw() {}

void keyPressed() {
  if (port != null)
    port.write(keyCode);
    
  println(key);
  println(keyCode);
  println("----");
}