void setup() {
  Serial.begin(9600);
  pinMode(6, OUTPUT); //A
  pinMode(7, OUTPUT);

  pinMode(2, OUTPUT); //B
  pinMode(3, OUTPUT);

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT); //C
}

void loop() {
  analogWrite(13, 255);
  analogWrite(12, 0);
  delay(1000);
  analogWrite(13, 0);
  analogWrite(12, 255); //A
  delay(1000);
  analogWrite(12, 0);

  analogWrite(2, 255);
  analogWrite(3, 0);
  delay(1000);
  analogWrite(2, 0);
  analogWrite(3, 255); //B
  delay(1000);
  analogWrite(3, 0);

  analogWrite(6, 255);
  analogWrite(7, 0);
  delay(1000);
  analogWrite(6, 0);
  analogWrite(7, 255); //C
  delay(1000);
  analogWrite(7, 0);
}
