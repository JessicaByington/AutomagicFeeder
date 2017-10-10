#include <EEPROM.h>
#include "EEPROMAnything.h"

/*typedef struct
{
  long am_time;
  long pm_time;
  int amount;
} user_config;*/

typedef struct
{
  long alarm;
  int mode;
} c;

c test;
c ttest;

void setup()
{
  EEPROM_readAnything(0, test);
  Test();
}

void Test()
{
  Serial.begin(9600);

  
  test.alarm = 55555;
  test.mode = 1;
  

  Serial.println(test.alarm);
  Serial.println(test.mode);
  EEPROM_writeAnything(0, test);

  EEPROM_readAnything(0, ttest);
  Serial.println(ttest.alarm);
  Serial.println(ttest.mode);
}

void loop()
{}
