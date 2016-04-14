//-------------buzzer-------------------------
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_C4  262
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_C5  523
#define forBuzzer 9
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0
};
//Underwolrd tempo
int underworld_tempo[] = {
  18, 18, 18, 18, 18, 18, 18, 3
};
int song = 0;
//-------------------keypad---------------------
int g = 0;
byte h = 0;
byte v = 0;  //variables used in for loops
const unsigned long period = 50; //little period used to prevent error
unsigned long kdelay = 0;      // variable used in non-blocking delay
const byte rows = 4;           //number of rows of keypad
const byte columns = 3;          //number of columnss of keypad
const byte Output[rows] = {8, 7, 6, 5}; //array of pins used as output for rows of keypad
const byte Input[columns] = {4, 3, 2}; //array of pins used as input for columns of keypad
int password[5] = {1, 2, 3, 4, 14}; //password
int input[5] = {0, 0, 0, 0, 0}; //default input
//------------ultrasonic sensorWindow---------------
#define wTRIG_PIN A3
#define wECHO_PIN A2
int wToWall = 12; // the distance from the window sensor to the wall
//------------ultrasonic sensorDoor---------------
#define dTRIG_PIN A5
#define dECHO_PIN A4
int dToWall = 21; // the distance from the door sensor to the wall
//-------------detecting distance--------------
float duration;
float distance;
int reading = 0;
//------------mark whether the door or window is open
bool opened = false;
//------------Temperature Sensor--------------
#define TEMP_PIN A0
//---------------Servo----------------
#include <Servo.h>
Servo myservo;
#define SERVO A1
int pos = 40;
int faceDoor = 125;
int faceWindow = 35;
//-----------varibles for time-----------
unsigned long startTime = 0;
unsigned long endTime = 0;
const long safeTime =  30000;
int disableSystem = 0;
//--------------------RGBLed------------
int bluePin = 11;//RED
int redPin = 13;//BLUE
int greenPin = 12;//GREEN
//--------------------SingleColorLed---------
int led = 10;
//-------------------the code send from Raspberry Pi--------
char action;

void setup() {
  //--------buzzer----------------------
  pinMode(forBuzzer, OUTPUT);
  //--------------------keypad---------------------
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (byte i = 0; i < rows; i++) //for loop used to make pin mode of outputs as output
  {
    pinMode(Output[i], OUTPUT);
  }
  for (byte s = 0; s < columns; s++) //for loop used to makk pin mode of inputs as inputpullup
  {
    pinMode(Input[s], INPUT_PULLUP);
  }

  //-----ultrasonic sensorDoor---------------
  pinMode(dTRIG_PIN, OUTPUT);
  pinMode(dECHO_PIN, INPUT);
  //-----ultrasonic sensorWindow---------------
  pinMode(wTRIG_PIN, OUTPUT);
  pinMode(wECHO_PIN, INPUT);
  //------------Temperature Sensor--------------
  pinMode(TEMP_PIN, INPUT);
  //--------------servo------------------
  myservo.attach(SERVO);
  //------------------------RGBLed-------------------
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  //------------SingleColorLed---------
  pinMode(led, OUTPUT);
  //---------------Setting the RGB to off----------
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, HIGH);
  digitalWrite(bluePin, HIGH);
}

