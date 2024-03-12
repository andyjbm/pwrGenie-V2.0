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


   #define PWR_GENIE_MODE_EM21   // Options: EM21, DSE, SPL
   //#define PWR_GENIE_MODE_DSE    // Options: EM21, DSE, SPL
   //#define PWR_GENIE_MODE_SPL      // Options: EM21, DSE, SPL


   //Modbus Device Selection:
   //#define MODBUS_DEVICE_EM21
   #define MODBUS_DEVICE_APM303
   //#define MODBUS_DEVICE_APM403

   #define  CONFIG_FILE_VERSION "1.00"

   // Wifi Manger Defs
   #define USE_AVAILABLE_PAGES

   #define LOOP_INFO_TIME 10 // in Seconds
   #define FIRMWAREVERSION "0.6.0"

   //#define SECURE_ENABLED   // Enables SSL sending on 443 for emoncms. Breaks pages - heap corruption? Blank pages sent.
   
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