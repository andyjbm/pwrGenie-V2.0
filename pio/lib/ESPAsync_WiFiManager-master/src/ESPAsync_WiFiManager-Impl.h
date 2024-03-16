/****************************************************************************************************************************
  ESPAsync_WiFiManager-Impl.h
  For ESP8266 / ESP32 boards

  ESPAsync_WiFiManager is a library for the ESP8266/Arduino platform, using (ESP)AsyncWebServer to enable easy
  configuration and reconfiguration of WiFi credentials using a Captive Portal.

  Modified from
  1. Tzapu               (https://github.com/tzapu/WiFiManager)
  2. Ken Taylor          (https://github.com/kentaylor)
  3. Alan Steremberg     (https://github.com/alanswx/ESPAsyncWiFiManager)
  4. Khoi Hoang          (https://github.com/khoih-prog/ESP_WiFiManager)

  Built by Khoi Hoang https://github.com/khoih-prog/ESPAsync_WiFiManager
  Licensed under MIT license

  Version: 1.15.1

  Version Modified By  Date      Comments
  ------- -----------  ---------- -----------
  1.0.11  K Hoang      21/08/2020 Initial coding to use (ESP)AsyncWebServer instead of (ESP8266)WebServer. Bump up to v1.0.11
                                  to sync with ESP_WiFiManager v1.0.11
  ...
  1.10.0  K Hoang      29/12/2021 Fix `multiple-definitions` linker error and weird bug related to src_cpp
  1.11.0  K Hoang      17/01/2022 Enable compatibility with old code to include only ESP_WiFiManager.h
  1.12.0  K Hoang      10/02/2022 Add support to new ESP32-S3
  1.12.1  K Hoang      11/02/2022 Add LittleFS support to ESP32-C3. Use core LittleFS instead of Lorol's LITTLEFS for v2.0.0+
  1.12.2  K Hoang      13/03/2022 Optimize code by using passing by `reference` instead of by `value`
  1.13.0  K Hoang      18/08/2022 Using AsynsDNSServer instead of DNSServer
  1.14.0  K Hoang      09/09/2022 Fix ESP32 chipID and add ESP_getChipOUI()
  1.14.1  K Hoang      15/09/2022 Remove dependency on ESP_AsyncWebServer, ESPAsyncTCP and AsyncTCP in `library.properties`
  1.15.0  K Hoang      07/10/2022 Optional display Credentials (SSIDs, PWDs) in Config Portal
  1.15.1  K Hoang      25/10/2022 Using random channel for softAP without password. Add astyle using allman style
 *****************************************************************************************************************************/

#pragma once

#ifndef ESPAsync_WiFiManager_Impl_h
#define ESPAsync_WiFiManager_Impl_h


ESPAsync_WMParameter::ESPAsync_WMParameter(const char *custom)
{
  _WMParam_data._id = NULL;
  _WMParam_data._placeholder = NULL;
  _WMParam_data._length = 0;
  _WMParam_data._value = NULL;
  _WMParam_data._labelPlacement = WFM_LABEL_BEFORE;
  
  _WMParam_data._type = WMParam_type::isNormal;
  _WMParam_data._selectionType_Options = nullptr;
  _WMParam_data._selectionType_Options_Count = 0;

  _customHTML = custom;
}


ESPAsync_WMParameter::ESPAsync_WMParameter(const char *id, const __FlashStringHelper *placeholder, const char *defaultValue, const int& length,
                                 const char *custom, const int& labelPlacement,
                                 const WMParam_type type, const char **selectionType_Options, const byte selectionType_Options_Count)
{
  init(id, placeholder, defaultValue, length, custom, labelPlacement, type, selectionType_Options, selectionType_Options_Count);
}

// KH, using struct
ESPAsync_WMParameter::ESPAsync_WMParameter(const WMParam_Data& WMParam_data)
{
  init(WMParam_data._id, WMParam_data._placeholder, WMParam_data._value, WMParam_data._length, "",
       WMParam_data._labelPlacement, WMParam_data._type, _WMParam_data._selectionType_Options, _WMParam_data._selectionType_Options_Count);
}


void ESPAsync_WMParameter::init(const char *id, const __FlashStringHelper *placeholder, const char *defaultValue, const int& length,
                           const char *custom, const int& labelPlacement,
                           const WMParam_type type, const char **selectionType_Options, const byte selectionType_Options_Count)
{
  _WMParam_data._id = id;
  _WMParam_data._placeholder = placeholder;
  _WMParam_data._length = length;
  _WMParam_data._labelPlacement = labelPlacement;
  _WMParam_data._type = type;
  
  _WMParam_data._value = new char[_WMParam_data._length + 1];

  if (_WMParam_data._value != NULL)
  {
      this->setValue(defaultValue, length);
  }

  _customHTML = custom;

  _WMParam_data._selectionType_Options = selectionType_Options;
  _WMParam_data._selectionType_Options_Count = selectionType_Options_Count;
}


void ESPAsync_WMParameter::setValue(const char *defaultValue, int length) {
  if(!_WMParam_data._id){
    // Serial.println("cannot set value of this parameter");
    return;
  }
 
  LOGINFO2(F("WMP setValue: "),_WMParam_data._id, defaultValue);

  _WMParam_data._length = length;
  _WMParam_data._value  = new char[length + 1];
  memset(_WMParam_data._value, 0, length + 1);  // explicit null

  if (defaultValue != NULL) {
    strncpy(_WMParam_data._value, defaultValue, length);
  }
}


ESPAsync_WMParameter::~ESPAsync_WMParameter()
{
  if (_WMParam_data._value != NULL)
  {
    delete[] _WMParam_data._value;
  }
}

// Using Struct to get/set whole data at once
void ESPAsync_WMParameter::setWMParam_Data(const WMParam_Data& WMParam_data)
{
  LOGINFO(F("setWMParam_Data"));

  memcpy(&_WMParam_data, &WMParam_data, sizeof(_WMParam_data));
}


void ESPAsync_WMParameter::getWMParam_Data(WMParam_Data& WMParam_data)
{
  LOGINFO(F("getWMParam_Data"));

  memcpy(&WMParam_data, &_WMParam_data, sizeof(WMParam_data));
}


const char* ESPAsync_WMParameter::getValue()
{
  return _WMParam_data._value;
}


const char* ESPAsync_WMParameter::getID()
{
  return _WMParam_data._id;
}


const __FlashStringHelper* ESPAsync_WMParameter::getPlaceholder()
{
  return _WMParam_data._placeholder;
}


int ESPAsync_WMParameter::getValueLength()
{
  return _WMParam_data._length;
}


int ESPAsync_WMParameter::getLabelPlacement()
{
  return _WMParam_data._labelPlacement;
}

WMParam_type ESPAsync_WMParameter::getType()
{
  return _WMParam_data._type;
}

const char* ESPAsync_WMParameter::getCustomHTML()
{
  return _customHTML;
}


/**
   [getParameters description]
   @access public
*/
ESPAsync_WMParameter** ESPAsync_WiFiManager::getParameters()
{
  return _params;
}


/**
   [getParametersCount description]
   @access public
*/
int ESPAsync_WiFiManager::getParametersCount()
{
  return _paramsCount;
}


char* ESPAsync_WiFiManager::getRFC952_hostname(const char* iHostname)
{
  memset(RFC952_hostname, 0, sizeof(RFC952_hostname));

  size_t len = (RFC952_HOSTNAME_MAXLEN < strlen(iHostname)) ? RFC952_HOSTNAME_MAXLEN : strlen(iHostname);

  size_t j = 0;

  for (size_t i = 0; i < len - 1; i++)
  {
    if (isalnum(iHostname[i]) || iHostname[i] == '-')
    {
      RFC952_hostname[j] = iHostname[i];
      j++;
    }
  }

  // no '-' as last char
  if (isalnum(iHostname[len - 1]) || (iHostname[len - 1] != '-'))
    RFC952_hostname[j] = iHostname[len - 1];

  return RFC952_hostname;
}


ESPAsync_WiFiManager::ESPAsync_WiFiManager(AsyncWebServer * webserver, AsyncDNSServer *dnsserver, const char *iHostname)
{

  server    = webserver;
  dnsServer = dnsserver;

  wifiSSIDs     = NULL;

  // KH
  wifiSSIDscan  = true;
  //wifiSSIDscan  = false;
  //////

  _modeless     = false;
  shouldscan    = true;

#if USE_DYNAMIC_PARAMS
  _max_params = WIFI_MANAGER_MAX_PARAMS;
  _params = (ESPAsync_WMParameter**)malloc(_max_params * sizeof(ESPAsync_WMParameter*));
#endif

  //WiFi not yet started here, must call WiFi.mode(WIFI_STA) and modify function WiFiGenericClass::mode(wifi_mode_t m) !!!
  WiFi.mode(WIFI_STA);

  setRFC952_hostname(iHostname);

  networkIndices = NULL;
}


ESPAsync_WiFiManager::~ESPAsync_WiFiManager()
{
#if USE_DYNAMIC_PARAMS

  if (_params != NULL)
  {
    LOGINFO(F("freeing allocated params!"));

    free(_params);
  }

#endif

  if (networkIndices)
  {
    free(networkIndices); //indices array no longer required so free memory
  }
}


void ESPAsync_WiFiManager::setRFC952_hostname(const char *iHostname)
{
if (iHostname[0] == 0)
  {
#ifdef ESP8266
    String _hostname = "ESP8266-" + String(ESP.getChipId(), HEX);
#else    //ESP32
    String _hostname = "ESP32-" + String(ESP_getChipId(), HEX);
#endif

    _hostname.toUpperCase();

    getRFC952_hostname(_hostname.c_str());

  }
  else
  {
    // Prepare and store the hostname only not NULL
    getRFC952_hostname(iHostname);
  }

  LOGWARN1(F("RFC925 Hostname ="), RFC952_hostname);

  setHostname();
  }

#if USE_DYNAMIC_PARAMS
  bool ESPAsync_WiFiManager::addParameter(ESPAsync_WMParameter *p)
#else
  void ESPAsync_WiFiManager::addParameter(ESPAsync_WMParameter *p)
#endif
{
#if USE_DYNAMIC_PARAMS

  if (_paramsCount == _max_params)
  {
    // rezise the params array
    _max_params += WIFI_MANAGER_MAX_PARAMS;
    LOGINFO1(F("Increasing _max_params to:"), _max_params);
    ESPAsync_WMParameter** new_params = (ESPAsync_WMParameter**)realloc(_params, _max_params * sizeof(ESPAsync_WMParameter*));

    if (new_params != NULL)
    {
      _params = new_params;
    }
    else
    {
      LOGINFO(F("ERROR: failed to realloc params, size not increased!"));
      return false;
    }
  }

  _params[_paramsCount] = p;
  _paramsCount++;

  LOGINFO2(F("Adding parameter"), p->getID(), p->getValue());

  return true;

#else

  // Danger here. Better to use Tzapu way here
  if (_paramsCount < (WIFI_MANAGER_MAX_PARAMS))
  {
    _params[_paramsCount] = p;
    _paramsCount++;

    LOGINFO2(F("Adding parameter"), p->getID(), p->getValue());
  }
  else
  {
    LOGINFO("Can't add parameter. Full");
  }

#endif
}


void ESPAsync_WiFiManager::setupConfigPortal()  // Most of this is prepping the AP to host a websever for the Portal.
{
  stopConfigPortal = false; //Signal not to close config portal

  LOGINFO(F("[wm] Entering setupConfigPortal..."));

  /*This library assumes autoconnect is set to 1. It usually is
    but just in case check the setting and turn on autoconnect if it is off.
    Some useful discussion at https://github.com/esp8266/Arduino/issues/1615*/
  if (WiFi.getAutoConnect() == 0)
    WiFi.setAutoConnect(1);

#if !( USING_ESP32_S2 || USING_ESP32_C3 )
  #ifdef ESP8266
    // KH, mod for Async
    server->reset();
  #else   //ESP32
    server->reset();
  #endif

  if (!dnsServer)
    dnsServer = new AsyncDNSServer;

#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )

  // optional soft ip config
  // Must be put here before dns server start to take care of the non-default ConfigPortal AP IP.
  // Check (https://github.com/khoih-prog/ESP_WiFiManager/issues/58)
  if (_WiFi_AP_IPconfig._ap_static_ip)
  {
    LOGWARN3(F("Custom AP IP/GW/Subnet = "), _WiFi_AP_IPconfig._ap_static_ip, _WiFi_AP_IPconfig._ap_static_gw,
             _WiFi_AP_IPconfig._ap_static_sn);

    WiFi.softAPConfig(_WiFi_AP_IPconfig._ap_static_ip, _WiFi_AP_IPconfig._ap_static_gw, _WiFi_AP_IPconfig._ap_static_sn);
  }

  /* Setup the DNS server redirecting all the domains to the apIP */
  if (dnsServer)
  {
    dnsServer->setErrorReplyCode(AsyncDNSReplyCode::NoError);

    // AsyncDNSServer started with "*" domain name, all DNS requests will be passsed to WiFi.softAPIP()
    if (! dnsServer->start(DNS_PORT, "*", WiFi.softAPIP()))
    {
      // No socket available
      LOGERROR(F("Can't start DNS Server. No available socket"));
    }
  }
  else
  {
    // No space available
    LOGERROR(F("Can't initiate DNS Server. No enough space"));
  }

  _configPortalStart = millis();

  LOGINFO1(F("\nConfiguring AP SSID ="), _apName);

  if (_apPassword != NULL)
  {
    if (strlen(_apPassword) < 8 || strlen(_apPassword) > 63)
    {
      // fail passphrase to short or long!
      LOGERROR(F("Invalid AccessPoint password. Ignoring"));

      _apPassword = NULL;
    }

    LOGINFO1(F("AP PWD ="), _apPassword);
  }

  // KH, To enable dynamic/random channel
  static int channel;

  // Use random channel if  _WiFiAPChannel == 0
  if (_WiFiAPChannel == 0)
    channel = (_configPortalStart % MAX_WIFI_CHANNEL) + 1;
  else
    channel = _WiFiAPChannel;

  LOGINFO1(F("AP Channel ="), channel);

  WiFi.softAP(_apName, _apPassword, channel);

  delay(500); // Without delay I've seen the IP address blank

  LOGINFO1(F("AP IP address ="), WiFi.softAPIP());

  startPortalWebserver();
}

