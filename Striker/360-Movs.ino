#include<EEPROM.h>
#include "I2Cdev.h"
#include "HMC5883L.h"
#include <HTInfraredSeeker.h>
HMC5883L mag;

int16_t mx, my, mz;
int dir;
float a,b,c,v;
bool st;
float deg=0, pk=.75, ps=.2;
float zero=0.00f, current;
long long begin_time=0;

int ir[]={150,180,210,225,310,0,50,120,150,210};
/*
A---B
|   |
\-C-/
*/
void reset(float heading){
  EEPROM.put(0,heading);
  zero=heading;
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

void setup() {
  EEPROM.get(0,zero);
  Wire.begin();
  Serial.begin(9600);
  mag.initialize();
  InfraredSeeker::Initialize();
  
  zero*=180/M_PI;

  pinMode(A1,INPUT);

  pinMode(6, OUTPUT); //A
  pinMode(7, OUTPUT);
  pinMode(2, OUTPUT); //B
  pinMode(3, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT); //C

}

void loop() {
  Serial.print(zero);
  Serial.print('\t');
  
  InfraredResult IR = InfraredSeeker::ReadAC();
  Serial.print(IR.Direction);
  Serial.print("\t");
  Serial.print(IR.Strength);
  Serial.print("\t");

  
  mag.getHeading(&mx, &my, &mz);
  float heading = atan2(my, mx);
  if(heading<0) heading+= 2*M_PI;
  if(analogRead(1))reset(heading);
  heading=heading * 180/M_PI; 
  current= heading;

  deg = current-zero;
  if(deg<0) deg+=360;
  if(deg>180) deg-=360;
  Serial.print(deg);
  Serial.print('\t');
  //deg*=pk;

  dir= 90;
  
  a=sin((dir-300)*M_PI/180)*230;
  b=sin((dir-60)*M_PI/180)*230;
  c=sin((dir-180)*M_PI/180)*230;
  v= abs(a)+abs(b)+abs(c);
  //pk=v/180;
  deg*=pk;
  
  a>0? st=0: st=1;
  a=abs(a);
  st? a+=deg : a-=deg;
  if(IR.Strength<=120)a-=IR.Strength*ps;
  a=min(max(a,0),255);
  analogWrite(12,st*a);
  analogWrite(13,(!st)*a);

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
  analogWrite(6,st*c);
  analogWrite(7,(!st)*c);
  
  
  Serial.print(a);
  Serial.print('\t');
  Serial.print(b);
  Serial.print('\t');
  Serial.print(c);
  Serial.print('\t');
  Serial.println(pk);
}
