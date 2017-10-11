#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

void setup() 
{
  Serial.begin(9600);
  while(!Serial);
  delay(200);
}

void loop() 
{
  tmElements_t tm;
  RTC.read(tm);
  
  unsigned char m_hour = 16, m_min = 53, m_sec = 00;
  
  if (tm.Hour == m_hour)
    if (tm.Minute == m_min)
      if (tm.Second == m_sec)
      {
        Serial.println("dispense");
        dispense();
      }
 }

 void dispense ()
 {
    Serial.println("look at me");
    Serial.println("totally dispensing food");
    Serial.println("right now!");
    Serial.println("no really.");
    Serial.println("totz.");
    delay(2000);
 }

