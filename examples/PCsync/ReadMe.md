This example works to synchronize the DS3231 RTC clock chip with NTP or computer time by way of microUSB connection to a computer.

**To synchronize the clock:**

1. Ensure that you have the Sodaq library for the [DS3231](https://github.com/SodaqMoja/Sodaq_DS3231) available on your system.
2. Power your board and RTC chip.  Attach your board to your computer and make sure it is visible to your system.
3. Download PCsync.ino and upload it to your board.
4. Verify on the serial port monitor that your board is outputting the current date and time.  (Set the serial port baudrate to 57600.)
5. To synchronize manually:  Send the current unix time preceeded by a T over the serial port (ie, T1484241080).  It is best to send a time just a few seconds in advance of the current time because it does take a few seconds for it to initialize.  The current unix time stamp can be found at http://www.unixtimestamp.com/ or http://time.sodaq.net/
5. To synchronize automatically:  After uploading PCsync.ino to the device, close the serial port monitor.  Download and run PCsync.exe.  This will automatically detect and communicate with the RTC and set the clock to within 1 second of either the local computer time or, if the computer is connected to the internet, the US Network Time Protocol service.  The time will be set in **_UTC_**, not whatever the local timezone is.
6. If desired, verify that your clock is set correctly by monitoring your device on the serial port and comparing the output time to http://www.time.gov/.  Remeber that the time on http://www.time.gov/ will be shown in your current time zone and the clock will be set in UTC.

**Requirements**

 * Arduino wire library (generally built into IDE)
 * Sodaq DS3231 library (https://github.com/SodaqMoja/Sodaq_DS3231)
