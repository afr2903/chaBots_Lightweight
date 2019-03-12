/*  #include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif*/

int line[10];

void setup() {
  /*display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.clearDisplay();*/
  
  pinMode(A7,INPUT);
  pinMode(A8,INPUT);  //Left
  pinMode(A9,INPUT);
  
  pinMode(A10,INPUT);
  pinMode(A11,INPUT); //Middle
  pinMode(A12,INPUT);

  pinMode(A13,INPUT);
  pinMode(A14,INPUT); //Right
  pinMode(A15,INPUT);
  for(int i=39; i<=53; i++){
    pinMode(i,OUTPUT);
  }
  
  Serial.begin(9600);
}

void loop() {
  digitalWrite(44,1);
  digitalWrite(46,0); //Left
  digitalWrite(48,0);
  
  digitalWrite(39,0);
  digitalWrite(41,0); //Middle
  digitalWrite(43,1);
  
  digitalWrite(47,0);
  digitalWrite(49,0); //Right
  digitalWrite(53,1);
 

  for(int i=7; i<=15; i++){
    line[i-6]=analogRead(i);
  }
  for(int i=1; i<=9; i++){
    Serial.print(line[i]);
    Serial.print('\t');
  }
  Serial.println();

}
