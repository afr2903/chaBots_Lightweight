void setup() {
  Serial.begin(9600);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(6,OUTPUT);  //A
  pinMode(7,OUTPUT);
  
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);  //B
  pinMode(3,OUTPUT);

  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(10,OUTPUT); //C
  pinMode(11,OUTPUT);
  

  digitalWrite(9,1);
  digitalWrite(8,0);
  
  digitalWrite(2,0);
  digitalWrite(3,1);
 
  digitalWrite(13,1);
  digitalWrite(12,0);
}

void loop() {
  analogWrite(10,255);
  analogWrite(11,5);
  delay(1000);
  analogWrite(10,5);
  analogWrite(11,255); //C
  delay(1000);
  analogWrite(11,5);

  analogWrite(7,255);
  analogWrite(6,5);
  delay(1000);
  analogWrite(7,5);
  analogWrite(6,255);  //A
  delay(1000);
  analogWrite(6,5);

  analogWrite(4,255);
  analogWrite(5,5);
  delay(1000);
  analogWrite(4,5);
  analogWrite(5,255);  //B
  delay(1000);
  analogWrite(5,5);
}