void ESPAsync_WiFiManager::startPortalWebserver(){

#if !( USING_ESP32_S2 || USING_ESP32_C3 )
  server->reset();
#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )

  stopConfigPortal = false;
  
  if ( _webservercallback != NULL) {
    LOGINFO(F("[CB] _webservercallback calling"));
    _webservercallback();
  }

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */

  //server->on("/",         std::bind(&ESPAsync_WiFiManager::handleRoot,        this, std::placeholders::_1)).setFilter(ON_AP_FILTER);
  server->on("/",         std::bind(&ESPAsync_WiFiManager::handleRoot,        this, std::placeholders::_1));
  server->on("/wifi",     std::bind(&ESPAsync_WiFiManager::handleWifi,        this, std::placeholders::_1));
  server->on("/wifisave", std::bind(&ESPAsync_WiFiManager::handleWifiSave,    this, std::placeholders::_1));
  server->on("/close",    std::bind(&ESPAsync_WiFiManager::handleServerClose, this, std::placeholders::_1));
  server->on("/i1",       std::bind(&ESPAsync_WiFiManager::handleInfo1,       this, std::placeholders::_1));
  server->on("/i2",       std::bind(&ESPAsync_WiFiManager::handleInfo2,       this, std::placeholders::_1));
  server->on("/cfg",      std::bind(&ESPAsync_WiFiManager::handleConfig,      this, std::placeholders::_1));
  server->on("/cfgsave",  std::bind(&ESPAsync_WiFiManager::handleConfigSave,  this, std::placeholders::_1));
  server->on("/restart",  std::bind(&ESPAsync_WiFiManager::handleRestart,     this, std::placeholders::_1));
  server->on("/erase",    std::bind(&ESPAsync_WiFiManager::handleErase,       this, std::placeholders::_1));
  //server->on("/state",    std::bind(&ESPAsync_WiFiManager::handleState,     this, std::placeholders::_1));  // JSON wifi state?
  //server->on("/scan",     std::bind(&ESPAsync_WiFiManager::handleScan,        this, std::placeholders::_1));
  
  server->on("/update",   std::bind(&ESPAsync_WiFiManager::handleUpdate,      this, std::placeholders::_1));
  server->on("/done",   std::bind(&ESPAsync_WiFiManager::handleUpdateDone,    this, std::placeholders::_1));
  server->on("/doUpdate",  HTTP_POST,  
                  std::bind(&ESPAsync_WiFiManager::handleUpdateDone,  this, std::placeholders::_1),
                  std::bind(&ESPAsync_WiFiManager::handleUpdating,    this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
  
  server->on("/favicon.ico",   std::bind(&ESPAsync_WiFiManager::handleFavicon,  this, std::placeholders::_1));
  server->onNotFound (std::bind(&ESPAsync_WiFiManager::handleNotFound,          this, std::placeholders::_1));

  server->begin(); // Web server start

  LOGINFO(F("HTTP server started"));
}


bool ESPAsync_WiFiManager::autoConnect()
{
  String ssid = "ESP_" + String(ESP_getChipId());
  return autoConnect(ssid.c_str(), NULL);
}


/* This is not very useful as there has been an assumption that device has to be
  told to connect but Wifi already does it's best to connect in background. Calling this
  method will block until WiFi connects. Sketch can avoid
  blocking call then use (WiFi.status()==WL_CONNECTED) test to see if connected yet.
  See some discussion at https://github.com/tzapu/WiFiManager/issues/68
*/

// To permit autoConnect() to use STA static IP or DHCP IP.
#ifndef AUTOCONNECT_NO_INVALIDATE
  #define AUTOCONNECT_NO_INVALIDATE true
#endif

bool ESPAsync_WiFiManager::autoConnect(char const *apName, char const *apPassword)
{
  LOGINFO(F("\n[WM] AutoConnect Starting..."));
  LOGINFO2(F("Access point SSID/PW: "),apName,apPassword);  
  LOGINFO2(F("User config SSID/PW: "), _ssid, _pass);  
  LOGINFO2(F("User config SSID1/PW1: "), _ssid1, _pass1);  
  LOGINFO2(F("ESP Stored SSID/PW"),WiFi_SSID(),WiFi_Pass());

#if AUTOCONNECT_NO_INVALIDATE
  LOGINFO(F("\n[WM] AutoConnect using user config saved SSID/PW. "));
  // Connect to previously saved SSID/PW, but keep previous settings
  reconnectWifi();
#else
  LOGINFO(F("\nAutoConnect using wifi creds saved on ESP wifi."));
  // Connect to previously saved SSID/PW, but invalidate previous settings
  connectWifi(WiFi_SSID(), WiFi_Pass());
#endif

  if (WiFi.status() == WL_CONNECTED)
  {
    LOGINFO(F("AutoConnect connected and done!"));

    // Start the Portal without the AP? Would happen here if so.
    //startPortalWebserver();   // Can do this here or in the hosting code. There is no option to signal yes/no at the moment.

    return true;
  }

  LOGINFO(F("AutoConnect NOT connected to host wifi, starting an AP and the Config Portal on the way out."));

  // If connecting to wifi fails then we start an AP and then start the portal on that AP.
  startConfigPortal(apName, apPassword);

  return false;
}


String ESPAsync_WiFiManager::networkListAsString()
{
  String pager ;

  //display networks in page
  for (int i = 0; i < wifiSSIDCount; i++)
  {
    if (wifiSSIDs[i].duplicate == true)
      continue; // skip dups

    int quality = getRSSIasQuality(wifiSSIDs[i].RSSI);

    if (_minimumQuality == -1 || _minimumQuality < quality)
    {
      String item = FPSTR(WM_HTTP_ITEM);
      String rssiQ;

      rssiQ += quality;
      item.replace("{v}", wifiSSIDs[i].SSID);
      item.replace("{r}", rssiQ);
      //item.replace("{q}", (String)int(round(map(quality,0,100,1,4)))); //quality icon 1-4 -> Needs adding to <style> if you want to use it.

#if defined(ESP8266)
      if (wifiSSIDs[i].encryptionType != ENC_TYPE_NONE)
#else
      if (wifiSSIDs[i].encryptionType != WIFI_AUTH_OPEN)
#endif
      {
        item.replace("{i}", "l");
      } else {
        item.replace("{i}", "");
      }

      pager += item;
    }
    else
    {
      LOGDEBUG(F("Skipping due to quality"));
    }
  }

  return pager;
}


String ESPAsync_WiFiManager::scanModal()
{
  shouldscan = true;
  scan();

  String pager = networkListAsString();

  return pager;
}


void ESPAsync_WiFiManager::scan()
{
  if (!shouldscan)
    return;

  LOGDEBUG(F("About to scan"));

  if (wifiSSIDscan)
  {
    delay(100);
  }

  if (wifiSSIDscan)
  {
    LOGDEBUG(F("Start scan"));

    wifi_ssid_count_t n = WiFi.scanNetworks(false, true);

    LOGDEBUG(F("Scan done"));

    if (n == WIFI_SCAN_FAILED)
    {
      LOGDEBUG(F("WIFI_SCAN_FAILED!"));
    }
    else if (n == WIFI_SCAN_RUNNING)
    {
      LOGDEBUG(F("WIFI_SCAN_RUNNING!"));
    }
    else if (n < 0)
    {
      LOGDEBUG(F("Failed, unknown error code!"));
    }
    else if (n == 0)
    {
      LOGDEBUG(F("No network found"));
      // page += F("No networks found. Refresh to scan again.");
    }
    else
    {
      if (wifiSSIDscan)
      {
        /* WE SHOULD MOVE THIS IN PLACE ATOMICALLY */
        if (wifiSSIDs)
          delete [] wifiSSIDs;

        wifiSSIDs     = new WiFiResult[n];
        wifiSSIDCount = n;

        if (n > 0)
          shouldscan = false;

        for (wifi_ssid_count_t i = 0; i < n; i++)
        {
          wifiSSIDs[i].duplicate = false;

#if defined(ESP8266)
          WiFi.getNetworkInfo(i, wifiSSIDs[i].SSID, wifiSSIDs[i].encryptionType, wifiSSIDs[i].RSSI, wifiSSIDs[i].BSSID,
                              wifiSSIDs[i].channel, wifiSSIDs[i].isHidden);
#else
          WiFi.getNetworkInfo(i, wifiSSIDs[i].SSID, wifiSSIDs[i].encryptionType, wifiSSIDs[i].RSSI, wifiSSIDs[i].BSSID,
                              wifiSSIDs[i].channel);
#endif
        }

        // RSSI SORT
        // old sort
        for (int i = 0; i < n; i++)
        {
          for (int j = i + 1; j < n; j++)
          {
            if (wifiSSIDs[j].RSSI > wifiSSIDs[i].RSSI)
            {
              std::swap(wifiSSIDs[i], wifiSSIDs[j]);
            }
          }
        }

        // remove duplicates ( must be RSSI sorted )
        if (_removeDuplicateAPs)
        {
          String cssid;

          for (int i = 0; i < n; i++)
          {
            if (wifiSSIDs[i].duplicate == true)
              continue;

            cssid = wifiSSIDs[i].SSID;

            for (int j = i + 1; j < n; j++)
            {
              if (cssid == wifiSSIDs[j].SSID)
              {
                LOGDEBUG("DUP AP: " + wifiSSIDs[j].SSID);
                // set dup aps to NULL
                wifiSSIDs[j].duplicate = true;
              }
            }
          }
        }
      }
    }
  }
}


void ESPAsync_WiFiManager::startConfigPortalModeless(char const *apName, char const *apPassword, bool shouldConnectWiFi)
{
  _modeless     = true;
  _apName       = apName;
  _apPassword   = apPassword;

  WiFi.mode(WIFI_AP_STA);

  LOGDEBUG("SET AP STA");

  // try to connect with ESP chip saved creds.
  if (shouldConnectWiFi && connectWifi("", "") == WL_CONNECTED)
  {
    LOGDEBUG1(F("IP Address:"), WiFi.localIP());

    if ( _savecallback != NULL)
    {
      //todo: check if any custom parameters actually exist, and check if they really changed maybe
      _savecallback();
    }
  }

  if ( _apcallback != NULL)
  {
    _apcallback(this);
  }

  connect = false;
  setupConfigPortal();
  scannow = -1 ;
}

// Anything that accesses WiFi, ESP or EEPROM goes here
void ESPAsync_WiFiManager::criticalLoop()
{
  LOGDEBUG(F("criticalLoop: Enter"));

  if (_modeless)
  {
    if (scannow == -1 || ( millis() > scannow + TIME_BETWEEN_MODELESS_SCANS) )
    {
      LOGDEBUG(F("criticalLoop: modeless scan"));

      scan();
      scannow = millis();
    }

    if (connect)
    {
      connect = false;

      LOGDEBUG(F("criticalLoop: Connecting to new AP"));

      // using user-provided  _ssid, _pass in place of system-stored ssid and pass
      if (reconnectWifi() != WL_CONNECTED)
      {
        LOGDEBUG(F("criticalLoop: Failed to connect."));
      }
      else
      {
        //connected
        // alanswx - should we have a config to decide if we should shut down AP?
        if (_shouldBreakAfterConfig)
        {
          //WiFi.mode(WIFI_STA);
         }

        //notify that configuration has changed and any optional parameters should be saved
        if ( _savecallback != NULL)
        {
          //todo: check if any custom parameters actually exist, and check if they really changed maybe
          _savecallback();
        }

        return;
      }
    }
  }
}


void ESPAsync_WiFiManager::startConfigPortal(char const *apName, char const *apPassword)
{
  WiFi.mode(WIFI_AP_STA);   // Start an Acceess Point to put the Portal Server on.

  _apName = apName;
  _apPassword = apPassword;

  //notify we entered AP mode
  if (_apcallback != NULL)
  {
    LOGINFO("_apcallback");
    _apcallback(this);
  }

  connect = false;
  setupConfigPortal();

  LOGINFO(F("startConfigPortal: About to modal scan"));

  // since we are modal, we can scan every time
  shouldscan = true;

#if defined(ESP8266)
  // we might still be connecting, so that has to stop for scanning
  ETS_UART_INTR_DISABLE ();
  wifi_station_disconnect ();
  ETS_UART_INTR_ENABLE ();
#else
  WiFi.disconnect (false);
#endif
  scan();
}


void ESPAsync_WiFiManager::setWifiStaticIP()
{
#if USE_CONFIGURABLE_DNS

  if (_WiFi_STA_IPconfig._sta_static_ip)
  {
    LOGWARN(F("Custom STA IP/GW/Subnet"));

    //***** Added section for DNS config option *****
    if (_WiFi_STA_IPconfig._sta_static_dns1 && _WiFi_STA_IPconfig._sta_static_dns2)
    {
      LOGWARN(F("DNS1 and DNS2 set"));

      WiFi.config(_WiFi_STA_IPconfig._sta_static_ip, _WiFi_STA_IPconfig._sta_static_gw, _WiFi_STA_IPconfig._sta_static_sn,
                  _WiFi_STA_IPconfig._sta_static_dns1, _WiFi_STA_IPconfig._sta_static_dns2);
    }
    else if (_WiFi_STA_IPconfig._sta_static_dns1)
    {
      LOGWARN(F("Only DNS1 set"));

      WiFi.config(_WiFi_STA_IPconfig._sta_static_ip, _WiFi_STA_IPconfig._sta_static_gw, _WiFi_STA_IPconfig._sta_static_sn,
                  _WiFi_STA_IPconfig._sta_static_dns1);
    }
    else
    {
      LOGWARN(F("No DNS server set"));

      WiFi.config(_WiFi_STA_IPconfig._sta_static_ip, _WiFi_STA_IPconfig._sta_static_gw, _WiFi_STA_IPconfig._sta_static_sn);
    }

    //***** End added section for DNS config option *****

    LOGINFO1(F("setWifiStaticIP IP ="), WiFi.localIP());
  }
  else
  {
    LOGWARN(F("Can't use Custom STA IP/GW/Subnet"));
  }

#else

  // check if we've got static_ip settings, if we do, use those.
  if (_WiFi_STA_IPconfig._sta_static_ip)
  {
    WiFi.config(_WiFi_STA_IPconfig._sta_static_ip, _WiFi_STA_IPconfig._sta_static_gw, _WiFi_STA_IPconfig._sta_static_sn);

    LOGWARN1(F("Custom STA IP/GW/Subnet : "), WiFi.localIP());
  }

#endif
}


int ESPAsync_WiFiManager::reconnectWifi()
{
  LOGINFO(F("reconnectWifi Starting..."));

  int connectResult;

  // Try and connect each with _ssid, _ssid1 and ESP creds in turn.
  if (_ssid != ""){
    if ((connectResult = connectWifi(_ssid, _pass)) == WL_CONNECTED){
      LOGINFO1(F("Connected to"), _ssid);
      return connectResult;
    } else {
      LOGERROR1(F("Failed to connect to"), _ssid);
    }
  } else {
      LOGERROR(F("Can't connect using _ssid: empty string"));
  }

  if (_ssid1 !="") {
    if ((connectResult = connectWifi(_ssid1, _pass1)) == WL_CONNECTED) {
      LOGINFO1(F("Connected to"), _ssid1);
      return connectResult;
    } else {
      LOGERROR1(F("Failed to connect to"), _ssid1);
    }
  } else {
    LOGERROR(F("Can't connect using _ssid1 either: empty string"));
  }
  
  LOGINFO(F("Attempting to connect to previous ESP creds..."));
  if ((connectResult = connectWifi()) == WL_CONNECTED) {
      LOGINFO1(F("Yay! Connected with prev ESP creds."), WiFi.SSID());
    } else {
      LOGERROR1(F("Failed to connect with prev ESP creds: "), WiFi_SSID());
    }

  return connectResult;
}


int ESPAsync_WiFiManager::connectWifi(const String& ssid, const String& pass)
{
  LOGINFO(F("connectWifi Starting..."));
  // Add option if didn't input/update SSID/PW => Use the previous saved Credentials.
  // But update the Static/DHCP options if changed.
  if ( (ssid != "") || ( (ssid == "") && (WiFi_SSID() != "") ) )
  {
    //fix for auto connect racing issue to avoid resetSettings()
    if ((WiFi.status() == WL_CONNECTED) && (ssid == WiFi_SSID()))
    { //Only bail if we are going to reconnect to the same wifi we are already connected to.
      LOGWARN(F("Already connected. Bailing out."));
      return WL_CONNECTED;
    }

    // We (may) have a new wifi to connect to.
    // We (may) have new IP settings to use.
    if (ssid != "")
      // We have specific user creds requested. 
      resetSettings();

#ifdef ESP8266
    setWifiStaticIP();
#endif

    WiFi.mode(WIFI_STA); //Try to connect to wifi 1st b4 starting an AP.
    setHostname();

    // KH, Fix ESP32 staticIP after exiting CP
#ifdef ESP32
    setWifiStaticIP();
#endif

    if (ssid != "")
    {
      // Start Wifi with new values.
      LOGWARN1(F("Connect to new WiFi creds using new IP parameters"),ssid);
      WiFi.begin(ssid.c_str(), pass.c_str());
    }
    else
    {
      // Start Wifi with old values.
      LOGWARN1(F("Connect to previous ESP WiFi creds using new IP parameters"),WiFi_SSID());
      WiFi.begin();
    }
  }
  else if (WiFi_SSID() == "")
  {
    LOGWARN(F("No saved credentials to connect with!"));
  }

  int connRes = waitForConnectResult();

  LOGWARN1("Connection result: ", getStatus(connRes));

  //not connected, WPS enabled, no pass - first attempt
  if (_tryWPS && connRes != WL_CONNECTED && pass == "")
  {
    startWPS();
    //should be connected at the end of WPS
    connRes = waitForConnectResult();
  }

  return connRes;
}


wl_status_t ESPAsync_WiFiManager::waitForConnectResult()
{
  unsigned long startedAt = millis();

  WiFi.waitForConnectResult();
  wl_status_t result = WiFi.status();

  float waited = (millis() - startedAt);

  if (result == WL_CONNECTED){
    LOGWARN1(F("waitForConnectResult: connected after waiting (s) :"), waited / 1000);
    LOGWARN1(F("Local ip ="), WiFi.localIP());
  } else {
    LOGWARN1(F("waitForConnectResult: connection failed after waiting (s) :"), waited / 1000);
  }
  
  return result;
}


void ESPAsync_WiFiManager::startWPS()
{
#ifdef ESP8266
  LOGINFO("START WPS");
  WiFi.beginWPSConfig();
  LOGINFO("END WPS");
#else   //ESP32
  // TODO
  LOGINFO("ESP32 WPS TODO");
#endif
}

//Convenient for debugging but wasteful of program space.
//Remove if short of space
const char* ESPAsync_WiFiManager::getStatus(const int& status)
{
  switch (status)
  {
    case WL_IDLE_STATUS:
      return "WL_IDLE_STATUS";

    case WL_NO_SSID_AVAIL:
      return "WL_NO_SSID_AVAIL";

    case WL_CONNECTED:
      return "WL_CONNECTED";

    case WL_CONNECT_FAILED:
      return "WL_CONNECT_FAILED";

#if ( ESP8266 && (USING_ESP8266_CORE_VERSION >= 30000) )

    case WL_WRONG_PASSWORD:
      return "WL_WRONG_PASSWORD";
#endif

    case WL_DISCONNECTED:
      return "WL_DISCONNECTED";

    default:
      return "UNKNOWN";
  }
}


String ESPAsync_WiFiManager::getConfigPortalSSID()
{
  return _apName;
}


String ESPAsync_WiFiManager::getConfigPortalPW()
{
  return _apPassword;
}


void ESPAsync_WiFiManager::resetSettings()
{
  LOGINFO(F("Previous ESP Chip settings invalidated."));

#ifdef ESP8266
  WiFi.disconnect(true);
#else
  WiFi.disconnect(true, true);

  // Temporary fix for issue of not clearing WiFi SSID/PW from flash of ESP32
  // See https://github.com/khoih-prog/ESPAsync_WiFiManager/issues/25 and https://github.com/espressif/arduino-esp32/issues/400
  WiFi.begin("0", "0");
#endif

  delay(200);

  return;
}


void ESPAsync_WiFiManager::setTimeout(const unsigned long& seconds)
{
  setConfigPortalTimeout(seconds);
}


void ESPAsync_WiFiManager::setConfigPortalTimeout(const unsigned long& seconds)
{
  _configPortalTimeout = seconds * 1000;
}


void ESPAsync_WiFiManager::setConnectTimeout(const unsigned long& seconds)
{
  _connectTimeout = seconds * 1000;
}

void ESPAsync_WiFiManager::setDebugOutput(bool debug)
{
  _debug = debug;
}

// KH, To enable dynamic/random channel
int ESPAsync_WiFiManager::setConfigPortalChannel(const int& channel)
{
  // If channel < MIN_WIFI_CHANNEL - 1 or channel > MAX_WIFI_CHANNEL => channel = 1
  // If channel == 0 => will use random channel from MIN_WIFI_CHANNEL to MAX_WIFI_CHANNEL
  // If (MIN_WIFI_CHANNEL <= channel <= MAX_WIFI_CHANNEL) => use it
  if ( (channel < MIN_WIFI_CHANNEL - 1) || (channel > MAX_WIFI_CHANNEL) )
    _WiFiAPChannel = 1;
  else if ( (channel >= MIN_WIFI_CHANNEL - 1) && (channel <= MAX_WIFI_CHANNEL) )
    _WiFiAPChannel = channel;

  return _WiFiAPChannel;
}


void ESPAsync_WiFiManager::setAPStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn)
{
  LOGINFO(F("setAPStaticIPConfig"));

  _WiFi_AP_IPconfig._ap_static_ip = ip;
  _WiFi_AP_IPconfig._ap_static_gw = gw;
  _WiFi_AP_IPconfig._ap_static_sn = sn;
}


void ESPAsync_WiFiManager::setAPStaticIPConfig(const WiFi_AP_IPConfig& WM_AP_IPconfig)
{
  LOGINFO(F("setAPStaticIPConfig"));

  memcpy((void *) &_WiFi_AP_IPconfig, &WM_AP_IPconfig, sizeof(_WiFi_AP_IPconfig));
}


void ESPAsync_WiFiManager::getAPStaticIPConfig(WiFi_AP_IPConfig  &WM_AP_IPconfig)
{
  LOGINFO(F("getAPStaticIPConfig"));

  memcpy((void *) &WM_AP_IPconfig, &_WiFi_AP_IPconfig, sizeof(WM_AP_IPconfig));
}


void ESPAsync_WiFiManager::setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn)
{
  LOGINFO(F("setSTAStaticIPConfig"));

  _WiFi_STA_IPconfig._sta_static_ip = ip;
  _WiFi_STA_IPconfig._sta_static_gw = gw;
  _WiFi_STA_IPconfig._sta_static_sn = sn;
}


