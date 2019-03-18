bool ball_detected;
void setup(){  
  pinMode(A0,INPUT); // TSOP is connected on the 15ht pin  
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  Serial.begin(9600);  
}  
unsigned long long tiempo;
int mas, r;
void loop(){  
  r=analogRead(0);
  if(r<=100) Serial.print(r);
  Serial.print("\t");
  r=analogRead(4);
  if(r<=100) Serial.print(r);
  Serial.print("\t");
  r=analogRead(5);
  if(r<=100) Serial.print(r);
  Serial.println();
  ball_detected= (analogRead(0)<75&&analogRead(4)<75)||(analogRead(4)<75&&analogRead(5)<75);
  Serial.println(ball_detected);
}
