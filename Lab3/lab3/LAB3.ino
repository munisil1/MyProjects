#define RS  12 //HIGH: DATA  LOW: INSTRUCTIONS
#define E  11 //ENABLE READ OR WRITE DATA
#define D4  5 //LOW: USE 4-BIT charERFACE
#define D5  4 // R/W HIGH: READ  LOW: WRITE
#define D6  3
#define D7  2


//Write values to data pins
void writeLCD(unsigned char data){
  //unsigned char dataToSend =0;
  digitalWrite(RS, HIGH);
 digitalWrite(D4, data&0x10);
  digitalWrite(D5, data&0x20);
  digitalWrite(D6, data&0x40);
  digitalWrite(D7, data&0x80);
    
  digitalWrite(E,HIGH);
  delay(1);
  digitalWrite(E,LOW);
  delay(1);

  digitalWrite(RS, HIGH);
  digitalWrite(D4, data&0x01);
  digitalWrite(D5, data&0x02);
  digitalWrite(D6, data&0x04);
  digitalWrite(D7, data&0x08);
 
  digitalWrite(E,HIGH);
   delay(1);
  digitalWrite(E,LOW);
   delay(1);

}


void commandLCD(unsigned char command) { // (char command){
 
  digitalWrite(RS, LOW);
  digitalWrite(D4,command&0x10);
  digitalWrite(D5, command&0x20);
  digitalWrite(D6, command&0x40);
  digitalWrite(D7, command&0x80);
 
digitalWrite(E,HIGH);
  delay(1);
  digitalWrite(E,LOW);
   delay(1);
   
  digitalWrite(RS, LOW);
  digitalWrite(D7, command&0x08);
  digitalWrite(D6, command&0x04);
  digitalWrite(D5, command&0x02);
  digitalWrite(D4, command&0x01);
 
  digitalWrite(E,HIGH);
  delay(1);
  digitalWrite(E,LOW);
   delay(1);
 
}
void commandLCD8(unsigned char command) { // (char command){
 
  digitalWrite(RS, LOW);
  digitalWrite(D4,command&0x10);
  digitalWrite(D5, command&0x20);
  digitalWrite(D6, command&0x40);
  digitalWrite(D7, command&0x80);
 
digitalWrite(E,HIGH);
  delay(1);
  digitalWrite(E,LOW);
   delay(1);
}

void setup() {
 
pinMode(RS,OUTPUT); //HIGH: DATA  LOW: INSTRUCTIONS
pinMode(E, OUTPUT); //TELL LCD WHETHER SHOULD READ OR WRITE
pinMode(D4,OUTPUT);
pinMode(D5,OUTPUT);//HIGH:READ MODE  LOW: WRITE MODE
pinMode(D6,OUTPUT);
pinMode(D7,OUTPUT);
delay(100);

//digitalWrite(RS,LOW);
digitalWrite(E,LOW);
delay(16);
commandLCD8(0x30);
delay(5);
commandLCD8(0x30);
delay(1);
commandLCD8(0x30);
delay(1);
commandLCD8(0x30);
commandLCD8(0x20);

commandLCD(0x28);

//display control
commandLCD(0x10);
commandLCD(0x0C);
commandLCD(0x06);

//


delay(1);

}

void loop() {
  //displayClear();
  commandLCD(0x28);
  commandLCD(0x80);
  writeLCD('H');
  commandLCD(0x81);   delay(50);
  writeLCD('e');   delay(50);  commandLCD(0x82);
  writeLCD('l');   delay(50);   commandLCD(0x83);
  writeLCD('l');   delay(50);   commandLCD(0x84);
  writeLCD('o');   delay(50);  commandLCD(0x85);  //skip the number after this since you want space
  writeLCD(' ');   delay(50);  commandLCD(0x86);   //skip after this too
  writeLCD('&');   delay(50);  commandLCD(0x87);   //skip after this too
   writeLCD(' ');   delay(50);  commandLCD(0x88);   //skip after this too
  writeLCD('w');   delay(50);   commandLCD(0x89);
  writeLCD('e');   delay(50); commandLCD(0x8A);
  writeLCD('l');   delay(50);   commandLCD(0x8B);
  writeLCD('c');   delay(50);   commandLCD(0x8C);
  writeLCD('o');   delay(50);   commandLCD(0x8D);
  writeLCD('m');   delay(50);   commandLCD(0x8E);
  writeLCD('e');   delay(50); commandLCD(0x8F);  //SKIP AFTER THIS SINCE WE WANT SPACE
   writeLCD(' ');   delay(50);  commandLCD(0xC0);   //skip after this too
  writeLCD('t');   delay(50); commandLCD(0xC1);
  writeLCD('o');   delay(50);   commandLCD(0xC2);  //SKIP
   writeLCD(' ');   delay(50);  commandLCD(0xC3);   //skip after this too
  writeLCD('o');   delay(50); commandLCD(0xC4);
  writeLCD('u');   delay(50);  commandLCD(0xC5);
  writeLCD('r');   delay(50);   commandLCD(0xC6); //SKIP
   writeLCD(' ');   delay(50);  commandLCD(0xC7);   //skip after this too
  writeLCD('d');   delay(50); commandLCD(0xC8);
  writeLCD('e');   delay(50);   commandLCD(0xC9);
  writeLCD('m');   delay(50); commandLCD(0xCA);
  writeLCD('o');   delay(50);   commandLCD(0xCB);
  writeLCD('!');   delay(1000);   
  commandLCD(0x01);  //FUNCTION THAT CLEARS THE SCREEN OF THE LCD
  delay(3000);  //Delay for 3 seconds after clearing the screen
 // commandLCD(0x18); // the command for shifting text
  
for(int count = 0x80; count < 0x70; count -- ){
  commandLCD(count);
  writeLCD('E');   delay(50);  commandLCD(count + 1);   delay(100);
  writeLCD('L');   delay(50);  commandLCD(count + 2);    delay(100);
  writeLCD('E');   delay(50);   commandLCD(count + 3);delay(100);
  writeLCD('C');   delay(50);   commandLCD(count + 4);delay(100);
  writeLCD(' ');   delay(50);  commandLCD(count + 5);  delay(100);
  writeLCD('2');   delay(50);  commandLCD(count + 6);   
  writeLCD('9');   delay(50);  commandLCD(count + 7);
  writeLCD('1');   delay(50);  commandLCD(count + 8);  
  writeLCD('-');   delay(50);   commandLCD(count + 9);
  writeLCD('2');   delay(50); commandLCD(count + 0xA);
  writeLCD('0');   delay(50);   commandLCD(0x8B);
  writeLCD('C');   delay(50);   commandLCD(0x8C);
  writeLCD(' ');   delay(50);   commandLCD(0x8D);
  writeLCD('T');   delay(50);   commandLCD(0x8E);
  writeLCD('e');   delay(50); commandLCD(0x8F);  
}
  writeLCD('a');   delay(50);  commandLCD(0xC0);  
  writeLCD('m');   delay(50); commandLCD(0xC1);
  writeLCD(' ');   delay(50);   commandLCD(0xC2);  
  writeLCD('L');   delay(50);  commandLCD(0xC3);   
  writeLCD('2');   delay(50); commandLCD(0xC4);
  writeLCD('D');   delay(50);  commandLCD(0xC5);
  writeLCD('-');   delay(50);   commandLCD(0xC6);   
  writeLCD('5');   delay(50);  commandLCD(0xC7);   
  writeLCD('C');   delay(50); commandLCD(0xC8);
  delay(3000);
 
   }


