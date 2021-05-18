#include <Servo.h> 
#include <Wire.h> 
#include "DHT.h"
#include "SSD1306.h"
#define DHTTYPE DHT22 
#define serrv 2 //(D4)
#define button 15//(D8)
#define holl 13//(D7)
#define DHTPin 12//(D6)
#define Pinrel 16//(D0)
//#include "RTClib.h"

uint32_t myTimer1, myTimer2, myTimer3;

//RTC_DS3231 rtc; 
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

DHT dht(DHTPin, DHTTYPE);                
float Temperatur;
float Humidity;
SSD1306  display(0x3c, 0, 14);//D3,D5
int f=0;
int r=1;
Servo myservo;

int UART;

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5);
  pinMode(0, OUTPUT);
  myservo.attach(serrv);
  pinMode(holl, INPUT);
  pinMode(button, INPUT);
  pinMode(Pinrel, OUTPUT); //rele
  digitalWrite(Pinrel, HIGH);//rele
  
  pinMode(DHTPin, INPUT);
  dht.begin();   
  
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
//rtc.adjust(DateTime(2021, 5, 8, 22, 9, 0));
}


void loop() { 


 /* if (millis() - myTimer1 >= 1000){
   myTimer1 = millis();  
  DateTime now = rtc.now();
  Serial.print("now DateTime: ");     // Отправка текста в Serial Port
  Serial.print(now.year(), DEC);      // Отправка в Serial Port год 
  Serial.print('/');                  // Отправка текста в Serial Port 
  Serial.print(now.month(), DEC);     // Отправка в Serial Port месяц 
  Serial.print('/');                  // Отправка текста в Serial Port
  Serial.print(now.day(), DEC);       // Отправка в Serial Port дня
  Serial.print(' ');                  // Отправка текста в Serial Port
  Serial.print(now.hour(), DEC);      // Отправка в Serial Port час
  Serial.print(':');                  // Отправка текста в Serial Port
  Serial.print(now.minute(), DEC);    // Отправка в Serial Port минуты
  Serial.print(':');                  // Отправка текста в Serial Port
  Serial.print(now.second(), DEC);    // Отправка в Serial Port секунды
  Serial.print(" DOW: ");             // Отправка текста в Serial Port
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);  // Отправка в Serial Port день недели
  Serial.print(" - Temp: ");          // Отправка текста в Serial Port
  Serial.print(rtc.getTemperature()); // Отправка в Serial Port температуру
  Serial.println();  
}*/

if (millis() - myTimer3 >= 500){
   myTimer3 = millis();
   UART = Serial.read();
   Serial.println(UART);
}

  if (millis() - myTimer2 >= 100){
   myTimer2 = millis();
    
  boolean b = digitalRead(button);
  Humidity = dht.readHumidity(); 
  Temperatur = dht.readTemperature();
 
  if (Humidity > 60) {
   digitalWrite(Pinrel, HIGH); 
   myservo.write(0);
  } else {
   myservo.write(360);
   digitalWrite(Pinrel, LOW);
  }

if (Humidity > 60&&r==2) {
   digitalWrite(Pinrel, LOW); 
  }
  

if(digitalRead(button)==HIGH&&f==0){
r++; 
f=1;
if(r>2){
  r=1;
}
}
if(digitalRead(button)==LOW&&f==1){
  f=0;
  }
if (r==1){
  display.display();
  display.clear();
  display.drawString(0, 0, "Humid: " + String(Humidity) + "%\t"); 
  display.drawString(0, 16, "Temp: " + String(Temperatur) + "°C");       
  if (!digitalRead(holl)) {
  display.drawString(20, 37, "LOCKED"); 
  } else {
  display.drawString(10, 37, "UNLOCKED"); 
}               
}
if(r==2){ 
  display.display();
  display.clear();
  display.drawString(0, 0, "MANUAL"); 
  display.drawString(0, 16, "CONTROL");  
  if (!digitalRead(holl)) {
  display.drawString(20, 37, "LOCKED"); 
  } else {
   display.drawString(10, 37, "UNLOCKED"); 
}       
}
}
}
