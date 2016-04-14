// libraries
#include <Servo.h>
#include <LiquidCrystal.h>
#include <IRremote.h>

// initializations
LiquidCrystal lcd(1, 8, 9, 10, 12, 13); // CHECK BEFORE USE
Servo myServo;

//remote setting
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
//remote commands
#define POWER 0x10EFD827
#define A 0x10EFF807
#define B 0x10EF7887
#define C 0x10EF58A7
#define UP 0x10EFA05F
#define DOWN 0x10EF00FF
#define Left 0x10EF10EF
#define RIGHT 0x10EF807F
#define STOP 0x10EF20DF
#define FAST 220
#define MEDIAN 180
#define SLOW 120

// pin declarations
// HC-SR04 pins
#define TRIG_PIN A5
#define ECHO_PIN A4
#define TEMP_PIN A3
// Servo pin
#define SERVO 0
// motor pins
#define E1 5 // pin used for right motor
#define M1 4 // pin used for right motor
#define E2 6 // pin used for left motor
#define M2 7 // pin used for left motor
// hall-effect sensor pins
#define L_HALL_EFF 2 // pin used for hall effect sensor for left wheel
#define R_HALL_EFF 3 // pin used for hall effect sensor for the right wheel

// constants
#define SAMPLE_SIZE 8 // used in drowing noise from LM35
#define WHEEL_DIAM 6.5 // measured in cm
#define LEFT 1 // used in identifying next direction the robot will turn
#define SERVO_OFFSET 0 // offset used for servo to position the HC-SR04
#define MIN_TIME_DIFF 0.3347598729 // the minimum time in seconds for max speed of 61 cm/s
#define BASE_SPEED 220
#define MAX_SPEED 255

// global variables declarations
float duration = 0.0; // duration of pulse from HC-SR04
float distance = 0.0; // the calculated distance of an object relative to robot
int tempReading = 0; // temperature detected by LM35; used in improving distance calculations
int analogSpeedOfRobot = 0; // the analog equivalent of the robot's speed
float speedOfRobot = 0.0; // the actual speed of the robot in cm/s
int stopped = 0; // a flag used in identifying if the robot has due to an object in front of it
int servoFlag = 0; // a flag used in preventing the servo from scanning while it is trying to compute the next direction
float maxDistR = 0.0; // the maximum distance found from scanning to the right
float maxDistL = 0.0; // the maximum distance found from scanning to the left
int nxtDirection = 0; // the next direction that the robot should take; 1 for left and 2 for right
//int adjust = 1; // adjust speed of wheels flag
int adjustR = 0; // used to adjust speed of right wheel
int adjustL = 0; // used to adjust speed of left wheel
volatile float currTimeL; // used in calculating speed of left wheel
volatile float lastTimeL; // used in calculating speed of left wheel
volatile float currSpeedL; // current speed of left wheel
volatile float currTimeR; // used in calculating speed of right wheel
volatile float lastTimeR; // used in calculating speed of right wheel
volatile float currSpeedR; // current speed of right wheel
volatile int ignoreFirstR = 0; // flag to ignore first ISR of right wheel
volatile int ignoreFirstL = 0; // flag to ignore first ISR of left wheel

uint16_t lastCode = 0; // This keeps track of the last code RX'd

int A_flag = 0;
int B_flag = 0;
int C_flag = 0;
volatile int speed_flag = 0;
int speedLevel[3] = {SLOW, MEDIAN, FAST};
int index = 1000;
volatile int stop_flag = 1;
#define sV 180
#define tV 255


void setup()
{
  // pin setup for HC-SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
  // pin setup for motor
  pinMode(M1, OUTPUT); // right motor
  pinMode(M2, OUTPUT); // left motor

  //Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver

  // setup the LCD screen to a 2x16 (rows x cols) display
  lcd.begin(16, 2); //lcd setup

  // setup servo
  myServo.attach(SERVO);

  // setup interrupts for speed calculation on each wheel of the robot
  attachInterrupt(digitalPinToInterrupt(R_HALL_EFF), wheelSpeedR, FALLING);
  attachInterrupt(digitalPinToInterrupt(L_HALL_EFF), wheelSpeedL, FALLING);
}