void ESPAsync_WiFiManager::setSTAStaticIPConfig(const WiFi_STA_IPConfig& WM_STA_IPconfig)
{
  LOGINFO(F("setSTAStaticIPConfig"));

  memcpy((void *) &_WiFi_STA_IPconfig, &WM_STA_IPconfig, sizeof(_WiFi_STA_IPconfig));
}


void ESPAsync_WiFiManager::getSTAStaticIPConfig(WiFi_STA_IPConfig& WM_STA_IPconfig)
{
  LOGINFO(F("getSTAStaticIPConfig"));

  memcpy((void *) &WM_STA_IPconfig, &_WiFi_STA_IPconfig, sizeof(WM_STA_IPconfig));
}


#if USE_CONFIGURABLE_DNS
void ESPAsync_WiFiManager::setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn,
                                                const IPAddress& dns_address_1, const IPAddress& dns_address_2)
{
  LOGINFO(F("setSTAStaticIPConfig for USE_CONFIGURABLE_DNS"));

  _WiFi_STA_IPconfig._sta_static_ip = ip;
  _WiFi_STA_IPconfig._sta_static_gw = gw;
  _WiFi_STA_IPconfig._sta_static_sn = sn;
  _WiFi_STA_IPconfig._sta_static_dns1 = dns_address_1; //***** Added argument *****
  _WiFi_STA_IPconfig._sta_static_dns2 = dns_address_2; //***** Added argument *****
}
#endif

