#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
 
const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);
 
// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;
 
char data[16]="Hola mundo" ;
 
void setup(void)
{
   radio.begin();
   radio.openWritingPipe(pipe);
}
 
void loop(void)
{
   radio.write(data, sizeof data);
   delay(1000);
}
