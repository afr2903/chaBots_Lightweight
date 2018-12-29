#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;

int16_t mx, my, mz;
float deg; //degree
float pk= 1.35;
int dir=3;
int PWM= 60;
bool movs[9][10]=
//  RF RB LB LF BR BL
{{0,0,0,0,0,0,0,0,0,0},
 {0,1,0,1,0,1,0,0,0,0}, //North
 {0,0,0,0,0,1,0,1,0,1}, //Northeast
 {0,0,1,0,0,1,0,1,0,1}, //East
 {0,0,1,0,0,0,0,1,0,1}, //Southeast
 {0,0,1,0,1,0,1,0,0,0}, //South
 {0,0,0,0,1,0,1,0,1,0}, //Southwest
 {0,1,0,1,1,0,1,0,1,0}, //West
 {0,1,0,1,0,0,0,0,1,0}};//Northwest

void setup() {
    pinMode(3,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);  

    Wire.begin();

    Serial.begin(38400);
    mag.initialize();
}
bool first = true;
float zero, current;

void loop() {
  //dir=millis()/1000;
  if(dir>8)dir=0;
  mag.getHeading(&mx, &my, &mz);
  float heading = atan2(my, mx);
  if(heading<0) heading+= 2*M_PI;
  heading=heading * 180/M_PI; 
  
  current= heading;
  if(first){
    zero = heading-2;
    first = false;
  }
  else{
    deg = current-zero;
    if(deg<0) deg+=360;
  }
  if(deg>180) deg-=360;
  
  Serial.println(deg);
  if(dir==3)deg+=17;
  if(dir==7)deg-=17;
  deg*=pk;
    
   digitalWrite(23,movs[dir][1]);
   digitalWrite(25,movs[dir][2]);  //Right
   movs[dir][3]? analogWrite(3,min(max(PWM+deg,5),255)) : analogWrite(3,min(max(PWM-deg,5),255));
    
   digitalWrite(22,movs[dir][4]);
   digitalWrite(24,movs[dir][5]);  //Left
   movs[dir][6]? analogWrite(2,min(max(PWM+deg,5),255)) : analogWrite(2,min(max(PWM-deg,5),255));

   movs[dir][7]? movs[dir][9]? analogWrite(6,min(max(PWM+20+deg,5),255)) : analogWrite(6,min(max(PWM+20-deg,5),255)) : digitalWrite(6,0);
   movs[dir][8]? movs[dir][9]? analogWrite(7,min(max(PWM+20+deg,5),255)) : analogWrite(7,min(max(PWM+20-deg,5),255)) : digitalWrite(7,0);
}
