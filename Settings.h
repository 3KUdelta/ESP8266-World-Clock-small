// NTP settings ************************************************************************************************

#define NTP_SERVER "ch.pool.ntp.org"                         // for NTP Server - us any NPT server nearby
#define TZ 0                                                 // grabbing UTC from time server
#define TZ_SEC ((TZ)*3600)                                   // do not change this

#define NTPUPDATE (1 * 3600)                                 // Frequency to update time in RCT from NTP
                                                             // (default 1 * 3600) equals hourly update

// Timezone settings *******************************************************************************************

#include <Timezone.h>  // Calculating the right timezone including DST https://github.com/JChristensen/Timezone

// Central European Time (Zurich, Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);

// United Kingdom (London, Belfast)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
Timezone UK(BST, GMT);

// Singapore
TimeChangeRule SINT = {"ST", 1, 1, 1, 0, 480};              // Singapore standard +8 hours, no DST
Timezone SINGA(SINT, SINT);

// Tokio
TimeChangeRule JST = {"JST", 1, 1 ,1, 0, 540};              // Toko standard +9 hours, no DST
Timezone TOKIO(JST, JST);

// Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours
Timezone ausET(aEDT, aEST);

TimeChangeRule *tcr;                        //pointer to the time change rule, use to get the TZ abbrev

// *************************************************************************************************************
