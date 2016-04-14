// initialize the serial port to read data
import processing.serial.*;
Serial myPort;
// variable that takes the input
float inByte = 0.0;
// set the flag for buttons
boolean up = false;
boolean down = false; 
// colors for the buttons
color onUp = color(255, 150, 0);
color onDown = onUp;
color upColor = color(0, 150, 255);
color downColor = upColor;
// set the vertices for up and down buttons; w for width, h for hight
int upX = 640;
int downX = 670;
int w = 15;
int upYTop = 410;
int h = 20;
int downYTop = 430;
// set the position of the graph 
float xPos = 50.0;
int yPos = 250;
// shift the result half of the 5V measured
float shift = 2.5;
// flag for new value that we take in
boolean newVal = false;
// measure range is from 52.0 to 54.0 Hz
void setup() {
  size(900, 500); // set the size of the window
  println((Object)Serial.list()); // enabling printing of the values on the console window
  //initialize serial communication and the port being used 
  myPort =  new Serial(this, Serial.list()[0], 3200 );
  // generate a new serialEvent() when gets a newline character:
  myPort.bufferUntil('\n');
  background(0);
}

void draw() {
  drawStuff(); // draw the screen of the oscillator
  update(mouseX, mouseY);   //check the position of the mouse
  //fill different colors depending on whether the mouse is over the button
  if (up) {  //if the mouse is over the up button
    fill(onUp);
  } else {
    fill(upColor);
  }
  stroke(0); //setting the background to black
  triangle(upX, upYTop, upX - w, upYTop + h, upX + w, upYTop + h);//draw the button as a triangle pointing upwards
  // do the same as the above one
  if (down) {   //if the mouse is over the down button
    fill(onDown);
  } else {
    fill(downColor);
  }
  stroke(0);
  triangle(downX, downYTop, downX - w, downYTop - h, downX + w, downYTop - h);
  stroke(255, 200, 100);
  // when the graph reaches the end of the screen, go back to the origin.
  if (newVal) {
    strokeWeight(2.0);
    line(xPos, height - yPos - inByte + 15, xPos, height - yPos - inByte);
    if ((xPos +=1) >= 550) {
      xPos =50;
      //draw the background again to clear the screen
      background(0);
    }
    newVal = false;
  }
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');  //serial port reads until end of new line

  if (inString != null) {    
    // trim off any whitespace:
    inString = trim(inString);
    // convert to an float and map to the screen height:
    inByte =  float(inString) - shift ;//separate the String with “,” to read different data
    //the temperature button is pressed so that temperature values can keep looping until stopped
    inByte = map(inByte, 0, 5, 0, height - 180);
    // set a flag for the value received
    newVal = true;
  }
}

//function that draws all the elements of the window to create a screen of oscillator
void drawStuff() {
  //cover the screen with black rectangles and leave one part as blue
  noStroke();
  fill(50, 50, 150);
  rect(550, 0, 900, 500 ); //drawing the rectangles that act as the frame of the oscillator
  rect(50, 0, 550, 50);
  rect(0, 0, 50, 550);
  rect(50, 450, 550, 50);
  // set the color of the rectangle frame of the oscilloscope;
  stroke(255, 180, 205);
  strokeWeight(3.0);
  strokeCap(ROUND); 
  line(50, 50, 550, 50);
  line(50, 50, 50, 450);
  line(550, 50, 550, 450);
  line(50, 450, 550, 450);
  // draw the lines on the left of the screen
  drawBar(50);
  drawBar(150);
  drawBar(250);
  drawBar(350);
  drawBar(450);
  // set the stroke weight to normal
  strokeWeight(1.0);
  // draw the dot lines on the screen
  for (int m = 100; m < 550; m +=50) {
    if ( m == 300);
    else 
    dotLine(m, 50, m, 450);  //for the vertical gridlines
    if ( m == 250);
    else
      dotLine(50, m, 550, m);  //for the horizontal grid lines
  }
  // draw the dash lines on the screen (the background lines)
  drawDashVer(248);
  drawDashVer(50);
  drawDashVer(446);
  drawDashHor(298);
  drawDashHor(50);
  drawDashHor(546);
  // put all the text
  textSize(16);
  fill(255, 230, 0);
  textAlign(CENTER);
  text("MEASURE", 650, 46);
  text("CH1", 650, 75);
  text("CH1", 650, 175);
  text("Period", 650, 195);
  text("0.020", 650, 225);
  text("CH1", 650, 275);
  text("Freq", 650, 295);
  text("52.2", 650, 325);
  text("CH1", 650, 375);
  text("Position", 650, 395);
  text("GROUP 5C ~ ARDUINO OSCILLOSCOPE", 300, 46);
  text("Volts", 30, 80);
  text("Time", 500, 468);
}

void drawBar(int y) {
  line(600, y, 700, y);
}
// draw vertical dash line
void drawDashVer(int y) {
  for ( int x = 50; x < 550; x += 10)
    line(x, y, x, y + 4);
}
// draw horizontal dash line
void drawDashHor(int x) {
  for (int y = 50; y < 450; y += 10) 
    line(x, y, x + 4, y);
}
// draw dot line
void dotLine(int x1, int y1, int x2, int y2) {
  for (int i = 0; i <=100; i++) {
    float x = lerp(x1, x2, i/100.0);
    float y = lerp(y1, y2, i/100.0);
    point(x, y);
  }
}

//if the mouse is on one of the button, set the “over” flag of this button as true, the others’ as false
void update(int x, int y) {
  if ( overUp(upX, upYTop, w, h) ) {
    up = true; // the mouse is over temperature button
    down = false;   //set “over” flag of humidity and light as false
  } else if ( overDown(downX, downYTop, w, h) ) {
    down = true;  // the mouse is over humidity button
    up = false;   //set “over” flag of the other two as false
  }
}

//check if the mouse is over the up button
boolean overUp(int x, int y, int width, int height) {
  if (mouseX >= x - width&& mouseX <= x + width &&
    mouseY >= y && mouseY <= y + height) {
    return true;
  } else {
    return false;
  }
}

//check if the mouse is over the down button
boolean overDown(int x, int y, int width, int height) {
  if (mouseX >= x-width && mouseX <= x + width &&
    mouseY >= y-height && mouseY <= y) {
    return true;
  } else {
    return false;
  }
}
// if click on the up buttons, increase the shift on y axis; same for the down button
void mousePressed() {
  if (up) {
    yPos += 5;
  }
  if (down) {
    yPos -=10;
  }
}