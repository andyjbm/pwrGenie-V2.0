// Globals.h

#pragma once
#ifndef GLOBALS_H
    #define GLOBALS_H

    //#include <ESP.h>
    //#include "Arduino.h"
    #include "defs.h"

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
    
    // Defined in Secrets.h, or here if you're not posting to github...
    //#define EMONCMS_ANDYJBM_APIKEY  "Not-Telling-You"
    //#define EMONCMS_LPSSPL_APIKEY   "Another-Blah-de-Blah"
    //#define EMONCMS_CARLO2_APIKEY   "Nopey-Nothing-2C-Here"
    //#define EMONCMS_ANDYS_SERVER    "your-emoncms-server.co.uk"
   
    // Emoncms Factory Reset Parameters
    #define EMONCMS_FACTORY_NODE   "NewPwrGenie"
    #define EMONCMS_DEFAULT_URI    "/emoncms/input/post"
    #define EMONCMS_DEFAULT_URL    EMONCMS_ANDYS_SERVER
    #ifdef EMONCMS_APIKEY
      #define EMONCMS_DEFAULT_APIKEY EMONCMS_APIKEY // Defined in Secrets.h
    #else
      #define EMONCMS_DEFAULT_APIKEY "*NOT-SET*"
    #endif


    //Parameters that are saved to Flash:
    //This Device parameters
    //char my_FileVersion[NAMESIZE] ="";   // To track json file version changes and resave for a new version.
    
    //char my_devicename[NAMESIZE] = "";
    char pgHostname[NAMESIZE] = "";
    char pg_APSSID[SSIDSIZE] ="";
    char pg_APPassword[PWDSIZE] ="";
    //uint16_t my_port = 80;

    // Host wifi connection info
    char pg_wifiSSID[SSIDSIZE]     = "";
    char pg_wifiPassword[PWDSIZE]  = "";
    char pg_wifiSSID1[SSIDSIZE]    = "";
    char pg_wifiPassword1[PWDSIZE] = "";

    //LoRa Params
    //char my_LoRa_1[APIKEYSIZE] ="";
    //char my_LoRa_2[APIKEYSIZE] ="";
    //uint32_t my_channel;

    //pwrGenie Mode
    uint8_t pgMode = 0; // Default is disabled

    //Config Interface
    //char my_username[USRNSIZE]="";
    //char my_password[PWDSIZE]="";

    // Battery Volts calibration factor.
    // float pg_vfact = ADCDIVISOR;

    float psuVolts;                     // Used by wifimanager to show on the info pages. No Touchy!

#endif