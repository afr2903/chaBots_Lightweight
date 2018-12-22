#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;

int16_t mx, my, mz;
float compass;
int Pi=200;
int Pe=200;
int VM=255;
float x=true;

void setup() {
    pinMode(3,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);  

    Wire.begin();

    Serial.begin(9800);
    mag.initialize();

    Serial.println("Testing device connections...");
    Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
      
}

void ad(){//Adelante        // 3 y 5 = Pi       11 y 10 = Pe
 
 if(x){
   for(int c=4;c<VM;c=c*1.3){
    Pi=c;
    Pe=c;

    if(compass>=2 && compass<=180){
       Pi=c-((c*compass*1.1)/60);
       analogWrite(3,Pi);
       digitalWrite(5,LOW);
       digitalWrite(9,LOW);
       digitalWrite(6,LOW);
       analogWrite(11,Pe); 
       digitalWrite(10,LOW);
       Serial.println(c);
       delay(50); 
    }
    else if(compass>180 && compass<=358){  
      Pe=c-((c*(360-compass)*1.1)/60);
      analogWrite(3,Pi);
      digitalWrite(5,LOW);
      digitalWrite(9,LOW);
      digitalWrite(6,LOW);
      analogWrite(11,Pe); 
      digitalWrite(10,LOW);
      Serial.println(c);
      delay(50);  
    }
    else{
      Pi=c;
      Pe=c;
      analogWrite(3,Pi);
      digitalWrite(5,LOW);
      digitalWrite(9,LOW);
      digitalWrite(6,LOW);
      analogWrite(11,Pe); 
      digitalWrite(10,LOW);
      Serial.println(c);
      delay(50);
    }
    }
    x=false;    
 }
 else{ 
 analogWrite(3,Pi);
 digitalWrite(5,LOW);
 digitalWrite(9,LOW);
 digitalWrite(6,LOW);
 analogWrite(11,Pe); 
 digitalWrite(10,LOW);   
 } 
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
  Serial.print(compass);
  Serial.print("\t");
  Serial.print(Pi);
  Serial.print("\t");
  Serial.println(Pe);
//-------------------------------------------------

if(compass>=2 && compass<=180){
  Pi=VM-((VM*compass*1.1)/60);
  ad();  
}

else if(compass>180 && compass<=358){  
  Pe=VM-((VM*(360-compass*1.1))/60);
  ad();  
}

//if(compass<2 && compass>358){
else{
  ad();
  Pi=VM;
  Pe=VM;
}  

}
