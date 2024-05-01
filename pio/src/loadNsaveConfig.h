#ifndef LOADNSAVE_H
  #define LOADNSAVE_H

  #include <Arduino.h>
  #include <ESP.h>

  #include <LittleFS.h>
  #include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

  #include "defs.h"
  #include "Globals.h"

  extern ESPAsync_WiFiManager wm;

  //Reset the device ident type parameters to factory defaults
  void resetAPcreds(){
    String defaultstr = "pwrGenie-" + String(wm.getChipID(),HEX); // For esp8266
    CONSOLE(F("Reset Creds>> defaultstr will be: ")); CONSOLELN(defaultstr);

    wm.getParameter(pgParam::devicename)->setValue(defaultstr.c_str());
    wm.getParameter(pgParam::hostname)  ->setValue(defaultstr.c_str());
    wm.getParameter(pgParam::APSSID)    ->setValue(defaultstr.c_str());
    wm.getParameter(pgParam::APPassword)->setValue(default_AP_PASSWORD);

    strcpy(pg_wifiSSID,     String(FPSTR(default_SSID)).c_str());
    strcpy(pg_wifiPassword, String(FPSTR(default_SSID_PASSWORD)).c_str());

    pg_wifiSSID1[0] = 0;
    pg_wifiPassword1[0] = 0;
    
    strcpy(pgHostname, defaultstr.c_str());
    strcpy(pg_APSSID, defaultstr.c_str());
    strcpy(pg_APPassword, String(FPSTR(default_AP_PASSWORD)).c_str());
  }

  //Save Parameters to Flash Memory using LittleFS
  bool saveConfig(){

    DynamicJsonDocument jsonDoc(1024);

    jsonDoc["FileVersion"] = CONFIG_FILE_VERSION;
    jsonDoc["P01"] = wm.getParameter(pgParam::devicename)->getParam_asString();// my_devicename;
    jsonDoc["P02"] = wm.getParameter(pgParam::hostname)->getParam_asString();
    jsonDoc["P03"] = wm.getParameter(pgParam::APSSID)->getParam_asString();         // STA (Access Point) SSID
    jsonDoc["P04"] = wm.getParameter(pgParam::APPassword)->getParam_asString();
    jsonDoc["P05"] = wm.getParameter(pgParam::port)->getParam_asString();
    jsonDoc["P06"] = wm.getSSID();       // Host SSID for client mode.
    jsonDoc["P07"] = wm.getPW();

    jsonDoc["P08"] = wm.getParameter(pgParam::LoRa_1)->getParam_asString();
    jsonDoc["P09"] = wm.getParameter(pgParam::LoRa_2)->getParam_asString();
    jsonDoc["P10"] = wm.getParameter(pgParam::channel)->getParam_asString();
    jsonDoc["P11"] = wm.getParameter(pgParam::username)->getParam_asString();
    jsonDoc["P12"] = wm.getParameter(pgParam::password)->getParam_asString();
    jsonDoc["P13"] = wm.getSSID1();   
    jsonDoc["P14"] = wm.getPW1(); 
    
    jsonDoc["P21"] = wm.getParameter(pgParam::vfact)->getParam_asFloat();
    jsonDoc["P22"] = wm.getParameter(pgParam::pg_Mode)->getParam_asInt();

    jsonDoc["E01"] = wm.getParameter(pgParam::e_server)->getParam_asString();
    jsonDoc["E02"] = wm.getParameter(pgParam::e_uri)->getParam_asString();
    jsonDoc["E03"] = wm.getParameter(pgParam::e_Port)->getParam_asString();
    jsonDoc["E04"] = wm.getParameter(pgParam::e_node)->getParam_asString();
    jsonDoc["E05"] = wm.getParameter(pgParam::e_apikey)->getParam_asString();

    //Get globals from the parameters after a config page save.

    strcpy(pg_wifiSSID, wm.getSSID().c_str());
    strcpy(pg_wifiPassword, wm.getPW().c_str());
    strcpy(pg_wifiSSID1, wm.getSSID1().c_str());
    strcpy(pg_wifiPassword1, wm.getPW1().c_str());

    strcpy(pgHostname, wm.getParameter(pgParam::hostname)->getParam_asString().c_str());
    pgMode     = wm.getParameter(pgParam::pg_Mode)->getParam_asInt();
    strcpy(pg_APSSID,     wm.getParameter(pgParam::APSSID)->getParam_asString().c_str()); 
    strcpy(pg_APPassword, wm.getParameter(pgParam::APPassword)->getParam_asString().c_str()); 

    CONSOLE(F("Saving config: "));

    bool saveFailed = false;

    if (!LittleFS.begin())
    {
      CONSOLE(F("Failed to mount file system, need to format LittleFS: "));
      LittleFS.end();
      LittleFS.begin();
      CONSOLELN(LittleFS.format());
      if (!LittleFS.begin())
      {
        CONSOLELN(F("Failed to format file system - hardware issues?"));
        saveFailed = true;
      } else {
        CONSOLELN(F("Format successful."));
      }
    }

    File configFile = LittleFS.open(CFGFILE, "w");
    if (!configFile) {
      CONSOLELN(F("Failed to open config file for writing!"));
      LittleFS.end();
      saveFailed = true;
    }
    if (saveFailed)
    {
      CONSOLELN(F("\n\n*** Config file save failed! BROKEN ESP? Save unsuccessful, sorry. ***\n\n"));
      return false;
    }

    serializeJson(jsonDoc, configFile);
    serializeJson(jsonDoc, Serial);     // Dump to serial monitor.
    configFile.close();
    LittleFS.end();
    CONSOLELN(F("\nConfig saved successfully!\n"));
    return true; 
  }  
  
  //Load Parameters from Flash Memory using LittleFS
  bool loadConfig(){
    CONSOLE(F("Loading config: mounting FS, "));

    bool resave = false;
    bool resetCreds = false;

    if (!LittleFS.begin()) {
      CONSOLELN(F("\nFailed to mount FS - formatting..."));
      CONSOLELN(LittleFS.format());
      resetCreds = true;  
    } else {
      String jsonFileVersion ="";
      CONSOLE(F("mounted file system, "));
      if (!LittleFS.exists(CFGFILE)) {
        CONSOLELN(F("No config file!"));
        resetCreds = true;
      } else {
        //file exists, reading and loading
        CONSOLE(F("Opening config file, "));
        File configFile = LittleFS.open(CFGFILE, "r");
        if (!configFile) {
          CONSOLELN(F("Config file failed to open!"));
          resetCreds = true;
        } else {
          CONSOLE(F("opened config file, reading, size is: "));
          size_t size = configFile.size();
          CONSOLE(size); CONSOLE(F(", "));
          DynamicJsonDocument jsonDoc(size *3);
          DeserializationError deserializeError = deserializeJson(jsonDoc, configFile);
          configFile.close();
          CONSOLELN(F("config file read and closed, parsing json: "));

          if (deserializeError) {
            CONSOLE(F("Failed to parse json config: "));
            CONSOLELN(deserializeError.c_str());
            resetCreds=true;
          } else {

            // This outputs the file we read to the console:
            CONSOLELN(F("\nParsed json: "));
            serializeJson(jsonDoc, Serial);
            CONSOLELN("\n");
            
            //Let's load the config file!
            // Put the values directly into the WMParameter collection and don't bother with global Vars.
            if (jsonDoc.containsKey("FileVersion")) jsonFileVersion = (String)jsonDoc["FileVersion"];
            if (jsonDoc.containsKey("P01")) wm.getParameter(pgParam::devicename)->setValue(jsonDoc["P01"]);
            if (jsonDoc.containsKey("P02")) wm.getParameter(pgParam::hostname)  ->setValue(jsonDoc["P02"]);
            if (jsonDoc.containsKey("P03")) wm.getParameter(pgParam::APSSID)    ->setValue(jsonDoc["P03"]);
            if (jsonDoc.containsKey("P04")) wm.getParameter(pgParam::APPassword)->setValue(jsonDoc["P04"]);
            if (jsonDoc.containsKey("P05")) wm.getParameter(pgParam::port)      ->setValue(jsonDoc["P05"]);
            if (jsonDoc.containsKey("P06")) strcpy(pg_wifiSSID,     jsonDoc["P06"]); // = (const char *)jsonDoc["wifiSSID"];
            if (jsonDoc.containsKey("P07")) strcpy(pg_wifiPassword, jsonDoc["P07"]); // = (const char *)jsonDoc["wifiPassword"];
            if (jsonDoc.containsKey("P08")) wm.getParameter(pgParam::LoRa_1)    ->setValue(jsonDoc["P08"]);
            if (jsonDoc.containsKey("P09")) wm.getParameter(pgParam::LoRa_2)    ->setValue(jsonDoc["P09"]);
            if (jsonDoc.containsKey("P10")) wm.getParameter(pgParam::channel)   ->setValue(jsonDoc["P10"]);
            if (jsonDoc.containsKey("P11")) wm.getParameter(pgParam::username)  ->setValue(jsonDoc["P11"]);
            if (jsonDoc.containsKey("P12")) wm.getParameter(pgParam::password)  ->setValue(jsonDoc["P12"]);

            if (jsonDoc.containsKey("P13")) strcpy(pg_wifiSSID1, jsonDoc["P13"]); // = (const char *)jsonDoc["wifiSSID"];
            if (jsonDoc.containsKey("P14")) strcpy(pg_wifiPassword1, jsonDoc["P14"]); // = (const char *)jsonDoc["wifiPassword"];

            if (jsonDoc.containsKey("P21")) wm.getParameter(pgParam::vfact)     ->setValue(jsonDoc["P21"]);
            if (jsonDoc.containsKey("P21")) wm.getParameter(pgParam::vfact)     ->setValue(jsonDoc["P21"]);
            if (jsonDoc.containsKey("P22")) wm.getParameter(pgParam::pg_Mode)   ->setValue(jsonDoc["P22"]);

            if (jsonDoc.containsKey("E01")) wm.getParameter(pgParam::e_server)  ->setValue(jsonDoc["E01"]);
            if (jsonDoc.containsKey("E02")) wm.getParameter(pgParam::e_uri)     ->setValue(jsonDoc["E02"]);
            if (jsonDoc.containsKey("E03")) wm.getParameter(pgParam::e_Port)    ->setValue(jsonDoc["E03"]);
            if (jsonDoc.containsKey("E04")) wm.getParameter(pgParam::e_node)    ->setValue(jsonDoc["E04"]);
            if (jsonDoc.containsKey("E05")) wm.getParameter(pgParam::e_apikey)  ->setValue(jsonDoc["E05"]);

            //Globals we do need...
            strcpy(pgHostname,    wm.getParameter(pgParam::hostname)->getParam_asString().c_str());
            pgMode     = wm.getParameter(pgParam::pg_Mode)->getParam_asInt();
            strcpy(pg_APSSID,     wm.getParameter(pgParam::APSSID)->getParam_asString().c_str()); 
            strcpy(pg_APPassword, wm.getParameter(pgParam::APPassword)->getParam_asString().c_str()); 

            if (String(jsonFileVersion).isEmpty()) {
              CONSOLELN(F("Config File Version missing - resaving..."));
              resave = true;
            } else {
              if (!(jsonFileVersion == CONFIG_FILE_VERSION)) {
                CONSOLELN(F("Config File Version mismatch - resaving..."));
                resave = true;
              }
            }
            
            //Check wifi connection creds are valid.
            if (String(pg_wifiSSID).isEmpty()) {
              CONSOLE(F("Config missing wifiSSID, trying credentials stored in ESP: "));
              if (String(wm.WiFi_SSID().c_str()).isEmpty()) {
                CONSOLELN(F("No valid wifiSSID stored in ESP either!"));
              } else {
                CONSOLELN(F("Found valid wifiSSID stored in ESP, using those."));
                resave = true;
              }
            }
            //Check Access point Creds.
            if (String(wm.getParameter(pgParam::APSSID)->getValue()).isEmpty()) {
              CONSOLELN(F("Config missing AP SSID."));
              resetCreds=true;
            }
            // Loading Successful. Done.
          }
        }
      }
    }

    if (resetCreds || resave) { CONSOLELN("\n");}
    
    if (resetCreds){
      CONSOLELN(F("resetCreds True, Erase Called>> Loading default AP creds and saving."));
      resetAPcreds();
      resave=true;
    }

    if (resave) {
      CONSOLELN(F("Config Loaded, but resave required.\n"));
      return saveConfig();
    }
    CONSOLELN(F("Config Loaded! No issues!\n"));
    return true;
  }
#endif