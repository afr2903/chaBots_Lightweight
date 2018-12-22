#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;

int16_t mx, my, mz;
float compass;

void setup() {
    pinMode(7,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(2,OUTPUT);  

    Wire.begin();

    Serial.begin(38400);
    mag.initialize();

    Serial.println("Testing device connections...");
    Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
}

  bool compinbool = true;
  float compin, compac;

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

if(compass>=10 && compass<=180){
  digitalWrite(2,LOW);
  digitalWrite(4,LOW);
  digitalWrite(6,LOW);
  digitalWrite(3,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(7,HIGH);
}

else if(compass>180 && compass<=350){  
  digitalWrite(2,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(7,LOW);
}

//if(compass<10 && compass>350){
else{
  digitalWrite(2,LOW);
  digitalWrite(4,LOW);
  digitalWrite(6,LOW);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(7,LOW); 
}  

}
