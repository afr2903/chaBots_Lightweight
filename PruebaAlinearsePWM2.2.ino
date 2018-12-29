#include "Wire.h"
#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;

int16_t mx, my, mz;
float compass;

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

    Serial.println("Testing device connections...");
    Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
}

  bool compinbool = true;
  float compin, compac;
  int PWM;
void loop() {
  mag.getHeading(&mx, &my, &mz);
  float heading = atan2(my, mx);
  if(heading < 0)
      heading += 2 * M_PI;
  heading=heading * 180/M_PI; 
  
  compac= heading;
  if(compinbool){
    compin = heading;
    compinbool = false;
  }
  else{
    compass = compac-compin;
    if(compass<0){
      compass = compass+360;
    }
  }

  Serial.println(compass);
//-------------------------------------------------

if(compass>5 && compass<=180){
  PWM = compass*1.35;
  if(PWM<=40)
    PWM = 40;
 
 analogWrite(6,PWM);
 digitalWrite(7,0); 
  
 digitalWrite(23,1);
 digitalWrite(25,0);
 analogWrite(3,PWM); 
  
 digitalWrite(22,1);
 digitalWrite(24,0);
 analogWrite(2,PWM);   
}

else if(compass>180 && compass<355){  
  PWM = (360-compass)*1.35;  
  if(PWM<=40)
    PWM = 40;
    
  digitalWrite(6,0);
 analogWrite(7,PWM);

 digitalWrite(23,0);
 digitalWrite(25,1);
 analogWrite(3,PWM); 
  
 digitalWrite(22,0);
 digitalWrite(24,1);
 analogWrite(2,PWM);  
 
    
}

//if(compass<=5 && compass>=355){
else{
 digitalWrite(6,LOW);
 digitalWrite(7,LOW);
 digitalWrite(22,LOW);
 digitalWrite(23,LOW);
 digitalWrite(24,LOW);
 digitalWrite(25,LOW); 
}  

}
