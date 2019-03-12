#include<EEPROM.h>
#include "I2Cdev.h"
#include "HMC5883L.h"
#include <HTInfraredSeeker.h>
HMC5883L mag;

int16_t mx, my, mz;
int dir=0, last_pos=0;
float a,b,c, x;
int r, h=15;
bool st;
float deg=0, pk=1.25, ps=.7;
float zero=0.00f, current;

bool line_detected;
long long begin_time=0;

int ir[]={150,180,210,225,310,0,40,120,160,210};
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
void turnoff(){
  analogWrite(12,0);
  analogWrite(13,0);
  analogWrite(2,0);
  analogWrite(3,0);
  analogWrite(6,0);
  analogWrite(7,0);
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
    analogWrite(7,st*deg);
    analogWrite(6,(!st)*deg);
    analogWrite(2,st*deg);
    analogWrite(3,(!st)*deg);
    analogWrite(13,st*deg);
    analogWrite(12,(!st)*deg);
  }
}
void getOut(){
  turnoff();
  delay(100);
  align();
  turnoff();
  delay(100);
  a=sin((dir-300)*M_PI/180)*180;
  b=sin((dir-60)*M_PI/180)*180;
  c=sin((dir-180)*M_PI/180)*180;
  
  a>0? st=0: st=1;
  a=abs(a);
  a=min(max(a,0),255);
  analogWrite(13,st*a);
  analogWrite(12,(!st)*a);

  b>0? st=0: st=1;
  b=abs(b);
  b=min(max(b,0),255);
  analogWrite(2,st*b);
  analogWrite(3,(!st)*b);
  
  c>0? st=0: st=1;
  c=abs(c);
  c=min(max(c,0),255);
  analogWrite(7,st*c);
  analogWrite(6,(!st)*c);
}

void setup() {
  EEPROM.get(0,zero);
  Wire.begin();
  Serial.begin(9600);
  mag.initialize();
  InfraredSeeker::Initialize();
  
  zero*=180/M_PI;

  pinMode(A1,INPUT);
  
  pinMode(6,OUTPUT);  //A
  pinMode(7,OUTPUT);
  pinMode(2,OUTPUT);  //B
  pinMode(3,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT); //C
  turnoff();
  

  pinMode(A7,INPUT);
  pinMode(A8,INPUT);  //Left
  pinMode(A9,INPUT);
  
  pinMode(A10,INPUT);
  pinMode(A11,INPUT); //Middle
  pinMode(A12,INPUT);

  pinMode(A13,INPUT);
  pinMode(A14,INPUT); //Right
  pinMode(A15,INPUT);
  for(int i=39; i<=53; i++){
    pinMode(i,OUTPUT);
  }
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

  for(int i=7; i<=15; i++){
    line[i-6]=analogRead(i);
  }
  for(int i=1; i<=9; i++){
    Serial.print(line[i]);
    Serial.print('\t');
  }
  
  if(line[3]>=175||line[2]>=265||line[1]>=210){
    if(dir<50&&dir>300) dir=180;
    else dir=90;
    line_detected=1;
  }
  if(line[9]>=135||line[8]>=175||line[7]>=150){
    if(dir<50&&dir>300) dir=180;
    else dir=270;
    line_detected=1;
  }
  if((line[9]>=135||line[8]>=175||line[7]>=150)&&(line[3]>=175||line[2]>=265||line[1]>=210)){
    if(dir<90&&dir>270) dir=180;
    else dir=0;
    line_detected=1;
  }
  if(line[6]>=210||line[5]>=225||line[4]>=270){
    dir=180;
    line_detected=1;
  }

  if(line_detected){
    getOut();
    delay(400);
    turnoff();
    align();
    turnoff();
    delay(100);
    line_detected=0;
  }
  
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
  
  dir=ir[IR.Direction];
    
  a=sin((dir-300)*M_PI/180)*200;
  b=sin((dir-60)*M_PI/180)*200;
  c=sin((dir-180)*M_PI/180)*200;
  
  a>0? st=0: st=1;
  a=abs(a);
  st? a+=deg : a-=deg;
  if(IR.Strength<=120)a-=IR.Strength*ps;
  a=min(max(a,0),255);
  analogWrite(13,st*a);
  analogWrite(12,(!st)*a);

  b>0? st=0: st=1;
  b=abs(b);
  st? b+=deg : b-=deg;
  if(IR.Strength<=120)b-=IR.Strength*ps;
  b=min(max(b,0),255);
  analogWrite(2,st*b);
  analogWrite(3,(!st)*b);
  
  c>0? st=0: st=1;
  c=abs(c);
  st? c+=deg : c-=deg;
  if(IR.Strength<=120)c-=IR.Strength*ps;
  c=min(max(c,0),255);
  analogWrite(7,st*c);
  analogWrite(6,(!st)*c);

  Serial.print(a);
  Serial.print('\t');
  Serial.print(b);
  Serial.print('\t');
  Serial.println(c);
}
