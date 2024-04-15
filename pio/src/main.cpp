/*
Copyright (c) 2024, AndiusPandius
All rights reserved.

This source code is licensed under the GPL-style license found in the
LICENSE file in the root directory of this source tree.

Credits to: 
  khoih-prog Khoi Hoang for ESPAsync_WiFiManager 
  https://github.com/khoih-prog/ESPAsync_WiFiManager

  ArminJo for JK-BMSToPylontechCAN
  https://github.com/ArminJo/JK-BMSToPylontechCAN
*/

#include <Arduino.h>
#include <ESP.h>

#include <FS.h>                    //this needs to be first, or it all crashes and burns...
#include <LittleFS.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include "defs.h"
#include "Globals.h"
#include "Utils.h"

#include "uriwildcard.h"          // Must come b4 wifimanager to override uri.h
#include "ESPAsync_WiFiManager.h"

#ifdef PWR_GENIE_MODE_MODBUS
  #include "modbus.h"
#endif
#ifdef PWR_GENIE_MODE_SPL
  #include "spl.h"
#endif
#ifdef PWR_GENIE_MODE_JKBMS
  #include "JKBMS.h"
#endif

#include "emoncms.h"
#include "wmparams.h"
#include "loadNsaveConfig.h"

#ifdef BearSSL_DEBUG
  #include <StackThunk.h>
  uint16_t max_stack_usage = 0;
#endif

AsyncWebServer webServer(80); //Async_webserver_HTTP_PORT);
AsyncDNSServer dnsServer;
ESPAsync_WiFiManager wm(&webServer, &dnsServer, "ESP_WebServer");

unsigned long milliCounter;   // For the loop timer.

//Callback after changes to Wifi page or Config page.
void paramsChangedCallback() {
  CONSOLELN(F("paramsChanged Callback: "));
  CONSOLELN(F("Get Params from WM: "));
  retrieve_WM_Params();
  
  saveConfig();     // save the custom parameters to FS
}

AsyncWebServerResponse *addHeaders(AsyncWebServerResponse *response){
  response->addHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate")); // @HTTPHEAD send cache
  response->addHeader(F("Pragma"), F("no-cache"));
  response->addHeader(F("Expires"), F("-1"));
  return response;
}

//Handle http call from IOT client to this device and forward it onto the emoncms server.
void handleEmoncms(AsyncWebServerRequest *request){
  // The magic happens here.
  CONSOLELN(F("Page Request: handleEmoncms call."));

  String message = FPSTR(F("Call to EMONCMS Handler\n\n"));
  message += FPSTR(S_uri);     // @token uri
  message += request->url();
  message += FPSTR(S_method);  // @token method
  message += (request->method() == HTTP_GET) ? FPSTR(S_GET) : FPSTR(S_POST);
  message += FPSTR(S_args); // @token args
  message += request->args();
  message += F("\n");

  for ( uint8_t i = 0; i < request->args(); i++ ) {
    message += " " + request->argName ( i ) + ": " + request->arg ( i ) + "\n";
  }
  CONSOLELN(message);
  
  request->send(addHeaders(request->beginResponse(200, FPSTR(WM_HTTP_HEAD_CT2), message)));  //CT2 Plain text.
  //SO that's adknowledged the source. 
  //Now we need to relay it on to emoncms over lora or whatever method we choose.
}

String debugStrInfo(const __FlashStringHelper * title, String sPass, String sFail, bool resultOk = true, bool first = false)
{
  String str = "";
  if (first){
    str = FPSTR("<h4 style='margin-top:0px;'>{T}:</h4><small>{Y}</small>");
  } else {
    str = FPSTR("<h4>{T}: {R}:</h4><small>{Y}</small>");
  }

  str.replace(FPSTR("{T}"), title);  
  if (resultOk) {
    str.replace(FPSTR("{R}"), FPSTR("Ok"));
    str.replace(FPSTR("{Y}"), sPass);
  } else {
    str.replace(FPSTR("{R}"), FPSTR("failed"));
    str.replace(FPSTR("{Y}"), sFail);
  }
  return str;
}

