int enc;

void setup() {
  pinMode(2, INPUT);
  Serial.begin(9600);

}

void loop() {
  enc = digitalRead(2);
  Serial.println(enc);

}
