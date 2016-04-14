
int ledPin = 11;    // LED connected to digital pin 9
int analogPin = A0;

void setup() {
  pinMode(ledPin, OUTPUT); // nothing happens in setup
  pinMode(analogPin, INPUT);
}

void loop() {

  int val = analogRead(analogPin);   // read the input pin
  analogWrite(ledPin, val / 4);
  if (val <= 900)

  {
    // fade in from min to max in increments of 5 points:

    for (int fadeValue = 0 ; fadeValue <= 1000; fadeValue += 60) {
      // sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(200);
    }
  }


 if (val > 900) {
  analogWrite(ledPin, 0);
      // wait for 30 milliseconds to see the dimming effect
      delay(200);
    }
  
}


