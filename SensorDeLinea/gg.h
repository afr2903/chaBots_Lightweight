typedef struct RGBSensor{
     const byte port;
     int red;
     int green;
     int blue;

     void readRed(){
      red = analogRead(port); 
     }
     void readGreen(){
      green = analogRead(port);
     }
     void readBlue(){
      blue = analogRead(port);
      //Serial.print(blue);
      //Serial.print(" ");
     }
     bool isGreen(){
      return blue<300;
     }

     RGBSensor(byte port):port(port){
      
     }
};
