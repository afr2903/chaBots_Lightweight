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

if(compass>=5 && compass<=180){
  PWM = compass*1.35;
  if(PWM<=40)
    PWM = 40;
  analogWrite(5,PWM);
  digitalWrite(3,LOW);
  analogWrite(9,PWM);
  digitalWrite(6,LOW);
  analogWrite(11,PWM);
  digitalWrite(10,LOW);   
}

else if(compass>180 && compass<=355){  
  PWM = (360-compass)*1.35;
  
  if(PWM<=40)
    PWM = 40;
  analogWrite(3,PWM);
  digitalWrite(5,LOW);
  analogWrite(6,PWM);
  digitalWrite(9,LOW);
  analogWrite(10,PWM);
  digitalWrite(11,LOW);
    
}

//if(compass<5 && compass>355){
else{
  analogWrite(6,255);
  digitalWrite(9,LOW);
  analogWrite(11,255);
  digitalWrite(10,LOW);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW); 
}  

}
