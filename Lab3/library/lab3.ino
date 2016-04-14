char RS = 12;//HIGH: DATA  LOW: INSTRUCTIONS
char E = 11; //ENABLE READ OR WRITE DATA
char D4 = 5; //LOW: USE 4-BIT charERFACE
char D5 = 4;// R/W HIGH: READ  LOW: WRITE
char D6 = 3;
char D7 = 2;
char x = 0x80;
//NUMBER FOR COMMANDS
char bit8 = 0x03;   //8-bit charerface
char bit4 = 0x02;   //4-bit charerface
char L1FS = 0x00;  //display on 1 line with small font
char L1FB = 0x04;  //display on 1 line with big font
char L2 = 0x08;     //display on 2 lines with only small font

//Characters coding:
char H = 0x48;
char e = 0x65;
char l = 0x6C;
char o = 0x6F;
char ampercent= 0x26;
char w= 0x77;
char c = 0x63;
char m = 0x6D;
char t = 0x74;
char u= 0x75;
char r= 0x7D;
char d= 0x64;
char exclamation = 0x21;





void commandLCD(char command);

//Write values to data pins
void writeLCD(char data){
  //unsigned char dataToSend =0;
  digitalWrite(RS, HIGH);
 digitalWrite(D4, data&0x80);
  digitalWrite(D5, data&0x40);
  digitalWrite(D6, data&0x20);
  digitalWrite(D7, data&0x10);
    
  digitalWrite(E,HIGH);
  delayMicroseconds(1);
  digitalWrite(E,LOW);
  delayMicroseconds(1000);

  digitalWrite(RS, HIGH);
  digitalWrite(D4, data&0x01);
  digitalWrite(D5, data&0x02);
  digitalWrite(D6, data&0x04);
  digitalWrite(D7, data&0x08);
  
  digitalWrite(E,HIGH);
  delayMicroseconds(1);
  digitalWrite(E,LOW);
  delayMicroseconds(100);

}


void commandLCD(char command) { // (char command){
  char array[8];
  for(char i=0;i<=7;i++){
    array[i] = command & x;
    x = x >>1;
  }
  //WE DON'T CARE ABOUT ARRAY[0]&[1];
  //POSITIVE LOGIC
  if(array[2]== 0x00){ //RS
    //digitalWrite(RS, HIGH);
    digitalWrite(RS, LOW);
  } else{
    digitalWrite(RS, HIGH);
    //digitalWrite(RS, LOW);
  }
  
  //4-BUSLINE FROM D4 TO D7
   if(array[4]== 0x00){
   //digitalWrite(D7, HIGH);
    digitalWrite(D7, LOW);
  } else{
    digitalWrite(D7, HIGH);
    //digitalWrite(D7, LOW);
  }
  if(array[5]== 0x00){
     digitalWrite(D6, LOW);
  } else{
    digitalWrite(D6, HIGH);  }
  if(array[6]== 0x00){
     digitalWrite(D5, LOW);
   } else{
    digitalWrite(D5, HIGH);
  }
  if(array[7] == 0x00){
     digitalWrite(D4, LOW);
   } else{
  digitalWrite(D4, HIGH); 
  }
 x=0x80;
  digitalWrite(E,HIGH);
  delayMicroseconds(1);
  digitalWrite(E,LOW);
  delayMicroseconds(100);
  
  //delay(10);
}
//all the instructions
//clear display
void displayClear(){
  commandLCD(0x00);
  commandLCD(0x01);
}



//return the cursor and the display to their original positons
//DDAM remains unchanged
void cursorAtHome(){
commandLCD(0x00);
commandLCD(0x02);
delay(2);}
//cursor move direction and the shift during the write and read
//RS R/W = 00
// 0 1 I/D S  I/D for increment or decrement the address conter after subsequent //DDRAM read or write
//S 1 for left shift, 0 for right shift(see command page for more)
void entryModeSet(char command){
commandLCD(0x00);
delay(1);
commandLCD(command);
//what does I/D do???
delay(1);//40us
}
//RS R/W = 00
// 1 D C B; D for display on or off; C for cursor display;
//B for character on cursor blinks
void displayControl(char command){
 commandLCD(0x00);
 delay(1);
 commandLCD(command);
 delay(1);//40us
}

//void write4bitMode(char data){
//  
//  
//  digitalWrite(D4, data&0x01);
//  digitalWrite(D5, data>>1&0x01);
//  digitalWrite(D6, data>>2&0x01);
//  digitalWrite(D7, data>>3&0x01);
////
//  digitalWrite(E,LOW);
//  delayMicroseconds(1);
//  digitalWrite(E,HIGH);
//  delayMicroseconds(1);
//  digitalWrite(E,LOW);
//  delayMicroseconds(100);
//  
// }



//RS R/W = 00
// S/C R/L * * see command page for more
void CDShift(char command){
commandLCD(0x01);
delay(1);
commandLCD(command);
delay(0.1);//40us
}
//001DL NF**  RS R/W = 00
//DL for charerface data length
//N for number of display lines
//F for charactor font
void functionSet(char command1, char command2){
commandLCD(command1);
delay(3);
commandLCD(command2);
delay(0.1);
}






void initialize();

void setup() {
  
pinMode(RS,OUTPUT); //HIGH: DATA  LOW: INSTRUCTIONS
pinMode(E, OUTPUT); //TELL LCD WHETHER SHOULD READ OR WRITE
pinMode(D4,OUTPUT);
pinMode(D5,OUTPUT);//HIGH:READ MODE  LOW: WRITE MODE
pinMode(D6,OUTPUT);
pinMode(D7,OUTPUT);
delay(50);

digitalWrite(RS,LOW);
digitalWrite(E,LOW);
//delay(16);
commandLCD(0x03);
delayMicroseconds(4300);
commandLCD(0x03);
delayMicroseconds(200);
commandLCD(0x03);
delayMicroseconds(100);
//delay(1);
//command
commandLCD(0x02);

// 
commandLCD(0x02);
commandLCD(0x0C);


//display control
commandLCD(0x00);
commandLCD(0x08);

//Clear display
commandLCD(0x00);
commandLCD(0x01);

//entryset
commandLCD(0x00);
commandLCD(0x04);

delay(1);

}
void initialize(){
// DELAY
//if (analogRead(5V) > 4.5)
// delay(20);
//else if(analogRead(5) > 2.7)
delay(25);
commandLCD(0x03);
delay(10);
commandLCD(0x03);
delay(1);
commandLCD(0x03);
delay(1);
//command
commandLCD(0x02);
 
functionSet(0x02,0x08);
displayControl(0x08);
displayClear();
entryModeSet(0x07);//no shift for the first line
delay(1);


}


void loop() {
  //displayClear();
    digitalWrite(RS,HIGH);
    commandLCD(0x08);
    commandLCD(0x00);
  writeLCD('1'); // e
  delay(5000);
//  digitalWrite(RS,LOW);
// write4bitMode(0x06);
//  write4bitMode(0x05);
 
}



