#include<EEPROM.h>
#include "I2Cdev.h"
#include "HMC5883L.h"
#include "HTInfraredSeeker.h"
HMC5883L mag;

int16_t mx, my, mz;
int dir=0;
float a,b,c;
bool st;
float deg=0, pk=3;
float zero=0.00f, current;

int ir[]={180,180,210,225,240,0,30,120,150,180};
/*
A---B
|   |
\-C-/
*/

void setup() {
  Wire.begin();
  Serial.begin(9600);
  mag.initialize();
  InfraredSeeker::Initialize();
  EEPROM.get(0,zero);
  zero*=180/M_PI;
  
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT); //A

  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT); //B

  pinMode(5,OUTPUT);
  pinMode(3,OUTPUT);  //C
}

void reset(float heading){
  EEPROM.put(0,heading);
  while(analogRead(1));
}

void loop() {
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

  dir= ir[IR.Direction];
  
  a=sin((dir-300)*M_PI/180)*230;
  b=sin((dir-60)*M_PI/180)*230;
  c=sin((dir-180)*M_PI/180)*230;
  
  a>0? st=0: st=1;
  a=abs(a);
  st? a+=deg : a-=deg;
  a=min(max(a,5),255);
  analogWrite(7,st*a);
  analogWrite(6,(!st)*a);

  b>0? st=0: st=1;
  b=abs(b);
  st? b+=deg : b-=deg;
  b=min(max(b,5),255);
  analogWrite(11,st*b);
  analogWrite(10,(!st)*b);

  c>0? st=0: st=1;
  c=abs(c);
  st? c+=deg : c-=deg;
  c=min(max(c,5),255);
  analogWrite(5,st*c);
  analogWrite(3,(!st)*c);

  Serial.print(a);
  Serial.print('\t');
  Serial.print(b);
  Serial.print('\t');
  Serial.println(c);
}
