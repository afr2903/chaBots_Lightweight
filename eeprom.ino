#include<EEPROM.h>
#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;

int16_t mx, my, mz;
float deg; //degree
float zero=0.00f, current;

void setup() {  
  Wire.begin();
  Serial.begin(9600);
  mag.initialize();
  EEPROM.get(0,zero);
  zero*=180/M_PI;
}

void reset(float heading){
  EEPROM.put(0,heading);
  while(analogRead(1));
}

void loop() {
  mag.getHeading(&mx, &my, &mz);
  float heading = atan2(my, mx);
  if(heading<0) heading+= 2*M_PI;
  if(analogRead(1)) reset(heading);
  Serial.print(zero);
  Serial.print('\t');
  Serial.print(heading);
  Serial.print('\t');
  heading=heading * 180/M_PI; 
  
  current= heading;

  deg = current-zero;
  if(deg<0) deg+=360;
  if(deg>180) deg-=360;
  
  Serial.println(deg);
  
}
