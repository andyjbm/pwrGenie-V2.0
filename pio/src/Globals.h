// Globals.h

#pragma once
#ifndef GLOBALS_H
    #define GLOBALS_H

    //#include <ESP.h>
    //#include "Arduino.h"
    #include "defs.h"
    #include "Secrets.h"

    enum pgMode_Opt
    {
        pgMode_Opt_Disabled,
        pgMode_Opt_Send_vBat_Only,
        pgMode_Opt_Receive_Source_Only,
        pgMode_Opt_Both_Source_n_Send
    };

    #ifndef DEFAULT_AP_PASSWORD
        #define DEFAULT_AP_PASSWORD "pwrgenie"
    #endif
    #ifndef DEFAULT_SSID
        #define DEFAULT_SSID "pwrGenie-cfg"
    #endif
    #ifndef DEFAULT_SSID_PW
        #define DEFAULT_SSID_PW "pwrgenie"
    #endif

    const char default_AP_PASSWORD[]    PROGMEM = DEFAULT_AP_PASSWORD;
    const char default_SSID[]           PROGMEM = DEFAULT_SSID;
    const char default_SSID_PASSWORD[]  PROGMEM = DEFAULT_SSID_PW;
       
    // Emoncms Factory Reset Parameters
    #define EMONCMS_FACTORY_NODE   "NewPwrGenie"
    #define EMONCMS_DEFAULT_URI    "/emoncms/input/post"
    
    #ifdef EMONCMS_SERVER
        #define EMONCMS_DEFAULT_URL    EMONCMS_SERVER // Defined in Secrets.h
    #else
        #define EMONCMS_DEFAULT_URL    "*NOT-SET*"
    #endif

    #ifdef EMONCMS_APIKEY
      #define EMONCMS_DEFAULT_APIKEY EMONCMS_APIKEY // Defined in Secrets.h
    #else
      #define EMONCMS_DEFAULT_APIKEY "*NOT-SET*"
    #endif

    //Parameters that are saved to Flash:
    //This Device parameters
    char pgHostname[NAMESIZE] = "";
    char pg_APSSID[SSIDSIZE] ="";
    char pg_APPassword[PWDSIZE] ="";

    // Host wifi connection info
    char pg_wifiSSID[SSIDSIZE]     = "";
    char pg_wifiPassword[PWDSIZE]  = "";
    char pg_wifiSSID1[SSIDSIZE]    = "";
    char pg_wifiPassword1[PWDSIZE] = "";

    //pwrGenie Mode
    uint8_t pgMode = 0; // Default is disabled

    float psuVolts;                     // Used by wifimanager to show on the info pages. No Touchy!

#endif