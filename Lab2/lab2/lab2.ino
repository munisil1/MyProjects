int ledPin = 2;  // the pin that the LED is attached to

//connecting the 7seg to arduino’s board
int forA = 3; // se
int forB = 4;
int forC = 5;
int forDP = 6;
int forD = 12;
int forE = 10;
int forF = 8;
int forG = 9;

int forBuzzer = 11;//buzzer connected to pin 11(PWM pin)

int incomingByte;    // a variable to read incoming serial data into

int WPM = 5; //

int dotLength = 1200/WPM; // getting dotLength for morse code
int dashLength = dotLength * 3; // getting dashLength for morse code

int flag = 1;
int flag2 = 1;
 
//general function to display the letters and numerals
void lightforall(int number){
   int array[8];
  int x = 0x80;
 
//each array element contains 0 or 1 depending on whether the specified seg is on or off
// array elements encoded to represent the segments as following: a,b,c,d e,f,g,DP 
  for(int i=0;i<=7;i++){
    array[i] = number & x;
    x = x >>1;
  }


  //if array[0]= 0 , seg a will be off
  
if(array[0]== 0x00){
    digitalWrite(forA, HIGH);
  }else{
    digitalWrite(forA, LOW);  //otherwise seg a will be on
  }
   
  if(array[1]== 0x00){
    digitalWrite(forB, HIGH);
  } else{
    digitalWrite(forB, LOW);  
  }
  
  if(array[2]== 0x00){
    digitalWrite(forC, HIGH);
  } else{
    digitalWrite(forC, LOW);  
  }
  
  if(array[3]== 0x00){
    digitalWrite(forD, HIGH);
  } else{
    digitalWrite(forD, LOW);  
  }
  
   if(array[4]== 0x00){
    digitalWrite(forE, HIGH);
   } else{
     digitalWrite(forE, LOW); 
   }
  
    if(array[5]== 0x00){
      digitalWrite(forF, HIGH);
    } else{
    digitalWrite(forF, LOW);  }
  
    if(array[6]== 0x00){
      digitalWrite(forG, HIGH);
   } else{
     digitalWrite(forG, LOW);
  }
  
  
    if(array[7]== 0x00){
      digitalWrite(forDP, HIGH);
   } else{
     digitalWrite(forDP, LOW); delay(2000);}
 }
 
//setting the LED on 
void on() {
 digitalWrite(ledPin, HIGH);
}

//setting the LED off
void off() {
 digitalWrite(ledPin, LOW);
}

//lightening up the LED for dotLength and setting the buzzer for dotLength
void dot() {
 on();
 analogWrite(forBuzzer,LOW);
tone(forBuzzer,700);
  //noTone(forBuzzer);
 delay(dotLength);
 analogWrite(forBuzzer, HIGH);

}

//lightening up the LED for dashLength and setting the buzzer for dashLength
void dash() {
 on();
  analogWrite(forBuzzer, LOW);
tone(forBuzzer,700);
 // noTone(forBuzzer);
 delay(dotLength * 3);
  analogWrite(forBuzzer, HIGH);
   
}

//turning off the LED to represent the space
void space() {
 off();
 delay(dotLength);
}

//resting for dashLength between each letter
void btnLetters() {
 off();
 delay(dotLength *3);
}

//restting for 7*dotLength between each word
void btnWords() {
 off();
 delay(dotLength * 7);
}
void SevenSegLEDOff(){
lightforall(0x00);
}
 
//displaying letters

void lightA(){
lightforall(0xEE);
 off();
}

void lightB(){
lightforall(0x3E);
 off();
}

void lightC(){
lightforall(0x9C);
 off();
 }


void lightD(){
lightforall(0x7A);
 off();
 }


void lightE(){
lightforall(0x9E);
 off();}



void lightF(){
lightforall(0x8E);
 off();}

 

void lightG(){
lightforall(0xF6);
 off();
}

 
void lightH(){
lightforall(0x2E);
 off();}


void lightI(){
lightforall(0x0C);
 off();}


void lightJ(){
lightforall(0x78);
 off();}


void lightK(){
lightforall(0x6E);  
off();
}

void lightL(){
lightforall(0x1C); 
off();
}

void lightM(){
lightforall(0xA8);
 off();}

void lightN(){
lightforall(0x2A);
 off();
}

void lightO(){
lightforall(0xFC);
 off();
}

void lightP(){
lightforall(0xCE); 
 off();}

void lightQ(){
lightforall(0xE6); 
 off();}