void loop() {
  if (irrecv.decode(&results))
  {
    /* read the RX'd IR into a 16-bit variable: */
    uint16_t resultCode = (results.value & 0xFFFF);

    /* The remote will continue to spit out 0xFFFFFFFF if a
      button is held down. If we get 0xFFFFFFF, let's just
      assume the previously pressed button is being held down */
    if (resultCode == 0xFFFF)
      resultCode = lastCode;
    else
      lastCode = resultCode;

    switch (resultCode)
    {
      case POWER:
        //Serial.println("Power");
        //Serial.println("Please stop");
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        A_flag = 0;
        B_flag = 0;
        C_flag = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Choose a Function ");
        break;
      case A:  //FOR FUNCTION ONE
        //Serial.println("A");
        A_flag = 1;
        B_flag = 0;
        C_flag = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function A");
        break;
      case B:    //FOR THE SECOND FUNCTION
        //Serial.println("B");
        A_flag = 0;
        B_flag = 1;
        C_flag = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function B");
        break;
      case C:
        //Serial.println("C");  //STOP IT AND MAKE THE USER CONTROL IT
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        A_flag = 0;
        B_flag = 0;
        C_flag = 1;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function C");
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Choose a Function ");
        break;
    }
    irrecv.resume();
  }


  //-------------reset variables in functionA ---------------------
  distance = 0.0;
  analogSpeedOfRobot = 0; // the analog equivalent of the robot's speed
  speedOfRobot = 0.0; // the actual speed of the robot in cm/s
  stopped = 0; // a flag used in identifying if the robot has due to an object in front of it
  servoFlag = 0; // a flag used in preventing the servo from scanning while it is trying to compute the next direction
  maxDistR = 0.0; // the maximum distance found from scanning to the right
  maxDistL = 0.0; // the maximum distance found from scanning to the left
  nxtDirection = 0; // the next direction that the robot should take; 1 for left and 2 for right
  adjustR = 0; // used to adjust speed of right wheel
  adjustL = 0; // used to adjust speed of left wheel
  ignoreFirstR = 0; // flag to ignore first ISR of right wheel
  ignoreFirstL = 0; // flag to ignore first ISR of left wheel
  //----------------------------------------------------------------
  while (A_flag == 1) {
    func_A();
  }
  while (B_flag == 1) {
    func_B();
  }
  while (C_flag == 1) {
    func_C();
  }
}

void func_A() {
  if (irrecv.decode(&results))
  {
    switch (results.value)
    {
      case POWER:
        //Serial.println("Power");
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        A_flag = 0;
        B_flag = 0;
        C_flag = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Choose a Function ");
        return;
        break;
      case A:  //FOR FUNCTION ONE
        //Serial.println("A");
        A_flag = 1;
        B_flag = 0;
        C_flag = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function A");
        break;
      case B:
        //Serial.println("B");
        A_flag = 0;
        B_flag = 1;
        C_flag = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function B");
        return;
        break;
      case C:
        //Serial.println("C");
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        A_flag = 0;
        B_flag = 0;
        C_flag = 1;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function C");
        return;
        break;
      default:
        A_flag = 1;
        break;
    }
    irrecv.resume();
  }

  ------------------------------------------------
  // calc distance
  distance = calcDist();

  // LCD print
  if (currSpeedL < currSpeedR) {
    speedOfRobot = currSpeedL;
  } else {
    speedOfRobot = currSpeedR;
  }

  if (distance <= 10 && !stopped ) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Determining ");
    lcd.setCursor(0, 1);
    lcd.print("direction.");
    stopped = 1;
    stopWheels();
  }

  // speed control logic when approaching an object
  if (!stopped) {
    //120 for old battery
    analogSpeedOfRobot = map(constrain(distance, 10, 50), 10, 50, 120 , BASE_SPEED); // NOTE: may need to change mapping when switching over to batteries
    if (distance > 50) {
      digitalWrite(M1, HIGH);
      analogWrite(E1, analogSpeedOfRobot + adjustR);
      digitalWrite(M2, HIGH);
      analogWrite(E2, ((analogSpeedOfRobot + adjustL) * 115) / 100);
    }
    else if (distance <= 50) {
      digitalWrite(M1, HIGH);
      analogWrite(E1, (analogSpeedOfRobot + adjustR) * 0.75);
      digitalWrite(M2, HIGH);
      analogWrite(E2, analogSpeedOfRobot + adjustL);
    }
    lcd.setCursor(0, 0);
    lcd.print("Speed(cm/s):");
    lcd.setCursor(12, 0);
    lcd.print(speedOfRobot);
    lcd.setCursor(0, 1);
    lcd.print("Dist(cm):");
    lcd.setCursor(9, 1);
    lcd.print(distance);
    lcd.setCursor(14, 1);
    lcd.print("  ");
  }

  // scan logic for next direction i.e left or right if stopped == 1
  if (stopped) {
    nxtDirection = getDirection();
    if (nxtDirection == LEFT) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Turning left.");
      turnLeft();
      lcd.clear();
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Turning right.");
      turnRight();
      lcd.clear();
    }
  }
}

