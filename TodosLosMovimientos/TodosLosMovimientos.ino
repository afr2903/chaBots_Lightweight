void setup() {
  // put your setup code here, to run once:
 pinMode(3,OUTPUT);
 pinMode(5,OUTPUT);
 pinMode(6,OUTPUT);
 pinMode(9,OUTPUT);
 pinMode(10,OUTPUT);
 pinMode(11,OUTPUT);
}
int PWM=200;
void loop() {
  // put your main code here, to run repeatedly:
sd();
}

void ad(){//Adelante
 analogWrite(3,PWM);
 digitalWrite(5,LOW);
 digitalWrite(9,LOW);
 digitalWrite(6,LOW);
 analogWrite(11,PWM); 
 digitalWrite(10,LOW);   
}

void at(){//Atras
 analogWrite(5,PWM);
 digitalWrite(3,LOW);
 digitalWrite(9,LOW);
 digitalWrite(6,LOW);
 analogWrite(10,PWM); 
 digitalWrite(11,LOW); 
}
 
void ii(){//Inferior Izquierda
 analogWrite(5,PWM);
 digitalWrite(3,LOW);
 digitalWrite(6,PWM);
 digitalWrite(9,LOW);
 analogWrite(10,LOW); 
 digitalWrite(11,LOW); 
}

void sd(){//Superior Derecha
 analogWrite(3,PWM);
 digitalWrite(5,LOW);
 digitalWrite(9,PWM);
 digitalWrite(6,LOW);
 analogWrite(10,LOW); 
 digitalWrite(11,LOW); 
}

void si(){//Superior Izquierda
 analogWrite(5,LOW);
 digitalWrite(3,LOW);
 digitalWrite(6,PWM);
 digitalWrite(9,LOW);
 analogWrite(10,LOW); 
 digitalWrite(11,PWM);
}

void id(){//Inferior Derecha
 analogWrite(5,LOW);
 digitalWrite(3,LOW);
 digitalWrite(9,PWM);
 digitalWrite(6,LOW);
 analogWrite(11,LOW); 
 digitalWrite(10,PWM);
}

void gi(){//Giro Izquierda 
 analogWrite(5,PWM);
 digitalWrite(3,LOW);
 analogWrite(9,PWM);
 digitalWrite(6,LOW);
 analogWrite(11,PWM); 
 digitalWrite(10,LOW);
}
 
void gd(){//Giro Derecha
 analogWrite(3,PWM);
 digitalWrite(5,LOW);
 analogWrite(6,PWM);
 digitalWrite(9,LOW);
 analogWrite(10,PWM);
 digitalWrite(11,LOW);
}

void ap(){//APAGADO
 digitalWrite(3,LOW);
 digitalWrite(5,LOW);
 digitalWrite(6,LOW);
 digitalWrite(9,LOW);
 digitalWrite(10,LOW);
 digitalWrite(11,LOW); 
}
