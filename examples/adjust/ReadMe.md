This if for manually setting the DS3231 clock.

**To synchronize the clock:**

1. Ensure that you have the Sodaq library for the [DS3231](https://github.com/SodaqMoja/Sodaq_DS3231) available on your system.
2. Power your board and RTC chip.  Attach your board to your computer and make sure it is visible to your system.
3. Download adjust.ino and open it in your IDE.  Scroll to line 11.  Modify the text in line 11 to be a date time about a minute in the future.  Save and upload the sketch to your board.
4. Navigate to http://www.time.gov/ or look at another clock displaying the correct time.  When the currrent time matches the time that you entered in the sketch, hit the reset button on your board.
5. Use the serial port monitor to verify that the time on your RTC is now correct.  You may need to repeat steps 3 and 4 several times to get the time right.
