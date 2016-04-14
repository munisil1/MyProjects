#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
#define POWER 0x10EFD827
#define A 0x10EFF807
#define B 0x10EF7887
#define C 0x10EF58A7
#define UP 0x10EFA05F
#define DOWN 0x10EF00FF
#define LEFT 0x10EF10EF
#define RIGHT 0x10EF807F
#define STOP 0x10EF20DF
#define FUNCA 1
#define FUNCB 2
#define FUNCC 3

// motor pins
#define E1 5 // pin used for right motor
#define M1 4 // pin used for right motor
#define E2 6 // pin used for left motor
#define M2 7 // pin used for left motor

uint16_t lastCode = 0; // This keeps track of the last code RX'd

int A_flag = 0;
int B_flag = 0;
int C_flag = 0;

void setup()
{

  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results))
  {
    /* read the RX'd IR into a 16-bit variable: */
    uint16_t resultCode = (results.value & 0xFFFF);

    /* The remote will continue to spit out 0xFFFFFFFF if a
      button is held down. If we get 0xFFFFFFF, let's just
      assume the previously pressed button is being held down */
    if (resultCode == 0xFFFF)
      resultCode = lastCode;
    else
      lastCode = resultCode;

    switch (resultCode)
    {
      case POWER:

        analogWrite(E1, 0);
        analogWrite(E2, 0);
        C_flag = 0;
        break;
      case A:  //FOR FUNCTION ONE
        C_flag = 0;
        func_A();
        break;
      case B:    //FOR THE SECOND FUNCTION
        C_flag = 0;
        func_B();
        break;
      case C:
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        C_flag = 1;
        break;
      default:
        break;
    }

    irrecv.resume();
  }

  while(C_flag == 1){
    func_C();
  }
}

void func_A(){
  
}

void func_B(){
  
}

void func_C() {

  if (irrecv.decode(&results))
  {
    /* read the RX'd IR into a 16-bit variable: */
    uint16_t resultCode = (results.value & 0xFFFF);

    /* The remote will continue to spit out 0xFFFFFFFF if a
      button is held down. If we get 0xFFFFFFF, let's just
      assume the previously pressed button is being held down */
    if (resultCode == 0xFFFF)
      resultCode = lastCode;
    else
      lastCode = resultCode;

    switch (resultCode)
    {
      case POWER:
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        C_flag = 0;
        break;
      case A:  //FOR FUNCTION ONE
        Serial.println("A");
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        C_flag = 0;
        //functionone();
        break;
      case B:    //FOR THE SECOND FUNCTION
        Serial.println("B");
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        C_flag = 0;
        //functiontwo();
        break;
      case C:
        Serial.println("C");  //STOP IT AND MAKE THE USER CONTROL IT
        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH); 
        C_flag = 1;
        break;
      case UP:   //MOVE IT FOWARD
        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH);
        analogWrite(E1, 255);
        analogWrite(E2, 255);
        C_flag = 1;
        break;
      case DOWN:  //REVERSE IT
        analogWrite(M1, LOW);
        analogWrite(M2, LOW);
        analogWrite(E1, 255);
        analogWrite(E2, 255);
        C_flag = 1;
        break;
      case RIGHT: //TURN IT RIGHT
        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH);
        analogWrite(E1, 0);
        analogWrite(E2, 255);
        C_flag = 1;
        break;
      case LEFT:  //TURN IT LEFT
        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH);
        analogWrite(E1, 255);
        analogWrite(E2, 0);
        C_flag = 1;
        break;
      case STOP:
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        break;
      default:
       // C_flag = 1;
        break;
    }

    irrecv.resume();
  }
}

