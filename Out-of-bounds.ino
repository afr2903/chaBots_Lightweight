#include<EEPROM.h>
#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;

int16_t mx, my, mz;
int dir=0;
float a,b,c;
bool st;
float deg=0, pk=3;
bool line_detected;
unsigned long long begin_time;
float zero=0.00f, current;

int ir[]={180,180,210,225,270,0,45,4,5,5};
int line[4];
/*
A---B
|   |
\-C-/
*/

void setup() {
  Wire.begin();
  Serial.begin(9600);
  mag.initialize();
  EEPROM.get(0,zero);
  zero*=180/M_PI;
  
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT); //A

  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT); //B

  pinMode(5,OUTPUT);
  pinMode(3,OUTPUT);  //C


  pinMode(A7,INPUT);
  pinMode(A8,INPUT);  //Right
  pinMode(A9,INPUT);
  
  pinMode(A10,INPUT);
  pinMode(A11,INPUT); //Middle
  pinMode(A12,INPUT);

  pinMode(A13,INPUT);
  pinMode(A14,INPUT); //Left
  pinMode(A15,INPUT);
  for(int i=39; i<=53; i++){
    pinMode(i,OUTPUT);
  }
  digitalWrite(44,1);
  digitalWrite(46,1); //Right
  digitalWrite(48,0);
  
  digitalWrite(39,0);
  digitalWrite(41,1); //Middle
  digitalWrite(43,1);
  
  digitalWrite(47,0);
  digitalWrite(49,1); //Left
  digitalWrite(53,1);
}

void reset(float heading){
  EEPROM.put(0,heading);
  while(analogRead(1));
}

void loop() {
  //if(line_detected) if(millis()-begin_time >= 400) line_detected=0;
  
  for(int i=7; i<=15; i+=3){
    line[i/3-1]=(analogRead(i)+analogRead(i+1)+analogRead(i+2))/3;
  }
  Serial.print(line[3]);
  Serial.print('\t');
  Serial.print(line[2]);
  Serial.print('\t');
  Serial.print(line[1]);
  Serial.print('\t');
  if(line[2]>=70){
    dir=180;
    line_detected=1;
    begin_time= millis();
  }
  if(line[3]>=50){
    dir=90;
    line_detected=1;
    begin_time= millis();
  }
  if(line[1]>=75){
    dir=270;
    line_detected=1;
    begin_time= millis();
  }

  if(line_detected){
    if(deg>180) deg=(deg-180)*-1;
    a=sin((dir-300)*M_PI/180)*230;
    b=sin((dir-60)*M_PI/180)*230;
    c=sin((dir-180)*M_PI/180)*230; 

    a>0? st=0: st=1;
    a=abs(a);
    a=min(max(a,5),255);
    analogWrite(7,st*a);
    analogWrite(6,(!st)*a);
  
    b>0? st=0: st=1;
    b=abs(b);
    b=min(max(b,5),255);
    analogWrite(11,st*b);
    analogWrite(10,(!st)*b);
  
    c>0? st=0: st=1;
    c=abs(c);
    c=min(max(c,5),255);
    analogWrite(5,st*c);
    analogWrite(3,(!st)*c);
    delay(400);
    line_detected=0;
  }
  
  if(!line_detected){
    mag.getHeading(&mx, &my, &mz);
    float heading = atan2(my, mx);
    if(heading<0) heading+= 2*M_PI;
    if(analogRead(1)) reset(heading);
    heading=heading * 180/M_PI; 
    current= heading;
  
    deg = current-zero;
    if(deg<0) deg+=360;
    if(deg>180) deg-=360;
    Serial.println(deg);
    //Serial.println('\t');

    dir=90;
  }
  
  a=sin((dir-300)*M_PI/180)*230;
  b=sin((dir-60)*M_PI/180)*230;
  c=sin((dir-180)*M_PI/180)*230;
  
  a>0? st=0: st=1;
  a=abs(a);
  st? a+=pk*deg : a-=pk*deg;
  a=min(max(a,5),255);
  analogWrite(7,st*a);
  analogWrite(6,(!st)*a);

  b>0? st=0: st=1;
  b=abs(b);
  st? b+=pk*deg : b-=pk*deg;
  b=min(max(b,5),255);
  analogWrite(11,st*b);
  analogWrite(10,(!st)*b);

  c>0? st=0: st=1;
  c=abs(c);
  st? c+=pk*deg : c-=pk*deg;
  c=min(max(c,5),255);
  analogWrite(5,st*c);
  analogWrite(3,(!st)*c);

}
