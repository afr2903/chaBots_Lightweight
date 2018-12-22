#include <Wire.h> //Include the Wire Library
#include <HTInfraredSeeker.h> //Include the IR Seeker Library
#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;

int16_t mx, my, mz;
float compass;

//setup runs once at the beginning
int PWM=200;
void ad(){//Adelante
 analogWrite(3,PWM);
 digitalWrite(5,LOW);
 digitalWrite(9,LOW);
 digitalWrite(6,LOW);
 analogWrite(11,PWM); 
 digitalWrite(10,LOW);   
}

void at(){//Atras
 analogWrite(5,PWM);
 digitalWrite(3,LOW);
 digitalWrite(9,LOW);
 digitalWrite(6,LOW);
 analogWrite(10,PWM); 
 digitalWrite(11,LOW); 
}
 
void ii(){//Inferior Izquierda
 analogWrite(5,PWM);
 digitalWrite(3,LOW);
 digitalWrite(6,PWM);
 digitalWrite(9,LOW);
 analogWrite(10,LOW); 
 digitalWrite(11,LOW); 
}

void sd(){//Superior Derecha
 analogWrite(3,PWM);
 digitalWrite(5,LOW);
 digitalWrite(9,PWM);
 digitalWrite(6,LOW);
 analogWrite(10,LOW); 
 digitalWrite(11,LOW); 
}

void si(){//Superior Izquierda
 analogWrite(5,LOW);
 digitalWrite(3,LOW);
 digitalWrite(6,PWM);
 digitalWrite(9,LOW);
 analogWrite(10,LOW); 
 digitalWrite(11,PWM);
}

void id(){//Inferior Derecha
 analogWrite(5,LOW);
 digitalWrite(3,LOW);
 digitalWrite(9,PWM);
 digitalWrite(6,LOW);
 analogWrite(11,LOW); 
 digitalWrite(10,PWM);
}

void gi(){//Giro Izquierda 
 analogWrite(5,PWM);
 digitalWrite(3,LOW);
 analogWrite(9,PWM);
 digitalWrite(6,LOW);
 analogWrite(11,PWM); 
 digitalWrite(10,LOW);
}
 
void gd(){//Giro Derecha
 analogWrite(3,PWM);
 digitalWrite(5,LOW);
 analogWrite(6,PWM);
 digitalWrite(9,LOW);
 analogWrite(10,PWM);
 digitalWrite(11,LOW);
}

void ap(){//APAGADO
 digitalWrite(3,LOW);
 digitalWrite(5,LOW);
 digitalWrite(6,LOW);
 digitalWrite(9,LOW);
 digitalWrite(10,LOW);
 digitalWrite(11,LOW); 
}

void setup() {
  Wire.begin();
  Serial.begin(9600); //Starts the Serial Monitor so we can read output
  Serial.println("Dir\tStrength"); //Prints Dir & Strength at top
  InfraredSeeker::Initialize(); //initializes the IR sensor
  mag.initialize();

}

bool compinbool = true;
float compin, compac;  
int dir;

//loop runs repeatedly
void loop() {   
  //Read the AC (pulsed) information from the IR Sensor
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
  
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  
  Serial.print(InfraredBall.Direction); //Print the Direction Number
  Serial.print("\t"); //Print a tab
  Serial.print(InfraredBall.Strength); //Print the Strength Number //Print the Direction Number
  Serial.print("\t"); //Print a tab
  Serial.print(compass);
  Serial.println(); //Print a new line
  
  dir = InfraredBall.Direction;

if(compass>=10 && compass<=180){
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

else if(compass>180 && compass<=350){  
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

//if(compass<10 && compass>350){
else{
    PWM = 15
    0;
    switch(dir){
    case 0:
      at();
    break;
    case 1:
      ii();
    break;
    case 2:
      ii();
    break;
    case 3:
      si();
    break;
    case 4:
      si();
    break;
    case 5:
      ad();  
    break;
    case 6:
      sd();
    break;
    case 7:
      sd();
    break;
    case 8:
      id();
    break;
    case 9:
      id();
    break;
  } 
}

}
