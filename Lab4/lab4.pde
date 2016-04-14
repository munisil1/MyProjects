import processing.serial.*;   //initializing communication with the serial monitor

Serial myPort;    // intialize The serial port
int xPos = 100;     // set the horizontal position of the graph
float inByte = 0; //initialize the value used to hold the byte from the serial monitor
float inputs[];  //store the data read from myPort in order to distribute them later
//create flags for which button is pressed and set the loop in serialEvent()
flagH = 0;
int flagT = 0;
int flagL = 0;
int tX, tY;  // Position of temp button
int hX, hY;  // Position of humidity button
int lX, lY;  // Position of light level button
int tSize = 50;   //size of the temperature-controlling button
int lSize = 50;    //size of the light-controlling button
int hSize = 50;   //size of the humidity-controlling button
//change the title and labels color according to which data is printed(light/ humidity/ temp)
color current;
color tColor, hColor, lColor;     //setting up different colors for each variable
color tHighlight, hHighlight, lHighlight;

//initialize the state of the buttons. All not presssed
boolean tOver = false;
boolean hOver = false;
boolean lOver = false;

void setup() {

  size(1500, 1350);  //set the size of the display screen
  println((Object)Serial.list());    //enabling printing of the values on the console window

  //initialize serial communication and the port being used
  myPort = new Serial(this, Serial.list()[0], 9600);

  // generate a new serialEvent() when gets a newline character:
  myPort.bufferUntil('\n');

  // set initial background:
  background(255);
  //set up the different colors for each buttonto distinguish them
  tColor = color(150, 100, 200);
  hColor = color(50, 200, 100);
  lColor = color(200, 50, 150);

  //each button’s color when the mouse is over it
  tHighlight = color(200);
  hHighlight = color(200);
  lHighlight = color(200);

  //set the position of the Temperature button
  tX = width - tSize - 10;
  tY = height - tSize / 2 - 100;
  //set the position of the Humidity button
  hX = width - hSize * 2 - 30;
  hY = height - hSize / 2 - 100;
  //set the position of the Light level button
  lX = width - lSize * 3 - 50;
  lY = height - lSize / 2 - 100;

}
//The function that enables drawing
void draw() {
  drawStuff();// draw the coordinate system, numbers and the letters on the buttons

  update(mouseX, mouseY);   //check the position of the mouse

  //fill different colors depending on whether the mouse is over the button
  if (tOver) {  //if the mouse is over temperature button
    fill(tHighlight);
  } else {
    fill(tColor);
  }
  stroke(0); //setting the outline to black
  rect(tX, tY, tSize, tSize);//draw the button as a tSize * tSize rectangle at (tX, tY) position

  if (hOver) {     //if the mouse is over humidity button
    fill(hHighlight);
  } else {
    fill(hColor);
  }
  stroke(0);
  rect(hX, hY, hSize, hSize);

  if (lOver) {    //if the mouse is over light level button
    fill(lHighlight);
  } else {
    fill(lColor);
  }
  stroke(0);
  rect(lX, lY, lSize, lSize);

  textSize(32);   //size of the title of the graph
  //fill the following text with the color of the pressed button(also the same color of the graph)
  fill(current);
  //the title and its position
  text("Graph of Humidity, Temperature and Light Against time", 400, 100 );
  //VERTICAL AXIS
  textSize(32);
  int x = -450;   // Location of start of text.
  int y = 500;
  pushMatrix();    //rotating the text to make it vertical
  translate(x, y);
  rotate(-HALF_PI);
  //y-axis
  text("Temp/Hum/Light", x, y);
  popMatrix();
  //HORIZONTAL AXIS
  //change the font size
  textSize(32);
  text("Time/s", width - 200, height - 200 );
  // draw the graph;
  stroke(current);// use the same color of the pressed button(and the text above)
  line(xPos, (height - 200), xPos, (height - 200 - inByte));    //position to start the graph
  //draw the coordinates
  stroke(255, 0, 0);  //set the background color
  line(xPos, (height - 200), width - 200, (height - 200)); //horizontal line drawing
  line(100, (height - 200), 100, 150); //vertical line drawing

  //label buttons accordingly
  text("T", tX + 13, tY + 35);
  text("H", hX + 13, hY + 35);
  text("L", lX + 13, lY + 35);

  // at the edge of the screen, go back to the beginning of the x axis
  if (xPos >= 1200) {
    xPos = 100;
    background(255);
  } else {
    // increment the horizontal position:
    xPos++;

  }

}


