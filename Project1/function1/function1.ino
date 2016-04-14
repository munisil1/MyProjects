// libraries
#include <Servo.h>
#include <LiquidCrystal.h>

// initializations
LiquidCrystal lcd(1, 8, 9, 10, 12, 13);
Servo myServo;

// pin declarations
// HC-SR04 pins
#define TRIG_PIN A3
#define ECHO_PIN A2
#define TEMP_PIN A1
// Servo pin
#define SERVO A0
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
#define SERVO_OFFSET -5 // offset used for servo to position the HC-SR04
#define MIN_TIME_DIFF 0.3347598729 // the minimum time in seconds for max speed of 61 cm/s

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
int adjust = 1; // adjust speed of wheels flag
int adjustR = 0; // used to adjust speed of right wheel
int adjustL = 0; // used to adjust speed of left wheel
volatile float currTimeL; // used in calculating speed of left wheel
volatile float lastTimeL = 0.0; // used in calculating speed of left wheel
volatile float currSpeedL = 0.0; // current speed of left wheel
volatile float currTimeR; // used in calculating speed of right wheel
volatile float lastTimeR = 0.0; // used in calculating speed of right wheel
volatile float currSpeedR = 0.0; // current speed of right wheel

void setup() {
  // pin setup for HC-SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
  // pin setup for motor
  pinMode(M1, OUTPUT); // right motor
  pinMode(M2, OUTPUT); // left motor

  // setup the LCD screen to a 2x16 (rows x cols) display
  lcd.begin(16, 2);

  // setup servo
  myServo.attach(SERVO);

  // setup interrupts for speed calculation on each wheel of the robot
  attachInterrupt(digitalPinToInterrupt(R_HALL_EFF), wheelSpeedR, FALLING);
  attachInterrupt(digitalPinToInterrupt(L_HALL_EFF), wheelSpeedL, FALLING);
}

void loop() {
  // calc distance
  distance = calcDist();

  // adjust speed to make robot go straight
  if (millis() > 1500) {
    calcSpeedAdjust();
  }

  // LCD print
  if (currSpeedL < currSpeedR) {
    speedOfRobot = currSpeedL;
  } else {
    speedOfRobot = currSpeedR;
  }

  if (distance <= 15) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Determining ");
    lcd.setCursor(0, 1);
    lcd.print("direction.");

    stopped = 1;
    analogSpeedOfRobot = 0;
  }

  // speed control logic when approaching an object
  if (!stopped) {
    analogSpeedOfRobot = map(constrain(distance, 15, 50), 15, 50, 110, 255);
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

  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, analogSpeedOfRobot + adjustR); // PWM control with speed adjustments
  analogWrite(E2, analogSpeedOfRobot + adjustL); // PWM control with speed adjustments

  // scan logic for next direction i.e left or right if stopped == 1
  if (stopped) {
    nxtDirection = getDirection();
    if (nxtDirection == LEFT) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Turning left.");
      turnLeft();
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Turning right.");
      turnRight();
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

  if (stopped == 1 && servoFlag == 0) {
    // rotate servo right
    for (pos = 90 - SERVO_OFFSET; pos >= 0; pos -= 1) {
      myServo.write(pos);
    }
    maxDistR = calcDist(); // get distance on the right
    delay(1500); // wait for 1.5 seconds

    // rotate servo to left
    for (pos = 0; pos <= 180 - SERVO_OFFSET; pos += 1) {
      myServo.write(pos);
    }
    maxDistL = calcDist(); // get distance on left
    delay(1500); // wait for 1.5 seconds

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
  digitalWrite(M2, HIGH);
  // enable only the right wheel
  analogWrite(E1, 255);
  analogWrite(E2, 0);
  delay(1400); // TODO: change delay when robot is fully functional
  // disable both wheels
  analogWrite(E1, 0);
  analogWrite(E2, 0);
  delay(100);
  stopped = 0;
  servoFlag = 0;
  lcd.clear();
}

void turnRight() {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  // enable only the left wheel
  analogWrite(E1, 0);
  analogWrite(E2, 255);
  delay(1400); // TODO: change delay when robot is fully functional
  // disable both wheels
  analogWrite(E1, 0);
  analogWrite(E2, 0);
  delay(100);
  stopped = 0;
  servoFlag = 0;
  lcd.clear();
}

void wheelSpeedL() {
  detachInterrupt(digitalPinToInterrupt(L_HALL_EFF)); // mask pin 2 to prevent unwanted interrupts
  currTimeL = millis();
  float timeDiffL = (currTimeL - lastTimeL) / 1000;
  //Serial.print("timeL(s)");
  //Serial.println(timeDiff);

  if (timeDiffL < MIN_TIME_DIFF) {
    lastTimeL = currTimeL;
    attachInterrupt(digitalPinToInterrupt(L_HALL_EFF), wheelSpeedL, FALLING);
    return;
  }

  lastTimeL = currTimeL;
  currSpeedL = PI * WHEEL_DIAM / timeDiffL;
  //  Serial.print("speedL(cm/s)");
  //  Serial.println(currSpeedL);
  attachInterrupt(digitalPinToInterrupt(L_HALL_EFF), wheelSpeedL, FALLING); // re-enable for future interrupts
}

void wheelSpeedR() {
  detachInterrupt(digitalPinToInterrupt(R_HALL_EFF)); // mask pin 3 to prevent unwanted interrupts
  currTimeR = millis();
  float timeDiffR = (currTimeR - lastTimeR) / 1000;
  //  Serial.print("timeR(s)");
  //  Serial.println(timedifR);


  if (timeDiffR < MIN_TIME_DIFF) {
    lastTimeR = currTimeR;
    attachInterrupt(digitalPinToInterrupt(R_HALL_EFF), wheelSpeedR, FALLING);
    return;
  }

  lastTimeR = currTimeR;
  currSpeedR = PI * WHEEL_DIAM / timeDiffR;
  //  Serial.print("speedR(cm/s)");
  //  Serial.println(curspeedR);
  attachInterrupt(digitalPinToInterrupt(R_HALL_EFF), wheelSpeedR, FALLING); // re-enable for future interrupts
}

void calcSpeedAdjust() {
  if (abs(currSpeedL - currSpeedR) > 0.5 && adjust) {
    if (currSpeedL < currSpeedR) {
      adjustR -= 1;
    }
    else {
      adjustL -= 1;
    }
  } else {
    adjust = 0; // disable adjustments when wheels are same speed
  }
}
