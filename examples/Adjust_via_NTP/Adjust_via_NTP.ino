/**
 * @file Adjust_via_NTP
 *
 * Based on Arduino NTPClient example.
 * 
 * Connection information: https://github.com/pgollor/Sodaq_DS3231/blob/master/README.md
 */


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"


// ----- User changes begin -----

/// Your network SSID (name)
const char ssid[] = "********";

/// your network password
const char pass[] = "********";

/// NTP Server address
const char* ntpServerName = "0.de.pool.ntp.org";

/**
 * @brief Timezone
 * 
 * For CEST use 2.0
 */
const double timezone = 2.0;

// ----- User changes end -----



/// Weekday strings.
char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

// local port to listen for UDP packets
const unsigned int localPort = 2390;

/**
 * @brief NTP Server IP variable.
 * 
 * Don't hardwire the IP address or we won't get the benefits of the pool.
 */
IPAddress timeServerIP;

/// NTP time stamp is in the first 48 bytes of the message
const int NTP_PACKET_SIZE = 48;

/// buffer to hold incoming and outgoing packets
byte packetBuffer[ NTP_PACKET_SIZE];

/// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;



// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress *address)
{
  Serial.println("sending NTP packet...");

  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(*address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}


/**
 * @brief Adjust the DS3231 with current date and time.
 * 
 * <pre>
 * NTPv4 Basic Header Specification:
 * 
 * Bytes  : Description
 * -------:----------------------
 *       0: LI(2), VN(3), Mode(3) 
 *       1: Stratum
 *       2: Poll
 *       3: Precision
 *  4 -  7: Root Delay
 *  8 - 11: Root Dispersion
 * 12 - 15: Reference ID
 * 16 - 23: Reference Timestamp
 * 24 - 31: Origin Timestamp
 * 32 - 39: Receive Timestamp
 * 40 - 47: Transmit Timestamp
 * 
 * Transmit Timestamp:
 * Time at the server when the response left for the client, in NTP timestamp format.
 * 
 * 
 * NTP timestamp format
 * 
 * Bytes: Description
 * -----:-------------------
 * 0 - 3: Seconds since 1900
 * 4 - 7: Fraction of Second
 * </pre>
 * 
 */
static inline void adjust(void)
{
  unsigned long mi;
  int cb = 0;
  
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  // send an NTP packet to a time server
  sendNTPpacket(&timeServerIP);
  mi = millis();

  // wait to see if a reply is available
  while (millis() - mi < 2000 && !cb)
  {
    cb = udp.parsePacket();
    yield();
  }

  if (!cb)
  {
    Serial.println("[ERROR]: No packet available.");

    return;
  }

  //Serial.print("packet received, length=");
  //Serial.println(cb);
  // We've received a packet, read the data from it
  udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

  //the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:

  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  //Serial.print("Seconds since Jan 1 1900 = " );
  //Serial.println(secsSince1900);

  // now convert NTP time into everyday time:
  Serial.print("Unix UTC time = ");
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSince1900 - seventyYears;
  // print Unix time:
  Serial.println(epoch);

  // adjust to user timezone
  epoch += timezone * 3600;

  rtc.setEpoch(epoch);
}


/**
 * @brief Setup function.
 */
void setup () 
{
  // Serial init
  Serial.begin(115200);

  // I2C init
  Wire.begin();

  // RTC init
  rtc.begin();

  Serial.println("\r\n");

  // Connect to your WiFi network.
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  // Start UDP Server to receive the NTP response.
  Serial.println("Starting UDP");
  udp.begin(localPort);
}


/**
 * @brief Loop function.
 */
void loop () 
{
  static uint16_t lastMillis = 0;
  uint16_t currentMillis = millis() / 1000;

  if (currentMillis - lastMillis >= 1)
  {
    lastMillis = currentMillis;
    DateTime now = rtc.now(); //get the current date-time
  
  	Serial.printf("%02d-%02d-%02d ", now.year(), now.month(), now.date());
    Serial.printf("%02d:%02d:%02d ", now.hour(), now.minute(), now.second());
  	Serial.println(weekDay[now.dayOfWeek()]);
  	Serial.print("Seconds since Unix Epoch: "); 
  	Serial.print(now.getEpoch(), DEC);
  	Serial.println();
  }

  // handle serial
  if (Serial.available())
  {
    String message = Serial.readStringUntil('\n');
    message.trim();

    Serial.print("message: ");
    Serial.println(message);

    if (message.startsWith("update"))
    {
      Serial.println("Update via NTP.");

      adjust();
    }
  }
}
