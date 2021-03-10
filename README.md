# ESP8266-World-Clock-small
Highly accurate world clock with automatic global daylight saving time settings with a 3.12" display.

Author: Marc Staehli, initial upload March 2021

[![ESP8266-World-Clock](https://github.com/3KUdelta/ESP8266-World-Clock-small/blob/main/pics/IMG_20210310_105127.jpg)](https://github.com/3KUdelta/ESP8266-World-Clock-small)

Features:
- 5 different time zones
- automatic daylight saving time adjustments
- WiFi manager to change WiFi settings in browser (World_Clock_AP, 192.168.4.1)
- Easy 


Build:
- WEMOS D1 mini (ESP8266) - using WiFi to get NTP Time
- 4 4-digit,7-segment LED display for showing 4 individual time zones
- 1 RTC 3231 unit for keeping the accurate time if there is no WIFI available
- 1 Radar sensor (RCWL-0516) for switching on for 2 minutes if there is movement in the room (hidden sensor)
- LDR sensor for adjusting bightness of LEDs

Settings.h includes all individal settings. 
Including <Timezone.h> for calculating the right timezone including global DST settings (https://github.com/JChristensen/Timezone).

Print the box yourself: (https://www.thingiverse.com/thing:4655531)

Hint: You cannot power the LED segment displays off the 5v output pin from the ESP8266, the output has not enough Amps. Instead get the +5v from a solder point directly from the board behind the micro USB plug (see pictures and use volt meter to find correct solder point).

[![ESP8266-World-Clock](https://github.com/3KUdelta/ESP8266-World-Clock/blob/main/pics/WorldClock_Schematic.png)](https://github.com/3KUdelta/ESP8266-World-Clock)

[![ESP8266-World-Clock](https://github.com/3KUdelta/ESP8266-World-Clock/blob/main/pics/IMG_20201108_141833.jpg)](https://github.com/3KUdelta/ESP8266-World-Clock)

[![ESP8266-World-Clock](https://github.com/3KUdelta/ESP8266-World-Clock/blob/main/pics/IMG_20201108_141814.jpg)](https://github.com/3KUdelta/ESP8266-World-Clock)
