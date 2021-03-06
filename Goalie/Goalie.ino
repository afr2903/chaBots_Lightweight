#include<EEPROM.h>
#include "I2Cdev.h"
#include <HTInfraredSeeker.h>
#include <Wire.h>
#include <HMC5883L.h>
HMC5883L mag;

int16_t mx, my, mz;
int dir=0, last_pos=0, dis_back=60, cc=0, last_dir=0;
float a,b,c, x;
float r, h=10, w=135;
bool st, out=0, aligned=0, first=0, five=0;
float deg=0, pk=1.5, ps=.5;
float zero=0.00f, current;
bool color[][3]{
  0,0,0,
  1,0,0,
  0,1,0,
  0,0,1,
  1,1,0,
  1,0,1,
  0,1,1,
  1,1,1  
};

int trig[]= {0, 42, 35,5};
int echo[]= {0, 40, 37,4};
int us[3];

bool line_detected;
long long begin_time=0, change_color=0;;

int ir[]={150,270,270,270,270,0,90,90,90,90};
int line[10];

void mov(int pwm, int strength, int dir){
  a=sin((dir-300)*M_PI/180)*pwm;
  b=sin((dir-60)*M_PI/180)*pwm;
  c=sin((dir-180)*M_PI/180)*(pwm+10);
  
  a>0? st=0: st=1;
  a=abs(a);
  st? a+=deg : a-=deg;
  a-=strength*ps;
  a=min(max(a,5),255);
  analogWrite(13,st*a);
  analogWrite(12,(!st)*a);

  b>0? st=0: st=1;
  b=abs(b);
  st? b+=deg : b-=deg;
  b-=strength*ps;
  b=min(max(b,5),255);
  analogWrite(3,st*b);
  analogWrite(2,(!st)*b);
  
  c>0? st=0: st=1;
  c=abs(c);
  st? c+=deg : c-=deg;
  c-=strength*ps;
  c=min(max(c,5),255);
  analogWrite(6,st*c);
  analogWrite(7,(!st)*c); 
}
void turnoff(){
  analogWrite(12,0);
  analogWrite(13,0);
  analogWrite(2,0);
  analogWrite(3,0);
  analogWrite(6,0);
  analogWrite(7,0);
}
void fwd(){
  analogWrite(13,0);
  analogWrite(12,255);
  analogWrite(3,255);
  analogWrite(2,0);
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
//  if(analogRead(1)) reset(heading);
  heading=heading * 180/M_PI; 
  current= heading;
  deg = current-zero;
  if(deg<0) deg+=360;
  if(deg>180) deg-=360;
  Serial.print(deg);
  Serial.print('\t');
}
void nothing(){
  int dif=0;
  //align();
  while(true){
    us_read();
    //mag_read();
    // Prints the distance on the Serial Monitor
    Serial.print(us[1]);
    Serial.print("\t");
    Serial.println(us[2]);
    if(us[1]+us[2]<w)break;
    if(abs(us[1]-us[2])<10) break;
    dir= us[1]<us[2]? 90:270;
    mov(180,0,dir);
  }
  align();
  turnoff();
}
void align(){
  int d=20;
  while(d<-5||d>5){
    mag_read();
    //Serial.println(d);
    d=deg;
  
    deg>0? st=1:st=0;
    deg=abs(deg);
    deg*=2;
    deg+=60;
    deg=min(deg,255);
    analogWrite(6,st*deg);
    analogWrite(7,(!st)*deg);
    analogWrite(3,st*deg);
    analogWrite(2,(!st)*deg);
    analogWrite(13,st*deg);
    analogWrite(12,(!st)*deg);
  }
}
void getOut(){
  turnoff();
  align();
  turnoff();
     us_read();
  if(us[1]<55){
    if(us[3]>35&&us[3]<45) dir=0;
    else dir=85;
  }
  else if(us[2]<55){
    if(us[3]>35&&us[3]<45) dir=0;
    else dir=275;
  }
  else dir=180, out=0;
  
  a=sin((dir-300)*M_PI/180)*180;
  b=sin((dir-60)*M_PI/180)*180;
  c=sin((dir-180)*M_PI/180)*188;
  
  a>0? st=0: st=1;
  a=abs(a);
  a=min(max(a,0),255);
  analogWrite(13,st*a);
  analogWrite(12,(!st)*a);

  b>0? st=0: st=1;
  b=abs(b);
  b=min(max(b,0),255);
  analogWrite(3,st*b);
  analogWrite(2,(!st)*b);
  
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
  ++cc%=8;
  digitalWrite(39,color[cc][0]);
  digitalWrite(41,color[cc][1]); //Middle
  digitalWrite(43,color[cc][2]);
  
  Serial.print(zero);
  Serial.print('\t');

  for(int i=7; i<=15; i++){
    line[i-6]=analogRead(i);
  }
  for(int i=1; i<=9; i++){
    Serial.print(line[i]);
    Serial.print('\t');
  }

  us_back();
  //us_read();
  if(us[3]>22){
    Serial.print("xdxdxd");
    line_detected=1;
    /*if(us[1]<55) dir=85;
    else if(us[2]<55) dir=275;
    else dir=180, out=0;*/
  }
  Serial.println("US");
  Serial.print(us[3]);
  Serial.print("\t");
  Serial.print(us[2]);
  Serial.print("\t");
  Serial.println(us[1]);
  
  /*if(line[3]>=60||line[2]>=110||line[1]>=140){
    dir=90;
    line_detected=1;
  }
  if(line[9]>=95||line[8]>=80||line[7]>=40){
    dir=270;
    line_detected=1;
  }*/

  if(line_detected){
    getOut();
    if(dir==180) delay(280), Serial.print("FRONT");
    else delay(300);
    turnoff();
    align();
    turnoff();
    delay(50);
    if(dir==85) last_pos=4, out=1, Serial.print("LEFT");
    if(dir==275) last_pos=6, out=1, Serial.print("RIGHT");
    line_detected=0;
  }

  Serial.print(last_pos);
  Serial.print("\t");
  Serial.println(out);
  InfraredResult IR = InfraredSeeker::ReadAC();
  Serial.print(IR.Direction);
  Serial.print("\t");
  Serial.print(IR.Strength);
  Serial.print("\t");

  if(!five){
    mag.getHeading(&mx, &my, &mz);
    float heading = atan2(my, mx);
    if(heading<0) heading+= 2*M_PI;
//    if(analogRead(1)) reset(heading);
    heading=heading * 180/M_PI; 
    current= heading;
    if(!first){
      zero=current;
      first=1;  
    }
    deg = current-zero;
    if(deg<0) deg+=360;
    if(deg>180) deg-=360;
    Serial.print(deg);
    Serial.print('\t');
  
    if(deg<0) deg+=360;
    if(deg>180) deg-=360;
    Serial.print(deg);
    Serial.print('\t');
    deg*=pk;
  }

  dir=ir[IR.Direction];

  if(IR.Direction!=5)aligned=0, five=0;
  if(out) if((last_pos==4&&IR.Direction>=5)||(last_pos==6&&IR.Direction<=5)) out=0;
  if(!out){
    if(IR.Direction==0||IR.Direction==1||IR.Direction==9) nothing();
    else if(IR.Direction==5&&IR.Strength>40){
      five=1;
      if(IR.Strength>=110) mov(255,0,dir);
      else{
        if(!aligned){
          align();
          aligned=1;  
        }
        turnoff();
      }
    }
    else{
      if(IR.Strength>40){
        if(dir==270) last_dir=270;
        else last_dir=90;
        mov(230-((4-abs(IR.Direction-5))*10),0,dir);
      }
    }
    //4-abs(IR.Direction-5)
  }
  

  /*Serial.print(a);
  Serial.print('\t');
  Serial.print(b);
  Serial.print('\t');
  Serial.println(c);*/
}
