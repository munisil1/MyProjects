const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A5, A4, A3, A2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A1,A0, 8}; //connect to the column pinouts of the keypad

////
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
  Serial.begin(9600);
  pinMode(A5, INPUT_PULLUP);
   pinMode(A4, INPUT_PULLUP);
   pinMode(A3, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
     pinMode(A1, OUTPUT); 
     pinMode(A0,OUTPUT); 
     pinMode(8, OUTPUT);     
  // put your setup code here, to run once:
pinMode(RS,OUTPUT); //HIGH: DATA  LOW: INSTRUCTIONS
pinMode(E, OUTPUT); //TELL LCD WHETHER SHOULD READ OR WRITE
pinMode(D4,OUTPUT);
pinMode(D5,OUTPUT);//HIGH:READ MODE  LOW: WRITE MODE
pinMode(D6,OUTPUT);
pinMode(D7,OUTPUT);
delay(100);

initial();
}

void initial(){
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

delay(1);
}
////
//void setup() {
//  Serial.begin(9600);
//  pinMode(A5, INPUT_PULLUP);
//   pinMode(A4, INPUT_PULLUP);
//   pinMode(A3, INPUT_PULLUP);
//    pinMode(A2, INPUT_PULLUP);
//     pinMode(A1, OUTPUT); 
//     pinMode(A0,OUTPUT); 
//     pinMode(8, OUTPUT);     
//  // put your setup code here, to run once:
//}

void takeAGuess(){
   commandLCD(0x80);
  writeLCD('T');
  commandLCD(0x81);  
  writeLCD('a');
    commandLCD(0x82);  
  writeLCD('k');
    commandLCD(0x83);  
  writeLCD('e');  
  commandLCD(0x84);  
  writeLCD(' '); 
  commandLCD(0x85);  
  writeLCD('a'); 
  commandLCD(0x86);  
  writeLCD(' '); 
  commandLCD(0x87);  
  writeLCD('g');
    commandLCD(0x88);  
  writeLCD('u');
    commandLCD(0x89);  
  writeLCD('e');
    commandLCD(0x8A);  
  writeLCD('s');
    commandLCD(0x8B);  
  writeLCD('s');
    commandLCD(0x8C);  
  writeLCD(':');
  delay(1000);
  }


void lower(){
   commandLCD(0x80);
  writeLCD('G');
  commandLCD(0x81);  
  writeLCD('u');
    commandLCD(0x82);  
  writeLCD('e');
    commandLCD(0x83);  
  writeLCD('s');  
  commandLCD(0x84);  
  writeLCD('s'); 
  commandLCD(0x85);  
  writeLCD(' '); 
  commandLCD(0x86);  
  writeLCD('l');
    commandLCD(0x87);  
  writeLCD('o');
    commandLCD(0x88);  
  writeLCD('w');
    commandLCD(0x89);  
  writeLCD('e');
    commandLCD(0x8A);  
  writeLCD('r');
  delay(1000);
  }

  void higher(){
   commandLCD(0x80);
  writeLCD('G');
  commandLCD(0x81);  
  writeLCD('u');
    commandLCD(0x82);  
  writeLCD('e');
    commandLCD(0x83);  
  writeLCD('s');  
  commandLCD(0x84);  
  writeLCD('s'); 
  commandLCD(0x85);  
  writeLCD(' '); 
  commandLCD(0x86);  
  writeLCD('h');
    commandLCD(0x87);  
  writeLCD('i');
    commandLCD(0x88);  
  writeLCD('g');
    commandLCD(0x89);  
  writeLCD('h');
    commandLCD(0x8A);  
  writeLCD('e');
  commandLCD(0x8B);  
  writeLCD('r');
  delay(1000);
  }

    void youWon(){
   commandLCD(0x80);
  writeLCD('Y');
  commandLCD(0x81);  
  writeLCD('o');
    commandLCD(0x82);  
  writeLCD('u');
    commandLCD(0x83);  
  writeLCD(' ');  
  commandLCD(0x84);  
  writeLCD('w'); 
  commandLCD(0x85);  
  writeLCD('0');
    commandLCD(0x86);  
  writeLCD('n');
    commandLCD(0x87);  
  writeLCD('!');
  delay(1000);
  }

int scan(){
 int r1 = digitalRead(A5);
 int r2 = digitalRead(A4);
  int r3 = digitalRead(A3);
  int r4 = digitalRead(A2);
  
  int c1 = digitalRead(A1);
  int c2 = digitalRead(A0);
  int c3 = digitalRead(8);
  if(r1 == 0 && c1 == 1){
    Serial.println('1');
    return 1;
  }
  if(r1 == 0 && c2 == 1){
    Serial.println('2');
    return 2;
  }
  if(r1 == 0 && c3 == 1){
    Serial.println('3');
    return 3;
  }
  if(r2 == 0 && c1 == 1){
    Serial.println('4');
    return 4;
  }
  if(r2 == 0 && c2 == 1){
    Serial.println('5');
    return 5;
  }
  if(r2 == 0 && c3 == 1){
    Serial.println('6');
    return 6;
  }
  if(r3 == 0 && c1 == 1){
    Serial.println('7');
    return 7;
  }
  if(r3 == 0 && c2 == 1){
    Serial.println('8');
    return 8;
  }
   if(r3 == 0 && c3 == 1){
    Serial.println('9');
    return 9;
  }
   if(r4 == 0 && c2 == 1){
    Serial.println('0');
    return 0;
  }
}
  
void loop() {
 int chance = 3;
 int r = random(0,9);
 takeAGuess();
 Serial.println(r);
 delay(5000);
while(chance > 0){
  delay(5000);
int g = scan();
  delay(1000);
  
 if(g == r){
 youWon();
 chance = 0;
 }
 
  else if(g > r){
 lower();
  chance--;}
 
 else if(g < r){
 higher();
  chance--;}
  
}
}