// Handle our debug page.
void handleDebug(AsyncWebServerRequest *request){
  CONSOLELN(F("Page Request: handleDebug call."));

  #if (DEBUG_AUTOREFRESH == 0)  
    String page = wm.buildHeader(FPSTR("main_Debug"),FPSTR("Debug Info"));  // Without the auto refresh for debugging the webpage HTML
  #elif (DEBUG_AUTOREFRESH == 1)
    String page = wm.buildHeader(FPSTR("main_Debug"),FPSTR("Debug Info"), WM_META_AUTO_DEBUG_1s);
  #else
    String page = wm.buildHeader(FPSTR("main_Debug"),FPSTR("Debug Info"), WM_META_AUTO_DEBUG_10s);
  #endif

  page += FPSTR("<fieldset><div class = 'msg' style='word-wrap: break-word;'>");
  
  // The last Emoncms data string to be posted.
  page += debugStrInfo(F("Emoncms last json data"), strJsonData, FPSTR("Is Empty!"), !strJsonData.isEmpty(), true);
  
  // The last result from Emoncms:
  page += debugStrInfo(F("Emoncms last result"), ecms_LastResult, FPSTR("Is Empty!"), !ecms_LastResult.isEmpty());
 
  #ifdef PWR_GENIE_MODE_SPL
    bool SPLfailed = false;
    String SPLfailStr = "";
    if (ssreader::LEQInfo.isEmpty()){
      SPLfailed = true;
      SPLfailStr = FPSTR("Is Empty!");
    } else {
      if ((ssreader::LEQInfo.indexOf("nan") > 0) || (ssreader::LEQInfo.indexOf("inf") > 0)) {
        //SPLfailed = true;
        ssreader::LEQInfo = FPSTR("SPL is nan/inf! RESTART with SPL Meter in SLOW mode!<br>") + ssreader::LEQInfo; 
      }
    }
    page += debugStrInfo(F("SPL Data"), ssreader::LEQInfo, SPLfailStr, !SPLfailed);
  #endif
  
  #ifdef PWR_GENIE_MODE_MODBUS
    page += debugStrInfo(F("MODBUS last result"), modbus_LastResult, modbus_LastResult, modbusSuccess);
  #endif
  
  #ifdef PWR_GENIE_MODE_JKBMS
    page += debugStrInfo(F("JKBMS lifetime timeouts"), (String)sTimeoutFrameCounterLifetime, "");
    page += debugStrInfo(F("JKBMS last result"), JK_LastResultData, "", JK_LastFrameReceivedOK);
  #endif
  
  page += "</div></fieldset>";
  wm.HTTPSendPage(page, request);
  CONSOLELN(F("HandleDebug page sent."));
}

// This is a callback made when the wifimanager webserver is created for the purposes of
// adding our own custom page handlers to the webserver.
void configMyWebHandlers(){
  CONSOLELN(F("MAIN: Call to configMyWebHandlers..."));
  wm.server->on("/emoncms/*", handleEmoncms); //This requires wildcard mod to uri.h in ESP8266Webserver Lib
  wm.server->on("/debug", handleDebug); 
}

// Get everything ready.
void setup() {
  
  // Set up console serial and say hello...
  #ifdef SERIAL_115400
    Serial.begin(115400);
  #else
    Serial.begin(921600);
  #endif
  delay(200);

  CONSOLELN(F("\n\nMAIN: Hello there, it's a new day!"));
  CONSOLELN(F("MAIN: Firmware Info:"), COMPILED_FRAMEWORK_VERSIONS);

  resetAPcreds();  // Load factory defaults in case LittleFS config is blank and loading config fails.
  loadConfig();    // From LittleFS file.

  set_WM_Params();
  add_WM_Params();
  wm.setSaveConfigCallback(paramsChangedCallback); //   wm.setSaveParamsCallback(paramsChangedCallback);
  wm.setWebServerCallback(configMyWebHandlers);

  // This doesn't determine whether the portal will be closed. What it actually does is determine whether the save config handler will get called atuomatically when wifi creds change.
  // wm.setBreakAfterConfig(false);      // This is supposed to stop the portal from being closed after a new config is saved. Not sure if it works.

  if (!((String)my_hostname).isEmpty()) {wm.setRFC952_hostname(my_hostname);}

  CONSOLELN(F("\nMAIN: b4 wm.autoConnect..."));
  wm.setDebugOutput(true);

  wm.setCredentials(my_wifiSSID, my_wifiPassword, my_wifiSSID1, my_wifiPassword1);  //Set the creds for the access points we want to try to connect to.
  wm.scanModal();   //Perform an initial Scan b4 we try to connect.
  
  // If connection fails it starts an access point **with the creds supplied in this call**.  
  if (wm.autoConnect(my_APSSID,my_APPassword)) {  // These are the creds for starting an AP if connecting to ext wifi fails!
    CONSOLELN(F("MAIN: Connected to external wifi as a client... yeey!"));

    //If connecting to an external wifi as-a-client succeeds, then the portal webserver won't be started by wm.autoconnect...
    CONSOLELN(F("MAIN: Starting portal webserver after connecting to ext wifi."));

    //wm.setupConfigPortal();   // This starts both an AP and portal webserver manually. We don't need the AP because we connected to ext wifi aok.
    wm.startPortalWebserver();  // ...so we have to start it manually. This call starts just the webserver.
  }
  else {
    CONSOLELN(F("MAIN: Failed to connect to wifi as a client so started an AP..."));
    //If we end up here we will have a portal running on the ESP hosted AP...
    // wm.autoConnect will start an AP and the portal webserver if connecting to ext wifi fails.
  }

  CONSOLELN(F("MAIN: After wm.autoConnect...!"));

  // =================================================
  // Wifi or AP is up and config webserver is started.
  // =================================================

   // Setup our chosen modules.
  // Note changes to my_pg_Mode in the config do not initiate a restart.
  // so we need to initialise them regardless of whether receiving is enabled yet.
  #ifdef PWR_GENIE_MODE_SPL
      ssreader::begin();
  #endif

  #ifdef PWR_GENIE_MODE_MODBUS
      initModbus();
  #endif

  #ifdef PWR_GENIE_MODE_JKBMS
    pg_jkbms::setup_bms_serial();
  #endif

  strJsonData="";                 //Full of junk when debug page is called b4 1st send.
  milliCounter = millis() + 1500; // Ready for the 1st loop. Wait a second 1st tho. 
}

