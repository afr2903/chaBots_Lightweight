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
int trig[]= {0, 40, 37};
int echo[]= {0, 42, 35};
int us[3];

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

void setup() {
  EEPROM.get(0,zero);
  Wire.begin();
  Serial.begin(9600);
  mag.initialize();
  InfraredSeeker::Initialize();
  zero*=180/M_PI;
  
  for(int i=1; i<=2; i++){
    pinMode(trig[i], OUTPUT);
    pinMode(echo[i], INPUT);
  }
  Serial.begin(9600);

  for(int i=2; i<=13; i++) pinMode(i,OUTPUT); //Drivers
  digitalWrite(9,1);  //A
  digitalWrite(8,0);
  digitalWrite(2,0);  //B
  digitalWrite(3,1);
  digitalWrite(13,1); //C
  digitalWrite(12,0);
}

void loop() {
  for(int i=1; i<=2; i++){
    digitalWrite(trig[i], LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trig[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trig[i], LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    us[i]= pulseIn(echo[i], HIGH);
    // Calculating the distance
    us[i]= us[i]/29/2;
    // Prints the distance on the Serial Monitor
    Serial.print(us[i]);
    Serial.print("\t");
  }

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

  dir= us[1]<us[2]? max(0,80-us[1]) : 360-max(0,80-us[2]);
  Serial.println(dir);

  movement(230,0,dir);
}
