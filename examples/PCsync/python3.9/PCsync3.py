# -*- coding: utf-8 -*-

"""
Originally Created by Sara Geleskie Damiano on 1/9/2017
Modified by Matt Findley April 2021 to work with Python 3,
only modified slightly:
 - adding parentheses to the Python 2 print statements.
 - replacing raw_input() functions with input() functions.
 - using f-strings to assemble strings of text and variables.
 - info about serial devices accessed via object attributes rather than position in a tuple.
 - convert Python3 unicode strings to bytestrings (and visa versa) so that we can read/write data
   on serial port.

This program interacts with an FTDI connected Arduino device (like the EnviroDIY Mayfly)
running sync_clock_PC.ino to synchronize the RTC chip to Network Time Protocol clocks
(or to local PC time is NTP is unavailable)
"""


import datetime
import time
import warnings

import ntplib
import serial.tools.list_ports


# from tzlocal import get_localzone

# get local timezone
# local_tz = get_localzone()

# Set offset from unversal coordinated time (aka Greenwich Mean Time, aka UTC)

UTC_offset = -5


def get_device_time():
    # A helper function to get the current time from a device running sync_clock_PC.ino
    device_time_str = device.readline().decode()
    print(device_time_str)
    device_rtc = int(device_time_str.split("(")[1].split(")")[0])
    # Convert into a time-zone aware python datetime value
    # device_rtc_unaware = datetime.datetime.utcfromtimestamp(device_rtc)
    # device_rtc_aware = local_tz.localize(device_rtc_unaware, is_dst=False)
    return device_rtc  # , device_rtc_aware


def get_pc_time(notifications=False):
    # A helper function to get the current time of either the PC or the US national time protocol servers
    # Checks for internet connection and if that is available returns the US NTP time, otherwise it
    # returns the local PC clock time.
    # Returns the current time (localized to the current PC) as a unix timestamp

    try:
        c = ntplib.NTPClient()
        response = c.request('us.pool.ntp.org', version=3)
        utc_unix_time = response.orig_time + (UTC_offset*3600)
        if notifications:
            print("Using time from Network Time Protocol server us.pool.ntp.org")
    except:
        ts_utc = datetime.datetime.utcnow()
        utc_unix_time = (ts_utc - datetime.datetime(1970, 1, 1)).total_seconds()
        if notifications:
            print("Using local computer time")

    # Conversions to deal with local vs. UTC time
    # ntp_unaware = datetime.datetime.utcfromtimestamp(utc_unix_time)
    # ntp_aware = local_tz.localize(ntp_unaware, is_dst=False)
    # local_unix_time = (ntp_aware - datetime.datetime(1970, 1, 1)).total_seconds()
    if notifications:
        # print "RTC chip is being set to time zone %s" % local_tz
        print("RTC chip is being set to UTC")
        print("Please account for timezones in your sketch")
    return int(utc_unix_time)  # , ntp_aware


def parse_device_set_response():
    # Parses the device's response to the time-setting commands
    first_resp = device.readline().decode()  # This line should be another rep of the device datetime
    print(first_resp)
    sec_resp = device.readline().decode()  # This line should be "Recieved:###"
    # print sec_resp
    third_resp = device.readline().decode()  # This line should be "Updating RTC ..."
    # print third_resp
    diffts_abs = int(third_resp.split()[4])
    setline = device.readline().decode()
    # print setline
    
    oldts = int(setline.split()[7])
    newts = int(setline.split()[4])
    return oldts, newts, diffts_abs


# Check all available serial ports
ports = serial.tools.list_ports.comports()

# Keep only ports with the FTDI as the manufacturer
device_ports = [
    p for p in ports
    if p.manufacturer == 'FTDI'
    ]

# Give warnings if 0 or >1 Mayflies found
if not device_ports:
    print("No FTDI device found")
    input("Press Enter to Exit")
    exit()
elif len(device_ports) > 1:
    warnings.warn(f'Multiple FTDI devices found - using {device_ports[0].description}')
else:
    print(f"FTDI Device found at {device_ports[0].description}")

# Open up the device serial port
try:
    device = serial.Serial(device_ports[0].device, 57600, timeout=5)
except:
    print(f"Cannot access {device_ports[0].device}")
    print("Please close any other programs accessing the serial port")
    input("Press Enter to Exit")
    exit()

# Wait for the device to initialize
print("Waiting for device to initialize")
time.sleep(2)
timeout_time = time.time() + 10
print()
print(device.readline().decode())
print(device.readline().decode())

# Check that getting expected responses from the device
try:
    get_device_time()
except:
    print("Device is not sending expected output.")
    print("Please ensure that sync_clock_PC.ino has been uploaded to the device")
    input("Press Enter to Exit")
    exit()

# Send the time to the device
print("First attempt to set the clock")
run_time_check = datetime.datetime.now()
device.write(("T" + str(get_pc_time(notifications=True))).encode('utf-8'))
mf_resp1 = parse_device_set_response()
print(f"Clock set to {mf_resp1[1]}")
set_time_check = (datetime.datetime.now() - run_time_check).total_seconds()
print(f"Setting the clock took {set_time_check} seconds")

# Send the time to the device again to double-check the offsets
print("Checking the device response offset")
device.readline()
unix_time_string = "T" + str(get_pc_time())
device.write(unix_time_string.encode('utf-8'))
mf_resp2 = parse_device_set_response()

# Send the time to the device again adjusting for the offset
if mf_resp2[2] > 1:
    print(f"Device takes {mf_resp2[2]} seconds to respond to the PC time set command")
    print("Re-adjusting the device clock")
    device.readline()
    adjusted_unix_time_string = "T" + str(get_pc_time() + set_time_check + mf_resp2[2])
    device.write(adjusted_unix_time_string.encode('utf-8'))
else:
    print("Device responded in less than 1 second")
    print("Re-adjusting the device clock")
    device.readline().decode()
    device.write(("T" + str(get_pc_time() + set_time_check)).encode('utf-8'))

print("Device RTC is now within 1 second of computer or NTP clock")
print("It is not possible to adjust the DS3231 to millisecond precision.")
print(device.readline().decode())

device.close()

input("Press Enter to Exit")
exit()
