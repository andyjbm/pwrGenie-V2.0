#ifndef DEFS_H
   #define DEFS_H
   #pragma once

/*
********************************
** DEFS for the whple Project **
********************************
*/

   // Ought to move these to build types....
   //#define NO_CONSOLE
   //#define DEBUG false

   /*
    Here we define what this Firmware is going to be.
    A device is only going to be used for one data source.
    Options are:
   */
   
   #define TYPE_NONE    0
   #define TYPE_SPL     1  // SPL Meter source built into the device. Bit banging solution implementing LEQ in leqv2.h
   #define TYPE_EM21    2  // Carlo Gavazzi EM21 MODBUS over RS485 source
   #define TYPE_DSE     3  // Deep sea Genset Controller MODBUS over RS485 (Will define by model no when we get into it)
   #define TYPE_APM303  4  // APM 303 Genset Controller via MODBUS over RS485
   #define TYPE_APM403  5  // APM 403 Genset Controller via MODBUS over RS485
   #define TYPE_JKBMS   6  // Jing Kong BMS over TTL Serial. WIP.
   
   //#define PWR_GENIE_TYPE TYPE_NONE
   
   
   // Here we pick one of the above.
   #define PWR_GENIE_TYPE TYPE_JKBMS
   
   //Logic to set the correct code defines based on the mode that's been chosen.
   #if PWR_GENIE_TYPE == TYPE_NONE

   #elif PWR_GENIE_TYPE == TYPE_SPL
      #define PWR_GENIE_MODE_SPL

   #elif PWR_GENIE_TYPE == TYPE_EM21 
      #define PWR_GENIE_MODE_MODBUS
      #define MODBUS_DEVICE_EM21

   #elif PWR_GENIE_TYPE == TYPE_DSE
      #define PWR_GENIE_MODE_MODBUS
      #define MODBUS_DEVICE_DSE

   #elif PWR_GENIE_TYPE == TYPE_APM303
      #define PWR_GENIE_MODE_MODBUS
      #define MODBUS_DEVICE_APM303

   #elif PWR_GENIE_TYPE == TYPE_APM403
      #define PWR_GENIE_MODE_MODBUS
      #define MODBUS_DEVICE_APM403

   #elif PWR_GENIE_TYPE == TYPE_JKBMS
      #define PWR_GENIE_MODE_JKBMS

   #else
      #error "PWR_GENIE_TYPE not set in defs.h! Please pick one!"
   #endif

   // Used to force a wipe of the config file if things are really different.
   #define  CONFIG_FILE_VERSION "1.00" 

   // Wifi Manger Defs
   #define USE_AVAILABLE_PAGES

   #define LOOP_INFO_TIME 10 // in Seconds
   #define FIRMWAREVERSION "0.7.0"

   //#define SECURE_ENABLED   // Enables SSL sending on 443 for emoncms. Breaks pages - heap corruption? Blank pages sent.
   
   // Battery Volts calibration factor. Default Factory estimate.
   #define ADCDIVISOR 191.8
   #define LOWBATT 3.3        // Not used yet I don't think.

   #define CFGFILE "/config.json"

   #ifndef DEBUG
      #define DEBUG true
   #endif


/*
******************************
** End of Project wide Defs **
******************************
*/

   // Config Parameter defines
   #define NAMESIZE 25
   #define URLSIZE 40
   #define URISIZE 40
   #define APIKEYSIZE 40
   #define NODESIZE 10
   #define NUMBERSIZE 10
   #define BOOLSIZE 2

   #define PWDSIZE 20
   #define USRNSIZE 20
   #define SSIDSIZE 20

   // Console/debug defs.
   #ifdef NO_CONSOLE
      #define CONSOLE(x) \
         do             \
         {              \
         } while (0)
      #define CONSOLELN CONSOLE
      #define CONSOLEF CONSOLE
   #else
      #define CONSOLE(...)               \
         do                             \
         {                              \
            Serial.print(__VA_ARGS__); \
         } while (0)
      #define CONSOLELN(...)               \
         do                               \
         {                                \
            Serial.println(__VA_ARGS__); \
         } while (0)
   #endif
#endif