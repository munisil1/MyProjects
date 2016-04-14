1.)
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                  // wait for a second
  digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW
  delay(2000);                  // wait for a second
}
 
2a.)
void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(4, HIGH);   // turn the BLUE LED on (HIGH is the voltage level)
  delay(1000);                  // wait for a second
  digitalWrite(4, LOW);     // turn the BLUE LED off by making the voltage LOW
  delay(1000);                  // wait for a second
 
              digitalWrite(6, HIGH);   // turn the RED LED on (HIGH is the voltage level)
  delay(1000);                  // wait for a second
  digitalWrite(6, LOW);     // turn the RED LED off by making the voltage LOW
  delay(1000);                  // wait for a second
 
              digitalWrite(7, HIGH);   // turn the GREEN LED on (HIGH is the voltage level)
  delay(1000);                  // wait for a second
  digitalWrite(7, LOW);     // turn the GREEN LED off by making the voltage LOW
  delay(1000);                  // wait for a second
}

2b.)
//initialize the switches to the pin values
const int switch2 = 8;
const int switch3 = 9;
 
// variables will change:
int switchState2 = 0;
int switchState3 = 0;
 
void setup() {
  // put your setup code here, to run once:
  pinMode(switch2, INPUT); // FOR SWITCH 2
  pinMode(switch3, INPUT); // FOR SWITCH 3
  pinMode(4, OUTPUT);     // FOR THE BLUE PIN OF LED
  pinMode(6, OUTPUT);                 // FOR RED PIN OF LED
  pinMode(7, OUTPUT);               // FOR GREEN PIN OF LED
 
}
 
void loop() {
 
  switchState2 = digitalRead(switch2);
  switchState3 = digitalRead(switch3);
  delay(200);       //a delay of 200ms for each color change in the LED
  //OFF
  if (switchState2 == LOW && switchState3 == LOW)
  {
              digitalWrite(4, LOW);
              digitalWrite(6, LOW);
              digitalWrite(7, LOW);
  }
  delay(200);
 
 
  //BRG
  if (switchState2 == HIGH && switchState3 == HIGH) {
              digitalWrite(4, HIGH);   // turn the BLUE LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(4, LOW);       // turn the BLUE LED off by making the voltage LOW
              delay(1000);                    // wait for a second
               
              digitalWrite(6, HIGH);   // turn the RED LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(6, LOW);       // turn the RED LED off by making the voltage LOW
              delay(1000);                    // wait for a second
 
              digitalWrite(7, HIGH);   // turn the GREEN LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(7, LOW);       // turn the GREEN LED off by making the voltage LOW
              delay(1000);                    // wait for a second
  
  }
  delay(200);
 
 
  //GBR
  if (switchState2 == LOW && switchState3 == HIGH) {
              digitalWrite(7, HIGH);   // turn the GREEN LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(7, LOW);       // turn the GREENED off by making the voltage LOW
              delay(1000);                    // wait for a second
 
              digitalWrite(4, HIGH);   // turn the BLUE LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(4, LOW);       // turn the BLUE LED off by making the voltage LOW
              delay(1000);                    // wait for a second
 
              digitalWrite(6, HIGH);   // turn the RED LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(6, LOW);       // turn the RED LED off by making the voltage LOW
              delay(1000);                    // wait for a second
   Serial.println(BRG);
  }
  delay(200);
 
 
  //RGB
  if (switchState2 == HIGH && switchState3 == LOW)
  {
              digitalWrite(6, HIGH);   // turn the RED LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(6, LOW);       // turn the RED LED off by making the voltage LOW
              delay(1000);                    // wait for a second
 
              digitalWrite(7, HIGH);   // turn the GREEN LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(7, LOW);       // turn the GREEN LED off by making the voltage LOW
              delay(1000);                    // wait for a second
 
              digitalWrite(4, HIGH);   // turn the BLUE LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(4, LOW);       // turn the BLUE LED off by making the voltage LOW
              delay(1000);                    // wait for a second
 
  }
  delay(200);
}
 
2c.)  

  
int ledPin = 11;    // LED connected to digital pin 11
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
    // fade in from min to max in increments of 60 points:

    for (int fadeValue = 0 ; fadeValue <= 1000; fadeValue += 60) {
      // sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      // wait for 2 milliseconds to see the dimming effect
      delay(200);
    }
  }


 if (val > 900) {
  analogWrite(ledPin, 0);
      // wait for 2 milliseconds to see the dimming effect
      delay(200);
    }
  
}