void func_B() {
  if (irrecv.decode(&results))
  {
    switch (results.value)
    {
      case POWER:
        //pressing the power to enable the  user to choose a function
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        A_flag = 0;
        B_flag = 0;
        C_flag = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Choose a Function ");
        return;
        break;

      //just in case if the user wants to switch back to function A
      case A:  //FOR FUNCTION ONE
        A_flag = 1;
        B_flag = 0;
        C_flag = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function A");
        return;
        break;

      case B:
        A_flag = 0;
        B_flag = 1; // enable flag for function B
        C_flag = 0;
        lcd.clear();
        lcd.setCursor(0, 0); //set the cursor of the LCD so that it prints Function B from the beginning
        lcd.print("Function B");
        break;

      case C:
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        A_flag = 0;
        B_flag = 0;
        C_flag = 1;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function C");
        return;
        break;
      default:
        B_flag = 1;
        break;
    }
    irrecv.resume();
  }
  //-------------------------- setting variables for the function 2 ---------------------
  int R = analogRead(A2) ;  // the right sensor
  int L = analogRead(A0) ;  // the left sensor
  int Mid = analogRead(A1) ;// the middle sensor
  int T1 = abs(Mid - R);    // difference between the right and the middle sensors
  int T2 = abs(Mid - L);    // difference between the left and the middle sensors
  int T = abs(T1 - T2);     //difference between T1 and T2
  //----------------------- end of setting variables ----------------------------------


  if (Mid > 180) { //  check if the middle sensor on the black track
    //go straight if the middle sensor on black and the other sensors on white
    if (T <= 180) { //check if the other two sensors on the white
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      go(sV, sV);
    }
    // T > 180 consider the R and L sensors are on different colors
    else {
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      // turn left if the difference between the middle and the right sensor is higher than the left and middle
      if (T1 > T2) {
        // keep turning until the middle one is back on black
        if (Mid < 100)
          go(tV, 0);
        else
          go(0, 0);
      }
      else {
        if (Mid < 100)
          go(0, tV);
        else
          go(0, 0);
      }
    }
  }

  else {
    //the middle sensor is NOT on the black line, kee moving straight
    if (T <= 200) {
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      go(sV, sV);
    }
    // T > 180 consider the R and L sensors are on different colors
    else {
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      // turn right if the difference between the middle and the left sensor is higher than the right and middle
      if (T2 > T1) {

        if (Mid < 100)
          go(tV, 0);
        else
          go(0, 0);
      }
      else {
        if (Mid < 100)
          go(0, tV);
        else
          go(0, 0);
      }
    }
  }
}

