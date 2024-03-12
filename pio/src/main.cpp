#include <Arduino.h>
#include <ESP.h>

#include <FS.h>                    //this needs to be first, or it all crashes and burns...
#include <LittleFS.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include "uriwildcard.h"          // Must come b4 wifimanager to override uri.h

#include "defs.h"
#include "Globals.h"

#include "ESPAsync_WiFiManager.h"
#include "Utils.h"                // Needs WifiManager

#ifdef PWR_GENIE_MODE_EM21
  #include "modbus.h"
#endif
//#else
//  #define pgMODBUS_H  // Stop code from being included.
//#endif

#ifdef PWR_GENIE_MODE_SPL
  #include "spl.h"
#endif
//#else
//  #define SPL_H
//  #define LEQV2_H
//#endif

#include "emoncms.h"

AsyncWebServer webServer(80); //Async_webserver_HTTP_PORT);
AsyncDNSServer dnsServer;
ESPAsync_WiFiManager wm(&webServer, &dnsServer, "ESP_WebServer");

/*
AsyncWebServer webServer(); //Async_webserver_HTTP_PORT);

#if ( USING_ESP32_S2 || USING_ESP32_C3 )
  ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, NULL, "AsyncConfigOnStartup");
#else
  AsyncDNSServer dnsServer;
  ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, "AsyncConfigOnStartup");
#endif
*/

#include "wmparams.h"
#include "loadNsaveConfig.h"

