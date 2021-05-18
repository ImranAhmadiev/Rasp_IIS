#include "Wire.h"
#include <Servo.h>
#define pinX    A2  // ось X джойстика
#define pinY    A1  // ось Y джойстика
const int servo1 = 3; 
const int servo2 = 10;
Servo myservo1; 
Servo myservo2;
uint32_t mytime1, mytime2;
const int MPU_addr = 0x68;
int16_t data[7];  

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  myservo1.attach(servo1); 
  myservo2.attach(servo2);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
}

void loop() {
  getData();
  if (millis() - mytime1 >= 500){
  mytime1 = millis();
  for (byte i = 0; i < 7; i++) {
    Serial.write(data[i]);
  } 
}
if (millis() - mytime2 >= 50){
   mytime2 = millis();
   int X = map (analogRead(pinX), 0, 1023, 180, 70);            
  int Y = map (analogRead(pinY), 0, 1023, 180, 70);       
   myservo1.write(X);             
  myservo2.write(Y);   
}
}


void getData() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  for (byte i = 0; i < 7; i++) {
    data[i] = Wire.read() << 8 | Wire.read();
  }
}
