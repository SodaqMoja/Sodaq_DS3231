// Date and time functions using DS3231 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "Sodaq_DS3231.h"

void setup ()
{
    Serial.begin(57600);
    Wire.begin();
    rtc.begin();
}

void loop ()
{
    // DateTime now = rtc.now(); //get the current date-time
	// Serial.print(now.year(), DEC);
	// Serial.print('/');
	// Serial.print(now.month(), DEC);
	// Serial.print('/');
	// Serial.print(now.date(), DEC);
	// Serial.print(' ');
	// Serial.print(now.hour(), DEC);
	// Serial.print(':');
	// Serial.print(now.minute(), DEC);
	// Serial.print(':');
	// Serial.print(now.second(), DEC);
	// Serial.println();

    rtc.convertTemperature();             //convert current temperature into registers
    Serial.print(rtc.getTemperature()); //read registers and display the temperature
    Serial.println(" deg C");
    delay(250);
}
