#define DETECT 0 // pin 2 for  sensor


void setup() {
  Serial.begin(9600);
  Serial.println("Robojax.com Laser Module Test");
  pinMode(A0,INPUT);//define detect input pin

  // Laser sensor code for Robojax.com

}

void loop() {
     // Laser Sensor code for Robojax.com

  int detected = digitalRead(A0);// read Laser sensor
 
  if( detected == HIGH)
  {
    Serial.println("Detected!");

  }else{
    Serial.println("No laser");
     // Laser Sensor code for Robojax.com

  }
}
