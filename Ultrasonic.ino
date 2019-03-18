int trig[]= {0, 42, 35,5};
int echo[]= {0, 40, 37,4};
int us[3];
// defines variables
long duration;
int distance;
void setup() {
  for(int i=1; i<=3; i++) pinMode(trig[i],OUTPUT);
  for(int i=1; i<=3; i++) pinMode(echo[i],INPUT);
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  for(int i=1; i<=3; i++){
    digitalWrite(trig[i], LOW);
    delayMicroseconds(2);
    digitalWrite(trig[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trig[i], LOW);
    us[i]= pulseIn(echo[i], HIGH);
    us[i]= us[i]/29/2;
  }
  Serial.print(us[1]);
  Serial.print("\t");
  Serial.print(us[2]);
  Serial.print("\t");
  Serial.print(us[3]);
  Serial.println(":v");
}
