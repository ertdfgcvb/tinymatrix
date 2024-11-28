/**
 * This Processing sketch sends all the pixels of the canvas to the serial port.
 */

import processing.serial.*;

final int TOTAL_WIDTH  = 64;
final int TOTAL_HEIGHT = 32;
final int NUM_CHANNELS = 3;
final int BAUD_RATE    = 2000000;

Serial serial;
byte[]buffer;

PImage[] imgs = new PImage[68];

int frame = 0;

void setup() {
  // The Processing preprocessor only accepts literal values for size()
  // so we can't do: size(TOTAL_WIDTH, TOTAL_HEIGHT);
  size(64, 32);

  buffer = new byte[TOTAL_WIDTH * TOTAL_HEIGHT * NUM_CHANNELS];

  String[] list = Serial.list();
  printArray(list);

  try {
    // On macOS / Linux see the console for all wavailable ports
    // On Windows the ports are numbered
    // final String PORT_NAME = "COM3"; // Win
    final String PORT_NAME = "/dev/tty.SLAB_USBtoUART"; // Mac
    serial = new Serial(this, PORT_NAME, BAUD_RATE);
  } catch (Exception e) {
    println("Serial port not intialized...");
  }
  
  println(serial);
  
  for(int i=0; i<imgs.length; i++) {
    String num = nf(i+1, 4);
    imgs[i] = loadImage("ns/BLUESHORE_" + num + ".png");
  }
  
  
  frameRate(15);
}

void draw() {

  // Draw some things
  background(0,0,0);
  
  image(imgs[frame % imgs.length], 0, 0);
  /*
  noStroke();
  fill(255, 0, 0);
  float d = map(sin(frameCount * 0.1), -1, 1, 3, 30);
  ellipse(width/2, height/2, d, d);

  noFill();
  stroke(0,255,0);
  rect(0, 0, width-1, height-1);
  */

  // --------------------------------------------------------------------------
  // Write to the serial port (if open)
  if (serial != null) {
    loadPixels();
    int idx = 0;
    for (int i=0; i<pixels.length; i++) {
      color c = pixels[i];
      buffer[idx++] = (byte)(c >> 16 & 0xFF); // r
      buffer[idx++] = (byte)(c >> 8 & 0xFF);  // g
      buffer[idx++] = (byte)(c & 0xFF);       // b
    }
    serial.write('*');     // The 'data' command
    serial.write(buffer);  // ...and the pixel values
  }
  
  frame++;
}