//if the mouse is on one of the button, set the “over” flag of this button as true, the others’ as false
void update(int x, int y) {
  if ( overT(tX, tY, tSize, tSize) ) {
    tOver = true; // the mouse is over temperature button
    hOver = lOver = false;     //set “over” flag of humidity and light as false
  } else if ( overH(hX, hY, hSize, hSize) ) {
    hOver = true;  // the mouse is over humidity button
    tOver = lOver = false;   //set “over” flag of the other two as false
  } else// ( overL(lX, lY, lSize, lSize) )
  {
    lOver = true;        // the mouse is over light button
    tOver = hOver = false;     //set “over” flag of the other two as false
  }
}


void serialEvent (Serial myPort) {

  // get the ASCII string:
  String inString1 = myPort.readStringUntil('\n');  //serial port reads until end of new line

  if (inString1 != null) {
    // trim off any whitespace:
    inString1 = trim(inString1);
    // convert to an float and map to the screen height:
    inputs =  float(split(inString1, ','));//separate the String with “,” to read different data
    //the temperature button is pressed so that temperature values can keep looping until stopped
    if (flagT == 1) {
      println("temperature");   //print temperature on the console
      if (inputs.length == 3) {   //three values read from the serial port
        inByte = inputs[0];   //the first value of the array is temperature value
        float temp =  inByte;   //receive the temperature value
        println(temp);
        temp = map(temp, 0, 800, 0, height);     //graph  the temperature values
      }
    }
    { //the light level button is pressed so that light values can keep looping until stopped
      if (flagL == 1)
        println("light");     //print the values for light
      if (inputs.length == 3) {
        inByte = inputs[1];      //the second value of the array is light value
        float light =  inByte;
        println(light);
        light = map(light, 0, 800, 0, height);   //graph  the light values
      }
    }
    //the humidity button is pressed so that light values can keep looping until stopped
    if ( flagH == 1) {
      println("humidity");
      if (inputs.length == 3) {
        inByte = inputs[2];  //the third value of the array is humidity value
        float humid =  inByte;
        println(humid);
        humid = map(humid, 0, 800, 0, height);    //graph  the humidity values
      }
    }
  }
}

//check which button is pressed and set the print flag to 1, change the current color,
//LEFT CLICK STARTS PLOTTING, RIGHT CLICK STOPS UPDATING THE GRAPH
void mousePressed() {
  if (tOver) {
    flagT = 1;
    current = tColor;
    if (mouseButton == RIGHT)
      flagT = 0;
  }
  if (hOver) {
    flagH = 1;
    current = hColor;
    if (mouseButton == RIGHT)
      flagH = 0;
  }

  if (lOver) {
    flagL = 1;
    current = lColor;
    if (mouseButton == RIGHT)
      flagL = 0;
  }
}
//check if the mouse is over the temperature button
boolean overT(int x, int y, int width, int height)  {
  if (mouseX >= x && mouseX <= x + width &&
      mouseY >= y && mouseY <= y + height) {
    return true;
  } else {
    return false;
  }
}
//check if the mouse is over the humidity button
boolean overH(int x, int y, int width, int height)  {
  if (mouseX >= x && mouseX <= x + width &&
      mouseY >= y && mouseY <= y + height) {
    return true;
  } else {
    return false;
  }
}
//check if the mouse is over the light button
boolean overL(int x, int y, int width, int height)  {
  if (mouseX >= x && mouseX <= x + width &&
      mouseY >= y && mouseY <= y + height) {
    return true;
  } else {
    return false;
  }
}

//FUNCTION THAT SETS THE LABELS OF THE AXES AND THEIR POSITIONS
void drawStuff() {
  textSize(17);
  fill(tColor);//using the color of temperature button
  text("Temperature*C", 90, 75);
  fill(lColor);//using the color of light button
  text("Light Level/Lux", 90, 95);
  fill(hColor);//using the color of humidity button
  text("Humidity/%", 90, 115);
  //print the numbers besides of axes in black
  fill(50);
  textSize(11);
  //x axis
  for (int i = 0; i <= 1200; i += 50) {
    text(i , 100 + i, height - 185);
    stroke(300);
  }
  for (int j = 0; j < 1001; j += 50) {
    text(j, 80, height - 200 - j);
    stroke(300);
  }
}