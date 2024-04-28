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
    CONSOLE(F("Defaultstr will be: ")); CONSOLELN(defaultstr);

    strcpy(my_devicename, defaultstr.c_str());
    strcpy(my_hostname, defaultstr.c_str());
    strcpy(my_APSSID, defaultstr.c_str());
    strcpy(my_APPassword, String(FPSTR(default_AP_PASSWORD)).c_str());
    strcpy(my_wifiSSID, String(FPSTR(default_SSID)).c_str());
    strcpy(my_wifiPassword, String(FPSTR(default_SSID_PASSWORD)).c_str());
    my_wifiSSID1[0] = 0;
    my_wifiPassword1[0] = 0;
  }

  //Save Parameters to Flash Memory using LittleFS
  bool saveConfig(){

    DynamicJsonDocument jsonDoc(1024);

    jsonDoc["FileVersion"] = CONFIG_FILE_VERSION;
    jsonDoc["P01"] = my_devicename;
    jsonDoc["P02"] = my_hostname;
    jsonDoc["P03"] = my_APSSID;         // STA (Access Point) SSID
    jsonDoc["P04"] = my_APPassword;
    jsonDoc["P05"] = my_port;
    jsonDoc["P06"] = my_wifiSSID;       // Host SSID for client mode.
    jsonDoc["P07"] = my_wifiPassword; 
    jsonDoc["P08"] = my_LoRa_1;
    jsonDoc["P09"] = my_LoRa_2;
    jsonDoc["P10"] = my_channel;
    jsonDoc["P11"] = my_username;
    jsonDoc["P12"] = my_password;
    
    jsonDoc["P13"] = my_wifiSSID1;   
    jsonDoc["P14"] = my_wifiPassword1; 
    
    jsonDoc["P21"] = my_vfact;
    jsonDoc["P22"] = my_pg_Mode;

    jsonDoc["E01"] = ecmsParams.server;
    if ((String)ecmsParams.uri="") {strcpy(ecmsParams.uri, EMONCMS_DEFAULT_URI);}
    jsonDoc["E02"] = ecmsParams.uri;
    jsonDoc["E03"] = ecmsParams.Port;
    jsonDoc["E04"] = ecmsParams.node;
    jsonDoc["E05"] = ecmsParams.apikey;

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
            if (jsonDoc.containsKey("FileVersion")) strcpy(my_FileVersion, jsonDoc["FileVersion"]);
            if (jsonDoc.containsKey("P01")) strcpy(my_devicename, jsonDoc["P01"]);
            if (jsonDoc.containsKey("P02")) strcpy(my_hostname, jsonDoc["P02"]);
            if (jsonDoc.containsKey("P03")) strcpy(my_APSSID, jsonDoc["P03"]);
            if (jsonDoc.containsKey("P04")) strcpy(my_APPassword, jsonDoc["P04"]);
            if (jsonDoc.containsKey("P05")) my_port = jsonDoc["P05"];
            if (jsonDoc.containsKey("P06")) strcpy(my_wifiSSID, jsonDoc["P06"]); // = (const char *)jsonDoc["wifiSSID"];
            if (jsonDoc.containsKey("P07")) strcpy(my_wifiPassword, jsonDoc["P07"]); // = (const char *)jsonDoc["wifiPassword"];
            if (jsonDoc.containsKey("P08")) strcpy(my_LoRa_1, jsonDoc["P08"]);
            if (jsonDoc.containsKey("P09")) strcpy(my_LoRa_2, jsonDoc["P09"]);
            if (jsonDoc.containsKey("P10")) my_channel = jsonDoc["P10"];
            if (jsonDoc.containsKey("P11")) strcpy(my_username, jsonDoc["P11"]);
            if (jsonDoc.containsKey("P12")) strcpy(my_password, jsonDoc["P12"]);

            if (jsonDoc.containsKey("P13")) strcpy(my_wifiSSID1, jsonDoc["P13"]); // = (const char *)jsonDoc["wifiSSID"];
            if (jsonDoc.containsKey("P14")) strcpy(my_wifiPassword1, jsonDoc["P14"]); // = (const char *)jsonDoc["wifiPassword"];

            if (jsonDoc.containsKey("P21")) my_vfact = jsonDoc["P21"];
            if (jsonDoc.containsKey("P22")) my_pg_Mode = jsonDoc["P22"];

            if (jsonDoc.containsKey("E01")) strcpy(ecmsParams.server, jsonDoc["E01"]);
            if (jsonDoc.containsKey("E02")) strcpy(ecmsParams.uri, jsonDoc["E02"]);
            if (jsonDoc.containsKey("E03")) ecmsParams.Port = jsonDoc["E03"];
            if (jsonDoc.containsKey("E04")) strcpy(ecmsParams.node, jsonDoc["E04"]);
            if (jsonDoc.containsKey("E05")) strcpy(ecmsParams.apikey, jsonDoc["E05"]);
            
            //Check wifi connection creds are valid.
            if (String(my_wifiSSID).isEmpty()) {
              CONSOLE(F("Config missing wifiSSID, trying credentials stored in ESP: "));
              validateInput(wm.WiFi_SSID().c_str(), my_wifiSSID);
              validateInput(wm.WiFi_Pass().c_str(), my_wifiPassword);
              if (String(my_wifiSSID).isEmpty()) {
                CONSOLELN(F("No valid wifiSSID stored in ESP either!"));
              } else {
                CONSOLELN(F("Found valid wifiSSID stored in ESP, using those."));
                resave = true;
              }
            }
            //Check Access point Creds.
            if (String(my_APSSID).isEmpty()) {
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
      CONSOLELN(F("Loading default AP creds and saving."));
      resetAPcreds();
      resave=true;
    }

    if (String(my_FileVersion).isEmpty()) {
      CONSOLELN(F("Config File Version missing - resaving..."));
      resave = true;
    } else {
      if (!(strcmp(my_FileVersion, CONFIG_FILE_VERSION)==0)) {
        CONSOLELN(F("Config File Version mismatch - resaving..."));
        resave = true;
      }
    }

    if (resave) {
      CONSOLELN(F("Config Loaded, but resave required.\n"));
      return saveConfig();
    }
    CONSOLELN(F("Config Loaded! No issues!\n"));
    return true;
  }
#endif