




const int switch2 = 8;
const int switch3 = 9;

int ledPin = 11;    // LED connected to digital pin 9
int analogPin = A0;
int switchState2 = 0;
int switchState3 = 0;
int LIGHTControlledLED =0;
int LEDsB = 0;
int LEDsR = 0;
int LEDsG = 0;
int sensorValue = 0;
void setup() {
  Serial.begin(9600);
  pinMode(switch2, INPUT); // FOR SWITCH 2
  pinMode(switch3, INPUT); // FOR SWITCH 3
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(ledPin, OUTPUT); 
  pinMode(analogPin, INPUT);
}

void loop() {
  switchState2 = digitalRead(switch2);
  switchState3 = digitalRead(switch3);
  LIGHTControlledLED = digitalRead(ledPin);
  LEDsB = digitalRead(4);
  LEDsR = digitalRead(6);
  LEDsG = digitalRead(7);
  sensorValue = digitalRead(analogPin); 
 if (switchState2 == 0 && switchState3 == 1) 
 Serial.println("\rSWITCH:OFF ON" );

 if (switchState2 == 1 && switchState3 == 0) 
 Serial.println("\rSWITCH:ON OFF" );
 
 /* if (switchState2 == 1) 
 Serial.println("SWITCH2:ON" ); 
  delay(1);
 if (switchState3 == 1)
 Serial.println("SWITCH3:ON");
  if (switchState3 == 0)
 Serial.println("SWITCH3:OFF");
 delay(1);
*/

  Serial.println(LIGHTControlledLED);
 delay(1);
  Serial.println(LEDsB);
 delay(1);
  Serial.println(LEDsR);
 delay(1);
  Serial.println(LEDsG);
 delay(1);


 int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
  
 
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



