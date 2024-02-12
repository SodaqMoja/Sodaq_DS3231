//Interrupts for Battery management/saving using MCU power down mode. /INT from DS3231 is connected to INT0 of MCU.

#include <Wire.h>
#include <Sodaq_PcInt.h>  // To handle pin change interrupts from the clock
#include "Sodaq_DS3231.h"

static uint8_t prevSecond=0;
int interruptPin = A7;
int ledPin = 8;

// Interrupt service routine for external interrupt on INT0 pin conntected to /INT
void INT0_ISR()
{
    // Keep this as short as possible. Possibly avoid using function calls
    digitalWrite(ledPin, HIGH);
    Serial.println("-- External Interrupt detected --");
    digitalWrite(ledPin, LOW);
}


void setup ()
{
    pinMode(interruptPin, INPUT_PULLUP);
    PcInt::attachInterrupt(interruptPin, INT0_ISR);

     Serial.begin(57600);
     Wire.begin();

     rtc.begin();
     attachInterrupt(0, INT0_ISR, FALLING);

    // Enable Interrupt
    // To interrupt at EverySecond, EveryMinute, or EveryHour use this:
    // rtc.enableInterrupts(EveryMinute);
    // To interrupt once per day at exactly this hour, minute, and second use this:
    // rtc.enableInterrupts(0, 0, 15);    // interrupt at (h,m,s)
    // To interrupt on other intervals, use this:
    rtc.enableInterrupts(MATCH_SECONDS, 0, 0, 0, 15);    // interrupt at (type, day/date, h,m,s)
    }


void loop ()
{
    String timeString = "";
    DateTime now = rtc.now(); //get the current date-time
    now.addToString(timeString);
    if((now.second()) !=  prevSecond )
    {
        //print only when there is a change in seconds
        Serial.println(timeString);
    }
    prevSecond = now.second();

    // This clears the interrrupt flag in status register of the clock
    // The next timed interrupt will not be sent until this is cleared
    rtc.clearINTStatus();

}