void ESPAsync_WiFiManager::setMinimumSignalQuality(const int& quality)
{
  _minimumQuality = quality;
}


void ESPAsync_WiFiManager::setBreakAfterConfig(bool shouldBreak)
{
  _shouldBreakAfterConfig = shouldBreak;
}


void ESPAsync_WiFiManager::reportStatus(String& page)
{
  // Report Wifi connection Info.
  page += FPSTR(WM_FLDSET_START);
  page += F("<div class=\"msg\">");
  page += F("<h3>WiFi Information</h3>");
  //page += FPSTR(WM_HTTP_SCRIPT_NTP_MSG);

  if (WiFi_SSID() != "")
  {
    page += F("Configured to connect to AP <b>");
    page += WiFi_SSID();
    page += F("</b><br/>");
    if (WiFi.status() == WL_CONNECTED)
    {
      page += F("Connected on IP <a href=\"http://");
      page += WiFi.localIP().toString();
      page += F("/\">");
      page += WiFi.localIP().toString();
      page += F("</a>");
    }
    else
    {
      page += F("Not connected.");
    }
  }
  else
  {
    page += F("No network configured.");
  }
  page += F("</div>");
  page += FPSTR(WM_FLDSET_END);
}

// / root handler and captive portal redirect.
void ESPAsync_WiFiManager::handleRoot(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleRoot"));

  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;

  if (captivePortal(request))
  {
    // If captive portal redirect instead of displaying the error page.
    return;
  }

  String page = buildHeader("", FPSTR(S_brand));

  reportStatus(page);
  
  page += FPSTR(WM_FLDSET_START);
  page += FPSTR(WM_HTTP_PORTAL_OPTIONS); // Main Page button Panel.
  page += FPSTR(WM_FLDSET_END);

  HTTPSendPage(page, request);

  LOGDEBUG(F("Root Page Sent.")); //, page);
}

// /wifi handler
void ESPAsync_WiFiManager::handleWifi(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleWifi"));

  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;

  String page = buildHeader(FPSTR(S_titlewifi), FPSTR(HTTP_FORM_WIFI_HEAD));

  page += FPSTR(WM_FLDSET_START);
  if (wifiSSIDCount == 0)
  {
    LOGDEBUG(F("handleWifi: No network found"));
    page += F("<div>No network found. Refresh to scan again.</div>");
  }
  else
  {
    page += networkListAsString();      //display networks in page
  }
    page += FPSTR(WM_FLDSET_END);

  //page += "<small>*Hint: To reuse the saved WiFi credentials, leave SSID and PWD fields empty</small>";

  page += FPSTR(WM_HTTP_WIFIFORM_START);

#if DISPLAY_STORED_CREDENTIALS_IN_CP
  // Populate SSIDs and PWDs if valid
  page.replace("[[ssid]]",  _ssid );
  page.replace("[[pwd]]",   _pass );
  page.replace("[[ssid1]]", _ssid1 );
  page.replace("[[pwd1]]",  _pass1 );
#endif


  LOGDEBUG1(F("Static IP ="), _WiFi_STA_IPconfig._sta_static_ip.toString());

  // KH, Comment out to permit changing from DHCP to static IP, or vice versa
  // and add staticIP label in CP

  // To permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
  // You'll loose the feature of dynamically changing from DHCP to static IP, or vice versa
  // You have to explicitly specify false to disable the feature.

#if !USE_STATIC_IP_CONFIG_IN_CP

  if (_WiFi_STA_IPconfig._sta_static_ip)
#endif
  {
    page += FPSTR(WM_FLDSET_START);

    String item = FPSTR(WM_HTTP_WIFIFORM_LABEL_BEFORE);

    item.replace("{i}", "ip");
    item.replace("{n}", "ip");
    item.replace("{p}", "Static IP");
    item.replace("{l}", "15");
    item.replace("{v}", _WiFi_STA_IPconfig._sta_static_ip.toString());

    page += item;

    item = FPSTR(WM_HTTP_WIFIFORM_LABEL_BEFORE);
    
    item.replace("{i}", "gw");
    item.replace("{n}", "gw");
    item.replace("{p}", "Gateway IP");
    item.replace("{l}", "15");
    item.replace("{v}", _WiFi_STA_IPconfig._sta_static_gw.toString());

    page += item;
    
    item = FPSTR(WM_HTTP_WIFIFORM_LABEL_BEFORE);
    
    item.replace("{i}", "sn");
    item.replace("{n}", "sn");
    item.replace("{p}", "Subnet");
    item.replace("{l}", "15");
    item.replace("{v}", _WiFi_STA_IPconfig._sta_static_sn.toString());

#if USE_CONFIGURABLE_DNS
    //***** Added for DNS address options *****
    page += item;
    
    item = FPSTR(WM_HTTP_WIFIFORM_LABEL_BEFORE);
    
    item.replace("{i}", "dns1");
    item.replace("{n}", "dns1");
    item.replace("{p}", "DNS1 IP");
    item.replace("{l}", "15");
    item.replace("{v}", _WiFi_STA_IPconfig._sta_static_dns1.toString());

    page += item;
    
    item = FPSTR(WM_HTTP_WIFIFORM_LABEL_BEFORE);
    
    item.replace("{i}", "dns2");
    item.replace("{n}", "dns2");
    item.replace("{p}", "DNS2 IP");
    item.replace("{l}", "15");
    item.replace("{v}", _WiFi_STA_IPconfig._sta_static_dns2.toString());
    //***** End added for DNS address options *****
#endif

    page += item;
    
    page += FPSTR(WM_FLDSET_END);
    //page += "<br/>";
  }

  page += FPSTR(WM_HTTP_SCRIPT_NTP_HIDDEN);
  page += FPSTR(WM_HTTP_FORM_END);

  HTTPSendPage(page, request);

  LOGDEBUG(F("Sent config page"));
}

// /wifisave handler
void ESPAsync_WiFiManager::handleWifiSave(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleWifiSave"));

  //SAVE/connect here
  _ssid = request->arg("s").c_str();
  _pass = request->arg("p").c_str();

  _ssid1 = request->arg("s1").c_str();
  _pass1 = request->arg("p1").c_str();

  LOGDEBUG1(F("ssid returned: "), _ssid);
  LOGDEBUG1(F("password returned: "), _pass);
  LOGDEBUG1(F("ssid1 returned: "), _ssid1);
  LOGDEBUG1(F("password1 returned: "), _pass1);

#if USE_ESP_WIFIMANAGER_NTP

  if (request->hasArg("timezone"))
  {
    _timezoneName = request->arg("timezone");   //.c_str();

    LOGDEBUG1(F("TZ ="), _timezoneName);
  }
  else
  {
    LOGDEBUG(F("No TZ arg"));
  }

#endif

  if (request->hasArg("ip"))
  {
    String ip = request->arg("ip");

    optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_ip, ip.c_str());

    LOGDEBUG1(F("New Static IP ="), _WiFi_STA_IPconfig._sta_static_ip.toString());
  }

  if (request->hasArg("gw"))
  {
    String gw = request->arg("gw");

    optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_gw, gw.c_str());

    LOGDEBUG1(F("New Static Gateway ="), _WiFi_STA_IPconfig._sta_static_gw.toString());
  }

  if (request->hasArg("sn"))
  {
    String sn = request->arg("sn");

    optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_sn, sn.c_str());

    LOGDEBUG1(F("New Static Netmask ="), _WiFi_STA_IPconfig._sta_static_sn.toString());
  }

#if USE_CONFIGURABLE_DNS

  //*****  Added for DNS Options *****
  if (request->hasArg("dns1"))
  {
    String dns1 = request->arg("dns1");

    optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_dns1, dns1.c_str());

    LOGDEBUG1(F("New Static DNS1 ="), _WiFi_STA_IPconfig._sta_static_dns1.toString());
  }

  if (request->hasArg("dns2"))
  {
    String dns2 = request->arg("dns2");

    optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_dns2, dns2.c_str());

    LOGDEBUG1(F("New Static DNS2 ="), _WiFi_STA_IPconfig._sta_static_dns2.toString());
  }

  //*****  End added for DNS Options *****
#endif

  String page = FPSTR(WM_HTTP_HEAD_START);
  page.replace("{v}", "Credentials Saved");

  page += FPSTR(WM_HTTP_SCRIPT);
  page += FPSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  
  page += FPSTR(WM_META_AUTO_ROOT);  // This to reload the page after wifi has changed etc.

  page += FPSTR(WM_HTTP_HEAD_END);

  page += FPSTR(WM_HTTP_WIFISAVED);
  page.replace("{v}", _apName);
  page.replace("{x}", _ssid);
  page.replace("{x1}", _ssid1);

  HTTPSendPage(page, request);

  LOGDEBUG(F("Sent wifi save page"));

  //notify that configuration has changed and any optional parameters should be saved
  if ( _savecallback != NULL)
  {
    _savecallback();
  }

  delay(2000);    // Allow the ESP to send the page to the browser before we go out for the count!

  autoConnect(_apName, _apPassword);  // Reconnect. This call will start the portal if the new creds don't connect.

}

// /close handler
void ESPAsync_WiFiManager::handleServerClose(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleServerClose"));

  String page = FPSTR(WM_HTTP_HEAD_START);
  page.replace("{v}", "Close Server");

  page += FPSTR(WM_HTTP_SCRIPT);
  page += FPSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  page += FPSTR(WM_HTTP_HEAD_END);
  page += F("<div class=\"msg\">");
  page += F("My network is <b>");
  page += WiFi_SSID();
  page += F("</b><br>");
  page += F("IP address is <b>");
  page += WiFi.localIP().toString();
  page += F("</b><br><br>");
  page += F("Portal closed...<br><br>");

  //page += F("Push button on device to restart configuration server!");
  HTTPSendPage(page, request);

  stopConfigPortal = true; //signal ready to shutdown config portal

  LOGDEBUG(F("Sent server close page"));
}

// /i1 handler
void ESPAsync_WiFiManager::handleInfo1(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleInfo1"));

  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;

  String page = buildHeader(FPSTR(S_titleinfo1),FPSTR(HTTP_FORM_INFO_HEAD));
  
  //page += F("<dl>");
  reportStatus(page);
  
  //if (connect)
  //{
  //  page += F("<dt>Trying to connect</dt><dd>");
  //  page += wifiStatus;
  //  page += F("</dd>");
  //}
  page += FPSTR(WM_FLDSET_START);
  page += F("<h3>Device Data</h3>");
  page += F("<table class=\"table\">");
  page += F("<thead><tr><th>Name</th><th>Value</th></tr></thead><tbody><tr><td>Chip ID</td><td>");
  page += String(ESP_getChipId(), HEX);
  page += F("</td></tr>");

#ifdef   ESP32
  page += F("<tr><td>Chip OUI</td><td>");
  page += F("0x");
  page += String(getChipOUI(), HEX);
  page += F("</td></tr>");

  page += F("<tr><td>Chip Model</td><td>");
  page += ESP.getChipModel();
  page += F(" Rev");
  page += ESP.getChipRevision();
  page += F("</td></tr>");
#endif

  page += F("<tr><td>Flash Chip ID</td><td>");

#ifdef ESP8266
  page += String(ESP.getFlashChipId(), HEX);
#else   //ESP32
  // TODO
  page += F("TODO");
#endif

  page += F("</td></tr>");

  page += F("<tr><td>IDE Flash Size</td><td>");
  page += ESP.getFlashChipSize();
  page += F(" bytes</td></tr>");

  page += F("<tr><td>Real Flash Size</td><td>");

#ifdef ESP8266
  page += ESP.getFlashChipRealSize();
#else   //ESP32
  // TODO
  page += F("TODO");
#endif

  page += F(" bytes</td></tr>");

  page += F("<tr><td>Access Point IP</td><td>");
  page += WiFi.softAPIP().toString();
  page += F("</td></tr>");

  page += F("<tr><td>Access Point MAC</td><td>");
  page += WiFi.softAPmacAddress();
  page += F("</td></tr>");

  page += F("<tr><td>SSID</td><td>");
  page += WiFi_SSID();
  page += F("</td></tr>");

  page += F("<tr><td>Station IP</td><td>");
  page += WiFi.localIP().toString();
  page += F("</td></tr>");

  page += F("<tr><td>Station MAC</td><td>");
  page += WiFi.macAddress();
  page += F("</td></tr>");
  page += F("</tbody></table>");

  page += FPSTR(WM_FLDSET_END);

#ifdef USE_AVAILABLE_PAGES
  page += FPSTR(WM_FLDSET_START);
  page += FPSTR(WM_HTTP_AVAILABLE_PAGES);
  page += FPSTR(WM_FLDSET_END);
#endif

  HTTPSendPage(page, request);
  LOGDEBUG(F("Info1 page sent."));//, page);
  //delay(3000);
}