void loop() {
  //read the command send from Raspberry Pi and change corresponding status
  if (Serial.available()) {
    action = Serial.read() ;
    switch (action) {
      case '0'://activate the system
        disableSystem = 0;
        opened = false;
        Serial.println("-100");
        break;

      case '1'://disable the system
        disableSystem = 1;
        opened = false;
        Serial.println("-200");
        break;

      case '2'://turn on the light
        digitalWrite(led, HIGH);
        Serial.println("-300");
        break;

      case '3'://turn off the light
        digitalWrite(led, LOW);
        Serial.println("-400");
        break;

      default:
        break;
    }
  }

  reading = 0;
  // to reduce noise, we take several sample readings and take their average
  for (int i = 0; i < 8; i++) {
    reading = reading + analogRead(TEMP_PIN);
    delay(30);
  }
  // get average of temperature samples
  reading = reading >> 3;

  // Calculate temperature from LM35
  float TemperatureC = reading * (3.3 / 1024.0) * 100;
  //calculate the effective speed of sound
  float speed = (331.5 + (0.6 * TemperatureC)) * 29 / 343;
  Serial.println(TemperatureC);

  // if the system is on, the door and windwo was not opened before, and the door is open, and window is closed now
  if (detectOpen(dTRIG_PIN, dECHO_PIN, dToWall) && !disableSystem && !detectOpen(wTRIG_PIN, wECHO_PIN, wToWall) && !opened) {
    opened = true;
    Serial.println("tripped");
    //check the password and turn the webcam to the door
    securityCheck(faceDoor);
  }
  //if the system is on, the window and door was not opened before, and window is open now
  else if (!disableSystem && detectOpen(wTRIG_PIN, wECHO_PIN, wToWall) && !opened) {
    opened = true;
    Serial.println("tripped");
    //check the password and turn the webcam to the window
    securityCheck(faceWindow);
  }
  //if the system is disabled then set the RGB to off, alarm to off, and put the webcam back to the original position
  else if (disableSystem) {
    myservo.write(pos);
    Serial.println("-200");
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, HIGH);
    digitalWrite(bluePin, HIGH);
    alarm(0);
  }
  //if window or door is open, and the alarm is on, start scan the room
  if (opened && !disableSystem)
    scan();
}

//check the input with the password
void securityCheck(int servoPos) {
  digitalWrite(led, HIGH);
  Serial.println("-300");
  digitalWrite(redPin, LOW);
  Serial.println("-100");
  digitalWrite(greenPin, LOW);
  startTime = millis();
  endTime = millis();
  sing(1);//remind the user
  sing(0);
  while (endTime - startTime <= safeTime) {//set certain time for the user to input password
    endTime = millis();
    myservo.write(servoPos);
    if (!disableSystem) {
      //if the input from the user is wrong
      if (!checkPassword()) {
        //set alarm on and turn on the red light
        digitalWrite(bluePin, HIGH);
        digitalWrite(greenPin, HIGH);
        digitalWrite(redPin, LOW);
        Serial.println("-100");
        alarm(1);
      }
      //if the input from the user is correct
      else {
        //disable the system and turn the green light on for two seconds
        disableSystem = 1;
        Serial.println("-200");
        digitalWrite(bluePin, HIGH);
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);
        delay(2000);
        digitalWrite(greenPin, HIGH);
        //turn off the alarm
        alarm(0);
      }
      //if there is still more time, set the alarm off and remind the user to have another attampt
      if (!disableSystem) {
        alarm(0);
        sing(1);
      }
      //if the user choose to disable the system during the safetime, it will set the alarm off and not remind the user
      else {
        alarm(0);
        sing(0);
      }
    }
  }
  //after the safe time, if the correct password was not entered, set the alarm on
  if (!disableSystem ) {
    alarm(1);
    myservo.write(pos);
  }
}
//reminder
void sing(int s) {
  // iterate over the notes of the melody:
  song = s;
  if (song == 1) {
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
      int noteDuration = 1000 / underworld_tempo[thisNote];
      buzz(forBuzzer, underworld_melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      buzz(forBuzzer, 0, noteDuration);
    }
  }
}

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  // 1 second's worth of microseconds, divided by the frequency, then split in half since
  // there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  // multiply frequency, which is really cycles per second, by the number of seconds to
  // get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);
}

//alarm which makes noise controlled by the parameter sent to it
void alarm(int a) {
  analogWrite(forBuzzer, LOW);
  if (a == 1) {
    tone(forBuzzer, 700);
    delay(3000);
  }
  else
    noTone(forBuzzer);
}

// function used to detect which button is pressed
byte keypad() {
  static bool no_press_flag = 0; //static flag used to ensure no button is pressed
  for (byte x = 0; x < columns; x++) // for loop used to read all inputs of keypad to ensure no button is pressed
  {
    if (digitalRead(Input[x]) == HIGH); //read evry input if high continue else break;
    else
      break;    if (x == (columns - 1)) //if no button is pressed

    {
      no_press_flag = 1;
      h = 0;
      v = 0;
    }
  }
  if (no_press_flag == 1) //if no button is pressed
  {
    for (byte r = 0; r < rows; r++) //for loop used to make all output as low
      digitalWrite(Output[r], LOW);
    for (h = 0; h < columns; h++) // for loop to check if one of inputs is low
    {
      if (digitalRead(Input[h]) == HIGH) //if specific input is remain high (no press on it) continue
        continue;
      else    //if one of inputs is low
      {
        for (v = 0; v < rows; v++) //for loop used to specify the number of row
        {
          digitalWrite(Output[v], HIGH);  //make specified output as HIGH
          if (digitalRead(Input[h]) == HIGH) //if the input that selected from first sor loop is change to high
          {
            no_press_flag = 0;              //reset the no press flag;
            for (byte w = 0; w < rows; w++) // make all outputs as low
              digitalWrite(Output[w], LOW);
            return v * 4 + h; //return number of button
          }
        }
      }
    }
  }
  return 50;
}