void lightR(){
lightforall(0x0A);
 off();
}

void lightS(){
lightforall(0xB6); 
 off();}
 
void lightT(){
lightforall(0x1E); 
 off();}

void lightU(){
lightforall(0x7C); 
 off();}

void lightV(){
lightforall(0x38); 
 off();}

void lightW(){
lightforall(0x54);
 off();}

void lightX(){
lightforall(0x6E);  
 off();}

void lightY(){
lightforall(0x76); 
 off();}

void lightZ(){
lightforall(0xDA);
 off();}

//displaying the dot
void lightDot(){
 lightforall(0x01);
  off();
  }
  
 
// Methods for numbers:
void lightNum0(){
lightforall(0xFC); 
 off();}

void lightNum1(){
lightforall(0x60);
 off();}

void lightNum2(){
lightforall(0xDA);  off();}

void lightNum3(){
lightforall(0xF2);   off();}

void lightNum4(){
lightforall(0x66);  off(); }

void lightNum5(){
lightforall(0xB6);   off(); }

void lightNum6(){
lightforall(0x3E);  off(); }

void lightNum7(){
lightforall(0xE0); off(); }

void lightNum8(){
lightforall(0xFE);   off();}

void lightNum9(){
lightforall(0xF6);  off();}
 

 
 
 //to be checked
 
void inputFromUser(){
  Serial.print("Enter the WPM:"); 
  delay(500);
WPM = Serial.parseInt();
delay(200);
Serial.println(WPM);
}

void setup() {
 // initialize serial communication:
 Serial.begin(9600);
 // initialize the LED pin as an output:
 pinMode(ledPin, OUTPUT);
 pinMode(forA, OUTPUT);
 pinMode(forB, OUTPUT);
 pinMode(forC, OUTPUT);
 pinMode(forD, OUTPUT);
 pinMode(forE, OUTPUT);
 pinMode(forF, OUTPUT);
 pinMode(forG, OUTPUT);
 pinMode(forDP, OUTPUT); 
}
 



