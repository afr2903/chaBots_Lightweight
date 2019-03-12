void setup(){  
  pinMode(A0,INPUT); // TSOP is connected on the 15ht pin  
  Serial.begin(250000);  
}  
unsigned long long tiempo;
int mas, r;
void loop(){  
  r=analogRead(A0);
  if(r<900) Serial.println(r);
}
