import processing.serial.*;   //initializing communication with the serial monitor
import twitter4j.conf.*;
import twitter4j.*;
import twitter4j.auth.*;
import twitter4j.api.*;
import java.util.*;

int count = 0;
Twitter twitter;
Serial myPort;  // intialize The serial port
int xPos = 100;   // set the horizontal position of the graph
float inByte = 0; //initialize the value used to hold the byte from the serial monitor
float inputs[];  //store the data read from myPort in order to distribute them later
//create flags for which button is pressed and set the loop in serialEvent()
int flagT = 0;
int flagD = 0;
int tX, tY;  // Position of temp button
int dX, dY;  // Position of distance button
int tSize = 50;   //size of the temperature-controlling button
int dSize = 50;  //size of the distance-controlling button
//change the title and labels color according to which data is printed(temp/ distance)
color current;
color tColor, dColor;   //setting up different colors for each variable
color tHighlight, dHighlight;

//initialize the state of the buttons. All not presssed
boolean tOver = false;
boolean dOver = false;
//set application keys
String consumerKey = "oDrf60UwSZqHX3zrkVHRrOKFZ";
String consumerSecret = "8XMXCJF54UkQVDYCAON3ucEOTCKklZ9WcIjKsS0ISfKW5T9Qq4";
String accessToken = "4865065038-Lp8riluWFcnF4nsOgkbYVDfWnjygpRLynAtd69s";
String accessTokenSecret = "sKUk76croih3NblzTapb2OgyPJb24JKBLKnGapF7fSSUg";
void setup() {
  //create new instance and initialise ir with applicatio keys
  ConfigurationBuilder cb = new ConfigurationBuilder();
  cb.setOAuthConsumerKey(consumerKey);
  cb.setOAuthConsumerSecret(consumerSecret);
  cb.setOAuthAccessToken(accessToken);
  cb.setOAuthAccessTokenSecret(accessTokenSecret);
  //create a TwitterFactory object and pass it our ConfigurationBuilder
  twitter = new TwitterFactory(cb.build()).getInstance();
  
  size(1500, 1350);  //set the size of the display screen
  println((Object)Serial.list());  //enabling printing of the values on the console window

  //initialize serial communication and the port being used
  myPort = new Serial(this, Serial.list()[0], 9600);

  // generate a new serialEvent() when gets a newline character:
  myPort.bufferUntil('\n');

  // set initial background:
  background(255);
  //set up the different colors for each buttonto distinguish them
  tColor = color(150, 100, 200);
  dColor = color(50, 200, 100);


  //each button’s color when the mouse is over it
  tHighlight = color(200);
  dHighlight = color(200);


  //set the position of the Temperature button
  tX = width - tSize - 10;
  tY = height - tSize / 2 - 100;
  //set the position of the distance button
  dX = width - dSize * 2 - 30;
  dY = height - dSize / 2 - 100;

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

  if (dOver) {   //if the mouse is over humidity button
  fill(dHighlight);
  } else {
  fill(dColor);
  }
  stroke(0);
  rect(dX, dY, dSize, dSize);

  textSize(32);   //size of the title of the graph
  //fill the following text with the color of the pressed button(also the same color of the graph)
  fill(current);
  //the title and its position
  text("Graph of Temperature and Distance Against time", 400, 100 );
  //VERTICAL AXIS
  textSize(32);
  int x = -450;   // Location of start of text.
  int y = 500;
  pushMatrix();  //rotating the text to make it vertical
  translate(x, y);
  rotate(-HALF_PI);
  //y-axis
  text("Temp/Distance", x, y);
  popMatrix();
  //HORIZONTAL AXIS
  //change the font size
  textSize(32);
  text("Time/s", width - 200, height - 200 );
  // draw the graph;
  stroke(current);// use the same color of the pressed button(and the text above)
  line(xPos, (height - 200), xPos, (height - 200 - inByte));  //position to start the graph
  //draw the coordinates
  stroke(255, 0, 0);  //set the background color
  line(xPos, (height - 200), width - 200, (height - 200)); //horizontal line drawing
  line(100, (height - 200), 100, 150); //vertical line drawing

  //label buttons accordingly
  text("T", tX + 13, tY + 35);
  text("D", dX + 13, dY + 35);

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
  dOver = false;   //set “over” flag of humidity and light as false
  } else if ( overD(dX, dY, dSize, dSize) ) {
  dOver = true;  // the mouse is over humidity button
  tOver = false;   //set “over” flag of the other two as false
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
    if (inputs.length == 2) {   //three values read from the serial port
      inByte = inputs[0];   //the first value of the array is temperature value
      float tempc =  inByte;   //receive the temperature value
      println(tempc);
      tempc = map(tempc, 0, 800, 0, height);   //graph  the temperature values
    }
  }
  //the distance button is pressed so that light values can keep looping until stopped
  if ( flagD == 1) {
    println("distance");
    if (inputs.length == 2) {
      inByte = inputs[1];  //the third value of the array is humidity value
      float distance =  inByte;
      println(distance);
      distance = map(distance, 0, 800, 0, height);  //graph  the humidity values
    }
  }
    //tweet a tweeter if the object is closer than 15 cm
    if(inputs[1] <= 15)
    tweet();
    count ++;
   
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
  if (dOver) {
  flagD = 1;
  current = dColor;
  if (mouseButton == RIGHT)
    flagD = 0;
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
//check if the mouse is over the distance button
boolean overD(int x, int y, int width, int height)  {
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
  fill(dColor);//using the color of humidity button
  text("Distance/cm", 90, 95);
  //print the numbers besides of axes in black
  fill(50);
  textSize(11);
  //x axis
  for (int i = 0; i <= 1200; i += 50) {
  text(i , 100 + i, height - 185);
  stroke(300);
  }
  for (int j = 0; j < 100; j += 50) {
  text(j, 80, height - 200 - j);
  stroke(300);
  }
}

//attempt to update our status on Twitter and print a message to the console if it works
void tweet( ){
  try{
    Status status = twitter.updateStatus("Change this every time" + count);
    System.out.println("Status updated to [" + status.getText() + "].");
  }
  //catch any problem and print the error message
  catch(TwitterException te){
    System.out.println("Error: " + te.getMessage());
  }
}