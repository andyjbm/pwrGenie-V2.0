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

  // struct wmparamData{
  //     String jsonName;
  //     String htmlName;
  //     String customHTML;
  //     String paramValue;
  //     int tokenSize;
  //     ESPAsync_WMParameter wmParameter;
  // };

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length

  //devicename,hostname,APSSID,APPassword,port,LoRa_1,LoRa_2,channel,username,password,vfact,enable_MB_Post

  extern ESPAsync_WiFiManager wm;

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

  const char P31[] PROGMEM = "MODBUS to EMONCMS enabled: ";
  const char P32[] PROGMEM = "DSGenset to EMONCMS enabled: ";
  const char P33[] PROGMEM = "SPL to EMONCMS enabled: ";

  const char E01[] PROGMEM = "Emoncms Server";
  const char E02[] PROGMEM = "Emoncms uri";
  const char E03[] PROGMEM = "Emoncms Port";
  const char E04[] PROGMEM = "Emoncms Node";
  const char E05[] PROGMEM = "Emoncms apikey";
  /*
  const char E01[] PROGMEM = "Carlos Emoncms Server";
  const char E02[] PROGMEM = "Carlos Emoncms uri";
  const char E03[] PROGMEM = "Carlos Emoncms Port";
  const char E04[] PROGMEM = "Carlos Emoncms Node";
  const char E05[] PROGMEM = "Carlos Emoncms apikey";

  const char E11[] PROGMEM = "DeepSea Emoncms Server";
  const char E12[] PROGMEM = "DeepSea Emoncms uri";
  const char E13[] PROGMEM = "DeepSea Emoncms Port";
  const char E14[] PROGMEM = "DeepSea Emoncms Node";
  const char E15[] PROGMEM = "DeepSea Emoncms apikey";

  const char E21[] PROGMEM = "SPL Emoncms Server";
  const char E22[] PROGMEM = "SPL Emoncms uri";
  const char E23[] PROGMEM = "SPL Emoncms Port";
  const char E24[] PROGMEM = "SPL Emoncms Node";
  const char E25[] PROGMEM = "SPL Emoncms apikey";
  */

  ESPAsync_WMParameter custom_devicename( "P01", FPSTR(P01), my_devicename,  NAMESIZE);
  ESPAsync_WMParameter custom_hostname(   "P02", FPSTR(P02), my_hostname,    NAMESIZE);

  ESPAsync_WMParameter custom_APSSID(     "P03", FPSTR(P03), my_APSSID,      SSIDSIZE);
  ESPAsync_WMParameter custom_APPassword( "P04", FPSTR(P04), my_APPassword,  PWDSIZE);
  ESPAsync_WMParameter custom_port(       "P05", FPSTR(P05), String(my_port).c_str(),            NUMBERSIZE,  (const char*)FPSTR(TYPE_NUMBER));

  //ESPAsync_WMParameter custom_wifiSSID("wifiSSID", "wifi SSID", my_wifiSSID,TKIDSIZE);
  //ESPAsync_WMParameter custom_wifiPassword("wifiPassword", "wifi Password", my_wifiPassword, TKIDSIZE);

  ESPAsync_WMParameter custom_LoRa_1(     "P08", FPSTR(P08), my_LoRa_1,  APIKEYSIZE);
  ESPAsync_WMParameter custom_LoRa_2(     "P09", FPSTR(P09), my_LoRa_2,  APIKEYSIZE);
  ESPAsync_WMParameter custom_channel(    "P10", FPSTR(P10), String(my_channel).c_str(),     NUMBERSIZE, (const char*)FPSTR(TYPE_NUMBER));

  ESPAsync_WMParameter custom_username(   "P11", FPSTR(P11), my_username, USRNSIZE);
  ESPAsync_WMParameter custom_password(   "P12", FPSTR(P12), my_password, PWDSIZE);

  ESPAsync_WMParameter custom_vfact(      "P21", FPSTR(P21), String(my_vfact).c_str(), 7, (const char*)FPSTR(TYPE_NUMBER));

  ESPAsync_WMParameter custom_enable_MB_Post(  "P31", FPSTR(P31), String(enable_MB_Post ).c_str(), BOOLSIZE,"",1, true);
  ESPAsync_WMParameter custom_enable_DSG_Post( "P32", FPSTR(P32), String(enable_DSG_Post).c_str(), BOOLSIZE,"",1, true);
  ESPAsync_WMParameter custom_enable_SPL_Post( "P33", FPSTR(P33), String(enable_SPL_Post).c_str(), BOOLSIZE,"",1, true);

  //EM21_ECMS_Server,EM21_ECMS_uri,EM21_ECMS_Port,EM21_ECMS_Node,EM21_ECMS_apikey
  ESPAsync_WMParameter emoncms_server("E01", FPSTR(E01), EcmsParams.server, NAMESIZE);
  ESPAsync_WMParameter emoncms_uri(   "E02", FPSTR(E02), EcmsParams.uri,    URISIZE);
  ESPAsync_WMParameter emoncms_port(  "E03", FPSTR(E03), String(EcmsParams.Port).c_str(),   7, (const char*)FPSTR(TYPE_NUMBER));
  ESPAsync_WMParameter emoncms_node(  "E04", FPSTR(E04), EcmsParams.node,   NODESIZE);
  ESPAsync_WMParameter emoncms_apikey("E05", FPSTR(E05), EcmsParams.apikey, APIKEYSIZE);

  /*
  ESPAsync_WMParameter emoncms_DSG_server("E11", FPSTR(E11), deepSeaGensetEcmsParams.server, NAMESIZE);
  ESPAsync_WMParameter emoncms_DSG_uri(   "E12", FPSTR(E12), deepSeaGensetEcmsParams.uri,    URISIZE);
  ESPAsync_WMParameter emoncms_DSG_port(  "E13", FPSTR(E13), String(    deepSeaGensetEcmsParams.Port).c_str(),   7, (const char*)FPSTR(TYPE_NUMBER));
  ESPAsync_WMParameter emoncms_DSG_node(  "E14", FPSTR(E14), deepSeaGensetEcmsParams.node,   NODESIZE);
  ESPAsync_WMParameter emoncms_DSG_apikey("E15", FPSTR(E15), deepSeaGensetEcmsParams.apikey, APIKEYSIZE);

  ESPAsync_WMParameter emoncms_SPL_server("E21", FPSTR(E21), splEcmsParams.server, NAMESIZE);
  ESPAsync_WMParameter emoncms_SPL_uri(   "E22", FPSTR(E22), splEcmsParams.uri,    URISIZE);
  ESPAsync_WMParameter emoncms_SPL_port(  "E23", FPSTR(E23), String(    splEcmsParams.Port).c_str(),   7, (const char*)FPSTR(TYPE_NUMBER));
  ESPAsync_WMParameter emoncms_SPL_node(  "E24", FPSTR(E24), splEcmsParams.node,   NODESIZE);
  ESPAsync_WMParameter emoncms_SPL_apikey("E25", FPSTR(E25), splEcmsParams.apikey, APIKEYSIZE);
  */
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
    custom_enable_MB_Post.setValue(String(enable_MB_Post).c_str(), BOOLSIZE);
    custom_enable_DSG_Post.setValue(String(enable_DSG_Post).c_str(), BOOLSIZE);
    custom_enable_SPL_Post.setValue(String(enable_SPL_Post).c_str(), BOOLSIZE);

    emoncms_server.setValue(EcmsParams.server,URLSIZE);
    emoncms_uri.setValue(EcmsParams.uri, URISIZE);
    emoncms_port.setValue(String(EcmsParams.Port).c_str(),  NUMBERSIZE);
    emoncms_node.setValue(EcmsParams.node, NODESIZE);
    emoncms_apikey.setValue(EcmsParams.apikey, APIKEYSIZE);
  /*
    emoncms_EM21_server.setValue(mbCarloEM21EcmsParams.server,URLSIZE);
    emoncms_EM21_uri.setValue(mbCarloEM21EcmsParams.uri, URISIZE);
    emoncms_EM21_port.setValue(String(mbCarloEM21EcmsParams.Port).c_str(),  NUMBERSIZE);
    emoncms_EM21_node.setValue(mbCarloEM21EcmsParams.node, NODESIZE);
    emoncms_EM21_apikey.setValue(mbCarloEM21EcmsParams.apikey, APIKEYSIZE);

    emoncms_DSG_server.setValue(deepSeaGensetEcmsParams.server,URLSIZE);
    emoncms_DSG_uri.setValue(deepSeaGensetEcmsParams.uri, URISIZE);
    emoncms_DSG_port.setValue(String(deepSeaGensetEcmsParams.Port).c_str(),  NUMBERSIZE);
    emoncms_DSG_node.setValue(deepSeaGensetEcmsParams.node, NODESIZE);
    emoncms_DSG_apikey.setValue(deepSeaGensetEcmsParams.apikey, APIKEYSIZE);

    emoncms_SPL_server.setValue(splEcmsParams.server,URLSIZE);
    emoncms_SPL_uri.setValue(splEcmsParams.uri, URISIZE);
    emoncms_SPL_port.setValue(String(splEcmsParams.Port).c_str(),  NUMBERSIZE);
    emoncms_SPL_node.setValue(splEcmsParams.node, NODESIZE);
    emoncms_SPL_apikey.setValue(splEcmsParams.apikey, APIKEYSIZE);
    */
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
    wm.addParameter(&custom_enable_MB_Post);
    wm.addParameter(&custom_enable_DSG_Post);
    wm.addParameter(&custom_enable_SPL_Post);

    //wm.addParameter(&custom_LineBreak);
    wm.addParameter(&custom_LineBreak);

    wm.addParameter(&emoncms_server);
    wm.addParameter(&emoncms_uri);
    wm.addParameter(&emoncms_port);
    wm.addParameter(&emoncms_node);
    wm.addParameter(&emoncms_apikey);

    /*
    //MODBUS Parameters
    wm.addParameter(&emoncms_EM21_server);
    wm.addParameter(&emoncms_EM21_uri);
    wm.addParameter(&emoncms_EM21_port);
    wm.addParameter(&emoncms_EM21_node);
    wm.addParameter(&emoncms_EM21_apikey);

    wm.addParameter(&emoncms_DSG_server);
    wm.addParameter(&emoncms_DSG_uri);
    wm.addParameter(&emoncms_DSG_port);
    wm.addParameter(&emoncms_DSG_node);
    wm.addParameter(&emoncms_DSG_apikey);

    wm.addParameter(&emoncms_SPL_server);
    wm.addParameter(&emoncms_SPL_uri);
    wm.addParameter(&emoncms_SPL_port);
    wm.addParameter(&emoncms_SPL_node);
    wm.addParameter(&emoncms_SPL_apikey);
    */
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

    enable_MB_Post  = validateInt(custom_enable_MB_Post.getValue());
    enable_DSG_Post = validateInt(custom_enable_DSG_Post.getValue());
    enable_SPL_Post = validateInt(custom_enable_SPL_Post.getValue());

    validateInput(emoncms_server.getValue(), EcmsParams.server);
    validateInput(emoncms_uri.getValue(), EcmsParams.uri);
    EcmsParams.Port = validateInt(emoncms_port.getValue());
    validateInput(emoncms_node.getValue(), EcmsParams.node);     // Text. We allow for node to contain letters. I don't know if emoncms will be happy with that.
    validateInput(emoncms_apikey.getValue(), EcmsParams.apikey);
  /*
    // Modbus to emoncms parameters
    validateInput(emoncms_EM21_server.getValue(), mbCarloEM21EcmsParams.server);
    validateInput(emoncms_EM21_uri.getValue(), mbCarloEM21EcmsParams.uri);
    mbCarloEM21EcmsParams.Port = validateInt(emoncms_EM21_port.getValue());
    validateInput(emoncms_EM21_node.getValue(), mbCarloEM21EcmsParams.node);     // Text. We allow for node to contain letters. I don't know if emoncms will be happy with that.
    validateInput(emoncms_EM21_apikey.getValue(), mbCarloEM21EcmsParams.apikey);

    validateInput(emoncms_DSG_server.getValue(), deepSeaGensetEcmsParams.server);
    validateInput(emoncms_DSG_uri.getValue(), deepSeaGensetEcmsParams.uri);
    deepSeaGensetEcmsParams.Port = String(emoncms_DSG_port.getValue()).toInt();
    validateInput(emoncms_DSG_node.getValue(), deepSeaGensetEcmsParams.node);     // Text. We allow for node to contain letters. I don't know if emoncms will be happy with that.
    validateInput(emoncms_DSG_apikey.getValue(), deepSeaGensetEcmsParams.apikey);

    validateInput(emoncms_SPL_server.getValue(), splEcmsParams.server);
    validateInput(emoncms_SPL_uri.getValue(), splEcmsParams.uri);
    splEcmsParams.Port = String(emoncms_SPL_port.getValue()).toInt();
    validateInput(emoncms_SPL_node.getValue(), splEcmsParams.node);     // Text. We allow for node to contain letters. I don't know if emoncms will be happy with that.
    validateInput(emoncms_SPL_apikey.getValue(), splEcmsParams.apikey);
    */
  }

#endif