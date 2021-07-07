# EnviroDIY_DS3231

This is an Arduino library for the [DS3231](https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231.html), a low-cost, extremely accurate I²C real-time clock (RTC) with an integrated temperature-compensated crystal oscillator (TCXO) and crystal.  This RTC chip is integrated into Sodaq and EnviroDIY boards, some Seeeduino Stalker boards, and the Chronodot.

The RDS3231 Class used in this library is by Seeed Technology Inc (http://www.seeedstudio.com) and used is in Seeeduino Stalker v2.1 for battery management (MCU power saving mode) & to generate timestamp for data logging.  The original DateTime class and its utility code is by Jean-Claude Wippler at JeeLabs (http://jeelabs.net/projects/cafe/wiki/RTClib).

The DateTime class in this library is a modified version which supports properly setting and reading the day-of-week.

This entire library is a fork of the library created by Sodaq available at https://github.com/SodaqMoja/Sodaq_DS3231.

Released under MIT License http://opensource.org/licenses/mit-license.php

# Examples using this Library

[Now](https://github.com/EnviroDIY/Sodaq_DS3231/tree/master/examples/now) - Sends the current date and time to the serial port every second.

[Temperature](https://github.com/EnviroDIY/Sodaq_DS3231/tree/master/examples/temperature) - Sends the current chip temperature in Celsius to the serial port every second.

[Interrupts](https://github.com/EnviroDIY/Sodaq_DS3231/tree/master/examples/interrupts) - Demonstrates the use of interrupts for battery management/saving using MCU power down mode.

[Adjust](https://github.com/EnviroDIY/Sodaq_DS3231/tree/master/examples/adjust) - Allows manual setting of the time.

[PCsync](https://github.com/EnviroDIY/Sodaq_DS3231/tree/master/examples/PCsync) - Uses a python script or executable program to synchronize the DS3231 clock to Network Time Protocol or an attached computer.
