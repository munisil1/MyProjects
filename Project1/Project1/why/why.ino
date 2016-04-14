#define TRCT_PIN A1
void setup() {
 pinMode(TRCT_PIN, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  Serial.print("Color value = ");
  Serial.println(analogRead(TRCT_PIN));
  delay(700);
}

??????????
