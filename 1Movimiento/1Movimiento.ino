void setup() {
  // put your setup code here, to run once:
 pinMode(3,OUTPUT);
 pinMode(5,OUTPUT);
 pinMode(6,OUTPUT);
 pinMode(9,OUTPUT);
 pinMode(10,OUTPUT);
 pinMode(11,OUTPUT);
}

int PWM=200;

void loop() {
  // put your main code here, to run repeatedly:
 analogWrite(5,LOW);
 digitalWrite(3,LOW);
 digitalWrite(6,PWM);
 digitalWrite(9,LOW);
 analogWrite(10,LOW); 
 digitalWrite(11,PWM);
}