2d.)
const int switch2 = 8;  //switch2 connected to digital pin 8
const int switch3 = 9;  //switch3 connected to digital pin 9
 
int ledPin = 11;     // LED connected to digital pin 11
int analogPin = A0;           //analogPin connected to the pin A0
 
//initialize the variables to 0
int switchState2 = 0;  
int switchState3 = 0;
int LIGHTControlledLED =0;
int LEDsB = 0;
int LEDsR = 0;
int LEDsG = 0;
int sensorValue = 0;
int RGB =0;
int BRG =0;
int GBR =0;
 
//the code that will run once
void setup() {
  Serial.begin(9600);
  pinMode(switch2, INPUT); // FOR SWITCH 2
  pinMode(switch3, INPUT); // FOR SWITCH 3
  pinMode(4, OUTPUT);     //FOR LEDsB
  pinMode(6, OUTPUT);   // FOR LEDsR
  pinMode(7, OUTPUT);   // FOR LEDsG
  pinMode(ledPin, OUTPUT);  //FOR LEDPIN
  pinMode(analogPin, INPUT);   //FOR ANALOGPIN
}
 
void loop() {
  //READING THE STATES OF THE SWITCHES AND LEDs
  switchState2 = digitalRead(switch2);          
  switchState3 = digitalRead(switch3);
  LIGHTControlledLED = digitalRead(ledPin);
  LEDsB = digitalRead(4);
  LEDsR = digitalRead(6);
  LEDsG = digitalRead(7);
  sensorValue = digitalRead(analogPin);
 
//printing the output of switches on the serial monitor
  
if (switchState2 == LOW && switchState3 == HIGH) {
              digitalWrite(7, HIGH);   // turn the RED LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(7, LOW);       // turn the RED LED off by making the voltage LOW
              delay(1000);                    // wait for a second
 
              digitalWrite(4, HIGH);   // turn the GREEN LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(4, LOW);       // turn the GREEN LED off by making the voltage LOW
              delay(1000);                    // wait for a second
 
              digitalWrite(6, HIGH);   // turn the BLUE LED on (HIGH is the voltage level)
              delay(1000);                    // wait for a second
              digitalWrite(6, LOW);       // turn the BLUE LED off by making the voltage LOW
              delay(1000);                    // wait for a second
              Serial.print("\rSWITCH:OFF ON" );
              Serial.print("        ");
  }
 
  //IF STATEMENTS TO PRINT THE OUTPUTS ON SERIAL MONITOR
 if (switchState2 == 0 && switchState3 == 1)
 Serial.print("\rSWITCH:OFF ON" ); 
  Serial.print("      ");
 
 
 if (switchState2 == 1 && switchState3 == 0)
 Serial.print("\rSWITCH:ON OFF" );
  Serial.print("      ");
 
 
 if (switchState2 == 1 && switchState3 == 1)
 Serial.print("\rSWITCH:ON ON" );
  Serial.print("      ");
 
if (switchState2 == 0 && switchState3 == 0)
 Serial.print("\rSWITCH:OFF OFF" );
  Serial.print("      ");
 
if (switchState2 == 0 || switchState3 == 0)
 Serial.print("LEDs:0FF" );
  Serial.print("      ");
 
  if (switchState2 == 1 || switchState3 == 0)
 Serial.print("0N");
  Serial.print("      ");
 
  if (switchState2 == 1 || switchState3 == 1)
 Serial.print("0N");
  Serial.print("      ");
 
   if (switchState2 == 0 || switchState3 == 1)
 Serial.print("0N");
  Serial.print("      ");
 
 int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
   Serial.print("     ");
   Serial.print("Photocell value: ");
   Serial.print(voltage);
              Serial.print("mV");
              Serial.print("        ");
 
 int val = analogRead(analogPin);   // read the input pin
  analogWrite(ledPin, val / 4);
  Serial.println("Light-controlled LED: OFF");
 
int val = analogRead(analogPin); // read the input pin 
  analogWrite(ledPin, val / 4);
    if (val <= 900)
{ 
// fade in from min to max in increments of 60 points:
for (int fadeValue = 0 ; fadeValue <= 1000; fadeValue += 60) {
 // sets the value (range from 0 to 1000): 
   analogWrite(ledPin, fadeValue); 
// wait for 200 milliseconds to see the dimming effect
 delay(200);
 }
 }
if (val > 900) {
 analogWrite(ledPin, 0);
 // wait for 200 milliseconds to see the dimming effect 
delay(200); 
}
}
  }

