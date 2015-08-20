# Sodaq_DS3231

This is an Arduino library for the DS3231 RTC (Real Time Clock).

Table of Contents
=================

  * [Sodaq_DS3231](#sodaq_ds3231)
  * [Table of Contents](#table-of-contents)
  * [Changelog](#changelog)
    * [Version 1.2.1](#version-121)
  * [License](#license)
    * [Authors](#authors)
      * [Since Version 1.2.1](#since-version-121)
      * [Till Verison 1.2.0](#till-verison-120)
  * [Information](#information)
    * [Connection](#connection)
    * [NTP Specification](#ntp-specification)
      * [NTPv4 Basic Header](#ntpv4-basic-header)
        * [Transmit Timestamp](#transmit-timestamp)
      * [NTP Timestamp format](#ntp-timestamp-format)

# Changelog
## Version 1.2.1
- Change library to use with ESP8266
- Add now example fpr ESP8266

# License
Released under MIT License http://opensource.org/licenses/mit-license.php

## Authors
### Since Version 1.2.1
- Pascal Gollor (http://www.pgollor.de/cms/)

### Till Verison 1.2.0
- SodaqMoja (https://github.com/SodaqMoja/Sodaq_DS3231)
- Seeed Technology Inc(http://www.seeedstudio.com)
- Original DateTime Class and its utility code is by Jean-Claude Wippler at JeeLabs (http://jeelabs.net/projects/cafe/wiki/RTClib)

For more information see [old readme](OLD_README.md)

# Information
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

## NTP Specification
### NTPv4 Basic Header
Bytes | Description
------|------------
0 | LI (Bit 6-7), VersionNumber (3-5). Mode(0-2)
1 | Stratum
2 | Poll
3 | Precision
4 - 7 | Root Deleay
8 - 11 | Root Dispersion
12 - 15 | Reference ID
16 - 23 | Reference Timestamp
24 - 31 | Origin Timestamp
32 - 39 | Receive Timestamp
40 - 47 | Transmit Timestamp

#### Transmit Timestamp
Time at the server when the response left for the client, in NTP timestamp format.

### NTP Timestamp format
Bytes | Description
------|------------
0 - 3 | Seconds scince 1900
4 - 7 | Fraction of Second

