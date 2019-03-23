#include<EEPROM.h>
#include "I2Cdev.h"
#include "HMC5883L.h"
#include <HTInfraredSeeker.h>
HMC5883L mag;

int16_t mx, my, mz;
int dir=0, last_pos=0, dis_back=70, dif;
float a,b,c, x;
float r, h=5, w=135;
bool st, ball_detected, left, right, aligned, back;
float deg=0, pk=.7, ps=.5;
float zero=0.00f, current;

int tsop[]= {0, 0, 4, 5};
int trig[]= {0, 42, 35,5};
int echo[]= {0, 40, 37,4};
int us[3];

bool line_detected;
long long begin_time=0;

int ir[]={150,200,220,270,285,0,65,100,150,190};  //New
//int ir[]={150,180,210,225,310,0,50,120,150,210};
int line[10];

void mov(int pwm, int strength, int dir){
  if(dir==-1){
    turnoff();
    return;  
  }
  a=sin((dir-300)*M_PI/180)*pwm;
  b=sin((dir-60)*M_PI/180)*pwm;
  c=sin((dir-180)*M_PI/180)*pwm;
  
  a>0? st=0: st=1;
  a=abs(a);
  st? a+=deg : a-=deg;
  a-=strength*ps;
  a=min(max(a,5),255);
  analogWrite(12,st*a);
  analogWrite(13,(!st)*a);

  b>0? st=0: st=1;
  b=abs(b);
  st? b+=deg : b-=deg;
  b-=strength*ps;
  b=min(max(b,5),255);
  analogWrite(2,st*b);
  analogWrite(3,(!st)*b);
  
  c>0? st=0: st=1;
  c=abs(c);
  st? c+=deg : c-=deg;
  c-=strength*ps;
  c=min(max(c,5),255);
  analogWrite(6,st*c);
  analogWrite(7,(!st)*c); 
}
void fwd(){
  analogWrite(12,0);
  analogWrite(13,255);
  analogWrite(2,255);
  analogWrite(3,0);
}
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
void us_read(){
  for(int i=1; i<=2; i++){
    digitalWrite(trig[i], LOW);
    delayMicroseconds(2);
    digitalWrite(trig[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trig[i], LOW);
    us[i]= pulseIn(echo[i], HIGH);
    us[i]= us[i]/29/2;
  }
}
void us_back(){
  digitalWrite(trig[3], LOW);
  delayMicroseconds(2);
  digitalWrite(trig[3], HIGH);
  delayMicroseconds(10);
  digitalWrite(trig[3], LOW);
  us[3]= pulseIn(echo[3], HIGH);
  us[3]= us[3]/29/2;
}
void mag_read(){
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
}
void nothing(){
  dir=-1;
  us_read();
  mag_read();
  // Prints the distance on the Serial Monitor
  Serial.print(us[1]);
  Serial.print("\t");
  Serial.println(us[2]);
  if(us[1]+us[2]<w){
    Serial.println(":v");
    nothing_back();
    return;
  }
  if(us[1]+us[2]<w)return;
  if(abs(us[1]-us[2])<10){
    align();
    turnoff();
    aligned=1;
    return;
  }
  dir= us[1]<us[2]? 120:240;
  mov(180,0,dir);
  /*while(true){
    dir=-1;
    us_back();
    mag_read();
    if(us[3]<dis_back) break;
    mov(150,0,180);
  }
  align();
  turnoff();*/
}
void nothing_back(){
  dir=-1;
  us_back();
  int d=deg;
  if(d>5||d<-5) align();
  Serial.println(us[3]);
  //dir=180;
  if(us[3]>dis_back){
    Serial.println("v:<<<<");
    mov(120,0,180);
  }
  else{
    align();
    turnoff();
    Serial.println(">>>:v");
  }
}
void align(){
  int d=20;
  while(d<-10||d>10){
    mag_read();
    //Serial.println(d);
    d=deg;
  
    deg>0? st=1:st=0;
    deg=abs(deg);
    deg*=2;
    deg+=20;
    deg=min(deg,255);
    analogWrite(6,st*deg);
    analogWrite(7,(!st)*deg);
    analogWrite(2,st*deg);
    analogWrite(3,(!st)*deg);
    analogWrite(12,st*deg);
    analogWrite(13,(!st)*deg);
  }
}
void getOut(){
  turnoff();
  align();
  turnoff();
  delay(100);
  /*us_read();
  if(us[1]<40&&us[2]>40) right=1;
  if(us[2]<40&&us[1]>40) left=1;*/
  a=sin((dir-300)*M_PI/180)*180;
  b=sin((dir-60)*M_PI/180)*180;
  c=sin((dir-180)*M_PI/180)*180;
  
  a>0? st=0: st=1;
  a=abs(a);
  a=min(max(a,0),255);
  analogWrite(12,st*a);
  analogWrite(13,(!st)*a);

  b>0? st=0: st=1;
  b=abs(b);
  b=min(max(b,0),255);
  analogWrite(2,st*b);
  analogWrite(3,(!st)*b);
  
  c>0? st=0: st=1;
  c=abs(c);
  c=min(max(c,0),255);
  analogWrite(6,st*c);
  analogWrite(7,(!st)*c);
}

void setup() {
  EEPROM.get(0,zero);
  Wire.begin();
  Serial.begin(9600);
  mag.initialize();
  InfraredSeeker::Initialize();
  
  zero*=180/M_PI;

  pinMode(A1,INPUT);
  pinMode(A0,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  
  pinMode(6,OUTPUT);  //A
  pinMode(7,OUTPUT);
  pinMode(2,OUTPUT);  //B
  pinMode(3,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT); //C

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
  for(int i=1; i<=3; i++) pinMode(trig[i],OUTPUT);
  for(int i=1; i<=3; i++) pinMode(echo[i],INPUT);
   
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
  
  if(line[3]>=100||line[2]>=150||line[1]>=110){
    Serial.print("LEFT");
    //if(dir<50&&dir>300) dir=180;
    dir=100;
    line_detected=1;
  }
  if(line[9]>=80||line[8]>=100||line[7]>=100){
    Serial.print("RIGHT");
    //if(dir<50&&dir>300) dir=180;
    dir=260;
    line_detected=1;
  }
  if((line[9]>=80||line[8]>=100||line[7]>=100)&&(line[3]>=100||line[2]>=150||line[1]>=110)){
    Serial.print("BOTH");
    dir=180;
    line_detected=1;
  }
  if(line[6]>=100||line[5]>=110||line[4]>=150){
    Serial.print("FRONT");
    dir=180;
    line_detected=1;
  }

  if(line_detected){
    getOut();
    delay(400);
    turnoff();
    align();
    turnoff();
    delay(200);
    if(dir==180){
      Serial.println("AAAAAAA");
      begin_time=millis();
      while((millis()-begin_time)<2000){
        nothing(); 
      }
    }
    /*if(left) mov(180,0,270);
    if(right) mov(180,0,90);
    if(left||right) delay(350);
    right=0, left=0;*/
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
  
  r=max(5,130-IR.Strength);
  x= asin(h/r)*180/M_PI;
  dir= ir[IR.Direction]+ (IR.Direction>5? x:-x);
  last_pos= IR.Direction;
  if(IR.Direction>=4&&IR.Direction<=7) dir=ir[IR.Direction];
  Serial.println(x);

  ball_detected= (analogRead(0)<75||analogRead(4)<75||analogRead(5)<75)&&IR.Direction==5;
  if(ball_detected){
    deg*=2.5;
    Serial.println("XD");
    us_read();
    if(us[1]+us[2]<w) dir=0;
    else{
      dif= us[1]-us[2];
      if(dif<-40) dir= max(0,80-us[1]);
      else if(dif<-15) dir= max(0,80-us[1]);
      else if(dif<=15) dir= us[1]<us[2]? max(0,120-us[1]) : 360-max(0,120-us[2]);
      else if(dif<=40) dir= 360-max(0,80-us[2]);
      else dir= 360-max(0,80-us[2]);
    }
    //dir= us[1]+us[2]<w? 0:us[1]<us[2]? max(0,80-us[1]) : 360-max(0,80-us[2]);
    mov(255,0,dir);
  }
  else{
    if(IR.Direction==0){
      back=0;
      if(!aligned) nothing();
      else nothing_back();
    }
    else if(IR.Direction==1||IR.Direction==2||IR.Direction==8||IR.Direction==9){
      aligned=0, back=0;
      if(!back){
        digitalWrite(trig[3], LOW);
        delayMicroseconds(2);
        digitalWrite(trig[3], HIGH);
        delayMicroseconds(10);
        digitalWrite(trig[3], LOW);
        us[3]= pulseIn(echo[3], HIGH);
        us[3]= us[3]/29/2;
        if(us[3]<=dis_back){
          align();
          turnoff();
          back=1;
        }
        else mov(200,IR.Strength,dir);
      }
      else turnoff();
    }
    else{
      aligned=0, back=0;
      mov(200,IR.Strength,dir);
    }
  }

  Serial.print(a);
  Serial.print('\t');
  Serial.print(b);
  Serial.print('\t');
  Serial.println(c);
}
