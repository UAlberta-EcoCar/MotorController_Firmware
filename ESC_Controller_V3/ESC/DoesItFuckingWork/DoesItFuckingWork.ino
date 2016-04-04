int pin2 = A2;
int pin1 = A1;
int pin0 = A0;

void setup() {
  pinMode(pin2, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin0, OUTPUT);
}

void loop() {
  digitalWrite(pin2, HIGH);
  delay(200);
  digitalWrite(pin1, HIGH);
  delay(200);
  digitalWrite(pin0, HIGH);
  delay(200);
  digitalWrite(pin2, LOW);
  delay(200);
  digitalWrite(pin1, LOW);
  delay(200);
  digitalWrite(pin0, LOW);
  delay(200);
}
