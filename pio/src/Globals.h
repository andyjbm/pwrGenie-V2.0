// Globals.h

#include <ESP.h>
#include "Arduino.h"
#include "defs.h"
#include "Ecms_Struct.h"

#ifndef GLOBALS_H
    #define GLOBALS_H

    enum pgMode_Opt
    {
        pgMode_Opt_Disabled,
        pgMode_Opt_Send_vBat_Only,
        pgMode_Opt_Receive_Source_Only,
        pgMode_Opt_Both_Source_n_Send
    };

    const char DEFAULT_SSID[]           PROGMEM = "pwrGenie-cfg";
    const char DEFAULT_SSID_PASSWORD[]  PROGMEM = "pwrgenie";
    const char DEFAULT_AP_PASSWORD[]    PROGMEM = "pwrgenie";

    //Custom http:
    //const char TYPE_HIDDEN[] PROGMEM = "type=\"hidden\"";
    const char TYPE_NUMBER[] PROGMEM = "type=\"number\" step=\"any\"";

    //Parameters that are saved to Flash:
    //This Device parameters
    char my_FileVersion[NAMESIZE] ="";   // To track json file version changes and resave for a new version.
    char my_devicename[NAMESIZE] = "";
    char my_hostname[NAMESIZE] = "";
    char my_APSSID[SSIDSIZE] ="";
    char my_APPassword[PWDSIZE] ="";
    uint16_t my_port = 80;

    //3rd Party wifi connection info
    char my_wifiSSID[SSIDSIZE] =    "";
    char my_wifiPassword[PWDSIZE] = "";
    char my_wifiSSID1[SSIDSIZE] =   "";
    char my_wifiPassword1[PWDSIZE] ="";

    //LoRa Params
    char my_LoRa_1[APIKEYSIZE] ="";
    char my_LoRa_2[APIKEYSIZE] ="";
    uint32_t my_channel;

    //pwrGenie Mode
    int my_pg_Mode = 0; // Default is disabled

    //Config Interface
    char my_username[USRNSIZE]="";
    char my_password[PWDSIZE]="";

    // Battery Volts calibration factor.
    float my_vfact = ADCDIVISOR;

    float psuVolts;                     // Used by wifimanager to show on the info pages. No Touchy!
    emoncmsParams ecmsParams;           // Global config Parameters passed to EMONCMS Server.

#endif