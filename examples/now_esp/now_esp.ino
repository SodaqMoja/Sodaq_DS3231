// Date and time functions using RX8025 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "Sodaq_DS3231.h"

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup () 
{
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();

  Serial.println("\r\n");
}

void loop () 
{
  static uint16_t lastMillis = 0;
  uint16_t currentMillis = millis() / 1000;

  if (currentMillis - lastMillis >= 5)
  {
    lastMillis = currentMillis;
    DateTime now = rtc.now(); //get the current date-time
  
  	Serial.printf("%02d-%02d-%02d ", now.year(), now.month(), now.date());
    Serial.printf("%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  	Serial.println(weekDay[now.dayOfWeek()]);
  	Serial.print("Seconds since Unix Epoch: "); 
  	Serial.print(now.getEpoch(), DEC);
  	Serial.println();
  }
}
