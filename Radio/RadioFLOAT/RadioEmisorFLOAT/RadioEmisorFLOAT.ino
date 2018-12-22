#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);
 
// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;
 
float data[2];
 
void setup()
{
   radio.begin();
   radio.openWritingPipe(pipe);
}
 
void loop()
{ 
   data[0]= 3.14;
   data[1] = millis()/1000.0;
   
   radio.write(data, sizeof data);
   delay(1000);
}
