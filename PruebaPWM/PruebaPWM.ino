void setup() {
  // put your setup code here, to run once:
pinMode(3,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
pinMode(9,OUTPUT);
pinMode(10,OUTPUT);
pinMode(11,OUTPUT);
}
int v=50;

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(3,LOW);
analogWrite(5,v);
digitalWrite(6,LOW);
analogWrite(9,v);
digitalWrite(10,LOW);
analogWrite(11,v);

/*
delay(3000);
digitalWrite(5,LOW);
analogWrite(3,255);
delay(3000);
*/

/*
digitalWrite(3,HIGH);
digitalWrite(5,HIGH);
digitalWrite(6,HIGH);
digitalWrite(9,LOW);
digitalWrite(10,LOW);
digitalWrite(11,LOW);
delay(5000);
digitalWrite(3,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
digitalWrite(9,HIGH);
digitalWrite(10,HIGH);
digitalWrite(11,HIGH);
delay(5000);
*/
}
