#define E1 5
#define M1 4
#define E2 6
#define M2 7
#define sV 180 // velocity of going staright
#define tV 210  //velocity of turning
#define bV 80 // velocity of going forward when all on black

void setup() {
  // pins setup
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
}


void go(int v1, int v2) {
  analogWrite(E1, v1); // PWM control the right wheel
  analogWrite(E2, v2); // PWM control the left wheel
}

void loop() {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  int R = analogRead(A2) ;//right sensor
  int L = analogRead(A0) ;// left sensor
  int Mid = analogRead(A1) ; // middle sensor
  // middle one on black
  if (Mid > 100) {
    // left and right ones on white
    if (L < 100 && R < 100)
      go(sV, sV);
    //A2
    // all on black
    else if (L > 100 && R > 100)
      go(bV, bV);
    // left on black turn until it is on white
    else if (L > R) {
      if (L > 100)
        go(tV, 0);
      else
        go(sV, sV);
    }
    // right on black turn until it is on white
    else if ( R > L) {
      if (R > 100)
        go(0, tV);
      else
        go(sV, sV);
    }
  }

  // middle one on white
  else {
    // all on white
    if (L < 100 && R < 100)
      go(bV, bV);
    //A2 left
    //left and right ones on black
    else if (L > 100 && R > 100)
      go(bV, bV);
    // left on black turn until it is on white
    else if (L > R) {
      if ( L > 100)
        go(tV, 0 );
      else
        go(sV, sV);
    }
    // right on black turn until it is on white
    else if (L < R) {
      if (R > 100)
        go(0, tV);
      else
        go(sV, sV);
    }
  }
}
