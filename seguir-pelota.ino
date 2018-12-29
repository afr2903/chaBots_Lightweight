#include<Wire.h>
#include "HTInfraredSeeker.h" 

void turnleft(int pwm){
  analogWrite(2,pwm);
  digitalWrite(22,HIGH);
  digitalWrite(24,LOW);

  analogWrite(3,pwm);
  digitalWrite(23,HIGH);
  digitalWrite(25,LOW);

  analogWrite(6,pwm);
  analogWrite(7,LOW);
}
void turnright(int pwm){
  analogWrite(2,pwm);
  digitalWrite(22,LOW);
  digitalWrite(24,HIGH);

  analogWrite(3,pwm);
  digitalWrite(23,LOW);
  digitalWrite(25,HIGH);

  analogWrite(6,LOW);
  analogWrite(7,pwm);
}
void fwd(int pwm){
  analogWrite(2,pwm);
  digitalWrite(22,LOW);
  digitalWrite(24,HIGH);

  analogWrite(3,pwm);
  digitalWrite(23,HIGH);
  digitalWrite(25,LOW);

  analogWrite(6,LOW);
  analogWrite(7,LOW);
}
void off(){
  analogWrite(2,0);
  digitalWrite(22,LOW);
  digitalWrite(24,LOW);

  analogWrite(0,0);
  digitalWrite(23,LOW);
  digitalWrite(25,LOW);

  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
}

void setup() {
  Wire.begin();
  Serial.begin(9600); //Starts the Serial Monitor so we can read output
  InfraredSeeker::Initialize(); //initializes the IR sensor
  pinMode(22,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(A9,OUTPUT);

  pinMode(23,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(A12,OUTPUT);

  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);

  digitalWrite(A9,HIGH);
  digitalWrite(A12,HIGH);
}

void loop() {
  InfraredResult IR = InfraredSeeker::ReadAC();
  Serial.print(IR.Direction);
  Serial.print("\t");                   
  Serial.print(IR.Strength);
  Serial.println();
  delay(100); //optional
  int pwm=150-IR.Strength;
  int dir=abs(5-IR.Direction);
  
  if(IR.Direction==5) fwd(pwm);
  else if(IR.Direction>5) turnright(dir*15);
  else turnleft(dir*15);

  if(IR.Strength>=125) off();
  
}