void func_C() {

  if (irrecv.decode(&results))
  {
    /* read the RX'd IR into a 16-bit variable: */
    uint16_t resultCode = (results.value & 0xFFFF);

    /* The remote will continue to spit out 0xFFFFFFFF if a
      button is held down. If we get 0xFFFFFFF, let's just
      assume the previously pressed button is being held down */
//    if (resultCode == 0xFFFF)
//      resultCode = lastCode;
//    else
//      lastCode = resultCode;

    switch (resultCode)
    {
      case POWER:
        //Serial.println("Power");
        //Serial.println("Please stop");
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        A_flag = 0;
        B_flag = 0;
        C_flag = 0;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Choose a Function ");
        break;

      case A:  //FOR FUNCTION ONE
        //Serial.println("A");
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        A_flag = 1;
        B_flag = 0;
        C_flag = 0;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function A");
        break;

      case B:    //FOR THE SECOND FUNCTION
        //Serial.println("B");
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        A_flag = 0;
        B_flag = 1;
        C_flag = 0;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function B");
        //functiontwo();
        break;

      case C:
        //Serial.println("C");  //STOP IT AND MAKE THE USER CONTROL IT
        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH);
        A_flag = 0;
        B_flag = 0;
        C_flag = 1;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Function C");
        break;

      case UP:   //MOVE IT FOWARD

        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH);
        analogWrite(E1, speedLevel[index]);
        analogWrite(E2, speedLevel[index]);
        C_flag = 1;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Go straight");
        break;

      case DOWN:  //REVERSE IT

        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
        analogWrite(E1, speedLevel[index]);
        analogWrite(E2, speedLevel[index]);
        C_flag = 1;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Go Back");
        break;

      case RIGHT: //TURN IT RIGHT
        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH);
        analogWrite(E1, 0);
        analogWrite(E2, 255);
        C_flag = 1;
        stop_flag = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Turning Right");
        break;

      case Left:  //TURN IT LEFT
        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH);
        analogWrite(E1, 255);
        analogWrite(E2, 0);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Turning Left");
        C_flag = 1;
        stop_flag = 1;
        break;

      case STOP:
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Stop");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("choose the");
        lcd.setCursor(0, 1);
        lcd.print("speed level");
        speed_flag = 1;
        
       
         switchSpeed();
        
        break;

      default:
        C_flag = 1;
        break;
    }

    irrecv.resume();
  }
}



void switchSpeed() {
  while (speed_flag) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("1");
    delay(1000);
    lcd.clear();
    if (irrecv.decode(&results))
    {
      /* read the RX'd IR into a 16-bit variable: */
      uint16_t resultCode = (results.value & 0xFFFF);
      switch (resultCode)
      {
        case POWER:
          analogWrite(E1, 0);
          analogWrite(E2, 0);
          A_flag = 0;
          B_flag = 0;
          C_flag = 0;
          speed_flag = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Choose a Function ");
          break;

        case UP:
          A_flag = 0;
          B_flag = 0;
          C_flag = 1;
          speed_flag = 1;
          stop_flag = 1;
          index = (index + 1) % 3;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(speedLevel[index]);
          break;

        case DOWN:
          A_flag = 0;
          B_flag = 0;
          C_flag = 1;
          speed_flag = 1;
          stop_flag = 1;
          index = (index - 1) % 3;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(speedLevel[index]);
          break;

        case STOP:
        if(stop_flag=0){
          A_flag = 0;
          B_flag = 0;
          C_flag = 1;
          speed_flag = 0;
          stop_flag =1;
        }
          break;


        default:
          speed_flag = 1;
          stop_flag = 1;
          C_flag = 1;
          break;
      }

      irrecv.resume();
    }

  }
  
}

float calcDist() {
  // to reduce noise, we take several sample readings and take their average
  tempReading = 0;
  for (int i = 0; i < SAMPLE_SIZE; i++) {
    tempReading = tempReading + analogRead(TEMP_PIN);
    delay(30);
  }

  // get average of temperature samples
  tempReading = tempReading >> 3; // shift right by 3 to divide by 8 (our sample size)

  // initialize the trigger pin by doing a digitalWrite() to LOW for 50ms
  digitalWrite(TRIG_PIN, LOW);
  delay(50);
  // start the trigger sequence by performing a digitalWrite() of HIGH for 10ms
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  // set to LOW on the same pin
  digitalWrite(TRIG_PIN, LOW);

  // calculate temperature from LM35
  float temperatureC = tempReading * (5.0 / 1024.0) * 100;
  // calculate the effective speed of sound
  float speedOfSound = (331.5 + (0.6 * temperatureC)) * 29 / 343;
  // obtain the echo pulse duration (in microseconds)
  duration = pulseIn(ECHO_PIN, HIGH);
  // get a more accurate distance calcSpeed using speedOfSound of sound
  distance = duration / (2 * speedOfSound);

  return distance;
}

