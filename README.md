# ESP8266-World-Clock-small
Highly accurate world clock with automatic global daylight saving time settings with a 3.12" display.

Author: Marc Staehli, initial upload March 2021

[![ESP8266-World-Clock](https://github.com/3KUdelta/ESP8266-World-Clock-small/blob/main/pics/IMG_20210310_105127.jpg)](https://github.com/3KUdelta/ESP8266-World-Clock-small)

Features:
- Very simple build
- 5 different time zones
- automatic daylight saving time adjustments
- WiFi manager to change WiFi settings in browser (World_Clock_AP, 192.168.4.1)
- Easy settings for the 5 time zones in Settings.h

Build:
- WEMOS D1 mini pro (ESP8266) - using WiFi to get NTP Time
- OLED Display 3.12" 256*64 showing 5 timezones
- 1 RTC 3231 unit for keeping the accurate time if there is no WIFI available

Settings.h includes all individal settings. 
Including <Timezone.h> for calculating the right timezone including global DST settings (https://github.com/JChristensen/Timezone).

Print the box yourself: (https://www.thingiverse.com/thing:4655531)

[![ESP8266-World-Clock](https://github.com/3KUdelta/ESP8266-World-Clock/blob/main/pics/WorldClock_Schematic.png)](https://github.com/3KUdelta/ESP8266-World-Clock)

[![ESP8266-World-Clock](https://github.com/3KUdelta/ESP8266-World-Clock/blob/main/pics/IMG_20201108_141833.jpg)](https://github.com/3KUdelta/ESP8266-World-Clock)

[![ESP8266-World-Clock](https://github.com/3KUdelta/ESP8266-World-Clock/blob/main/pics/IMG_20201108_141814.jpg)](https://github.com/3KUdelta/ESP8266-World-Clock)