// /i2 handler
void ESPAsync_WiFiManager::handleInfo2(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleInfo2"));
  
  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;

  String page = buildHeader(FPSTR(S_titleinfo2),FPSTR(HTTP_FORM_INFO_HEAD));

  reportStatus(page);

  uint16_t infos = 0;

  //@todo convert to enum or refactor to strings
  //@todo wrap in build flag to remove all info code for memory saving
  #ifdef ESP8266
    infos = 28;
    String infoids[] = {
      F("esphead"),
      F("uptime"),
      F("chipid"),
      F("fchipid"),
      F("idesize"),
      F("flashsize"),
      F("corever"),
      F("bootver"),
      F("cpufreq"),
      F("freeheap"),
      F("memsketch"),
      F("memsmeter"),
      F("lastreset"),
      F("wifihead"),
      F("conx"),
      F("stassid"),
      F("staip"),
      F("stagw"),
      F("stasub"),
      F("dnss"),
      F("host"),
      F("stamac"),
      F("autoconx"),
      F("wifiaphead"),
      F("apssid"),
      F("apip"),
      F("apbssid"),
      F("apmac")
    };

  #elif defined(ESP32)
    // add esp_chip_info ?
    infos = 27;
    String infoids[] = {
      F("esphead"),
      F("uptime"),
      F("chipid"),
      F("chiprev"),
      F("idesize"),
      F("flashsize"),      
      F("cpufreq"),
      F("freeheap"),
      F("memsketch"),
      F("memsmeter"),      
      F("lastreset"),
      F("temp"),
      // F("hall"),
      F("wifihead"),
      F("conx"),
      F("stassid"),
      F("staip"),
      F("stagw"),
      F("stasub"),
      F("dnss"),
      F("host"),
      F("stamac"),
      F("apssid"),
      F("wifiaphead"),
      F("apip"),
      F("apmac"),
      F("aphost"),
      F("apbssid")
    };
  #endif

  page += FPSTR(WM_FLDSET_START);
  page += getInfoData(infoids[0]); // Header.
  page += F("<table class=\"table\">");
  page += F("<thead><tr><th>Name</th><th>Value</th></tr></thead><tbody>");
  
  for(size_t i=1; i<infos;i++){
    if(infoids[i] != NULL) 
    {
      String data = getInfoData(infoids[i]);

      //LOGDEBUG1(infoids[i],data);
      // This is/was a hack to change the HTML Tags...
      //data.replace(FPSTR("<dt>"), FPSTR("<td>"));
      //data.replace(FPSTR("</dt>"), FPSTR("</td>"));
      //data.replace(FPSTR("<dd>"), FPSTR("<td>"));
      //data.replace(FPSTR("</dd>"), FPSTR("</td>"));
      //data.replace(FPSTR("<br/>"),FPSTR("<td>"));  // For progress bar.

      page += F("<tr>");
      page += data;
      page += F("</tr>");
    }
  }
  
  page += F("</tbody></table>");
  page += FPSTR(WM_FLDSET_END);

  page += FPSTR(WM_FLDSET_START);
  page += F("<h3>About</h3><hr><table class=\"table\"><tbody>");

  page += getInfoData("aboutver");
  page += F("</tr><tr>");
  page += getInfoData("aboutarduinover");
  page += F("</tr><tr>");
  page += getInfoData("aboutidfver");
  page += F("</tr><tr>");
  page += getInfoData("aboutdate");
  page += F("</tr></tbody></table>");
  page += FPSTR(WM_FLDSET_END);

  //page += WM_HTTP_PORTAL_OPTIONS[8];
  //page += WM_HTTP_PORTAL_OPTIONS[9];

  HTTPSendPage(page, request);
  
  LOGDEBUG(F("Sent info2 page: ")); //, page);
}

// /cfg handler
void ESPAsync_WiFiManager::handleConfig(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleConfig"));

  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;

  String page = buildHeader(FPSTR(S_titleparam),FPSTR(HTTP_FORM_PARAM_HEAD));
  String submit = FPSTR(HTTP_FORM_POST);
  submit.replace("{v}","cfgsave");
  page += submit;

  reportStatus(page);

  page += FPSTR(WM_FLDSET_START);

  // add the extra parameters to the form
  for (int i = 0; i < _paramsCount; i++)
  {
    if (_params[i] == NULL)
    {
      break;
    }

// Andy added checkbox/boolean & select/option parameter.
     String tmp_FORM_PARAM;
     switch (_params[i]->getType()){
      case WMParam_type::isCheckbox:
        tmp_FORM_PARAM = FPSTR(WM_HTTP_CONFIGFORM_BOOL_PARAM);
        break;
      case WMParam_type::isSelection:
        tmp_FORM_PARAM = FPSTR(WM_HTTP_CONFIGFORM_SELECTION_PARAM);
        break;
      default: // Hopefully WMParam_type::isNormal:
        tmp_FORM_PARAM = FPSTR(WM_HTTP_CONFIGFORM_PARAM);
     }

     String pitem;
      switch (_params[i]->getLabelPlacement()) {
        case WFM_LABEL_BEFORE:
          pitem = FPSTR(WM_HTTP_CONFIGFORM_LABEL);
          pitem += tmp_FORM_PARAM;
          break;
        case WFM_LABEL_AFTER:
          pitem = tmp_FORM_PARAM;
          pitem += FPSTR(WM_HTTP_CONFIGFORM_LABEL);
          break;
        default:
          // WFM_NO_LABEL
          pitem = tmp_FORM_PARAM;
          break;
      }

    if (_params[i]->getID() != NULL)
    {
      pitem.replace("{i}", _params[i]->getID());          // T_i id name
      pitem.replace("{n}", _params[i]->getID());          // T_n id name alias
      pitem.replace("{p}", _params[i]->getPlaceholder()); // T_p replace legacy placeholder token

      char parLength[2];
      snprintf(parLength, 2, "%d", _params[i]->getValueLength());
      pitem.replace("{l}", parLength);                    // T_l value length
      
      //pitem.replace("{v}", _params[i]->getValue());

      // Added boolean/checkbox and selection/options datatypes.
      // Switch case approach paves the way for more datatypes in the future. 
      switch (_params[i]->getType())
      {
        case WMParam_type::isCheckbox:
        {
            // Boolean Parameter: Use checkbox. And render it correctly (ticked or unticked)
          int tmpbool = String(_params[i]->getValue()).toInt(); // This didn't convert to int easily.
          if (tmpbool == 1){
            pitem.replace("{c}", "checked");
          } else {
            pitem.replace("{c}", "");
          }
          break;
        }

        case WMParam_type::isSelection:
        {
          String tmpOptions ="";          
          // Selection. Have we got any options?
          if ((_params[i]->_WMParam_data._selectionType_Options == nullptr) || \
              (_params[i]->_WMParam_data._selectionType_Options_Count == 0))
          {
            break;
          }
          
          // Selection. Need to enumerate through the options.
          for (int k = 0; k < _params[i]->_WMParam_data._selectionType_Options_Count; k++)
          {
            // Check this option exists...
            if (_params[i]->_WMParam_data._selectionType_Options[k] == NULL)
            {
              break;
            }
            
            String thisOpt = FPSTR(WM_HTTP_CONFIGFORM_SEL_OPTIONS); // Make a new option.
            thisOpt.replace("{o}", String(k));                                 // The enum Value

            if (k == String(_params[i]->getValue()).toInt())
            {
              thisOpt.replace("{s}", " Selected");  // This one is selected
            } else {
              thisOpt.replace("{s}","");            // This one is not selected
            }
            thisOpt.replace("{d}", String(_params[i]->_WMParam_data._selectionType_Options[k]));  // The data (text) of the option.
            tmpOptions += thisOpt;
          } // End of building the options. Now add them to the selection.
          
          pitem.replace("{o}", tmpOptions);
          break;
        }

        // Original String type of config box.
        default: // Should be whats left is WMParam_type::isNormal
        {
          // Text input box parameters.
          pitem.replace("{v}", _params[i]->getValue()); // T_v value
          break;
        }  
      }

      pitem.replace("{c}", _params[i]->getCustomHTML());  // T_c meant for additional attributes, not html, but can stuff
    }
    else
    {
      pitem = _params[i]->getCustomHTML();
    }
    page +="<div>";
    page += pitem;
    page +="</div>";
  }

  if (_paramsCount > 0)
  {
    page += FPSTR(WM_FLDSET_END);
  }

  if (_params[0] != NULL)
  {
    page += "<br/>";
  }

  page += FPSTR(WM_FLDSET_END);
  page += FPSTR(WM_HTTP_FORM_END);

  HTTPSendPage(page, request);

  LOGDEBUG(F("Config page sent."));//, page);
}

// /CfgSave handler
void ESPAsync_WiFiManager::handleConfigSave(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleConfigSave"));

  // Save the returned page arg values into the WN parameters.
  // We go through the WM parameters looking for IDs that match page args.
  for (int i = 0; i < _paramsCount; i++)
  {
    if (_params[i] == NULL) {break;}

    String value = request->arg(_params[i]->getID()).c_str();                                   //read page arg
    value.toCharArray(_params[i]->_WMParam_data._value, _params[i]->_WMParam_data._length);     //store it in array

    LOGDEBUG2(F("Parameter and value :"), _params[i]->getID(), value);
  }

  String page = FPSTR(WM_HTTP_HEAD_START);
  page.replace("{v}", "Config Saved");
  
  page += FPSTR(WM_HTTP_SCRIPT);
  page += FPSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  
  page += FPSTR(WM_META_AUTO_ROOT); // This to reload the page after wifi has changed etc.

  page += FPSTR(WM_HTTP_HEAD_END);
  page += FPSTR(WM_HTTP_CONFIGSAVED);
  //page += FPSTR(WM_HTTP_END);

  HTTPSendPage(page, request);

  //notify that configuration has changed and any optional parameters should be saved
  if ( _savecallback != NULL)
  {
    //todo: check if any custom parameters actually exist, and check if they really changed maybe
    _savecallback();
  }

  LOGDEBUG(F("Sent config save page."));
}

/*
// Handle the state page
void ESPAsync_WiFiManager::handleState(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handle State-Json"));

  String page = F("{\"Soft_AP_IP\":\"");

  page += WiFi.softAPIP().toString();
  page += F("\",\"Soft_AP_MAC\":\"");
  page += WiFi.softAPmacAddress();
  page += F("\",\"Station_IP\":\"");
  page += WiFi.localIP().toString();
  page += F("\",\"Station_MAC\":\"");
  page += WiFi.macAddress();
  page += F("\",");

  if (WiFi.psk() != "")
  {
    page += F("\"Password\":true,");
  }
  else
  {
    page += F("\"Password\":false,");
  }

  page += F("\"SSID\":\"");
  page += WiFi_SSID();
  page += F("\"}");

#if ( USING_ESP32_S2 || USING_ESP32_C3 )
  request->send(200, WM_HTTP_HEAD_CT, page);

  // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
  delay(1);
#else

  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", page);
  response->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  response->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
#endif

  response->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
  response->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");

  request->send(response);
#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )

  LOGDEBUG(F("Sent state page in json format"));
}
*/

/*
// Handle the scan page
void ESPAsync_WiFiManager::handleScan(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("entering handleScan"));

  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;   //KH

  LOGDEBUG(F("Scan-Json"));

  String page = F("{\"Access_Points\":[");

  // KH, display networks in page using previously scan results
  for (int i = 0; i < wifiSSIDCount; i++)
  {
    if (wifiSSIDs[i].duplicate == true)
      continue; // skip dups

    if (i != 0)
      page += F(", ");

    LOGDEBUG1(F("Index ="), i);
    LOGDEBUG1(F("SSID ="), wifiSSIDs[i].SSID);
    LOGDEBUG1(F("RSSI ="), wifiSSIDs[i].RSSI);

    int quality = getRSSIasQuality(wifiSSIDs[i].RSSI);

    if (_minimumQuality == -1 || _minimumQuality < quality)
    {
      String item = FPSTR(JSON_ITEM);
      String rssiQ;

      rssiQ += quality;
      item.replace("{v}", wifiSSIDs[i].SSID);
      item.replace("{r}", rssiQ);

#if defined(ESP8266)

      if (wifiSSIDs[i].encryptionType != ENC_TYPE_NONE)
#else
      if (wifiSSIDs[i].encryptionType != WIFI_AUTH_OPEN)
#endif
      {
        item.replace("{i}", "true");
      }
      else
      {
        item.replace("{i}", "false");
      }

      page += item;
      delay(0);
    }
    else
    {
      LOGDEBUG(F("Skipping due to quality"));
    }
  }

  page += F("]}");

#if ( USING_ESP32_S2 || USING_ESP32_C3 )
  request->send(200, WM_HTTP_HEAD_CT, page);

  // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
  delay(1);
#else

  AsyncWebServerResponse *response = request->beginResponse(200, WM_HTTP_HEAD_JSON, page);

  response->addHeader(WM_HTTP_CACHE_CONTROL, WM_HTTP_NO_STORE);
  response->addHeader(WM_HTTP_PRAGMA, WM_HTTP_NO_CACHE);
  response->addHeader(WM_HTTP_EXPIRES, "-1");

  request->send(response);
#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )  

  LOGDEBUG(F("Sent WiFiScan Data in Json format"));
}
*/

