const int buttonPin = 3;     // the number of the pushbutton pin
const int signalPin =  A0;      // the number of the signal pin
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
const long interval = 20000; // the to run after interrupt
// flag for interrupt to avoid run automatically after power on
boolean interFlag = false;

void setup() {
  Serial.begin(3200);
  pinMode(signalPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // initialize the build-in led as a flag for us to see when interrupt happpens
  pinMode(13, OUTPUT);
  // jump to the isr() when the button changes
  attachInterrupt(digitalPinToInterrupt(3), isr, CHANGE);
}

void loop() {
  // read currentMillis to calculate time has passed
  currentMillis = millis();
  // if the time passed is smaller than the interval and we are in interrupt
  //then read data from analog pin and light up the led
  if ( interFlag && currentMillis - previousMillis <= interval) {
    float raw = analogRead(signalPin);
    float val = raw * 5.0 / 1023.0;
    Serial.println(val);
    digitalWrite(13, HIGH);
  }
  // otherwise, don't read and set the led to low
  else
    digitalWrite(13, LOW);
}
// set the interrupt flag as true and set previousMillis
void isr() {
  interFlag = true;
  previousMillis = millis();
}

