#define E1 5
#define M1 4
#define E2 6
#define M2 7
#define sV 180
#define tV 255
// global variables declarations
int stopped = 0;

void setup() {
  // pins setup
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
}


void go(int v1, int v2) {
  analogWrite(E1, v1); // PWM control
  analogWrite(E2, v2); // PWM control
}

void loop() {
  int R = analogRead(A2) ;//right
  int L = analogRead(A0) ;
  int Mid = analogRead(A1) ;
  int T1 = abs(Mid - R);
  int T2 = abs(Mid - L);
  int T = abs(T1 - T2);

  if (Mid > 180) {
    if (T <= 180) {
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      go(sV, sV);
    }
    //A2
    else {
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      if (T1 > T2) {
        if (T >= 100)
          go(tV, 0);
        else
          go(0, 0);
      }
      else {
        if (T >= 100)
          go(0, tV);
        else
          go(0, 0);
      }
    }
  }
  else {
    if (T <= 180) {
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      go(sV, sV);
    }
    //A2 left
    else {
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      if (T2 > T1) {
        if (T >= 100 && Mid <100)
          go(tV, 0);
        else
          go(0, 0);
      }
      else {
        if (T >= 100 && Mid < 100)
          go(0, tV);
        else
          go(0, 0);
      }
    }
  }
}