int getDirection() {
  int pos;
  stopWheels();
  if (stopped == 1 && servoFlag == 0) {
    // rotate servo right
    for (pos = 90 - SERVO_OFFSET; pos >= 0; pos -= 1) {
      myServo.write(pos);
    }
    maxDistR = calcDist(); // get distance on the right
    delay(1000); // wait for 1.5 seconds

    // rotate servo to left
    for (pos = 0; pos <= 180 - SERVO_OFFSET; pos += 1) {
      myServo.write(pos);
    }
    maxDistL = calcDist(); // get distance on left
    delay(1000); // wait for 1.5 seconds

    // rotate servo to middle
    for (pos = 180 - SERVO_OFFSET; pos >= 90 - SERVO_OFFSET; pos -= 1) {
      myServo.write(pos);
    }
  }

  if (maxDistR <= maxDistL) {
    return 1; // turn LEFT
  }
  else {
    return 2; // turn RIGHT
  }

  servoFlag = 1;
}

void turnLeft() {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  // enable only the right wheel
  analogWrite(E1, 255);
  analogWrite(E2, 255);
  delay(580); // TODO: change delay when robot is fully functional
  // disable both wheels
  analogWrite(E1, 0);
  analogWrite(E2, 0);
  delay(100);
  adjustL = 0;
  adjustR = 0;
  stopped = 0;
  servoFlag = 0;
  lcd.clear();
}

void turnRight() {
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  // enable only the left wheel
  analogWrite(E1, 255);
  analogWrite(E2, 255);
  delay(550);
  // disable both wheels
  analogWrite(E1, 0);
  analogWrite(E2, 0);
  delay(100);
  adjustL = 0; // added
  adjustR = 0; // added
  stopped = 0;
  servoFlag = 0;
  lcd.clear();
}

void wheelSpeedL()
{
  if (ignoreFirstL < 1) {
    lastTimeL = millis();
    ignoreFirstL++;
    return;
  }

  currTimeL = millis();
  float timeDiff = (currTimeL - lastTimeL) / 1000.0;

  if (timeDiff < MIN_TIME_DIFF) {
    lastTimeL = currTimeL;
    //attachInterrupt(0, wheelSpeedL, FALLING);
    //attachInterrupt(1, wheelSpeedR, FALLING);
    return;
  }

  lastTimeL = currTimeL;
  currSpeedL = PI * WHEEL_DIAM / timeDiff;

  if (ignoreFirstR) {
    calcSpeedAdjust();
  }

  //Serial.print("speedL(cm/s)");
  //Serial.println(currSpeedL);
}

void wheelSpeedR()
{
  if (ignoreFirstR < 1) {
    lastTimeR = millis();
    ignoreFirstR++;
    return;
  }

  currTimeR = millis();
  float timeDiff = (currTimeR - lastTimeR) / 1000.0;

  if (timeDiff < MIN_TIME_DIFF) {
    lastTimeR = currTimeR;
    //attachInterrupt(1, wheelSpeedR, FALLING);
    //attachInterrupt(0, wheelSpeedL, FALLING);
    return;
  }

  lastTimeR = currTimeR;
  currSpeedR = PI * WHEEL_DIAM / timeDiff;

  if (ignoreFirstL) {
    calcSpeedAdjust();
  }

  //Serial.print("speedR(cm/s)");
  //Serial.println(currSpeedR);
}

void calcSpeedAdjust() {
  if (abs(currSpeedL - currSpeedR) > 0.5) {
    if (currSpeedR > currSpeedL && (((analogSpeedOfRobot + adjustL + 1) * 115) / 100) < MAX_SPEED && analogSpeedOfRobot + adjustR + 1 < MAX_SPEED) {
      adjustL += 1;
    } else if (currSpeedL > currSpeedR && (((analogSpeedOfRobot + adjustL + 1) * 115) / 100) < MAX_SPEED && analogSpeedOfRobot + adjustR + 1 < MAX_SPEED) {
      adjustR += 1;
    }
  } else {
    //Serial.println("REACHED SAME SPEED");
    return;
  }
}

void stopWheels() {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, 0); // PWM control with speed adjustments
  analogWrite(E2, 0); // PWM control with speed adjustments
}

void go(int v1, int v2) {
  analogWrite(E1, v1); // PWM control
  analogWrite(E2, v2); // PWM control
}

