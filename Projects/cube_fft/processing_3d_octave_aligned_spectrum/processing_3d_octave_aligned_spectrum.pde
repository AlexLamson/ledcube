import ddf.minim.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;
import ddf.minim.signals.*;
import ddf.minim.spi.*;
import ddf.minim.ugens.*;

import processing.serial.*;

Minim minim;
AudioPlayer sound;

AudioSource src;

FFT fft;

Serial port;

// ===== C O N F I G U R E =====
boolean showMic = false;
float volumeMultiplier = 1;

int size = 8; //more than 8 gives dead bands (nyquist)

float startingFreq = 55.0;
float bandCoverage = pow(2, 1.0/size); // covers from starting freq to bandCoverage times starting freq, so 2 would be a full octave;
float bandStep = pow(2, 1.0/size); // starting freq * bandStep for next band;
float finalFreq = startingFreq * pow(2, size);

float _scale = 10;
float voxel = 10;

float[] octaveMultiplier = {4.0, 2.0, 1.5}; // The bottom octave spreads out the energy too much

String fname = "jingle.mp3";

int refreshMillis = 20;
int lastRefresh = 0;

float[] heights = new float[64];

void setup() {
  size(1920, 1080, P3D);
  //size(3800, 2060, P3D);
  background(0);

  port = new Serial(this, Serial.list()[0], 115200);

  minim = new Minim(this);

  if (showMic) {
    src = minim.getLineIn();
  } else {

    sound = minim.loadFile(fname);
    sound.play();

    src = sound;
  }

  fft = new FFT(src.bufferSize(), src.sampleRate());
  
  //for (int i = 0; i < 20; i++) {
  //  print(fft.indexToFreq(i) + ", ");
  //}
  //println();
  
  for (int i = 0; i < 64; i++) {
    heights[i] = 0;
  }
}

void draw() {
  background(0);

  fft.forward(src.mix);

  camera( -65 * _scale * float(size)/8, 88 * _scale * float(size)/8, 90 * _scale * float(size)/8, 
    0, 0, size * _scale * voxel / 2, 
    0, 0, -1);
  //perspective(PI/3.0, float(width)/height, 0, 100);

  stroke(255);

  scale(_scale);
  translate(-4 * voxel + voxel/2, -4 * voxel - voxel/2, 0);
  pushMatrix();

  float currFreq = startingFreq;
  int index = 0;
  
  // Serial header
  //if (port != null) {
  //  port.write(byte(254));
  //  port.write(byte(254));
  //  port.write(byte(254));
  //}

  while (currFreq < finalFreq) {
    float nextFreq = currFreq * bandStep;
    float endFreq = currFreq * bandCoverage;

    float level = integrateOverFreqs(currFreq, endFreq);

    level *= volumeMultiplier;

    level *= 0.25; // put in reasonable range (0.5 before normalization)

    int barX = index%size;
    int barY = (index/size);

    if (barY < octaveMultiplier.length) {
      level *= octaveMultiplier[barY];
    }


    // prefer stronger signals
    level /= size * voxel;
    level = pow(level, 1.1); // prefer stronger signals to weaker ones
    level *= size * voxel;
    // clip to cube
    level = min( size * voxel, level );
    
    int arrayIndex = barX + barY*8;
    if (heights[ arrayIndex] < level) {
      heights[ arrayIndex ] = level;
    }

    // draw
    int colStep = 255/size;

    fill(lerpColor(color(0, 0, 0), color(255 - colToPhotons(barY*colStep), photonsToCol(barY * colStep), photonsToCol(barX * colStep)), min(1, max(0, level/(size * voxel) * size))));
    translate(voxel*barX, voxel*barY, level/2);
    box(voxel, voxel, level);
    
    popMatrix();
    pushMatrix();
    index++;

    currFreq = nextFreq;
  }
  popMatrix();


  if (port != null && millis() >= lastRefresh + refreshMillis) {
    lastRefresh = millis();
    
    for (int i = 0; i < 64; i++) {
      byte toWrite = byte( 255.0 * heights[ i ] / (size*voxel) );
      //println(toWrite);
      port.write( toWrite );
      
      heights[i] = 0;
    }
  }
}

float integrateOverFreqs(float startFreq, float endFreq) {
  float total = 0;

  int startBand = fft.freqToIndex(startFreq);
  int endBand = fft.freqToIndex(endFreq);

  if (fft.indexToFreq(startBand) < startFreq) {
    startBand++;
  }
  if (endFreq < fft.indexToFreq(endBand)) {
    endBand--;
  }

  // if start and end freq fall between same two zones
  if (startBand > endBand) {
    total = interpolateFreq(startFreq) + interpolateFreq(endFreq);
    total /= 2;
    total *= (endFreq - startFreq) / (fft.indexToFreq(startBand) - fft.indexToFreq(endBand));

    return total;
  }

  float preTotal = interpolateFreq(startFreq) * (fft.indexToFreq(startBand) - startFreq) / (fft.indexToFreq(startBand) -fft.indexToFreq(startBand - 1));
  float postTotal = interpolateFreq(endFreq) * (endFreq - fft.indexToFreq(endBand)) / (fft.indexToFreq(endBand + 1) -fft.indexToFreq(endBand)); 

  total = preTotal + postTotal;

  if (startBand == endBand) {
    return total;
  }

  total += (fft.getBand(startBand) + fft.getBand(endBand)) / 2;

  int currBand = startBand + 1;
  while (currBand < endBand) {
    total += fft.getBand(currBand++);
  }

  return total;
}

float interpolateFreq(float freq) {
  int band = fft.freqToIndex(freq);
  float bandFreq = fft.indexToFreq(band);

  if (abs(freq - bandFreq) < 0.001) { 
    return fft.getBand(band);
  }

  int secondaryBand;
  if (freq < bandFreq) {
    secondaryBand = band - 1;
  } else {
    secondaryBand = band + 1;
  }
  float secondaryFreq = fft.indexToFreq(secondaryBand);

  float primaryWeight = (freq - secondaryFreq) / (bandFreq - secondaryFreq);

  return primaryWeight * fft.getBand(band) + (1 - primaryWeight) * fft.getBand(secondaryBand);
}

int colToPhotons(int col) {
  return int( 255 * pow((float(col) / 255), 2) );
}

int photonsToCol(int photons) {
  return int( 255 * pow(float(photons) / 255, 0.5) );
}