//read the input from the keypad and return it
int readKeypad() {
  delay(200);
  if (millis() - kdelay > period) //used to make non-blocking delay
  {
    byte key = keypad();  // key is set to be which button was pressed
    kdelay = millis();   //capture time from millis function

    // waiting until one of the buttons are pressed
    while (key == 50 && endTime - startTime <= safeTime) {
      key = keypad();
      endTime = millis();
    }

    switch (key)  //switch used to specify which button
    {
      // in case user pressed 1
      case 0:
        {
          g = 1;
          //          Serial.println(g);
        }
        break;
      // in case user pressed 2
      case 1:
        {
          g = 2;
          //          Serial.println(g);
        }
        break;

      // in case user pressed 3
      case 2:
        {
          g = 3;
          //          Serial.println(g);
        }
        break;

      // in case user pressed 4
      case 4:
        {
          g = 4;
          //          Serial.println(g);
        }
        break;
      // in case user pressed 5
      case 5:
        {
          g = 5;
          //          Serial.println(g);
        };
        break;

      // in case user pressed 6
      case 6:
        {
          g = 6;
          //          Serial.println(g);
        }
        break;

      // in case user pressed 7
      case 8:
        {
          g = 7;
          //          Serial.println(g);
        }
        break;

      // in case user pressed 8
      case 9:
        {
          g = 8;
          //          Serial.println(g);
        };
        break;
      // in case user pressed 9
      case 10:
        {
          g = 9;
          //          Serial.println(g);
        };
        break;
      case 12:
        {
          g = 12;
          //          Serial.println("Star");
        }
        break;
      // in case user pressed 0
      case 13:
        {
          g = 0;
          //          Serial.println(g);
        }
        break;
      // in case user pressed #
      case 14:
        {
          g = 14;
          //          Serial.println("numberSign");
        }
        break;

      default:
        ;
    }
  }
  return g;
}

//get the input from the user
void getInput() {
  int i = 0;
  while ( i < 5) {
    input[i] = readKeypad();
    i ++;
  }
}

//check if the input matches the password and return true or false
boolean checkPassword() {
  int count = 0;
  getInput();
  for (int i = 0; i < 5; i ++) {
    count ++;
    if (input[i] != password[i])
      return false;
    else if ( count == 5)
      return true;
  }
}

//----------------------detect if door or window is open---------------------
bool detectOpen(int trig, int echo, int toWall) {
  reading = 0;
  // to reduce noise, we take several sample readings and take their average
  for (int i = 0; i < 8; i++) {
    reading = reading + analogRead(TEMP_PIN);
    delay(30);
  }
  //initialize the trigger pin by doing a digitalWrite() to LOW for 50ms
  digitalWrite(trig, LOW);
  delay(50);
  //start the trigger sequence by performing a digitalWrite() of HIGH for 10ms
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  //set to LOW on the same pin
  digitalWrite(trig, LOW);

  // get average of temperature samples
  reading = reading >> 3;

  // Calculate temperature from LM35
  float TemperatureC = reading * (3.3 / 1024.0) * 100;
  //calculate the effective speed of sound
  float speed = (331.5 + (0.6 * TemperatureC)) * 29 / 343;
  //Serial.println(TemperatureC);
  //obtain the echo pulse duration (in microseconds)
  duration = pulseIn(echo, HIGH);
  //get a more accurate distance value using speed of sound
  distance = duration / (2 * speed);
  if (abs(distance - toWall) > 2 && distance > 0.00) {
    return true;
  }
  else {
    return false;
  }
}

//----------------Scan function for the servo and webcam------------
void scan() {
  for (pos = 0; pos <= 140; pos += 1) {
    delay(60);
    myservo.write(pos);
  }
  for (pos = 140; pos >= 0; pos -= 1) {
    delay(60);
    myservo.write(pos);
  }
}


