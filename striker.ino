#include<EEPROM.h>
#include "I2Cdev.h"
#include "HMC5883L.h"
#include "HTInfraredSeeker.h"
HMC5883L mag;

int16_t mx, my, mz;
int dir=0;
float a,b,c;
bool st;
float deg=0, pk=2, ps=.8;
float zero=0.00f, current;
bool line_detected;
unsigned long long begin_time;

int ir[]={150,180,210,225,240,0,30,120,150,180};
int line[10];
/*
A---B
|   |
\-C-/
*/
void reset(float heading){
  EEPROM.put(0,heading);
  delay(500);
}
void movement(int pwm, int strength, int dir){
  a=sin((dir-300)*M_PI/180)*pwm;
  b=sin((dir-60)*M_PI/180)*pwm;
  c=sin((dir-180)*M_PI/180)*pwm;
  
  a>0? st=0: st=1;
  a=abs(a);
  st? a+=deg : a-=deg;
  a-=strength*ps;
  a=min(max(a,5),255);
  analogWrite(11,st*a);
  analogWrite(10,(!st)*a);

  b>0? st=0: st=1;
  b=abs(b);
  st? b+=deg : b-=deg;
  b-=strength*ps;
  b=min(max(b,5),255);
  analogWrite(5,st*b);
  analogWrite(4,(!st)*b);
  
  c>0? st=0: st=1;
  c=abs(c);
  st? c+=deg : c-=deg;
  c-=strength*ps;
  c=min(max(c,5),255);
  analogWrite(7,st*c);
  analogWrite(6,(!st)*c);
  
}
void turnoff(){
  analogWrite(11,0);
  analogWrite(10,0);
  analogWrite(7,0);
  analogWrite(6,0);
  analogWrite(5,0);
  analogWrite(4,0);
}
void align(){
  int d=20;
  while(d<-5||d>5){
    mag.getHeading(&mx, &my, &mz);
    float heading = atan2(my, mx);
    if(heading<0) heading+= 2*M_PI;
    if(analogRead(1)) reset(heading);
    heading=heading * 180/M_PI; 
    current= heading;
    deg = current-zero;
    if(deg<0) deg+=360;
    if(deg>180) deg-=360;
    Serial.println(d);
    d=deg;
  
    deg>0? st=1:st=0;
    deg=abs(deg);
    deg*=2;
    deg+=60;
    deg=min(deg,255);
    analogWrite(6,st*deg);
    analogWrite(7,(!st)*deg);
    analogWrite(5,st*deg);
    analogWrite(4,(!st)*deg);
    analogWrite(11,st*deg);
    analogWrite(10,(!st)*deg);
  }
}
void getOut(int dir){
  turnoff();
  deg=0;
  movement(180,0,dir);
  delay(400);
  turnoff();
  delay(100);
  align();
  turnoff();
  delay(100);
}

void setup() {
  EEPROM.get(0,zero);
  Wire.begin();
  Serial.begin(9600);
  mag.initialize();
  InfraredSeeker::Initialize();
  zero*=180/M_PI;

  pinMode(A1,INPUT);
  
  for(int i=2; i<=13; i++) pinMode(i,OUTPUT); //Drivers
  digitalWrite(9,1);  //A
  digitalWrite(8,0);
  digitalWrite(2,0);  //B
  digitalWrite(3,1);
  digitalWrite(13,1); //C
  digitalWrite(12,0);

  for(int i=7; i<=15; i++) pinMode(i,INPUT); //Line sensors
  for(int i=39; i<=53; i++) pinMode(i,OUTPUT); //Line sensor leds
  digitalWrite(44,1);
  digitalWrite(46,0); //Left
  digitalWrite(48,0);
  digitalWrite(39,0);
  digitalWrite(41,0); //Middle
  digitalWrite(43,1);
  digitalWrite(47,0);
  digitalWrite(49,0); //Right
  digitalWrite(53,1);
}


void loop() {
  Serial.print(zero);
  Serial.print('\t');

  for(int i=7; i<=15; i++) line[i-6]=analogRead(i);
  
  for(int i=1; i<=9; i++){
    Serial.print(line[i]);
    Serial.print('\t');
  }
  if(line[6]>=110||line[5]>=110||line[4]>=110) getOut(180);
  if(line[9]>=100||line[8]>=120||line[7]>=110) getOut(270);
  if(line[3]>=100||line[2]>=100||line[1]>=100) getOut(90);

  InfraredResult IR = InfraredSeeker::ReadAC();
  Serial.print(IR.Direction);
  Serial.print("\t");
  Serial.print(IR.Strength);
  Serial.print("\t");

  mag.getHeading(&mx, &my, &mz);
  float heading = atan2(my, mx);
  if(heading<0) heading+= 2*M_PI;
  if(analogRead(1)) reset(heading);
  heading=heading * 180/M_PI; 
  current= heading;

  deg = current-zero;
  if(deg<0) deg+=360;
  if(deg>180) deg-=360;
  Serial.print(deg);
  Serial.print('\t');
  deg*=pk;
  
  movement(230,IR.Strength,IR.Direction);

  Serial.print(a);
  Serial.print('\t');
  Serial.print(b);
  Serial.print('\t');
  Serial.println(c);
}
