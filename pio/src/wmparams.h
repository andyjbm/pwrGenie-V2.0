#ifndef WMPARAMS_H
  #define WMPARAMS_h

  #include <ESP.h>
  #include <Arduino.h>
  #include <string.h>
  #include "defs.h"
  #include "Globals.h"
  #include "Utils.h"

  #include "uriwildcard.h"          // Must come b4 wifimanager to override uri.h
  #include "ESPAsync_WiFiManager.h"

  #include "Ecms_Struct.h"

  extern ESPAsync_WiFiManager wm;

  // IDs and descriptions for all our configurable parameters.
  const char P01[] PROGMEM = "Device Name";
  const char P02[] PROGMEM = "HostName";
  const char P03[] PROGMEM = "Access Point SSID";
  const char P04[] PROGMEM = "Access Point Password";
  const char P05[] PROGMEM = "WebServer Port";
  //const char P06[] PROGMEM = "";
  //const char P07[] PROGMEM = "";
  const char P08[] PROGMEM = "LoRa_1 Parameter";
  const char P09[] PROGMEM = "LoRa_2 Parameter";
  const char P10[] PROGMEM = "Channel Number";
  const char P11[] PROGMEM = "Config Page Username";
  const char P12[] PROGMEM = "Config Page Password";

  const char P21[] PROGMEM = "Battery conversion factor";
  const char P22[] PROGMEM = "pwrGenie Source & Send Enabled";

  // Keep in sync with enum pgmode_opt
  const char * pgModeOptions[] = {"Disabled","Send vBat Only","Receive Source Only","Both Source & Send"};
  const byte pgModeOptionsCount = 4;

  const char E01[] PROGMEM = "Emoncms Server";
  const char E02[] PROGMEM = "Emoncms uri";
  const char E03[] PROGMEM = "Emoncms Port";
  const char E04[] PROGMEM = "Emoncms Node";
  const char E05[] PROGMEM = "Emoncms apikey";
  
  ESPAsync_WMParameter custom_devicename( "P01", FPSTR(P01), my_devicename,  NAMESIZE);
  ESPAsync_WMParameter custom_hostname(   "P02", FPSTR(P02), my_hostname,    NAMESIZE);

  ESPAsync_WMParameter custom_APSSID(     "P03", FPSTR(P03), my_APSSID,      SSIDSIZE);
  ESPAsync_WMParameter custom_APPassword( "P04", FPSTR(P04), my_APPassword,  PWDSIZE);
  ESPAsync_WMParameter custom_port(       "P05", FPSTR(P05), String(my_port).c_str(),    NUMBERSIZE,  (const char*)FPSTR(TYPE_NUMBER));

  //ESPAsync_WMParameter custom_wifiSSID("wifiSSID", "wifi SSID", my_wifiSSID,TKIDSIZE);
  //ESPAsync_WMParameter custom_wifiPassword("wifiPassword", "wifi Password", my_wifiPassword, TKIDSIZE);

  ESPAsync_WMParameter custom_LoRa_1(     "P08", FPSTR(P08), my_LoRa_1,  APIKEYSIZE);
  ESPAsync_WMParameter custom_LoRa_2(     "P09", FPSTR(P09), my_LoRa_2,  APIKEYSIZE);
  ESPAsync_WMParameter custom_channel(    "P10", FPSTR(P10), String(my_channel).c_str(), NUMBERSIZE, (const char*)FPSTR(TYPE_NUMBER));

  ESPAsync_WMParameter custom_username(   "P11", FPSTR(P11), my_username, USRNSIZE);
  ESPAsync_WMParameter custom_password(   "P12", FPSTR(P12), my_password, PWDSIZE);

  // Battery Volts calibration factor.
  ESPAsync_WMParameter custom_vfact(      "P21", FPSTR(P21), String(my_vfact).c_str(), 7, (const char*)FPSTR(TYPE_NUMBER));

  ESPAsync_WMParameter custom_pgMode(     "P22", FPSTR(P22), String(my_pg_Mode).c_str(), NUMBERSIZE,"",1, WMParam_type::isSelection, pgModeOptions, pgModeOptionsCount);
 
  //Emoncms Parameters
  ESPAsync_WMParameter emoncms_server("E01", FPSTR(E01), ecmsParams.server, NAMESIZE);
  ESPAsync_WMParameter emoncms_uri(   "E02", FPSTR(E02), ecmsParams.uri,    URISIZE);
  ESPAsync_WMParameter emoncms_port(  "E03", FPSTR(E03), String(ecmsParams.Port).c_str(),   7, (const char*)FPSTR(TYPE_NUMBER));
  ESPAsync_WMParameter emoncms_node(  "E04", FPSTR(E04), ecmsParams.node,   NODESIZE);
  ESPAsync_WMParameter emoncms_apikey("E05", FPSTR(E05), ecmsParams.apikey, APIKEYSIZE);

  ESPAsync_WMParameter custom_LineBreak("<hr>");

  //Set the values of the parameters from the global var values.
  void set_WM_Params(){
    
    custom_devicename.setValue(my_devicename, NAMESIZE);
    custom_hostname.setValue(my_hostname, NAMESIZE);

    custom_APSSID.setValue(my_APSSID, SSIDSIZE);
    custom_APPassword.setValue(my_APPassword, PWDSIZE);
    custom_port.setValue(String(my_port).c_str(), NUMBERSIZE);

// Handled by wifi config page - not needed here.
    //custom_wifiSSID.setValue( my_wifiSSID, TKIDSIZE);
    //custom_wifiPassword.setValue(my_wifiPassword, TKIDSIZE);

    custom_LoRa_1.setValue( my_LoRa_1, APIKEYSIZE);
    custom_LoRa_2.setValue( my_LoRa_2, APIKEYSIZE);
    custom_channel.setValue( String(my_channel).c_str(), NUMBERSIZE);

    custom_username.setValue( my_username, USRNSIZE);
    custom_password.setValue( my_password, PWDSIZE);

    custom_vfact.setValue(String(my_vfact).c_str(), NUMBERSIZE);
    custom_pgMode.setValue(String(my_pg_Mode).c_str(), NUMBERSIZE);

    emoncms_server.setValue(ecmsParams.server,URLSIZE);
    emoncms_uri.setValue(ecmsParams.uri, URISIZE);
    emoncms_port.setValue(String(ecmsParams.Port).c_str(),  NUMBERSIZE);
    emoncms_node.setValue(ecmsParams.node, NODESIZE);
    emoncms_apikey.setValue(ecmsParams.apikey, APIKEYSIZE);
  }

  //Add our custom parameters to the WiFi manager parameter menu
  void add_WM_Params(){

    // The extra parameters to be configured (can be either global or just in the setup)
    // After connecting, parameter.getValue() will get you the configured value
    // id/name placeholder/prompt default length

    wm.addParameter(&custom_devicename);
    wm.addParameter(&custom_hostname);
    
    //ESPAsync_WMParameter custom_api_hint("<hr><label for=\"API\">Service Type</label>");
    //wm.addParameter(&custom_api_hint);

    wm.addParameter(&custom_APSSID);
    wm.addParameter(&custom_APPassword);
    wm.addParameter(&custom_port);

//Don't need these - they are handled by the wifi config page.
    //wm.addParameter(&custom_wifiSSID);
    //wm.addParameter(&custom_wifiPassword);
    
    wm.addParameter(&custom_LoRa_1);
    wm.addParameter(&custom_LoRa_2);
    wm.addParameter(&custom_channel);

    wm.addParameter(&custom_username);
    wm.addParameter(&custom_password);

    wm.addParameter(&custom_vfact);
    wm.addParameter(&custom_LineBreak);
    wm.addParameter(&custom_pgMode);
    wm.addParameter(&custom_LineBreak);

    wm.addParameter(&emoncms_server);
    wm.addParameter(&emoncms_uri);
    wm.addParameter(&emoncms_port);
    wm.addParameter(&emoncms_node);
    wm.addParameter(&emoncms_apikey);
  }

  //Write the parameters from the Wifi manager into the global Vars.
  void retrieve_WM_Params(){
    validateInput(custom_devicename.getValue(), my_devicename);
    validateInput(custom_hostname.getValue(), my_hostname);

    validateInput(custom_APSSID.getValue(), my_APSSID);
    validateInput(custom_APPassword.getValue(), my_APPassword);
    my_port = validateInt(custom_port.getValue());
    
    //Get the wifi SSID and pw from the wifimanager direct.
    validateInput(wm.getSSID().c_str(), my_wifiSSID);
    validateInput(wm.getPW().c_str(), my_wifiPassword);
    validateInput(wm.getSSID1().c_str(), my_wifiSSID1);
    validateInput(wm.getPW1().c_str(), my_wifiPassword1);
    
    validateInput(custom_LoRa_1.getValue(), my_LoRa_1);
    validateInput(custom_LoRa_2.getValue(), my_LoRa_2);
    my_channel = validateInt(custom_channel.getValue());

    validateInput(custom_username.getValue(), my_username);
    validateInput(custom_password.getValue(), my_password);
    
    my_vfact = validateFloat(custom_vfact.getValue());
    //if (my_vfact < ADCDIVISOR * 0.8 || my_vfact > ADCDIVISOR * 1.25) {my_vfact = ADCDIVISOR;}
    my_pg_Mode      = validateInt(custom_pgMode.getValue());

    validateInput(emoncms_server.getValue(), ecmsParams.server);
    validateInput(emoncms_uri.getValue(), ecmsParams.uri);
    ecmsParams.Port = validateInt(emoncms_port.getValue());
    validateInput(emoncms_node.getValue(), ecmsParams.node);     // Text. We allow for node to contain letters. I don't know if emoncms will be happy with that.
    validateInput(emoncms_apikey.getValue(), ecmsParams.apikey);
  }

#endif