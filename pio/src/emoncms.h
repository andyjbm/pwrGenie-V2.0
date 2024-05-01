#pragma once
#ifndef pgEMONCMS_H
  #define pgEMONCMS_H

  #include <Arduino.h>
  #include "Ecms_Struct.h"

  #if defined(ARDUINO_ARCH_ESP8266)
    #include <ESP8266HTTPClient.h>
  #elif defined(ESP32)
    #include <HTTPClient.h>
  #else
    #error Architecture unrecognized by this code.   
  #endif

  #ifdef SECURE_ENABLED  
    #include <WiFiClientSecure.h>
  #endif

  WiFiClient ecms_client;
  #ifdef SECURE_ENABLED
    WiFiClientSecure ecms_secureClient;
  #endif
  HTTPClient ecms_http;

  extern uint8_t pgMode;
  extern float psuVolts;
  
  String ecms_LastResult  = "";          // Used to transfer to debug page call
  String strJsonData      = ""; // new char[1024];  // Global. Keep our place on the heap.

  namespace emoncms {
    bool send2emoncms(const char * const ecmsDataNames[], float * ecmsResults, int arraySize);
    bool send2emoncms(const String &strJsonData);
  } // end namespace

  bool emoncms::send2emoncms(const char * const ecmsDataNames[], float * ecmsResults, int arraySize){
    // Make a name:value pair JSON out of the two arrays names and results. Eg: "n1:v1","n2:v2",...,"n99:v99"
    strJsonData = ""; 
    char bufr[20];
    for (int i=0; i < arraySize; i++){
      if (i!=0) strJsonData += ","; // Comma Separate name:Value pairs
      strcpy_P(bufr, ecmsDataNames[i]);
      strJsonData += "\"" + (String)bufr + ":" + ecmsResults[i] + "\""; // Add name:value pair to the json
    }
    
    // Only post the data if the option is enabled in the config.
    if (pgMode == pgMode_Opt::pgMode_Opt_Both_Source_n_Send)
    {
      return send2emoncms(strJsonData);
    } 
    return true;
  }
  
  bool emoncms::send2emoncms(const String &strJsonData ){
    // The uri looks like "/emoncms/input/post".

#ifdef SECURE_ENABLED      
    bool useHttps = false; if (ecmsParams.Port == 443) useHttps = true;

    if (useHttps){
      #ifndef QUIET_LOOP
        CONSOLE(F("EMONCMS using https: ")); CONSOLELN((String)ecmsParams.server + ":" + String(ecmsParams.Port) + ecmsParams.uri);
        CONSOLE(F("Connecting..."));
      #endif
      ecms_secureClient.setInsecure(); // unfortunately necessary, ESP8266 does not support SSL without hard coding certificates
      if (!ecms_secureClient.connect(ecmsParams.server, 443)){
        #ifndef QUIET_LOOP
          CONSOLE(F("secureClient.connect failed... ")); CONSOLELN(ecmsParams.server);
        #endif
        return false;
      } 
      #ifndef QUIET_LOOP
        CONSOLE(F("secureClient connected... "));
      #endif
      ecms_http.begin(ecms_secureClient, (String)ecmsParams.server, (int)ecmsParams.Port, (String)ecmsParams.uri, true);
      #ifndef QUIET_LOOP
        CONSOLE(F("http.begin connected... "));
      #endif
    } else {
#endif
      // Insecure, no SSL. Need to do the POST way so we can accomodate a different port.
      #ifndef QUIET_LOOP
        CONSOLE(F("EMONCMS using http: ")); CONSOLELN((String) ecmsParams::server() + ":" + String(ecmsParams::Port()) + ecmsParams::uri());
        CONSOLE(F("Connecting..."));
      #endif
      ecms_http.begin(ecms_client, (String)ecmsParams::server(), ecmsParams::Port(), (String)ecmsParams::uri());
      #ifndef QUIET_LOOP
        CONSOLE(F("http.begin connected... "));
      #endif
#ifdef SECURE_ENABLED  
    }
#endif

    ecms_http.addHeader(F("User-Agent"), F("PwrGenieV1"));
    ecms_http.addHeader(F("Connection"), F("close"));
    ecms_http.addHeader(F("Content-Type"), F("application/x-www-form-urlencoded"));
    
    // The POST data msg looks like "node=<channel>&data={"key1:value1","key2:value2","key3:value3"}&apikey=<apikey>" 
    // Add the psuVolts to the data string
    String psuV;
    if (strJsonData.length() != 0) psuV =",";
    psuV += "\"psuVolts:" + String(psuVolts,2) + "\"";
    
    String msg;
    msg =  "node="    + (String)ecmsParams::node();
    msg += "&data="   + (String)"{" + strJsonData + psuV + (String)"}";
    msg += "&apikey=" + (String)ecmsParams::apikey();

    psuV.~String();

    //CONSOLELN(msg);
    #ifndef QUIET_LOOP
      CONSOLE(F("calling http.POST()... "));
    #endif
    int httpCode;
    httpCode = ecms_http.POST(msg);    //Make the call.

    // httpCode will be 200 or 202 on success
    #ifndef QUIET_LOOP
      CONSOLE(String("EMONCMS Result: ") + httpCode + String(", "));
    #endif
    ecms_LastResult = "Last EMONCMS: " + String(httpCode) + ", ";
    if (httpCode == 200 || httpCode == 202){
      #ifndef QUIET_LOOP
        CONSOLELN(ecms_http.getString());
      #endif
      ecms_LastResult += ecms_http.getString();
    } else {
      #ifndef QUIET_LOOP
        CONSOLELN(ecms_http.getString());
        CONSOLELN(ecms_http.errorToString(httpCode));
        CONSOLELN(String("EMONCMS POST data: ") + msg);
      #endif
      ecms_LastResult += ecms_http.getString() + ", " + ecms_http.errorToString(httpCode) + ", JSON: " + msg;
    }
    ecms_http.end();
#ifdef SECURE_ENABLED
    if (useHttps) ecms_secureClient.stop();
#endif
    delay(100);
    return true;
  }

#endif