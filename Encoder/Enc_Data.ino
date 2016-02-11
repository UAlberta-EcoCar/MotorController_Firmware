#define PIN_A 10
#define PIN_B 11
#define PIN_C 12

#define DELAY 100

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_A, INPUT);
  pinMode(PIN_B, INPUT);
  pinMode(PIN_C, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int a = digitalRead(PIN_A);
  int b = digitalRead(PIN_B);
  int c = digitalRead(PIN_C);
  Serial.print(a);
  Serial.print(",");
  Serial.print(b);
  Serial.print(",");
  Serial.println(c);
  //delay(DELAY);
}
