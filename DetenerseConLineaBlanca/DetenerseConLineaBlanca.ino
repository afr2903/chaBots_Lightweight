#include "gg.h"
#define RED 2
#define BLUE 4
#define GREEN 7

RGBSensor S1(A0);
RGBSensor S2(A1);
RGBSensor S3(A2);

int PWM=160;
void ad(){//Adelante
 analogWrite(3,PWM);
 digitalWrite(5,LOW);
 digitalWrite(9,LOW);
 digitalWrite(6,LOW);
 analogWrite(11,PWM); 
 digitalWrite(10,LOW);   
}
void ap(){//APAGADO
 digitalWrite(3,LOW);
 digitalWrite(5,LOW);
 digitalWrite(6,LOW);
 digitalWrite(9,LOW);
 digitalWrite(10,LOW);
 digitalWrite(11,LOW); 
}
void at(){//Atras
 analogWrite(5,PWM);
 digitalWrite(3,LOW);
 digitalWrite(9,LOW);
 digitalWrite(6,LOW);
 analogWrite(10,PWM); 
 digitalWrite(11,LOW); 
}

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

int top,mid,bot;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);

  Serial.begin(9600);

  setState(true, false, true);  
  delay(3);
}

bool p=true;
void loop() {
  
  digitalWrite(7,HIGH);
  digitalWrite(2,HIGH);
  S1.readBlue();
  S2.readBlue();
  S3.readBlue();
  //Serial.println();
  top=S3.isGreen();
  mid=S2.isGreen();
  bot=S1.isGreen();
  
  Serial.print(top);
  Serial.print("\t");
  Serial.print(mid);
  Serial.print("\t");
  Serial.println(bot);

  if(top && p){
    PWM=255;
    ad();   
  }
  else{
    ap();
    p=false;  
  }

  if(!p){
    PWM=255;
    at();
    delay(8
    00);
    p=true;
  }  
}
