// Date and time functions using DS3231 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "DS3231.h"

DS3231 RTC; //Create the DS3231 object

void setup () 
{
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
}

void loop () 
{
    RTC.convertTemperature();             //convert current temperature into registers
    Serial.print(RTC.getTemperature()); //read registers and display the temperature
    Serial.println("deg C");
    delay(1000);
}
