#pragma once
#ifndef WMPARAMS_H
  #define WMPARAMS_h

//  #include <ESP.h>
//  #include <Arduino.h>
//  #include <string.h>

  #include "defs.h"
  #include "Globals.h"

  extern ESPAsync_WiFiManager wm;

  enum pgParam{
      devicename = 0,
      hostname,
      APSSID,         // STA (Access Point) SSID
      APPassword,
      port,
      //wifiSSID,       // Host SSID for client mode.
      //wifiPassword, 
      LoRa_1,
      LoRa_2,
      channel,
      username,       // Config page Username
      password,       // Config Password
      //wifiSSID1,      // 2nd Host SSID for client mode, stored in esp wifi.
      //wifiPassword1, 
      modbusID,

      vfact, // =21,
      lineBreak1,
      pg_Mode,
      lineBreak2,
      e_server, // =101,
      e_uri,
      e_Port,
      e_node,
      e_apikey
  };

  // IDs and descriptions for all our configurable parameters.
  const char Pn01[] PROGMEM = "Device Name";
  const char Pn02[] PROGMEM = "HostName";
  const char Pn03[] PROGMEM = "Access Point SSID";
  const char Pn04[] PROGMEM = "Access Point Password";
  const char Pn05[] PROGMEM = "WebServer Port";
  //const char P06[] PROGMEM = "";
  //const char P07[] PROGMEM = "";
  const char Pn08[] PROGMEM = "LoRa_1 Parameter";
  const char Pn09[] PROGMEM = "LoRa_2 Parameter";
  const char Pn10[] PROGMEM = "Channel Number";
  const char Pn11[] PROGMEM = "Config Page Username";
  const char Pn12[] PROGMEM = "Config Page Password";
  const char Pn15[] PROGMEM = "Modbus ID/Address";
  
  const char Pn21[] PROGMEM = "Battery conversion factor (-ve for calibration voltage)";
  const char Pn22[] PROGMEM = "pwrGenie Source & Send Enabled";

  // Keep in sync with enum pgmode_opt
  const char * pgModeOptions[] = {"Disabled","Send vBat Only","Receive Source Only","Both Source & Send"};
  const byte pgModeOptionsCount = 4;

  const char En01[] PROGMEM = "Emoncms Server";
  const char En02[] PROGMEM = "Emoncms uri";
  const char En03[] PROGMEM = "Emoncms Port";
  const char En04[] PROGMEM = "Emoncms Node";
  const char En05[] PROGMEM = "Emoncms apikey";
  const char emtFM[] PROGMEM = "";
  const char emtDM[]         = "";

  const char * pgpNames[] = {Pn01, Pn02, Pn03, Pn04, Pn05, Pn08, Pn09, Pn10, Pn11, Pn12, Pn15,
                             Pn21, emtFM, Pn22, emtFM, // Line Breaks have no name.
                             En01, En02, En03, En04, En05};

  // The array of Parameter definitions. Instantiated once and then pointed to by the WMParameter Collection.
  #define PG_PARAMCOUNT 20
  ESPAsync_WMParameter *pgParamsX[] = {
    // Format is:           ("HTMLID","LABEL NAME (appears on config page)", "default Value", "Value Size", "Param Type", "Options", "Number of Options" )
    new ESPAsync_WMParameter("P01", FPSTR(pgpNames[pgParam::devicename]),  emtDM, NAMESIZE),
    new ESPAsync_WMParameter("P02", FPSTR(pgpNames[pgParam::hostname]),    emtDM, NAMESIZE),
    new ESPAsync_WMParameter("P03", FPSTR(pgpNames[pgParam::APSSID]),      emtDM, SSIDSIZE),
    new ESPAsync_WMParameter("P04", FPSTR(pgpNames[pgParam::APPassword]),  emtDM, PWDSIZE),
    new ESPAsync_WMParameter("P05", FPSTR(pgpNames[pgParam::port]),        emtDM, NUMBERSIZE, (const char*)FPSTR(TYPE_NUMBER)),
    new ESPAsync_WMParameter("P08", FPSTR(pgpNames[pgParam::LoRa_1]),      emtDM, APIKEYSIZE),
    new ESPAsync_WMParameter("P09", FPSTR(pgpNames[pgParam::LoRa_2]),      emtDM, APIKEYSIZE),
    new ESPAsync_WMParameter("P10", FPSTR(pgpNames[pgParam::channel]),     emtDM, NUMBERSIZE, (const char*)FPSTR(TYPE_NUMBER)),
    new ESPAsync_WMParameter("P11", FPSTR(pgpNames[pgParam::username]),    emtDM, USRNSIZE),
    new ESPAsync_WMParameter("P12", FPSTR(pgpNames[pgParam::password]),    emtDM, PWDSIZE),
    new ESPAsync_WMParameter("P15", FPSTR(pgpNames[pgParam::modbusID]),    emtDM, NUMBERSIZE, (const char*)FPSTR(TYPE_NUMBER)),

    new ESPAsync_WMParameter("P21", FPSTR(pgpNames[pgParam::vfact]),       "150", 9, (const char*)FPSTR(TYPE_NUMBER)), // Default Voltage Scale.
    new ESPAsync_WMParameter("<hr>"), // Line Break
    new ESPAsync_WMParameter("P22", FPSTR(pgpNames[pgParam::pg_Mode]),     emtDM, NUMBERSIZE, "", 1, WMParam_type::isSelection, pgModeOptions, pgModeOptionsCount),
    new ESPAsync_WMParameter("<hr>"),  // Line Break
    
    new ESPAsync_WMParameter("E01", FPSTR(pgpNames[pgParam::e_server]),    EMONCMS_DEFAULT_URL,    NAMESIZE),
    new ESPAsync_WMParameter("E02", FPSTR(pgpNames[pgParam::e_uri]),       EMONCMS_DEFAULT_URI,    URISIZE),
    new ESPAsync_WMParameter("E03", FPSTR(pgpNames[pgParam::e_Port]),      "443", 7, (const char*)FPSTR(TYPE_NUMBER)),
    new ESPAsync_WMParameter("E04", FPSTR(pgpNames[pgParam::e_node]),      EMONCMS_FACTORY_NODE,   NODESIZE),
    new ESPAsync_WMParameter("E05", FPSTR(pgpNames[pgParam::e_apikey]),    EMONCMS_DEFAULT_APIKEY, APIKEYSIZE),
  };

  //Add our custom parameters to the WiFi manager parameter collection.
  void add_WM_Params(){
    for (uint8_t i=0; i<PG_PARAMCOUNT; i++){
      wm.addParameter(pgParamsX[i]);
    }
  }

#endif