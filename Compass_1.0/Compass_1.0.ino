#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;

int16_t mx, my, mz;
float compass;

void setup() {

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
}