void console_InfoPrint(){
  CONSOLELN(F("\n[PG]: Loop Start: "));

  CONSOLELN(F("Connected to SSID:"), WiFi.SSID(), F(" Obtained IP:"), WiFi.localIP());

  CONSOLELN(F("ConfigPortal SSID:"), wm.getConfigPortalSSID(), F(" ConfigPortal IP: "), WiFi.softAPIP())

  CONSOLELN(F("PSU Volts="), String(psuVolts,2), F("| Free heap:"), (String)ESP.getFreeHeap());
  CONSOLELN();
}

//The main code loop
void loop() {

#ifdef SPL_FAKE_READINGS
// Testing SPL Meter code leqv2.h
  static unsigned long tmpmiliC;
  // fake an SPL reading every 1.2 second.
  if ((millis() - tmpmiliC) > 1200) {
    tmpmiliC += 1200;
        ssreader::read_started = true;  
        ssreader::SPL_Complete = true;
  }
#endif

  // For OTA servicing, device reset and other restart needs.
  if (wm.restartMe)
  {
    CONSOLELN(F("Restart has been requested by WM_Manager..."));
    delay(1000);
    ESP.restart();
  }
  
  if (my_pg_Mode == pgMode_Opt::pgMode_Opt_Receive_Source_Only || my_pg_Mode == pgMode_Opt::pgMode_Opt_Both_Source_n_Send)
  {
    #if defined(PWR_GENIE_MODE_SPL)
      // SPL Meter specific background service loop code.
      ssreader::ssreaderLoop();

    #elif defined(PWR_GENIE_MODE_JKBMS)
      //JKBMS has its own poll loop for sending/receiving data. 
      pg_jkbms::bms_pollLoop();
    #endif
  }

  // Main Poll Loop.
  if ((milliCounter + LOOP_INFO_TIME - millis()) < LOOP_INFO_TIME)
  {
    milliCounter += LOOP_INFO_TIME;
    psuVolts = ReadPsuVolts(my_vfact);   // psuVolts is global. Also used by wifi manager on config portal.
   
    //CONSOLE(F("psuVolts Read: "));
    //CONSOLELN(String(psuVolts,2));

    //For debugging:
    //console_InfoPrint();
    //testhostByName();
  if (my_pg_Mode == pgMode_Opt::pgMode_Opt_Receive_Source_Only || my_pg_Mode == pgMode_Opt::pgMode_Opt_Both_Source_n_Send)
  {
    // Modbus as-a-source device specific code:
    #if defined(PWR_GENIE_MODE_MODBUS)
        CONSOLELN(F("MODBUS Source Enabled, Calling doModbusWork():"));
        modbusSuccess = doModbusWork();

    // SPL Meter Specific code:
    #elif defined(PWR_GENIE_MODE_SPL)
      CONSOLELN(F("SPL Post Enabled, Calling DoSPLSend(): "));
      ssreader::DoSPLSend();

    // JKBMS request for data.
    #elif defined(PWR_GENIE_MODE_JKBMS)
      readJKBMS = true;
      pg_jkbms::bms_pollLoop();
    #endif
    
    } else {
      CONSOLELN(F("Source fetching is **DISABLED** in config."));
    }
    
    // For posting ESP psuVolts when other posting is turned off.
    // The psuVolts will get posted automatically when source posting is enabled.
    if (my_pg_Mode == pgMode_Opt::pgMode_Opt_Send_vBat_Only) {
      CONSOLE(F("Emoncms sending ADCv: "));
      emoncms::send2emoncms("");  // Empty Params will just send the psuVolts.
    }
    
    #ifdef BearSSL_DEBUG
      uint16_t stack_usage = stack_thunk_get_max_usage();
      static uint16_t usage_increases = 0;
      CONSOLE("BSSL stack: "); CONSOLE(max_stack_usage); CONSOLE(", "); CONSOLELN(stack_usage);
        
      if ( stack_usage > max_stack_usage ) {
        ++usage_increases;
        max_stack_usage = stack_usage;
      }
    #endif
    
    CONSOLELN(F("Loop Done. "));
  }
}
