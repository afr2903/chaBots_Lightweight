#include "gg.h"
#define RED 2
#define BLUE 4
#define GREEN 7

RGBSensor S1(A0);
RGBSensor S2(A1);
RGBSensor S3(A2);


void setState(bool red, bool green, bool blue){ //el led esta al revez
  digitalWrite(RED, !red);
  digitalWrite(GREEN, !green);
  digitalWrite(BLUE, !blue);
}
void setColorGreen(){
  setState(false, true, false);  
}
void setColorRed(){
  setState(true, false, false);  
}
void setColorBlue(){
  setState(false, false, true);  
}


 

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);

  Serial.begin(9600);

  setState(true, false, true);  
  delay(3);
}


void loop() {
  bool p;
  S1.readBlue();
  S2.readBlue();
  S3.readBlue();
  //Serial.println();
  Serial.print(S1.isGreen());
  Serial.print("\t");
  Serial.print(S2.isGreen());
  Serial.print("\t");
  Serial.println(S3.isGreen());
}
