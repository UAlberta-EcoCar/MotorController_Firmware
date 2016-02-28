int BR = 9600; //Setup baud rate
void setup() {
  // put your setup code here, to run once:
    pinMode(A0, INPUT);
    Serial.begin(BR);
}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.print("Current Sensor: ");
    Serial.println(analogRead(A0));
    delay(400);
}
