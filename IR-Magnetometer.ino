#include<Wire.h>
#include "HTInfraredSeeker.h"
#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;

int16_t mx, my, mz;
float deg; //degree
float pk = 1.35;
int dir;
int PWM = 200;

bool first = true;
float zero, current;
bool movs[9][10] =
  //  RF RB LB LF BR BL
{ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0, 0, 0}, //1 North
  {0, 0, 0, 0, 0, 1, 0, 1, 0, 1}, //2 Northeast
  {0, 0, 1, 0, 0, 1, 0, 1, 0, 1}, //3 East
  {0, 0, 1, 0, 0, 0, 0, 1, 0, 1}, //4 Southeast
  {0, 0, 1, 0, 1, 0, 1, 0, 0, 0}, //5 South
  {0, 0, 0, 0, 1, 0, 1, 0, 1, 0}, //6 Southwest
  {0, 1, 0, 1, 1, 0, 1, 0, 1, 0}, //7 West
  {0, 1, 0, 1, 0, 0, 0, 0, 1, 0}};//8 Northwest
  
int ir[]={3,5,5,6,7,1,3,4,5,5};

void setup() {
  Wire.begin();
  Serial.begin(9600); //Starts the Serial Monitor so we can read output
  InfraredSeeker::Initialize(); //initializes the IR sensor
  pinMode(22, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(2, OUTPUT);

  pinMode(23, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);

  mag.initialize();
}

void loop() {
  InfraredResult IR = InfraredSeeker::ReadAC();
  Serial.print(IR.Direction);
  Serial.print("\t");
  Serial.print(IR.Strength);
  Serial.print("\t");

  mag.getHeading(&mx, &my, &mz);
  float heading = atan2(my, mx);
  if (heading < 0) heading += 2 * M_PI;
  heading = heading * 180 / M_PI;
  current = heading;
  if (first) {
    zero = heading - 2;
    first = false;
  }
  else {
    deg = current - zero;
    if (deg < 0) deg += 360;
  }
  if (deg > 180) deg -= 360;
  Serial.println(deg);
  if (dir == 3)deg += 17;
  if (dir == 7)deg -= 17;
  deg *= pk;

  dir = ir[IR.Direction];
  if (dir == 1 && IR.Strength >= 100) dir = 0;
  if (IR.Direction==0 && IR.Strength == 0) dir = 0;


  digitalWrite(23, movs[dir][1]);
  digitalWrite(25, movs[dir][2]); //Right
  movs[dir][3] ? analogWrite(3, min(max(PWM + deg, 5), 255)) : analogWrite(3, min(max(PWM - deg, 5), 255));

  digitalWrite(22, movs[dir][4]);
  digitalWrite(24, movs[dir][5]); //Left
  movs[dir][6] ? analogWrite(2, min(max(PWM + deg, 5), 255)) : analogWrite(2, min(max(PWM - deg, 5), 255));

  movs[dir][7] ? movs[dir][9] ? analogWrite(6, min(max(PWM + 20 + deg, 5), 255)) : analogWrite(6, min(max(PWM + 20 - deg, 5), 255)) : digitalWrite(6, 0);
  movs[dir][8] ? movs[dir][9] ? analogWrite(7, min(max(PWM + 20 + deg, 5), 255)) : analogWrite(7, min(max(PWM + 20 - deg, 5), 255)) : digitalWrite(7, 0);

}
