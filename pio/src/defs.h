/*
Copyright (c) 2024, Andy Moore
All rights reserved.

This source code is licensed under the GPL-style license found in the
LICENSE file in the root directory of this source tree. 
*/

#pragma once
#ifndef DEFS_H
   #define DEFS_H

   #include "console.h"
   #include "Secrets.h"

/*
********************************
** DEFS for the whple Project **
********************************
*/

   // Ought to move these to build types....
   //#define NO_CONSOLE
   //#define DEBUG false

   /*
    Here we determine from environment flags what this Firmware is going to be.
    A device is only going to be used for one data source.
    Options are now defined in the compile environments. Pick one and compile it.
   
   Old defines for reference:
   #define TYPE_SPL     1  // SPL Meter source built into the device. Bit banging solution implementing LEQ in leqv2.h
   #define TYPE_EM21    2  // Carlo Gavazzi EM21 MODBUS over RS485 source
   #define TYPE_DSE     3  // Deep sea Genset Controller MODBUS over RS485 (Will define by model no when we get into it)
   #define TYPE_APM303  4  // APM 303 Genset Controller via MODBUS over RS485
   #define TYPE_APM403  5  // APM 403 Genset Controller via MODBUS over RS485
   #define TYPE_JKBMS   6  // Jing Kong BMS over TTL Serial. WIP.
   */

   //Logic to set the correct code defines based on the mode
   //that's been chosen by the platformio environment. 
   #ifdef PWR_GENIE_TYPE_NONE
      #define PWR_GENIE_TYPE_STR "No Source"

   #elif defined(PWR_GENIE_TYPE_SPL)
      #define PWR_GENIE_MODE_SPL
      #define PWR_GENIE_TYPE_STR "SPL Meter"
      #define EMONCMS_APIKEY EMONCMS_LPSSPL_APIKEY

   #elif defined(PWR_GENIE_TYPE_EM21)
      #define PWR_GENIE_MODE_MODBUS
      #define MODBUS_DEVICE_EM21
      #define PWR_GENIE_TYPE_STR "EM21 Carlo Gavazzi"
      #define EMONCMS_APIKEY EMONCMS_PWRGENIE_APIKEY

   #elif defined(PWR_GENIE_TYPE_DSE)
      #define PWR_GENIE_MODE_MODBUS
      #define MODBUS_DEVICE_DSE
      #define PWR_GENIE_TYPE_STR "DeepSea"
      #define EMONCMS_APIKEY EMONCMS_PWRGENIE_APIKEY

   #elif defined(PWR_GENIE_TYPE_APM303)
      #define PWR_GENIE_MODE_MODBUS
      #define MODBUS_DEVICE_APM303
      #define PWR_GENIE_TYPE_STR "APM303"
      #define EMONCMS_APIKEY EMONCMS_PWRGENIE_APIKEY

   #elif defined(PWR_GENIE_TYPE_APM403)
      #define PWR_GENIE_MODE_MODBUS
      #define MODBUS_DEVICE_APM403
      #define PWR_GENIE_TYPE_STR "APM403"
      #define EMONCMS_APIKEY EMONCMS_PWRGENIE_APIKEY

   #elif defined(PWR_GENIE_TYPE_JKBMS)
      #define PWR_GENIE_MODE_JKBMS
      #define PWR_GENIE_TYPE_STR "JKBMS"
      #define EMONCMS_APIKEY EMONCMS_JKBMS_APIKEY
      
   #else
      #error "PWR_GENIE_TYPE not set in defs.h! Please pick one!"
   #endif

   // Used to force a wipe of the config file if things are really different.
   #define  CONFIG_FILE_VERSION   "1.00" 

   // Wifi Manger Defs
   #define USE_AVAILABLE_PAGES          // Displays the available pages panel on the Info pages.

   #define WIFI_CONECTION_TIMEOUT 10000 // 10 Seconds

   #define LOOP_INFO_TIME         10000 // in mSec
   #define PWRGENIE_FW_VERSION    "0.8.0"

   //#define SECURE_ENABLED   // Enables SSL sending on 443 for emoncms. Breaks pages - heap corruption? Blank pages sent.
   
   // Battery Volts calibration factor. Default Factory estimate.
   #define ADCDIVISOR 191.8
   #define LOWBATT 3.3        // Not used yet I don't think.

   #define CFGFILE "/config.json"   //Name of the parameter config file in LittleFS

   #ifndef DEBUG
      #define DEBUG true
   #endif

   #ifndef DEBUG_AUTOREFRESH
      #define DEBUG_AUTOREFRESH 10
   #endif

   //#define BearSSL_DEBUG  // This is only relevant on the ESP8266 which doesn't realistically have enough ram for all this anyway.
   
   #ifdef ESP8266 
      #define PIO_PACKAGE_FRAMEWORK_ARDUINO_DECODED_VERSION PIO_PACKAGE_FRAMEWORK_ARDUINOESPRESSIF8266_DECODED_VERSION
   #elif ESP32
      #define PIO_PACKAGE_FRAMEWORK_ARDUINO_DECODED_VERSION PIO_PACKAGE_FRAMEWORK_ARDUINOESPRESSIF32_DECODED_VERSION
   #endif

   #define COMPILED_FRAMEWORK_VERSIONS ((String)FPSTR("pwrGenie V") + FPSTR(PWRGENIE_FW_VERSION) + " " + FPSTR(PIO_PACKAGE_PLATFORM_NAME) + " V" + FPSTR(PIO_PLATFORM_VERSION_FULL) + FPSTR(" Arduino V") + FPSTR(PIO_PACKAGE_FRAMEWORK_ARDUINO_DECODED_VERSION))

   // Project wide constants
   // These are here to avoid circular dependancy between Globals.h and Ecms_Struct.h
   
   // Config page Parameter constants
   const uint8_t NAMESIZE   = 25;
   const uint8_t URLSIZE    = 40;
   const uint8_t URISIZE    = 40;
   const uint8_t APIKEYSIZE = 40;
   const uint8_t NODESIZE   = 15;
   const uint8_t NUMBERSIZE = 10;
   const uint8_t BOOLSIZE   = 2;

   const uint8_t PWDSIZE    = 20;
   const uint8_t USRNSIZE   = 20;
   const uint8_t SSIDSIZE   = 20;

#endif