//Callback after changes to Wifi page or Config page.
void paramsChangedCallback() {
  CONSOLE(F("\nparamsChanged Callback: "));
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

//Handle http call from IOT device to this device and push the emoncms data over LoRa.
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

void handleDebug(AsyncWebServerRequest *request){
  CONSOLELN(F("Page Request: handleDebug call."));
    
  String page = wm.buildHeader("main_Debug","Debug Info", WM_META_AUTO_DEBUG);
  page += "<fieldset><div class = 'msg'>";
  
  if (ecms_LastResult.isEmpty()){
    page += "<h4>emoncms not yet called.</h4>";
  } else {
    page += "<h4>" + ecms_LastResult + "</h4>";
  }
  #ifdef PWR_GENIE_MODE_SPL
    page += "<h4>SPL data:</h4>"
    page += "<small>" + LEQInfo + "</small>";
  #endif
  #ifdef PWR_GENIE_MODE_EM21
    if (modbusSuccess){
      page += "<h4>Carlo last result Ok:</h4><small>" + modbus_LastResult + "</small>";
    } else {
      page += "<h4>Carlo last result failed:</h4><small>" + modbus_LastResult + "</small>";
    }
  #endif
  page += "</div></fieldset>";
  wm.HTTPSendPage(page, request);
  CONSOLELN(F("HandleDebug page sent."));
}

/*
 * This is a callback made when the wifimanager webserver is created for the purposes of
 * adding our own custom page handlers to the webserver, specific to LoRa wifi data capture.
 */
void configMyWebHandlers(){
  CONSOLELN(F("Call to configMyWebHandlers..."));
  wm.server->on("/emoncms/*", handleEmoncms); //This requires wildcard mod to uri.h in ESP8266Webserver Lib
  wm.server->on("/debug", handleDebug); 
}

// Get everything ready.
void setup() {
  
  Serial.begin(115200);
  while (!Serial){;}

  delay(5000);

  CONSOLELN(F("\n\n\n\nHello there, it's a new day!"));

  resetAPcreds();  // Load factory defaults in case LittleFS config is blank and loading config fails.
  loadConfig();    // From LittleFS file.

//These are debug overrides for now to stop calls to post emoncms
//enable_MB_Post = false;
//enable_DSG_Post = false;
//enable_SPL_Post = false;

  set_WM_Params();
  add_WM_Params();
  wm.setSaveConfigCallback(paramsChangedCallback); //   wm.setSaveParamsCallback(paramsChangedCallback);
  wm.setWebServerCallback(configMyWebHandlers);

// This doesn't determine whether the portal will be closed. What it actually does is determine whether the save config handler will get called atuomatically when wifi creds change.
//  wm.setBreakAfterConfig(false);      // This is supposed to stop the portal from being closed after a new config is saved. Not sure if it works.

  if (!((String)my_hostname).isEmpty()) {wm.setRFC952_hostname(my_hostname);}

  CONSOLELN(F("\nb4 wm.autoConnect..."));
  wm.setDebugOutput(true);

  wm.setCredentials(my_wifiSSID, my_wifiPassword, my_wifiSSID1, my_wifiPassword1);  //Set the creds for the access points we want to try to connect to.
  wm.scanModal();   //Perform an initial Scan b4 we try to connect.
  
  //If connection fails it starts an access point **with the creds supplied in this call**.  
  if (wm.autoConnect(my_APSSID,my_APPassword)) {  // These are the creds for starting an AP if connecting to wifi fails!
    CONSOLELN(F("MAIN: Connected to external wifi as a client... yeey!"));

    //If connecting to an external wifi as-a-client succeeds, then the portal won't be started by wm.autoconnect.
    CONSOLELN(F("MAIN: Starting portal webserver after connecting to ext wifi."));

    //wm.setupConfigPortal();   // This starts both an AP and portal webserver manually.
    wm.startPortalWebserver();  // This starts just the webserver.
  }
  else {
    CONSOLELN(F("MAIN: Failed to connect to wifi as a client so started an AP..."));
    //If we end up here we will have a portal running on the ESP hosted AP...
  }

  CONSOLELN(F("After wm.autoConnect...!"));

    // Setup everything else now the framework is up.
    #ifdef PWR_GENIE_MODE_SPL
      if (enable_SPL_Post){ ssreader::begin(); }
    #endif

    #ifdef PWR_GENIE_MODE_EM21
      if (enable_MB_Post) { initModbus(); }
    #endif

  }


void console_InfoPrint(){
  CONSOLELN();
  CONSOLE(F("Loop Start: "));

  CONSOLE(F("Connected to SSID: ")); CONSOLE(WiFi.SSID());
  CONSOLE(F(" Obtained IP: ")); CONSOLELN(WiFi.localIP());

  CONSOLE(F("ConfigPortal SSID: ")); CONSOLE(wm.getConfigPortalSSID());
  CONSOLE(F(" ConfigPortal IP: ")); CONSOLELN(WiFi.softAPIP());

  CONSOLE(F("PSU Volts=")); CONSOLE(String(psuVolts,2));
  CONSOLE(F(", Free heap: ")); CONSOLELN((String)ESP.getFreeHeap());
  CONSOLELN();
}

//The main code loop
#ifdef BSSL_DEBUG
  #include <StackThunk.h>
  uint16_t max_stack_usage = 0;
#endif

void loop() {
  static unsigned long milliCounter;
  //static unsigned long tmpmiliC;

/*
  // fake an SPL reading every second.
  if ((millis() - tmpmiliC) > 1000) {
    tmpmiliC += 1000;
        ssreader::read_started = true;  
        ssreader::SPL_Complete = true;
  }
*/
  if (wm.restartMe){
    CONSOLELN(F("Restart has been requested by WM_Manager..."));
    delay(1000);
    ESP.restart();
  }

  #ifdef PWR_GENIE_MODE_SPL
    if (enable_SPL_Post){ ssreader::ssreaderLoop(); }
  #endif

  if ((millis() - milliCounter) > LOOP_INFO_TIME * 1000) {
    milliCounter += LOOP_INFO_TIME * 1000;
    psuVolts = ReadPsuVolts(my_vfact);   // psuVolts is global. Also used by wifi manager on web portal.
    CONSOLE(F("psuVolts Read: "));
    CONSOLELN(String(psuVolts,2));
    //console_InfoPrint();
    //testhostByName();

    #ifdef PWR_GENIE_MODE_EM21
      if (enable_MB_Post) {
        CONSOLE(F("MODBUS Post Enabled, Calling doModbusWork(): "));
        modbusSuccess = doModbusWork();
      } else {
        CONSOLELN(F("MODBUS Post DISABLED."));
      }
    #endif

    #ifdef PWR_GENIE_MODE_SPL
    if (enable_SPL_Post){
      //CONSOLELN(F("SPL Post Enabled, Calling DoSPLSend()..."));
      ssreader::DoSPLSend();
    } else {
      CONSOLELN(F("SPL Post DISABLED."));
    }
    #endif

     if (!enable_MB_Post && !enable_DSG_Post && !enable_SPL_Post) { // Check for all options being disabled here.
       CONSOLE(F("Emoncms sending ADCv: "));
       emoncms::send2emoncms(EcmsParams, "");                       //Empty Params will just send the psuVariable.
     }
    
    #ifdef BSSL_DEBUG
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