// /restart handler
void ESPAsync_WiFiManager::handleRestart(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleRestart"));

  String page = buildHeader(FPSTR("Restart"),FPSTR("Restarting the ESP!"), FPSTR(WM_META_AUTO_ROOT));
  page += F("<div class='msg'><h3>Restarting the device!</h3></div>");
  HTTPSendPage(page, request);

  LOGDEBUG(F("Sent Restart page"));
  LOGDEBUG(F("Request a restart..."));
  restartMe = true; // Will be executed by foreground code.
  // ESP.restart();  // If we do it here the webserver never gets chance to send the page request to the browser.
}

// /erase handler
void ESPAsync_WiFiManager::handleErase(AsyncWebServerRequest *request)
{
  LOGINFO(F("Entering handleErase"));

  String page = buildHeader(FPSTR("Erase"),FPSTR("Factory Reset the ESP!"), FPSTR(WM_META_AUTO_ROOT));
  page += F("<div class='msg'><h3>Erasing the device!</h3>You will need to reconnect to the Access point using default credentials after the ESP has rebooted.</div>");
  HTTPSendPage(page, request);

  LOGINFO(F("Sent Erase page"));
  LOGINFO(F("Erasing..."));

    if(LittleFS.begin()){
      LOGINFO(F("Erasing LittleFS..."));
      bool ret = LittleFS.format();
      LOGINFO1(F("LittleFS erase: "),ret ? "Success" : "ERROR");
    } else{
      LOGINFO(F("[ERROR] Could not start LittleFS"));
    }

  LOGINFO(F("Erasing WiFi Config"));
  #ifdef ESP8266
    ESP.eraseConfig();
    
  #elif defined(ESP32)
      bool ret;
      WiFi.mode(WIFI_AP_STA); // cannot erase if not in STA mode !
      WiFi.persistent(true);
      ret = WiFi.disconnect(true,true); // disconnect(bool wifioff, bool eraseap)
      delay(500);
      WiFi.persistent(false);
  #endif
  
  LOGINFO(F("Request a restart..."));
  restartMe = true; // Will be executed by foreground code.
  //ESP.restart();  // Not here or the browser never gets the page.
}

// notFound handler
void ESPAsync_WiFiManager::handleNotFound(AsyncWebServerRequest *request)
{
  LOGINFO(F("Entering handleNotFound"));
  if (captivePortal(request))
  {
    // If captive portal redirect instead of displaying the error page.
    return;
  }
  
  String reply = buildHeader(FPSTR("Erase"),FPSTR("Factory Reset the ESP!"), FPSTR(WM_META_AUTO_ROOT));
  
  reply += "Page Not Found\n\n";
  reply += "URI: ";
  reply += request->url();
  reply += "\nMethod: ";
  reply += (request->method() == HTTP_GET) ? "GET" : "POST";
  reply += "\nArguments: ";
  reply += request->args();
  reply += "\n";

  for (uint8_t i = 0; i < request->args(); i++)
  {
    reply += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }

  HTTPSendPage(reply, request);
  LOGINFO1(F("NotFound page sent: "), request->url());
}

// /update handler
void ESPAsync_WiFiManager::handleUpdate(AsyncWebServerRequest *request)
{
	LOGDEBUG(F("Entering handleupdate"));
	String page = buildHeader(FPSTR("update"),FPSTR("Upload New Firmware"));
	page += FPSTR(WM_HTTP_UPDATE);
	HTTPSendPage(page, request);
}

// upload via /doUpdate POST
void ESPAsync_WiFiManager::handleUpdating(AsyncWebServerRequest * request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
  LOGDEBUG(F("Entering handleUpdating"));
#ifdef ESP8266  
if(!index){
      LOGDEBUG1(F("Update Start: "), filename.c_str());
      Update.runAsync(true);
      if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000))
      {
        LOGDEBUG1(F("Update error: "),Update.getErrorString());
      }
    }
    if(!Update.hasError())
    {
      if(Update.write(data, len) != len)
      {
        LOGDEBUG1(F("Update: "),Update.getErrorString());
      }
    }
    if(final){
      if(Update.end(true)){
        LOGDEBUG1(F("Update Success: "), (index+len));
      } else {
        LOGDEBUG1(F("Update error: "), Update.getErrorString());
      }
    }
  #else
  if(!index){
      LOGDEBUG1(F("Update Start: "), filename.c_str());
      //Update.runAsync(true);
      if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000))
      {
        LOGDEBUG1(F("Update error: "),Update.errorString());
      }
    }
    if(!Update.hasError())
    {
      if(Update.write(data, len) != len)
      {
        LOGDEBUG1(F("Update: "),Update.errorString());
      }
    }
    if(final){
      if(Update.end(true)){
        LOGDEBUG1(F("Update Success: "), (index+len));
      } else {
        LOGDEBUG1(F("Update error: "), Update.errorString());
      }
    }
  #endif
}

// upload and ota done, show status
void ESPAsync_WiFiManager::handleUpdateDone(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleUpdateDone"));
    
    String page = buildHeader(FPSTR("update"),FPSTR("OTA Update Result"),FPSTR(WM_META_AUTO_ROOT)); 
    
    if (Update.hasError()) {
		page += FPSTR(WM_HTTP_UPDATE_FAIL);
    #ifdef ESP32
    page += "OTA Error: " + (String)Update.errorString();
    #else
    page += "OTA Error: " + (String)Update.getError();
    #endif
		LOGDEBUG(F("[OTA] update failed"));
	}
	else {
		page += FPSTR(WM_HTTP_UPDATE_SUCCESS);
		LOGDEBUG(F("[OTA] update ok"));
    restartMe = true; // Restart signalled to foreground loop.
	}
      
	HTTPSendPage(page, request);
}


const char favIconHeader[] PROGMEM = "HTTP/1.1 200 OK\nContent-Type: image/ico\nConnection: close\n\n";
// Base64 version:
//const char favicon[]       PROGMEM = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAtdJREFUOE+lk1toVGcUhb99zsxkGs8MU2ZGTag1TUJuEDGJomPAggZiqDrFS6sQ9aWWgiJSlT60eTIv0odCGWjM5SEXaUNKwYJaNYX6UGNIYlBJSdJqItYQw4hmZnKZTM755UzaqrR56n762Zu99r8WawmAUdNRIpYZAUKIuFHKbv+7RECpeaBHafrxxJXa38SoaisWXfWK0/CoxVlQ1n/uWkphWQrRNBwuA5VKxJUpm8Rb3Xodp1GlUollrkJ8JgXJRXDqsGiCruH1ByAV7xZPdeucQtzKsrCvJBdMdF3D7dJBIP5snty1Ps5+VEHlulXcfxzj664hrvVNoOkyL94d7cqyTBKxZBr57SyD6HSS2cQCNuXy4iD9ze+nf3dvNEppQYDniQWywhewEETb2qLKCvwc2J5LeYGfbeXZaa57Pu/m4uVRxq4eIWe1Qd4HnTz4/Sn+t7wEfW7GJuK4nDrClkZVUbKS/qYwc0mTusgtasNFrM/3U3PmKle+rObTSC9fNfThzjLSGjscgkPXlkSlskltKAnS1xhmb93P/NDYj6csi9hPh2m4OMwn4aJ/AHjDwccflnL+dCX5B7t4MBFbAqiweTaFqWsZoP7sDTbvyKenYTeH6m9wYl8JG4uCvLP/O8YHJjh2IkTkZIii2u8ZeTi9BFBWGOB2y5JQI+PPKczxpYUK1LRRmPMmQ+1707O7I1HWFQbS75z9nTx8kviLQnGQvqYw33bfJzfbw6OpGU5Fenk8NYOZMslb66P+6Aa2lK7ijz+nOddxh18GJ3G5bBErm1WodCU3v9nFzs+ucenHYfBmoLkdGJkuQBF7zUgWaILHm4GIII53W+by1/jcDadCfNE8wK+Dk2SucKJp8pozbcVNpdBE0O2ZaLbtbSO1XTe1zKqZaJQMw0WGS182S68iitOAVKJbjPfaimVR9Vr6Co+YswjLJPHvbdEQR+bLMP3fOL8AMWcuzi8HP64AAAAASUVORK5CYII=";
const char favicon[] PROGMEM = {
  0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 
0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF, 
0x61, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 0x42, 0x00, 0xAE, 0xCE, 0x1C, 0xE9, 0x00, 0x00, 
0x02, 0xD7, 0x49, 0x44, 0x41, 0x54, 0x38, 0x4F, 0xA5, 0x93, 0x5B, 0x68, 0x54, 0x67, 0x14, 0x85, 
0xBF, 0x7D, 0xCE, 0xCC, 0x64, 0x1A, 0xCF, 0x0C, 0x53, 0x66, 0x46, 0x4D, 0xA8, 0x35, 0x4D, 0x42, 
0x6E, 0x10, 0x31, 0x89, 0xA2, 0x63, 0xC0, 0x82, 0x06, 0x62, 0xA8, 0x3A, 0xC5, 0x4B, 0xAB, 0x10, 
0xF5, 0xA5, 0x96, 0x82, 0x22, 0x52, 0x95, 0x3E, 0xB4, 0x79, 0x32, 0x2F, 0xD2, 0x87, 0x42, 0x19, 
0x68, 0xCC, 0xE5, 0x21, 0x17, 0x69, 0x43, 0x4A, 0xC1, 0x82, 0x5A, 0x35, 0x85, 0xFA, 0x50, 0x63, 
0x48, 0x62, 0x50, 0x49, 0x49, 0xD2, 0x6A, 0x22, 0xD6, 0x10, 0xC3, 0x88, 0x66, 0x66, 0x72, 0x99, 
0x4C, 0xCE, 0xF9, 0xE5, 0x4C, 0xDA, 0xAA, 0xB4, 0x79, 0xEA, 0x7E, 0xFA, 0xD9, 0x9B, 0xBD, 0xF6, 
0xBF, 0x16, 0x6B, 0x09, 0x80, 0x51, 0xD3, 0x51, 0x22, 0x96, 0x19, 0x01, 0x42, 0x88, 0xB8, 0x51, 
0xCA, 0x6E, 0xFF, 0xBB, 0x44, 0x40, 0xA9, 0x79, 0xA0, 0x47, 0x69, 0xFA, 0xF1, 0xC4, 0x95, 0xDA, 
0xDF, 0xC4, 0xA8, 0x6A, 0x2B, 0x16, 0x5D, 0xF5, 0x8A, 0xD3, 0xF0, 0xA8, 0xC5, 0x59, 0x50, 0xD6, 
0x7F, 0xEE, 0x5A, 0x4A, 0x61, 0x59, 0x0A, 0xD1, 0x34, 0x1C, 0x2E, 0x03, 0x95, 0x4A, 0xC4, 0x95, 
0x29, 0x9B, 0xC4, 0x5B, 0xDD, 0x7A, 0x1D, 0xA7, 0x51, 0xA5, 0x52, 0x89, 0x65, 0xAE, 0x42, 0x7C, 
0x26, 0x05, 0xC9, 0x45, 0x70, 0xEA, 0xB0, 0x68, 0x82, 0xAE, 0xE1, 0xF5, 0x07, 0x20, 0x15, 0xEF, 
0x16, 0x4F, 0x75, 0xEB, 0x9C, 0x42, 0xDC, 0xCA, 0xB2, 0xB0, 0xAF, 0x24, 0x17, 0x4C, 0x74, 0x5D, 
0xC3, 0xED, 0xD2, 0x41, 0x20, 0xFE, 0x6C, 0x9E, 0xDC, 0xB5, 0x3E, 0xCE, 0x7E, 0x54, 0x41, 0xE5, 
0xBA, 0x55, 0xDC, 0x7F, 0x1C, 0xE3, 0xEB, 0xAE, 0x21, 0xAE, 0xF5, 0x4D, 0xA0, 0xE9, 0x32, 0x2F, 
0xDE, 0x1D, 0xED, 0xCA, 0xB2, 0x4C, 0x12, 0xB1, 0x64, 0x1A, 0xF9, 0xED, 0x2C, 0x83, 0xE8, 0x74, 
0x92, 0xD9, 0xC4, 0x02, 0x36, 0xE5, 0xF2, 0xE2, 0x20, 0xFD, 0xCD, 0xEF, 0xA7, 0x7F, 0x77, 0x6F, 
0x34, 0x4A, 0x69, 0x41, 0x80, 0xE7, 0x89, 0x05, 0xB2, 0xC2, 0x17, 0xB0, 0x10, 0x44, 0xDB, 0xDA, 
0xA2, 0xCA, 0x0A, 0xFC, 0x1C, 0xD8, 0x9E, 0x4B, 0x79, 0x81, 0x9F, 0x6D, 0xE5, 0xD9, 0x69, 0xAE, 
0x7B, 0x3E, 0xEF, 0xE6, 0xE2, 0xE5, 0x51, 0xC6, 0xAE, 0x1E, 0x21, 0x67, 0xB5, 0x41, 0xDE, 0x07, 
0x9D, 0x3C, 0xF8, 0xFD, 0x29, 0xFE, 0xB7, 0xBC, 0x04, 0x7D, 0x6E, 0xC6, 0x26, 0xE2, 0xB8, 0x9C, 
0x3A, 0xC2, 0x96, 0x46, 0x55, 0x51, 0xB2, 0x92, 0xFE, 0xA6, 0x30, 0x73, 0x49, 0x93, 0xBA, 0xC8, 
0x2D, 0x6A, 0xC3, 0x45, 0xAC, 0xCF, 0xF7, 0x53, 0x73, 0xE6, 0x2A, 0x57, 0xBE, 0xAC, 0xE6, 0xD3, 
0x48, 0x2F, 0x5F, 0x35, 0xF4, 0xE1, 0xCE, 0x32, 0xD2, 0x1A, 0x3B, 0x1C, 0x82, 0x43, 0xD7, 0x96, 
0x44, 0xA5, 0xB2, 0x49, 0x6D, 0x28, 0x09, 0xD2, 0xD7, 0x18, 0x66, 0x6F, 0xDD, 0xCF, 0xFC, 0xD0, 
0xD8, 0x8F, 0xA7, 0x2C, 0x8B, 0xD8, 0x4F, 0x87, 0x69, 0xB8, 0x38, 0xCC, 0x27, 0xE1, 0xA2, 0x7F, 
0x00, 0x78, 0xC3, 0xC1, 0xC7, 0x1F, 0x96, 0x72, 0xFE, 0x74, 0x25, 0xF9, 0x07, 0xBB, 0x78, 0x30, 
0x11, 0x5B, 0x02, 0xA8, 0xB0, 0x79, 0x36, 0x85, 0xA9, 0x6B, 0x19, 0xA0, 0xFE, 0xEC, 0x0D, 0x36, 
0xEF, 0xC8, 0xA7, 0xA7, 0x61, 0x37, 0x87, 0xEA, 0x6F, 0x70, 0x62, 0x5F, 0x09, 0x1B, 0x8B, 0x82, 
0xBC, 0xB3, 0xFF, 0x3B, 0xC6, 0x07, 0x26, 0x38, 0x76, 0x22, 0x44, 0xE4, 0x64, 0x88, 0xA2, 0xDA, 
0xEF, 0x19, 0x79, 0x38, 0xBD, 0x04, 0x50, 0x56, 0x18, 0xE0, 0x76, 0xCB, 0x92, 0x50, 0x23, 0xE3, 
0xCF, 0x29, 0xCC, 0xF1, 0xA5, 0x85, 0x0A, 0xD4, 0xB4, 0x51, 0x98, 0xF3, 0x26, 0x43, 0xED, 0x7B, 
0xD3, 0xB3, 0xBB, 0x23, 0x51, 0xD6, 0x15, 0x06, 0xD2, 0xEF, 0x9C, 0xFD, 0x9D, 0x3C, 0x7C, 0x92, 
0xF8, 0x8B, 0x42, 0x71, 0x90, 0xBE, 0xA6, 0x30, 0xDF, 0x76, 0xDF, 0x27, 0x37, 0xDB, 0xC3, 0xA3, 
0xA9, 0x19, 0x4E, 0x45, 0x7A, 0x79, 0x3C, 0x35, 0x83, 0x99, 0x32, 0xC9, 0x5B, 0xEB, 0xA3, 0xFE, 
0xE8, 0x06, 0xB6, 0x94, 0xAE, 0xE2, 0x8F, 0x3F, 0xA7, 0x39, 0xD7, 0x71, 0x87, 0x5F, 0x06, 0x27, 
0x71, 0xB9, 0x6C, 0x11, 0x2B, 0x9B, 0x55, 0xA8, 0x74, 0x25, 0x37, 0xBF, 0xD9, 0xC5, 0xCE, 0xCF, 
0xAE, 0x71, 0xE9, 0xC7, 0x61, 0xF0, 0x66, 0xA0, 0xB9, 0x1D, 0x18, 0x99, 0x2E, 0x40, 0x11, 0x7B, 
0xCD, 0x48, 0x16, 0x68, 0x82, 0xC7, 0x9B, 0x81, 0x88, 0x20, 0x8E, 0x77, 0x5B, 0xE6, 0xF2, 0xD7, 
0xF8, 0xDC, 0x0D, 0xA7, 0x42, 0x7C, 0xD1, 0x3C, 0xC0, 0xAF, 0x83, 0x93, 0x64, 0xAE, 0x70, 0xA2, 
0x69, 0xF2, 0x9A, 0x33, 0x6D, 0xC5, 0x4D, 0xA5, 0xD0, 0x44, 0xD0, 0xED, 0x99, 0x68, 0xB6, 0xED, 
0x6D, 0x23, 0xB5, 0x5D, 0x37, 0xB5, 0xCC, 0xAA, 0x99, 0x68, 0x94, 0x0C, 0xC3, 0x45, 0x86, 0x4B, 
0x5F, 0x36, 0x4B, 0xAF, 0x22, 0x8A, 0xD3, 0x80, 0x54, 0xA2, 0x5B, 0x8C, 0xF7, 0xDA, 0x8A, 0x65, 
0x51, 0xF5, 0x5A, 0xFA, 0x0A, 0x8F, 0x98, 0xB3, 0x08, 0xCB, 0x24, 0xF1, 0xEF, 0x6D, 0xD1, 0x10, 
0x47, 0xE6, 0xCB, 0x30, 0xFD, 0xDF, 0x38, 0xBF, 0x00, 0x31, 0x67, 0x2E, 0xCE, 0x2F, 0x07, 0x3F, 
0xAE, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};


