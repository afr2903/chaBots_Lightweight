#include <Wire.h> //Include the Wire Library
#include <HTInfraredSeeker.h> //Include the IR Seeker Library

//setup runs once at the beginning
void setup() {
  Wire.begin();
  Serial.begin(9600); //Starts the Serial Monitor so we can read output
  Serial.println("Dir\tStrength"); //Prints Dir & Strength at top
  InfraredSeeker::Initialize(); //initializes the IR sensor
}

//loop runs repeatedly
void loop() {   
  //Read the AC (pulsed) information from the IR Sensor
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  
  Serial.print(InfraredBall.Direction); //Print the Direction Number
  Serial.print("\t"); //Print a tab
  Serial.print(InfraredBall.Strength); //Print the Strength Number
  Serial.println(); //Print a new line
  delay(100); //delay a tenth of a second
}
