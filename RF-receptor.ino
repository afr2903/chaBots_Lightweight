#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
#define CE_PIN 9
#define CSN_PIN 10
byte direccion[5] ={'i','n','f','r','a'}; 
RF24 radio(CE_PIN, CSN_PIN);
float n;

#include <Wire.h>
struct InfraredResult
{
  byte Direction;
  byte Strength;
};

void setup()
{
  radio.begin();
  Serial.begin(9600); 
  radio.openReadingPipe(1, direccion);
  radio.startListening();
 
}
 
void loop() {
 uint8_t numero_canal;
 InfraredResult IR;
 //if ( radio.available(&numero_canal) )
 if ( radio.available() )
 {    
     //Leemos los datos y los guardamos en la variable datos[]
     radio.read(&IR,sizeof(IR));
     
     //reportamos por el puerto serial los datos recibidos
     Serial.print("Dato= " );
     Serial.print(IR.Direction);
     Serial.print("\t");
     Serial.println(IR.Strength);
 }
 else
 {
     Serial.println("No hay datos de radio disponibles");
 }
 delay(150);
}