void ESPAsync_WiFiManager::handleFavicon(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Favicon request"));

  size_t fiSize = sizeof(favicon);
  AsyncResponseStream *response = request->beginResponseStream("image/ico", fiSize);
  for (size_t i = 0; i < fiSize; i++) {
      //response->printf(favicon);
      response->write(pgm_read_byte(favicon + i));
  }
  
  // Can't use without an overload? Also need to inhibit HTTP_HELP and HTTP_END...
  // Here we need AsyncResponseStream and not AsyncWebServerResponse.
  //HTTPSendPage(page, request); 
    
#if ( USING_ESP32_S2 || USING_ESP32_C3 )
  #warning "This might not work yet? Trying to fix S2 C3 request->send for the response object." 
  //request->send(200, WM_HTTP_HEAD_CT, page);
  request->send(response);

  // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
  delay(1);
#else

 //AsyncWebServerResponse *response = request->beginResponse(200, "image/ico", page);
  response->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  // For configuring CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  response->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
#endif
  
  response->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
  response->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");

  request->send(response);
  
#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )
     
}

inline void ESPAsync_WiFiManager::HTTPSendPage(String &page, AsyncWebServerRequest *request)
{
  page += FPSTR(WM_HTTP_HELP);
  page += FPSTR(WM_HTTP_END);

  LOGINFO(F("Entering HTTP Send: "));  //, page);

#if ( USING_ESP32_S2 || USING_ESP32_C3 )
  request->send(200, WM_HTTP_HEAD_CT, page);

  // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
  delay(1);
#else
  
  AsyncWebServerResponse *response = request->beginResponse(200, WM_HTTP_HEAD_CT, page);
  response->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  // For configuring CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  response->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
#endif
  
  response->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
  response->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");

  request->send(response);

#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )
  LOGINFO(F("HTTP Send complete: ")); //, page);
}

/**
   HTTPD redirector
   Redirect to captive portal if we got a request for another domain.
   Return true in that case so the page handler do not try to handle the request again.
*/
bool ESPAsync_WiFiManager::captivePortal(AsyncWebServerRequest *request)
{
  if (!isIp(request->host()))
  { 
    LOGINFO1(F("Request received for: "), request->host());
    LOGINFO1(F("Request redirected to captive portal at http://"), toStringIp(request->client()->localIP()));

    AsyncWebServerResponse *response = request->beginResponse(302, WM_HTTP_HEAD_CT2, "");

    response->addHeader("Location", String("http://") + toStringIp(request->client()->localIP()));

    request->send(response);

    return true;
  }

  LOGINFO2(F("captivePortal: WebRequest made via IP: "), request->host(), request->url());

  return false;
}

// start up config portal callback
void ESPAsync_WiFiManager::setAPCallback(void(*func)(ESPAsync_WiFiManager* myWiFiManager))
{
  _apcallback = func;
}

// start up save config callback
void ESPAsync_WiFiManager::setSaveConfigCallback(void(*func)())
{
  _savecallback = func;
}

// sets a custom element to add to head, like a new style tag
void ESPAsync_WiFiManager::setCustomHeadElement(const char* element)
{
  _customHeadElement = element;
}

// if this is true, remove duplicated Access Points - defaut true
void ESPAsync_WiFiManager::setRemoveDuplicateAPs(bool removeDuplicates)
{
  _removeDuplicateAPs = removeDuplicates;
}


int ESPAsync_WiFiManager::getRSSIasQuality(const int& RSSI)
{
  int quality = 0;

  if (RSSI <= -100)
  {
    quality = 0;
  }
  else if (RSSI >= -50)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (RSSI + 100);
  }

  return quality;
}

// Is this an IP?
bool ESPAsync_WiFiManager::isIp(const String& str)
{
  for (unsigned int i = 0; i < str.length(); i++)
  {
    int c = str.charAt(i);

    if ( (c != '.') && (c != ':') && ( (c < '0') || (c > '9') ) )
    {
      return false;
    }
  }

  return true;
}

// IP to String
String ESPAsync_WiFiManager::toStringIp(const IPAddress& ip)
{
  String res = "";

  for (int i = 0; i < 3; i++)
  {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }

  res += String(((ip >> 8 * 3)) & 0xFF);

  return res;
}


#ifdef ESP32
// We can't use WiFi.SSID() in ESP32 as it's only valid after connected.
// SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
// Have to create a new function to store in EEPROM/SPIFFS for this purpose

String ESPAsync_WiFiManager::getESP32WiFiSSID()
{
  if (WiFi.getMode() == WIFI_MODE_NULL)
  {
    return String();
  }

  wifi_ap_record_t info;

  if (!esp_wifi_sta_get_ap_info(&info))
  {
    return String(reinterpret_cast<char*>(info.ssid));
  }
  else
  {
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);
    return String(reinterpret_cast<char*>(conf.sta.ssid));
  }

  return String();
}

//////////////////////////////////////////

String ESPAsync_WiFiManager::getESP32WiFiPass()
{
  if (WiFi.getMode() == WIFI_MODE_NULL)
  {
    return String();
  }

  wifi_config_t conf;

  esp_wifi_get_config(WIFI_IF_STA, &conf);

  return String(reinterpret_cast<char*>(conf.sta.password));
}

//////////////////////////////////////////

uint32_t ESPAsync_WiFiManager::getChipID()
{
  uint64_t chipId64 = 0;

  for (int i = 0; i < 6; i++)
  {
    chipId64 |= ( ( (uint64_t) ESP.getEfuseMac() >> (40 - (i * 8)) ) & 0xff ) << (i * 8);
  }

  return (uint32_t) (chipId64 & 0xFFFFFF);
}

//////////////////////////////////////////

uint32_t ESPAsync_WiFiManager::getChipOUI()
{
  uint64_t chipId64 = 0;

  for (int i = 0; i < 6; i++)
  {
    chipId64 |= ( ( (uint64_t) ESP.getEfuseMac() >> (40 - (i * 8)) ) & 0xff ) << (i * 8);
  }

  return (uint32_t) (chipId64 >> 24);
}

#elif defined ESP8266
  uint32_t ESPAsync_WiFiManager::getChipID() {return ESP_getChipId();};
#else
  #error "Unknown Architecture - Not ESP32 or ESP8266"
#endif

