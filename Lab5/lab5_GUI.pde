import processing.serial.*;   // import serial library
import twitter4j.conf.*;
import twitter4j.*;
import twitter4j.auth.*;
import twitter4j.api.*;
import java.util.*;
Twitter twitter;
Serial myPort;
int radius = 350;             // set the radius of objects
int w = 300;                  // set an arbitrary width value
int radarDist, count = 0;
float value ;                // value from sensor
PFont myFont;                 // setup fonts in Processing
float input[];

/* create background and serial buffer */
void setup() {
  //create new instance and initialise ir with applicatio keys
  ConfigurationBuilder cb = new ConfigurationBuilder();
  cb.setOAuthConsumerKey( "consumerkey");
  cb.setOAuthConsumerSecret("consumersecret");
  cb.setOAuthAccessToken( "4865065038-accesstoken");
  cb.setOAuthAccessTokenSecret( "accesstokensecret");
  //create a TwitterFactory object and pass it our ConfigurationBuilder
  twitter = new TwitterFactory(cb.build()).getInstance();

  // setup the background size, colour and font.
  size(750, 450);
  background (0); // 0 = black
  myFont = createFont("verdana", 12);
  textFont(myFont);
  // setup the serial port and buffer
  myPort = new Serial(this, Serial.list()[0], 9600);
}

/* draw the screen */
void draw() {
  fill(0);                            // set the following shapes to be black
  noStroke();                         // set the following shapes to have no outline
  ellipse(radius, radius, 750, 750);  // draw a circle with a width/ height = 750 with its center position (x and y) set by the radius
  rectMode(CENTER);                   // set the following rectangle to be drawn around its center
  rect(350, 402, 800, 100);           // draw rectangle (x, y, width, height)

  /* set the radar distance rings and out put their values, 50, 100, 150 etc.. */
  for (int i = 0; i <= 6; i++) {
    noFill();
    strokeWeight(1);
    stroke(0, 255 - (30 * i), 0);
    ellipse(radius, radius, (100 * i), (100 * i));
    fill(0, 100, 0);
    noStroke();
    text(Integer.toString((radarDist / 50 + 1) * 20), 380, (305 - radarDist), 50, 50);
    radarDist += 50;
  }
  radarDist = 0;
  /* draw the grid lines on the radar every 30 degrees and write their values 180, 210, 240 etc.. */
  for (int i = 0; i <= 6; i++) {
    strokeWeight(1);
    stroke(0, 55, 0);
    line(radius, radius, radius + cos(radians(180 + (30 * i)))*w, radius + sin(radians(180 + (30 * i)))*w);
    fill(0, 55, 0);
    noStroke();
    if (180 + (30 * i) >= 300) {
      text(Integer.toString(180 + (30 * i)), (radius + 10) + cos(radians(180 + (30 * i))) * (w + 10), (radius + 10) + sin(radians(180 + (30 * i))) * (w + 10), 25, 50);
    } else {
      text(Integer.toString(180 + (30 * i)), radius + cos(radians(180 + (30 * i)))*w, radius + sin(radians(180 + (30 * i)))*w, 60, 40);
    }
  }
  /* Write information text and values. */
  noStroke();
  fill(0);
  rect(350, 402, 800, 100);

  if (value == -1) {
    fill(0, 100, 0);
    text("Distance: OUT OF RANGE! ", 120, 400, 200, 50);
  }
  else {
    //tweet a tweeter if the object is closer than 15 cm
    if (value <= 15.0)
      tweet();
    count ++;
    fill(150, 0, 0);
    ellipse(350, (350 - value) - value / 20 * 30, 10, 10);
    fill(0, 100, 0);
    text("Distance: " + Float.toString(value), 120, 400, 200, 50);     // text(string, x, y, width, height)
  }

  fill(0);
  rect(70, 60, 150, 100);
  fill(0, 100, 0);
  text("Screen Key:", 100, 50, 150, 50);
  noFill();
  stroke(150, 0, 0);
  strokeWeight(1);
  ellipse(29, 53, 10, 10);
  fill(150, 0, 0);
  text(" D-5C Position", 115, 70, 150, 50);
  println(value);
}

/* get values from serial port */
void serialEvent (Serial myPort) {
  String inString = myPort.readStringUntil('\n');  // read the serial port until a new line

  if (inString != null) {  // if theres data in between the new lines
    inString = trim(inString); // get rid of any whitespace just in case
    input =  float(split(inString, ','));//separate the String with “,” to read different data
    if (input.length == 2) {
      value = input[1];
    }
  }
}

//attempt to update our status on Twitter and print a message to the console if it works
void tweet( ) {
  try {
    Status status = twitter.updateStatus("To close " + count);
    System.out.println("Status updated to [" + status.getText() + "].");
  }
  //catch any problem and print the error message
  catch (TwitterException te) {
    System.out.println("Error: " + te.getMessage());
  }
}