void loop() {
 
 SevenSegLEDOff();
  
if (flag2 == 1 ){
 inputFromUser();
 delay(2000);
 flag2 =0;}
 
     if(flag== 1){
   Serial.println("Enter the message:");
 flag=0;
 }

 
// checking for incoming serial data:
 if (Serial.available() > 0) {
   // read the oldest byte in the serial buffer:
   incomingByte = Serial.read();
   Serial.print("You have entered: ");
   Serial.println(incomingByte, DEC);  //this prints the ASCII code for whatever has been printed in the monitor


//encoding letters in Morse code:
 
 if (incomingByte == 'A' || incomingByte == 'a') {
  lightA();    dot(); space(); dash(); btnLetters();
  }       
 
 if (incomingByte == 'B' || incomingByte == 'b') {
     lightB();    dash(); space(); dot(); space(); dot(); space(); dot(); btnLetters();
 } 
  
if (incomingByte == 'C' || incomingByte == 'c') {
   lightC();   dash(); space(); dot(); space(); dash(); space(); dot(); btnLetters();
  } 
 
 if (incomingByte == 'D' || incomingByte == 'd') {
  lightD(); dash(); space(); dot(); space(); dot();  btnLetters(); 
  } 
 
 if (incomingByte == 'E' || incomingByte == 'e'){
    lightE(); dot();  btnLetters();
  }  
 
 if (incomingByte == 'F' || incomingByte == 'f') {
  lightF(); dot(); space(); dot(); space(); dash(); space(); dot(); btnLetters();  } 
  } 
 
 if (incomingByte == 'G' || incomingByte == 'g') {
       lightG();  dash(); space(); dash(); space(); dot(); btnLetters();
}  
  if (incomingByte == 'H' || incomingByte == 'h') {
  lightH();   dot(); space(); dot(); space(); dot(); space(); dot(); btnLetters();
}
 
 if (incomingByte == 'I' || incomingByte == 'i') {
    lightI(); dot(); space(); dot(); btnLetters();
}
 
 if (incomingByte == 'J' || incomingByte == 'j') {
      lightJ(); dot(); space(); dash(); space(); dash(); space(); dash(); btnLetters();
}

 if (incomingByte == 'K' || incomingByte == 'k'){
       lightK();  dash(); space(); dot(); space(); dash(); btnLetters();
} 
  
if (incomingByte == 'L' || incomingByte == 'l'){
      lightL(); dot(); space(); dash(); space(); dot(); space(); dot(); btnLetters();
} 
 
 if (incomingByte == 'M' || incomingByte == 'm') {
          lightM(); dash(); space(); dash();  btnLetters();
} 
  
if (incomingByte == 'N' || incomingByte == 'n') {
       lightN();   dash(); space(); dot();  btnLetters();
 }
 
 if (incomingByte == 'O' || incomingByte == 'o') {
  lightO();  dash(); space(); dash(); space(); dash(); btnLetters();
}
  
if (incomingByte == 'P' || incomingByte == 'p') { 
    lightP();  dot(); space(); dash(); space(); dash(); space(); dot(); btnLetters();
 }
 
 if (incomingByte == 'Q' || incomingByte == 'q') { 
    lightQ();  dash(); space(); dash(); space();dot(); space(); dash(); btnLetters();
 }
 
 if (incomingByte == 'R' || incomingByte == 'r') {
    lightR();    dot(); space(); dash(); space(); dot(); btnLetters();
 }
  if (incomingByte == 'S' || incomingByte == 's'){  
  lightS();dot(); space(); dot(); space(); dot(); btnLetters();
 }
 
 if (incomingByte == 'T' || incomingByte == 't') {
  lightT();dash(); btnLetters();
 }
  
if (incomingByte == 'U' || incomingByte == 'u') {
  lightU(); dot(); space(); dot(); space(); dash();  btnLetters();
 }
  
if (incomingByte == 'V' || incomingByte == 'v') {
  lightV();dot(); space(); dot(); space(); dot(); space(); dash(); btnLetters();
}

 if (incomingByte == 'W' || incomingByte == 'w') {
  lightW();dot(); space(); dash(); space(); dash(); btnLetters();
}

 if (incomingByte == 'X' || incomingByte == 'x') {
  lightX(); dash(); space(); dot(); space(); dot(); space(); dash(); btnLetters();
}
 
 if (incomingByte == 'Y' || incomingByte == 'y') {
  lightY();dash(); space(); dot(); space(); dash(); space(); dash(); btnLetters();
 }
 
if (incomingByte == 'Z' || incomingByte == 'z') {
  lightZ();dash(); space(); dash(); space(); dot(); space(); dot(); btnLetters();
}
  
//encoding numbers in Morse code
 if (incomingByte == '0') {
   lightNum0();dash(); space();dash(); space();dash(); space(); dash(); space(); dash(); btnLetters(); }
  
  if (incomingByte == '1'){
   lightNum1();dot(); space(); dash(); space();dash(); space(); dash(); space(); dash(); btnLetters(); }
  
  if (incomingByte == '2'){
   lightNum2();dot(); space(); dot(); space(); dash(); space(); dash(); space(); dash(); btnLetters(); }
  
  if (incomingByte == '3'){
   lightNum3();dot(); space(); dot(); space(); dot(); space(); dash(); space(); dash(); btnLetters(); }
  
  if (incomingByte == '4'){
   lightNum4();dot(); space(); dot(); space(); dot(); space(); dot(); space(); dash(); btnLetters(); }  
  
  if (incomingByte == '5'){
   lightNum5();dot(); space(); dot(); space(); dot(); space(); dot(); space(); dot(); btnLetters(); }
  
  if (incomingByte == '6'){
   lightNum6();dash(); space(); dot(); space(); dot(); space(); dot(); space(); dot(); btnLetters(); }
  
  if (incomingByte == '7'){
   lightNum7();dash(); space(); dash(); space(); dot(); space(); dot(); space(); dot(); btnLetters(); }
  
  if (incomingByte == '8'){
   lightNum8();dash(); space(); dash(); space(); dash(); space(); dot(); space(); dot(); btnLetters(); }
  
  if (incomingByte == '9'){
   lightNum9();dash(); space(); dash(); space(); dash(); space(); dash(); space(); dot(); btnLetters(); }
  
 
//encoding the four punctuation in Morse code

 if ( incomingByte == '.') {
     lightDot(); dot(); space(); dash(); space(); dot(); space(); dash(); space(); dot(); space(); dash();  }
 
 if (incomingByte == ','){
  dash(); space(); dash(); space(); dot(); space(); dot(); space(); dash(); space(); dash();} 

 if (incomingByte == ':'){
  dash(); space(); dash(); space(); dash(); space(); dot(); space(); dot(); space(); dot(); }
 
 if (incomingByte == '-'){
  dash(); space(); dot(); space(); dot(); space(); dot(); space(); dot(); space(); dash();}

}







