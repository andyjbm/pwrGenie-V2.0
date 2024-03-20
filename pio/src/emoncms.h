#ifndef pgEMONCMS_H
  #define pgEMONCMS_H

  #include <Arduino.h>
  // #include "defs.h"
  // #include "Utils.h"
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

  extern float psuVolts;
  extern String ecms_LastResult;
  String strJsonData = new char[1024];  // Global. Keep our place on the heap.

  namespace emoncms {
    bool send2emoncms(emoncmsParams ecmsParam, const char * const ecmsDataNames[], float * ecmsResults, int arraySize, bool postEnabled = true);
    bool send2emoncms(emoncmsParams ecmsParam, const String &strJsonData);
  } // end namespace

  bool emoncms::send2emoncms(emoncmsParams ecmsParam, const char * const ecmsDataNames[], float * ecmsResults, int arraySize, bool postEnabled){
    // Make a name:value pair JSON out of the two arrays names and results. Eg: "n1:v1","n2:v2",...,"n99:v99"
    strJsonData = ""; 
    int maxIndex = arraySize; // sizeof(mbResults)/sizeof(*mbResults);
    char bufr[20];
    for (int i=0; i < maxIndex; i++){
      if (i!=0) strJsonData += ",";
      memcpy_P(bufr, ecmsDataNames[i],20);
      strJsonData += "\"" + (String)bufr + ":" + ecmsResults[i] + "\"";
    }
    
    // Only post the data if the option is enabled in the config.
    if (postEnabled)
    {
      return send2emoncms(ecmsParam, strJsonData);
    } 
    return true;
  }
  
  WiFiClient ecms_client;
#ifdef SECURE_ENABLED
    WiFiClientSecure ecms_secureClient;
#endif
  HTTPClient ecms_http;
  bool emoncms::send2emoncms(emoncmsParams ecmsParam, const String &strJsonData ){
    // The uri looks like "/emoncms/input/post".

#ifdef SECURE_ENABLED      
    bool useHttps = false; if (ecmsParam.Port == 443) useHttps = true;

    if (useHttps){
      CONSOLE(F("EMONCMS using https: ")); CONSOLELN((String)ecmsParam.server + ":" + String(ecmsParam.Port) + ecmsParam.uri);
      CONSOLE(F("Connecting..."));
      ecms_secureClient.setInsecure(); // unfortunately necessary, ESP8266 does not support SSL without hard coding certificates
      if (!ecms_secureClient.connect(ecmsParam.server, 443)){
        CONSOLE(F("secureClient.connect failed... ")); CONSOLELN(ecmsParam.server);
        return false;
      } 
      CONSOLE(F("secureClient connected... "));
      ecms_http.begin(ecms_secureClient, (String)ecmsParam.server, (int)ecmsParam.Port, (String)ecmsParam.uri, true);
      CONSOLE(F("http.begin connected... "));

    } else {
#endif
      // Insecure, no SSL. Need to do the POST way so we can accomodate a different port.
      CONSOLE(F("EMONCMS using http: ")); CONSOLELN((String)ecmsParam.server + ":" + String(ecmsParam.Port) + ecmsParam.uri);
      CONSOLE(F("Connecting..."));
      ecms_http.begin(ecms_client, (String)ecmsParam.server, (int)ecmsParam.Port, (String)ecmsParam.uri);
      CONSOLE(F("http.begin connected... "));
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
    msg =  "node="    + (String)ecmsParam.node;
    msg += "&data="   + (String)"{" + strJsonData + psuV + (String)"}";
    msg += "&apikey=" + (String)ecmsParam.apikey;

    psuV.~String();

    //CONSOLELN(msg);
    CONSOLE(F("calling http.POST()... "));
    int httpCode;
    httpCode = ecms_http.POST(msg);    //Make the call.

    // httpCode will be 200 or 202 on success
    CONSOLE(String("EMONCMS Result: ") + httpCode + String(", "));
    ecms_LastResult = "Last EMONCMS: " + String(httpCode) + ", ";
    if (httpCode == 200 || httpCode == 202){
      CONSOLELN(ecms_http.getString());
      ecms_LastResult += ecms_http.getString();
    } else {
      CONSOLELN(ecms_http.getString());
      CONSOLELN(ecms_http.errorToString(httpCode));
      CONSOLELN(String("EMONCMS POST data: ") + msg);
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