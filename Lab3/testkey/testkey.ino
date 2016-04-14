#define RS  12 //HIGH: DATA  LOW: INSTRUCTIONS
#define E  11 //ENABLE READ OR WRITE DATA
#define D4  5 //LOW: USE 4-BIT charERFACE
#define D5  4 // R/W HIGH: READ  LOW: WRITE
#define D6  3
#define D7  2
//////
/*this keypad tutorial number one by omar tarek 29-06-2014 */
byte h=0,v=0;    //variables used in for loops
const unsigned long period=50;  //little period used to prevent error
unsigned long kdelay=0;        // variable used in non-blocking delay 
const byte rows=4;             //number of rows of keypad
const byte columns=3;            //number of columnss of keypad
const byte Output[rows]={A5,A4,A3,A2}; //array of pins used as output for rows of keypad
const byte Input[columns]={A1,A0,8}; //array of pins used as input for columns of keypad
byte keypad() // function used to detect which button is used 
{
 static bool no_press_flag=0;  //static flag used to ensure no button is pressed 
  for(byte x=0;x<columns;x++)   // for loop used to read all inputs of keypad to ensure no button is pressed 
  {
     if (digitalRead(Input[x])==HIGH);   //read evry input if high continue else break;
     else
     break;
     if(x==(columns-1))   //if no button is pressed 
     {
      no_press_flag=1;
      h=0;
      v=0;
     }
  }
  if(no_press_flag==1) //if no button is pressed 
  {
    for(byte r=0;r<rows;r++) //for loop used to make all output as low
    digitalWrite(Output[r],LOW);
    for(h=0;h<columns;h++)  // for loop to check if one of inputs is low
    {
      if(digitalRead(Input[h])==HIGH) //if specific input is remain high (no press on it) continue
      continue;
      else    //if one of inputs is low
      {
          for (v=0;v<rows;v++)   //for loop used to specify the number of row
          {
          digitalWrite(Output[v],HIGH);   //make specified output as HIGH
          if(digitalRead(Input[h])==HIGH)  //if the input that selected from first sor loop is change to high
          {
            no_press_flag=0;                //reset the no press flag;
            for(byte w=0;w<rows;w++) // make all outputs as low
            digitalWrite(Output[w],LOW);
            return v*4+h;  //return number of button 
          }
          }
      }
    }
  }
 return 50;
}

////

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
  ////
   for(byte i=0;i<rows;i++)  //for loop used to make pin mode of outputs as output
  {
  pinMode(Output[i],OUTPUT);
  }
  for(byte s=0;s<columns;s++)  //for loop used to makk pin mode of inputs as inputpullup
  {
    pinMode(Input[s],INPUT_PULLUP);
  }
  Serial.begin(9600); //to use serial monitor we set the buad 
  /////
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
  commandLCD(0x01);
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
  commandLCD(0x01);
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
    commandLCD(0x01);
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
    commandLCD(0x01);
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
  writeLCD('o');
    commandLCD(0x86);  
  writeLCD('n');
    commandLCD(0x87);  
  writeLCD('!');
  delay(1000);
  }

      void youLost(){
    commandLCD(0x01);
   commandLCD(0x80);
  writeLCD('Y');
  commandLCD(0x81);  
  writeLCD('o');
    commandLCD(0x82);  
  writeLCD('u');
    commandLCD(0x83);  
  writeLCD(' ');  
  commandLCD(0x84);  
  writeLCD('l'); 
  commandLCD(0x85);  
  writeLCD('o');
    commandLCD(0x86);  
  writeLCD('s');
    commandLCD(0x87);  
  writeLCD('t');
  commandLCD(0x87);  
  writeLCD('!');
  delay(1000);
  }
  
  int flag = 1;
  int chance = 3;
  
void compare(){
//  Serial.println("we are in compare");
int g = 0;
int r =0;
 if( flag == 1){
 r = random(0,9);
Serial.println(r);
flag = 0;}
delay(200);
  if(millis()-kdelay>period) //used to make non-blocking delay
  {
    kdelay=millis();  //capture time from millis function
switch (keypad())  //switch used to specify which button
   {
            case 0:
            {
              Serial.println(1);
             g = 1;
             Serial.println(g);} 
       break;  
            case 1:
            { g = 2;
            Serial.println(2);
            }
       break;
            case 2:
            { g = 3;
            Serial.println(3);}
       break;
            case 4:
            { g = 4;
            Serial.println(4);}
       break;
            case 5:
            { g = 5;
            Serial.println(5);
            }
       break;
            case 6:
            { g = 6;
            Serial.println(6);
            }
       break;
            case 8:
            { g = 7;
            Serial.println(7);
            }
       break;
            case 9:
            { g = 8;
            Serial.println(8);
            }
       break;
            case 10:
            { g = 9;
            Serial.println(9);
            }
       break;
           case 13:
            { g = 0;
            Serial.println(0);
            }
       break;
           
}  
}

// if(g == r){
// youWon();
// chance = -1;
// }
//    if(g > r){
// lower();
//  chance = chance - 1;
//  Serial.println("value" + chance);
//  Serial.println("we are in low");
//}
 
  if(g < r){
 higher();
  chance = chance - 1;
  Serial.println("we are in high");
  }
}
void loop() {

takeAGuess();
 delay(10);
 while(chance >=  0)
 {
compare();
 }
 if ( chance == 0){
 Serial.println("you lost");
 youLost();
 }
flag = 1;
 chance = 3;
}
 