extern float psuVolts;
//extern float my_vfact;
String ESPAsync_WiFiManager::buildHeader(String pageTitle, String pageHeading, String extra){//, bool c_refresh)
  
  String page = FPSTR(WM_HTTP_HEAD_START);  // CAUTION Changed from HTTP_HEAD_START

  String ptitle = WiFi.getHostname();
  if (!pageTitle.isEmpty()) {
    ptitle += " ";
    ptitle += pageTitle;
  }
  page.replace(FPSTR(T_v), ptitle);

  page += FPSTR(WM_HTTP_SCRIPT);
  //page += FPSTR(WM_HTTP_SCRIPT_NTP);  // Timezone
  page += FPSTR(WM_HTTP_STYLE);
  page += _customHeadElement;

  page += extra;                    // Allow <meta > etc and other bespoke tags in header.
  page += FPSTR(WM_HTTP_HEAD_END);

  String str  = FPSTR(HTTP_ROOT_MAIN); // Tokens {t} and {v}
  str.replace(FPSTR(T_t),pageHeading);

  String infoStr = getWiFiHostname() + " | " + WiFi.localIP().toString() + " | psu=" + String(psuVolts,2) +"v"; //, " + String(my_vfact,2);
  str.replace(FPSTR(T_v), infoStr);
  page += str;

  page += FPSTR(HTTP_HOMEBTN);

  str = getInfoData(F("uptime")); 
  // Hack to strip table format & replace with pretty row format.
  str.replace("</td><td>",": </b>");
  str.replace("<td>", "<div><b>");
  str.replace("</td>","</div>");
  
  page += str;

  return page;
}

String ESPAsync_WiFiManager::getWiFiHostname(){
  #ifdef ESP32
    return (String)WiFi.getHostname();
  #else
    return (String)WiFi.hostname();
  #endif
}

String ESPAsync_WiFiManager::getInfoData(String id){
  String p;
  // @todo add WM versioning
  if(id==F("esphead")){
    p = FPSTR(HTTP_INFO_esphead);
    #ifdef ESP32
      p.replace(FPSTR(T_1), (String)ESP.getChipModel());
    #endif
  }  
  else if(id==F("wifihead")){
    p = FPSTR(HTTP_INFO_wifihead);
    p.replace(FPSTR(T_1),getModeString(WiFi.getMode()));
  }
  else if(id==F("uptime")){
    // subject to rollover!
    p = FPSTR(HTTP_INFO_uptime);
    p.replace(FPSTR(T_1),(String)(millis() / 1000 / 60));
    p.replace(FPSTR(T_2),(String)((millis() / 1000) % 60));
  }
  else if(id==F("chipid")){
    p = FPSTR(HTTP_INFO_chipid);
    p.replace(FPSTR(T_1),String(ESP_getChipId(),HEX));
  }
  #ifdef ESP32
  else if(id==F("chiprev")){
      p = FPSTR(HTTP_INFO_chiprev);
      String rev = (String)ESP.getChipRevision();
      #ifdef _SOC_EFUSE_REG_H_
        String revb = (String)(REG_READ(EFUSE_BLK0_RDATA3_REG) >> (EFUSE_RD_CHIP_VER_RESERVE_S)&&EFUSE_RD_CHIP_VER_RESERVE_V);
        p.replace(FPSTR(T_1),rev+"<br/>"+revb);
      #else
        p.replace(FPSTR(T_1),rev);
      #endif
  }
  #elif defined ESP8266
  else if(id==F("fchipid")){
      p = FPSTR(HTTP_INFO_fchipid);
      p.replace(FPSTR(T_1),(String)ESP.getFlashChipId());
  }
  #else
    #error "Unknown Architecture - Not ESP32 or 8266!"
  #endif
  else if(id==F("idesize")){
    p = FPSTR(HTTP_INFO_idesize);
    p.replace(FPSTR(T_1),(String)ESP.getFlashChipSize());
  }
  else if(id==F("flashsize")){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_flashsize);
      p.replace(FPSTR(T_1),(String)ESP.getFlashChipRealSize());
    #elif defined ESP32
      p = FPSTR(HTTP_INFO_psrsize);
      p.replace(FPSTR(T_1),(String)ESP.getPsramSize());      
    #endif
  }
  else if(id==F("corever")){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_corever);
      p.replace(FPSTR(T_1),(String)ESP.getCoreVersion());
    #endif      
  }
  #ifdef ESP8266
  else if(id==F("bootver")){
      p = FPSTR(HTTP_INFO_bootver);
      p.replace(FPSTR(T_1),(String)system_get_boot_version());
  }
  #endif  
  else if(id==F("cpufreq")){
    p = FPSTR(HTTP_INFO_cpufreq);
    p.replace(FPSTR(T_1),(String)ESP.getCpuFreqMHz());
  }
  else if(id==F("freeheap")){
    p = FPSTR(HTTP_INFO_freeheap);
    p.replace(FPSTR(T_1),(String)ESP.getFreeHeap());
  }
  else if(id==F("memsketch")){
    p = FPSTR(HTTP_INFO_memsketch);
    p.replace(FPSTR(T_1),(String)(ESP.getSketchSize()));
    p.replace(FPSTR(T_2),(String)(ESP.getSketchSize()+ESP.getFreeSketchSpace()));
  }
  else if(id==F("memsmeter")){
    p = FPSTR(HTTP_INFO_memsmeter);
    p.replace(FPSTR(T_1),(String)(ESP.getSketchSize()));
    p.replace(FPSTR(T_2),(String)(ESP.getSketchSize()+ESP.getFreeSketchSpace()));
  }
  else if(id==F("lastreset")){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_lastreset);
      p.replace(FPSTR(T_1),(String)ESP.getResetReason());
    #elif defined(ESP32) && defined(_ROM_RTC_H_)
      // requires #include <rom/rtc.h>
      p = FPSTR(HTTP_INFO_lastreset);
      for(int i=0;i<2;i++){
        int reason = rtc_get_reset_reason(i);
        String tok = (String)T_ss+(String)(i+1)+(String)T_es;
        switch (reason)
        {
          //@todo move to array
          case 1  : p.replace(tok,F("Vbat power on reset"));break;
          case 3  : p.replace(tok,F("Software reset digital core"));break;
          case 4  : p.replace(tok,F("Legacy watch dog reset digital core"));break;
          case 5  : p.replace(tok,F("Deep Sleep reset digital core"));break;
          case 6  : p.replace(tok,F("Reset by SLC module, reset digital core"));break;
          case 7  : p.replace(tok,F("Timer Group0 Watch dog reset digital core"));break;
          case 8  : p.replace(tok,F("Timer Group1 Watch dog reset digital core"));break;
          case 9  : p.replace(tok,F("RTC Watch dog Reset digital core"));break;
          case 10 : p.replace(tok,F("Instrusion tested to reset CPU"));break;
          case 11 : p.replace(tok,F("Time Group reset CPU"));break;
          case 12 : p.replace(tok,F("Software reset CPU"));break;
          case 13 : p.replace(tok,F("RTC Watch dog Reset CPU"));break;
          case 14 : p.replace(tok,F("for APP CPU, reseted by PRO CPU"));break;
          case 15 : p.replace(tok,F("Reset when the vdd voltage is not stable"));break;
          case 16 : p.replace(tok,F("RTC Watch dog reset digital core and rtc module"));break;
          default : p.replace(tok,F("NO_MEAN"));
        }
      }
    #endif
  }
  else if(id==F("apip")){
    p = FPSTR(HTTP_INFO_apip);
    p.replace(FPSTR(T_1),WiFi.softAPIP().toString());
  }
  else if(id==F("apmac")){
    p = FPSTR(HTTP_INFO_apmac);
    p.replace(FPSTR(T_1),(String)WiFi.softAPmacAddress());
  }
  #ifdef ESP32
  else if(id==F("aphost")){
      p = FPSTR(HTTP_INFO_aphost);
      p.replace(FPSTR(T_1),WiFi.softAPgetHostname());
  }
  #endif
  #ifndef WM_NOSOFTAPSSID
  #ifdef ESP8266
  else if(id==F("apssid")){
    p = FPSTR(HTTP_INFO_apssid);
    p.replace(FPSTR(T_1),htmlEntities(WiFi.softAPSSID()));
  }
  #endif
  #endif
  else if(id==F("apbssid")){
    p = FPSTR(HTTP_INFO_apbssid);
    p.replace(FPSTR(T_1),(String)WiFi.BSSIDstr());
  }
  // softAPgetHostname // esp32
  // softAPSubnetCIDR
  // softAPNetworkID
  // softAPBroadcastIP

  else if(id==F("stassid")){
    p = FPSTR(HTTP_INFO_stassid);
    p.replace(FPSTR(T_1),htmlEntities((String)WiFi_SSID()));
  }
  else if(id==F("staip")){
    p = FPSTR(HTTP_INFO_staip);
    p.replace(FPSTR(T_1),WiFi.localIP().toString());
  }
  else if(id==F("stagw")){
    p = FPSTR(HTTP_INFO_stagw);
    p.replace(FPSTR(T_1),WiFi.gatewayIP().toString());
  }
  else if(id==F("stasub")){
    p = FPSTR(HTTP_INFO_stasub);
    p.replace(FPSTR(T_1),WiFi.subnetMask().toString());
  }
  else if(id==F("dnss")){
    p = FPSTR(HTTP_INFO_dnss);
    p.replace(FPSTR(T_1),WiFi.dnsIP().toString());
  }
  else if(id==F("host")){
    p = FPSTR(HTTP_INFO_host);
    #ifdef ESP32
      p.replace(FPSTR(T_1),WiFi.getHostname());
    #else
    p.replace(FPSTR(T_1),WiFi.hostname());
    #endif
  }
  else if(id==F("stamac")){
    p = FPSTR(HTTP_INFO_stamac);
    p.replace(FPSTR(T_1),WiFi.macAddress());
  }
  else if(id==F("conx")){
    p = FPSTR(HTTP_INFO_conx);
    p.replace(FPSTR(T_1),WiFi.isConnected() ? FPSTR(S_y) : FPSTR(S_n));
  }
  #ifdef ESP8266
  else if(id==F("autoconx")){
    p = FPSTR(HTTP_INFO_autoconx);
    p.replace(FPSTR(T_1),WiFi.getAutoConnect() ? FPSTR(S_enable) : FPSTR(S_disable));
  }
  #endif
  #ifdef ESP32
  else if(id==F("temp")){
    // temperature is not calibrated, varying large offsets are present, use for relative temp changes only
    p = FPSTR(HTTP_INFO_temp);
    p.replace(FPSTR(T_1),(String)temperatureRead());
    p.replace(FPSTR(T_2),(String)((temperatureRead()+32)*1.8));
    // p.replace(FPSTR(T_3),(String)hallRead());
    p.replace(FPSTR(T_3),"NA");
  }
  #endif
  else if(id==F("aboutver")){
    p = FPSTR(HTTP_INFO_aboutver);
    p.replace(FPSTR(T_1),F(ESP_ASYNC_WIFIMANAGER_VERSION));
  }
  else if(id==F("aboutarduinover")){
    #ifdef VER_ARDUINO_STR
    p = FPSTR(HTTP_INFO_aboutarduino);
    p.replace(FPSTR(T_1),String(VER_ARDUINO_STR));
    #endif
  }
  // else if(id==F("aboutidfver")){
  //   #ifdef VER_IDF_STR
  //   p = FPSTR(HTTP_INFO_aboutidf);
  //   p.replace(FPSTR(T_1),String(VER_IDF_STR));
  //   #endif
  // }
  else if(id==F("aboutsdkver")){
    p = FPSTR(HTTP_INFO_sdkver);
    #ifdef ESP32
      p.replace(FPSTR(T_1),(String)esp_get_idf_version());
      // p.replace(FPSTR(T_1),(String)system_get_sdk_version()); // deprecated
    #else
    p.replace(FPSTR(T_1),(String)system_get_sdk_version());
    #endif
  }
  else if(id==F("aboutdate")){
    p = FPSTR(HTTP_INFO_aboutdate);
    p.replace(FPSTR(T_1),String(__DATE__) + " " + String(__TIME__));
  }
  return p;
}

const char* const WIFI_MODES[] PROGMEM = { "NULL", "STA", "AP", "STA+AP" };
String ESPAsync_WiFiManager::getModeString(uint8_t mode){
  if(mode <= 3) return WIFI_MODES[mode];
  return FPSTR(S_NA);
}

String ESPAsync_WiFiManager::htmlEntities(String str, bool whitespace) {
  str.replace("&","&amp;");
  str.replace("<","&lt;");
  str.replace(">","&gt;");
  if(whitespace) str.replace(" ","&#160;");
  // str.replace("-","&ndash;");
  // str.replace("\"","&quot;");
  // str.replace("/": "&#x2F;");
  // str.replace("`": "&#x60;");
  // str.replace("=": "&#x3D;");
return str;
}

/**
 * setWebServerCallback, set a callback after webserver is reset, and before routes are setup
 * if we set webserver handlers before wm, they are used and wm is not by esp webserver
 * on events cannot be overrided once set, and are not mutiples
 * @access public 
 * @param {[type]} void (*func)(void)
 */
void ESPAsync_WiFiManager::setWebServerCallback( std::function<void()> func ) {
  _webservercallback = func;
}

#endif    // ESPAsync_WiFiManager_Impl_h