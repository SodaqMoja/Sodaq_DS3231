# Sodaq_DS3231

This is an Arduino library for the DS3231 RTC (Real Time Clock).

## Connection
DS3231 Schield | ESP8266
---------------|--------
Vcc | Vcc
GND | GND
SDA | D4
SCL | D5

[ESP8266 GPIO](https://github.com/esp8266/Arduino/blob/esp8266/hardware/esp8266com/esp8266/doc/reference.md#digital-io)
<br>Please use a DS3231 shield.
If you are using the DS3231 without any shield you have to add pull up resistors to `SDA` and `SCL`.

## Changelog
### Version 1.2.1
- Change library to use with ESP8266
- Add now example fpr ESP8266

## Authors
### Since Version 1.2.1
- Pascal Gollor (http://www.pgollor.de/cms/)

### Till Verison 1.2.0
- SodaqMoja (https://github.com/SodaqMoja/Sodaq_DS3231)
- Seeed Technology Inc(http://www.seeedstudio.com)
- Original DateTime Class and its utility code is by Jean-Claude Wippler at JeeLabs (http://jeelabs.net/projects/cafe/wiki/RTClib)

For more information see [old readme](OLD_README.md)


# License
Released under MIT License http://opensource.org/licenses/mit-license.php

