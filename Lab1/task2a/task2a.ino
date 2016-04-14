void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
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

