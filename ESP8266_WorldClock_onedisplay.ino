#include "Settings.h"
#include "Bitmap.h"

#include <Arduino.h>
#include <U8g2lib.h>                        // https://github.com/olikraus/u8g2
#include <SPI.h>
#include <ESP8266WiFi.h>                    // WiFI
#include <WiFiUdp.h>                        // For NTP Signal fetch
#include <EasyNTPClient.h>                  // For NTP Signal read https://github.com/aharshac/EasyNTPClient
#include <TimeLib.h>                        // For converting NTP time https://github.com/PaulStalloffregen/Time.git
#include "RTClib.h"                         // standard Arduino library handling ds3231 RTC module
#include <Wire.h>                           // I2C connection to ds3231
#include <WiFiManager.h>                    // https://github.com/tzapu/WiFiManager

#define SPI_CLOCK 14
#define SPI_DATA 12
#define SPI_CS 15
#define SPI_DC 0

DateTime now_3231;                                // RTC variable
time_t SYD_time, ZRH_time, LON_time, SIN_time, TOK_time;
char timestring[6];
unsigned long timestamp, epochtime_ntp;
bool online = false;
bool wifi = false;

WiFiManager wm;                                    // initializing WiFi Manager
WiFiUDP udp;                                       // WiFi UDP initialization for NTP
EasyNTPClient ntpClient(udp, NTP_SERVER, TZ_SEC);  // NTP initialization
RTC_DS3231 rtc_3231;                               // ds3231 RTC initialization

U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI u8g2(U8G2_R0, SPI_CLOCK, SPI_DATA, SPI_CS, SPI_DC);
//U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC, /* reset=*/ 0);

void setup(void) {

  Serial.begin(115200); while (!Serial); delay(200);
  Serial.println();
  Serial.printf("Starting WorldClock OneDisplay...\r\n");

  Wire.begin(4, 5);                                // ensuring read from SLA,SLC ports 4 and 5 on ESP8266

  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R1);                // 90 degree clockwise rotation
  u8g2.clearBuffer();                              // clear the internal memory
  u8g2.setFont(u8g2_font_tom_thumb_4x6_t_all);     // https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(0, 10, "Starting engine");
  u8g2.sendBuffer();
  delay(1000);

  if (! rtc_3231.begin()) {
    Serial.println("Couldn't find RTC");
    u8g2.drawStr(0, 20, "RTC error");
    u8g2.sendBuffer();
    delay(3000);
  }
  else {
    Serial.println("RTC found");
    u8g2.drawStr(0, 20, "RTC OK");
    u8g2.sendBuffer();
    delay(1000);
  }

  setTime(rtc_3231.now().unixtime());     // setting esp8266 to time stored in rtc3231
  // just in case we get no signal from wifi or from NTP server

  wm.setConfigPortalBlocking(false);      // setiing WiFi manager to non-blocking mode
  go_online();                            // go online and try to get NTP time

  timestamp = now();                      // reset timestamp
}

void loop(void) {

  wm.process();                           // WiFi Manager in non-blocking mode

  ZRH_time = CE.toLocal(now(), &tcr);
  LON_time = UK.toLocal(now(), &tcr);
  SIN_time = SINGA.toLocal(now(), &tcr);
  TOK_time = TOKIO.toLocal(now(), &tcr);
  SYD_time = ausET.toLocal(now(), &tcr);

  u8g2.clearBuffer();
  u8g2.setFontMode(1);                   // with background

  u8g2.setFont(u8g2_font_VCR_OSD_tn);
  do_timestring(ZRH_time);
  centerStr(20, timestring);
  u8g2.setFont(u8g2_font_7x13_tf);
  centerStr(30, "zurich");

  u8g2.setFont(u8g2_font_VCR_OSD_tn);
  do_timestring(LON_time);
  centerStr(70, timestring);
  u8g2.setFont(u8g2_font_7x13_tf);
  centerStr(80, "london");

  u8g2.setFont(u8g2_font_VCR_OSD_tn);
  do_timestring(SIN_time);
  centerStr(120, timestring);
  u8g2.setFont(u8g2_font_7x13_tf);
  centerStr(130, "singapore");

  u8g2.setFont(u8g2_font_VCR_OSD_tn);
  do_timestring(TOK_time);
  centerStr(170, timestring);
  u8g2.setFont(u8g2_font_7x13_tf);
  centerStr(180, "tokio");

  u8g2.setFont(u8g2_font_VCR_OSD_tn);
  do_timestring(SYD_time);
  centerStr(220, timestring);
  u8g2.setFont(u8g2_font_7x13_tf);
  centerStr(230, "sydney");

  u8g2.setFont(u8g2_font_5x8_tf);
  if (online)
    centerStr(256, "NTP data");
  else {
    if (wifi) centerStr(256, "WiFi RTC");
    else centerStr(256, "no WiFi RTC");
  }

  u8g2.sendBuffer();
  delay(1000);

  if ((now() - timestamp) > NTPUPDATE) {    // get all x hours (NTPUPDATE in Settings.h) a time update from NTP Server --> avoiding a constant read from time server
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    delay(500);
    go_online();
    timestamp = now();                      // reset timestamp
  }
}

