
const int switch2 = 8;
const int switch3 = 9;

// variables will change:
int switchState2 = 0;
int switchState3 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(switch2, INPUT); // FOR SWITCH 2
  pinMode(switch3, INPUT); // FOR SWITCH 3
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);


}

void loop() {

  switchState2 = digitalRead(switch2);
  switchState3 = digitalRead(switch3);
  delay(200);
  //OFF
  if (switchState2 == LOW && switchState3 == LOW)
  {
    digitalWrite(4, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
  delay(200);
  //RGB
  if (switchState2 == HIGH && switchState3 == HIGH) {
    digitalWrite(4, HIGH);   // turn the RED LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(4, LOW);    // turn the RED LED off by making the voltage LOW
    delay(1000);              // wait for a second

    digitalWrite(6, HIGH);   // turn the GREEN LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(6, LOW);    // turn the GREEN LED off by making the voltage LOW
    delay(1000);              // wait for a second

    digitalWrite(7, HIGH);   // turn the BLUE LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(7, LOW);    // turn the BLUE LED off by making the voltage LOW
    delay(1000);              // wait for a second

  }
  delay(200);
  //BRG
  if (switchState2 == LOW && switchState3 == HIGH) {
    digitalWrite(7, HIGH);   // turn the RED LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(7, LOW);    // turn the RED LED off by making the voltage LOW
    delay(1000);              // wait for a second

    digitalWrite(4, HIGH);   // turn the GREEN LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(4, LOW);    // turn the GREEN LED off by making the voltage LOW
    delay(1000);              // wait for a second

    digitalWrite(6, HIGH);   // turn the BLUE LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(6, LOW);    // turn the BLUE LED off by making the voltage LOW
    delay(1000);              // wait for a second

  }
  delay(200);
  //GBR
  if (switchState2 == HIGH && switchState3 == LOW)
  {
    digitalWrite(6, HIGH);   // turn the RED LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(6, LOW);    // turn the RED LED off by making the voltage LOW
    delay(1000);              // wait for a second

    digitalWrite(7, HIGH);   // turn the GREEN LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(7, LOW);    // turn the GREEN LED off by making the voltage LOW
    delay(1000);              // wait for a second

    digitalWrite(4, HIGH);   // turn the BLUE LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(4, LOW);    // turn the BLUE LED off by making the voltage LOW
    delay(1000);              // wait for a second

  }
  delay(200);
}

