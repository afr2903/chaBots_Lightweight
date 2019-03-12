#include<Wire.h>
#include "HTInfraredSeeker.h" 

void turnleft(int pwm){
  analogWrite(7,pwm);
  digitalWrite(6,LOW);

  analogWrite(11,pwm);
  digitalWrite(10,LOW);

  analogWrite(5,pwm);
  analogWrite(3,LOW);
}
void turnright(int pwm){
  analogWrite(6,pwm);
  digitalWrite(7,LOW);

  analogWrite(10,pwm);
  digitalWrite(11,LOW);

  analogWrite(3,pwm);
  analogWrite(5,LOW);
}
void fwd(int pwm){
  analogWrite(6,pwm);
  digitalWrite(7,LOW);

  analogWrite(11,pwm);
  digitalWrite(10,LOW);

  analogWrite(5,LOW);
  analogWrite(3,LOW);
}
void off(){
  digitalWrite(7,LOW);
  digitalWrite(6,LOW);

  digitalWrite(11,LOW);
  digitalWrite(10,LOW);

  digitalWrite(5,LOW);
  digitalWrite(3,LOW);
}

void setup() {
  Wire.begin();
  Serial.begin(9600); //Starts the Serial Monitor so we can read output
  InfraredSeeker::Initialize(); //initializes the IR sensor
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT); //A

  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT); //B

  pinMode(5,OUTPUT);
  pinMode(3,OUTPUT);  //C
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
  else if(IR.Direction>5) turnright(dir*20);
  else turnleft(dir*20);

  if(IR.Strength>=120) off();
  
}