void centerStr(int y, char *text)           // helper function to center text, params (y and "text")
{
  int x = u8g2.getStrWidth(text);
  x = (u8g2.getDisplayWidth() - x) / 2;
  u8g2.drawStr(x, y, text);
}

void do_timestring(time_t localt)          // helper function to construct the 08:30 format
{
  String hhour, hminute, stringconstruct;
  if (hour(localt) < 10) {
    hhour = "0" + String(hour(localt));
  }
  else {
    hhour = String(hour(localt));
  }
  if (minute(localt) < 10) {
    hminute = "0" + String(minute(localt));
  }
  else {
    hminute = String(minute(localt));
  }
  stringconstruct = hhour + ":" + hminute;
  stringconstruct.toCharArray(timestring, 6);
}

void go_online() {
  WiFi.mode(WIFI_STA);
  WiFi.hostname("WorldClock");
  Serial.print("---> Connecting to WiFi ");
  u8g2.setFont(u8g2_font_tom_thumb_4x6_t_all);
  centerStr(150, "World Clock");
  u8g2.drawXBM( 0, 150, Boxer_w_width, Boxer_w_height, Boxer_w_bits);
  centerStr(240, "Marc Staehli");
  centerStr(250, "01/2021");
  u8g2.drawStr(0, 30, "Adjusting Time");
  u8g2.drawStr(0, 40, "Going online...");
  u8g2.sendBuffer();
  delay(500);

  if (wm.autoConnect("WorldClock_AP")) {     // using WiFi manager in non-blocking mode
    Serial.println(" Wifi connected ok");
    u8g2.drawStr(0, 50, "WiFi OK");
    u8g2.sendBuffer();
    wifi = true;
  }
  else {
    Serial.println("Could not connect to WiFi!");
    Serial.println("Configportal running: 'WorldClock_AP' use 192.168.4.1 to access settings.");
    u8g2.drawStr(0, 55, "** No WiFi! **");
    u8g2.drawStr(0, 65, "WiFi settings at");
    u8g2.drawStr(0, 75, "'WorldClock_AP'");
    u8g2.drawStr(0, 85, "192.168.4.1");
    u8g2.sendBuffer();
    delay(10000);
    online = false;
    wifi = false;
  }
  delay(500);
  get_NTP_time();
}

void get_NTP_time() {

  Serial.println("---> Now reading time from NTP Server");
  u8g2.drawStr(0, 95, "Reading NTP");
  u8g2.sendBuffer();

  int inc = 0;
  while (!ntpClient.getUnixTime()) {
    delay(500);
    if (inc++ > 10) {
      Serial.println("Could not connect to NTP!");
      Serial.println("Using stored RTC time instead!");
      u8g2.drawStr(0, 115, "No NTP connect");
      u8g2.drawStr(0, 125, "Using RTC data");
      u8g2.sendBuffer();
      delay(3000);
      u8g2.clearBuffer();
      u8g2.sendBuffer();
      online = false;
      return;
    }
    u8g2.drawStr(inc, 105, "-");
    u8g2.sendBuffer();
    Serial.print(".");
  }
  u8g2.drawStr(0, 105, "NTP OK");
  u8g2.sendBuffer();
  Serial.println("NTP read success");
  delay(500);

  epochtime_ntp = ntpClient.getUnixTime(); //get UNIX timestamp (seconds from 1.1.1970)

  if (timedifference(rtc_3231.now().unixtime(), epochtime_ntp) != 0) {   // correct time in rtc_3231 if there is a time difference
    online = true;
    setTime(epochtime_ntp);                  // set systemtime in ESP8266 to UTC
    Serial.println("UTC set to processor");
    rtc_3231.adjust(DateTime(year(now()), month(now()), day(now()), hour(now()), minute(now()), second(now()))); // set UTC to 3231 RTC
    Serial.println("UTC set to RTC success");
    u8g2.drawStr(0, 80, "UTC set success");
    u8g2.sendBuffer();
    delay(500);
  }
} // end get_NTP_time()

int timedifference(time_t timeone, time_t timetwo) {
  int t_diff = 0;
  if (timeone > timetwo) {
    t_diff = timeone - timetwo;
  }
  if (timeone < timetwo) {
    t_diff = timetwo - timeone;
  }
  return t_diff;
} // end timedifference()
