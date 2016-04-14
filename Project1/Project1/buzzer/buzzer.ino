#define E1 5 // pin used for right motor
#define M1 4 // pin used for right motor
#define E2 6 // pin used for left motor
#define M2 7 // pin used for left motor
int dotLength = 200; // getting dotLength for morse code
int dashLength = dotLength * 3; // getting dashLength for morse code

void setup()  {
  // declare pin 10 to be an output:
  pinMode(10, OUTPUT);
  pinMode(M1, OUTPUT); // right motor
  pinMode(M2, OUTPUT); // left motor
 beep(50);
  beep(50);
//  beep(50);
  delay(1000);
}

void loop()  {
dash();
}

void dash() {
  analogWrite(10, LOW);
  tone(10, 700);
  delay(dotLength * 3);
  analogWrite(10, HIGH);
}

void beep(unsigned char delayms) {
  analogWrite(10, 70);      // Almost any value can be used except 0 and 255
  // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(10, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms
}

void startWheels() {
#define E1 5 // pin used for right motor
#define M1 4 // pin used for right motor
#define E2 6 // pin used for left motor
#define M2 7 // pin used for left motor
  beep(200);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, 100); // PWM control with speed adjustments
  analogWrite(E2, 100); // PWM control with speed adjustments
  delay(300000);
}
