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

void ESPAsync_WMParameter::setValue(const char *defaultValue) {
  if(!_WMParam_data._id){
    // Serial.println("cannot set value of this parameter");
    return;
  }
    // Use existing length or set a default if there isn't one.
    if (!(_WMParam_data._length>0)) {
        _WMParam_data._length = WFM_DEFAULT_LABEL_SIZE;
        PGLOGINFO(F("WMP setValue setting to default length: "));
    } else {
      PGLOGINFO(F("WMP setValue using existing length: "));
    }
    setValue(defaultValue, _WMParam_data._length);
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


// End of WMParameter Class.
//================================================

ESPAsync_WMParameter** ESPAsync_WiFiManager::getParameters()
{
  return _params;
}

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
  //WiFi.mode(WIFI_STA);  // This crashes framework v6.5.0 We shouldn't really be doing this in a class constructor? This is for setup(){}

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
{
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
}

#else

void ESPAsync_WiFiManager::addParameter(ESPAsync_WMParameter *p)
{
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
}
#endif

ESPAsync_WMParameter *ESPAsync_WiFiManager::getParameter(uint8_t index){
  return _params[index];
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
  server->on("/",         std::bind(&ESPAsync_WiFiManager::handleRoot,        this, std::placeholders::_1));
  server->on("/wifi",     std::bind(&ESPAsync_WiFiManager::handleWifi,        this, std::placeholders::_1));
  server->on("/wifisave", std::bind(&ESPAsync_WiFiManager::handleWifiSave,    this, std::placeholders::_1));
  server->on("/close",    std::bind(&ESPAsync_WiFiManager::handleServerClose, this, std::placeholders::_1));
  server->on("/i",        std::bind(&ESPAsync_WiFiManager::handleInfo,        this, std::placeholders::_1));
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
  
  server->on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, FPSTR("image/ico"), favicon, sizeof(favicon));});
  server->on("/style.css",   HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, FPSTR("text/css"),  WM_HTTP_STYLE_CSS);});
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

  int connectResult = WL_CONNECT_FAILED;

  // Try and connect each with ESP, _ssid, _ssid1 creds in turn.
  
  // NOTE the difference between:
  //  WiFi.SSID() > the currently connected SSID or "" if we are not connected.
  //  WiFi_SSID() > the ESP stored SSID whether we are connected or not.

  //// Just get the ESP to try to connect 1st:
  //if(WiFi.reconnect() == true)
  //{
  //  LOGDEBUG1(F("We reconnected with prev ESP creds aok: ESP SSID is:"), WiFi.SSID());
  //}

  if (_ssid == WiFi_SSID() || (_ssid == "" && WiFi_SSID() != "")){  // if _ssid & ESP creds are the same or _ssid not specified then just reconnect 1st.
    LOGINFO(F("Attempting to connect to previous ESP creds..."));
    if ((connectResult = connectWifi()) == WL_CONNECTED) {
        LOGINFO1(F("Yay! Connected with prev ESP creds."), WiFi.SSID());
        return connectResult;
      } else {
        LOGERROR1(F("Failed to connect with prev ESP creds: "), WiFi_SSID());
      }
  }

  if (_ssid != ""){
    // Either _ssid and ESP creds are different, or they're the same and attempting to 
    // connect using ESP stored creds just failed.
    // By specifying creds this will reset the ESP creds b4 attempting to connect this time.
    if ((connectResult = connectWifi(_ssid, _pass)) == WL_CONNECTED){
      LOGINFO1(F("Connected to new SSID"), _ssid);
      return connectResult;
    } else {
      LOGERROR1(F("Failed to connect to"), _ssid);
    }
  } else {
      LOGERROR(F("Can't connect using _ssid: empty string"));
  }

  // Both those failed so try the reserve creds.
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
      LOGWARN(F("Already connected to the correct SSID. Bailing out."));
      return WL_CONNECTED;
    }

    // We (may) have a new wifi to connect to.
    // We (may) have new IP settings to use.
    // So here we assume ssid = "" to mean we are trying the ESP creds so we don't reset.
    // *Either* we have specific user creds requested different from that on the ESP chip 
    // *or* the ESP creds are the same but we have chosen to reset because:
    //    The ESP creds connection attempt failed 1st time,
    //    We have different IP settings. 
    if (ssid != "") {
      LOGWARN(F("Reseting Wifi Settings."));
      resetSettings();
    }

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
      LOGWARN1(F("Connect to new WiFi creds maybe using new IP parameters"),ssid);
      WiFi.begin(ssid.c_str(), pass.c_str());
    }
    else
    {
      // Start Wifi with old values.
      LOGWARN1(F("Connect to previous ESP WiFi creds maybe using new IP parameters"),WiFi_SSID());
      WiFi.begin();
    }
  }
  else // this is a given from the original if clause above>>> if (WiFi_SSID() == "")
  {
    LOGWARN(F("No new credentials given and no ESP saved credentials to connect with!"));
  }

  int connRes = waitForConnectResult();

  LOGWARN1(F("Connection result: "), getStatus(connRes));

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

  WiFi.waitForConnectResult(WIFI_CONECTION_TIMEOUT);
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
const __FlashStringHelper * ESPAsync_WiFiManager::getStatus(const int& status)
{
  switch (status)
  {
    case WL_IDLE_STATUS:
      return F("WL_IDLE_STATUS");

    case WL_NO_SSID_AVAIL:
      return F("WL_NO_SSID_AVAIL");

    case WL_CONNECTED:
      return F("WL_CONNECTED");

    case WL_CONNECT_FAILED:
      return F("WL_CONNECT_FAILED");

#if ESP8266
    case WL_WRONG_PASSWORD:
      return F("WL_WRONG_PASSWORD");
#endif

    case WL_DISCONNECTED:
      return F("WL_DISCONNECTED");

    default:
      return F("UNKNOWN");
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
//  page += FPSTR(WM_HTTP_END);

  HTTPSendPage(page, request);

  LOGDEBUG(F("Sent wifi save page"));

  //notify that configuration has changed and any optional parameters should be saved
  if ( _savecallback != NULL)
  {
    _savecallback();
  }

  LOGDEBUG(F("Finished wifi save page delay."));
  // This doesn't allow the sent page to be returned b4 the reconnect is attempted.
  // Need to quit this function and let the foreground do it.
  //autoConnect(_apName, _apPassword);  // Reconnect. This call will start the portal if the new creds don't connect.
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

// /info handler
void ESPAsync_WiFiManager::handleInfo(AsyncWebServerRequest *request)
{
  LOGDEBUG(F("Entering handleInfo"));
  
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
    infos = 29;
    String infoids[] = {
      F("esphead"),
      F("uptime"),
      F("chipid"),
      F("chipOUI"),   //New
      F("chipmodel"), //Changed chiprev to chipModel
      F("fchipid"),   //New
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
  page += getInfoData("aboutsdkver");
  page += F("</tr><tr>");
  page += getInfoData("aboutdate");
  page += F("</tr></tbody></table>");
  page += FPSTR(WM_FLDSET_END);

  #ifdef USE_AVAILABLE_PAGES
    page += FPSTR(WM_FLDSET_START);
    page += FPSTR(WM_HTTP_AVAILABLE_PAGES);
    page += FPSTR(WM_FLDSET_END);
  #endif

  //page += WM_HTTP_PORTAL_OPTIONS[8];
  //page += WM_HTTP_PORTAL_OPTIONS[9];

  HTTPSendPage(page, request);
  
  LOGDEBUG(F("Sent info page: ")); //, page);
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
  page.replace(FPSTR("{v}"), FPSTR("Config Saved"));
  
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
  
//  String page = buildHeader(FPSTR("Not Found"),FPSTR("Request Not found"), FPSTR(WM_META_AUTO_ROOT));

  String page = FPSTR(WM_HTTP_HEAD_START);
  page.replace(FPSTR("{v}"), FPSTR("Not Found"));
  
  page += FPSTR(WM_HTTP_SCRIPT);
  page += FPSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  page += FPSTR(WM_META_AUTO_ROOT); // This to auto reload the home page.
  page += FPSTR(WM_HTTP_HEAD_END);
    
  String reply = FPSTR("URI: ");
  reply += request->url();
  reply += FPSTR("\nMethod: ");
  reply += (request->method() == HTTP_GET) ? FPSTR("GET") : FPSTR("POST");
  reply += FPSTR("\nArguments: ");
  reply += request->args();
  reply += FPSTR("\n");

  for (uint8_t i = 0; i < request->args(); i++)
  {
    reply += FPSTR(" ") + request->argName(i) + FPSTR(": ") + request->arg(i) + FPSTR("\n");
  }
  String str = FPSTR(WM_HTTP_NOTFOUND);
  str.replace(FPSTR("{nf}"), reply);
  page += str;

  HTTPSendPage(page, request);
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

inline void ESPAsync_WiFiManager::HTTPSendPage(String &page, AsyncWebServerRequest *request)
{
  //Framework Version *FIXME* Move to info page?
  String str = FPSTR(HTTP_PAGE_MAIN3);
  str.replace(T_h, COMPILED_FRAMEWORK_VERSIONS); 

  page += str;
  page += FPSTR(WM_HTTP_HELP);
  page += FPSTR(WM_HTTP_END);

  LOGINFO(F("Entering HTTP Send: "));  //, page);
  // Free heap b4 calling response. We need >12k free for webserver to be able to work.
  str = F("<strong>Free heap:</strong> "); str += (String)ESP.getFreeHeap();
  page.replace(FPSTR("{fh}"), str);

  AsyncWebServerResponse *response = request->beginResponse(200, WM_HTTP_HEAD_CT, page);
  response->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  // For configuring CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  response->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
#endif
  
  response->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
  response->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");

  request->send(response);

#if ( USING_ESP32_S2 || USING_ESP32_C3 )
  // This was fixed in webserver. Was it fixed in ASync_Webserver?
  // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
  delay(1);
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
  page.replace(T_v, ptitle);

  page += FPSTR(WM_HTTP_SCRIPT);
  //page += FPSTR(WM_HTTP_SCRIPT_NTP);  // Timezone
  page += FPSTR(WM_HTTP_STYLE);
  page += _customHeadElement;

  page += extra;                    // Allow <meta > etc and other bespoke tags in header.
  page += FPSTR(WM_HTTP_HEAD_END);

  // Title, Hostname and IP address:
  String str = FPSTR(HTTP_PAGE_MAIN1); // Tokens {t}, {h} {ip}
  str.replace(T_t, pageHeading);
  str.replace(T_h, getWiFiHostname());
  str.replace(T_i, WiFi.localIP().toString());
  page += str;

  page += FPSTR(HTTP_HOMEBTN);

  str = getUptimeAsString(FPSTR(HTTP_PAGE_MAIN2));
  //psu volts:
  str.replace(T_p, String(psuVolts,2));
  // Type is the base source device the firmware is compiled as. See defs.h
  str.replace(T_m, FPSTR(PWR_GENIE_TYPE_STR));
  page += str;
  
  return page;
}

String ESPAsync_WiFiManager::getUptimeAsString(const __FlashStringHelper * html_uptime){
  String str = FPSTR(html_uptime); // Tokens {1}, {2}, {psu} {m}
  uint32_t ut = millis()/1000;
  str.replace(T_1,(String)(ut / 3600));
  str.replace(T_2,(String)((ut / 60) % 60));
  str.replace(T_3,(String)(ut % 60));
  return str;
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
      p.replace(T_1, (String)ESP.getChipModel());
    #endif
  }  
  else if(id==F("wifihead")){
    p = FPSTR(HTTP_INFO_wifihead);
    p.replace(T_1,getModeString(WiFi.getMode()));
  }
  else if(id==F("uptime")){
    p = getUptimeAsString(FPSTR(HTTP_INFO_uptime));
  }
  else if(id==F("chipid")){
    p = FPSTR(HTTP_INFO_chipid);
    p.replace(T_1,String(ESP_getChipId(),HEX));
  }
  #ifndef ESP8266
    // Not available on ESP8266
    else if (id==F("chipOUI")){
      p = FPSTR(HTTP_INFO_chipOUI);
      p.replace(T_1,String(getChipOUI(),HEX));
    }
  #endif
  else if(id==F("chipmodel")){
    p = FPSTR(HTTP_INFO_chipmodel);
      #ifdef ESP32
        p.replace(T_1,ESP.getChipModel());
        String rev = (String)ESP.getChipRevision();
        #ifdef _SOC_EFUSE_REG_H_
          String revb = (String)(REG_READ(EFUSE_BLK0_RDATA3_REG) >> (EFUSE_RD_CHIP_VER_RESERVE_S)&&EFUSE_RD_CHIP_VER_RESERVE_V);
          p.replace(T_2,rev+"<br/>"+revb);
        #else
          p.replace(T_2,rev);
        #endif
    #elif defined ESP8266
      p.replace(T_1,F("TODO"));
    #endif
  }
  else if(id==F("fchipid")){
    p = FPSTR(HTTP_INFO_fchipid);
    #ifdef ESP32
      p.replace(T_1,F("TODO"));
    #else  
      p.replace(T_1,(String)ESP.getFlashChipId());
    #endif
  }
  else if(id==F("idesize")){
    p = FPSTR(HTTP_INFO_idesize);
    p.replace(T_1,(String)ESP.getFlashChipSize());
  }
  else if(id==F("flashsize")){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_flashsize);
      p.replace(T_1,(String)ESP.getFlashChipRealSize());
    #elif defined ESP32
      p = FPSTR(HTTP_INFO_psrsize);
      p.replace(T_1,(String)ESP.getPsramSize());      
    #endif
  }
  else if(id==F("corever")){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_corever);
      p.replace(T_1,(String)ESP.getCoreVersion());
    #endif      
  }
  #ifdef ESP8266
  else if(id==F("bootver")){
      p = FPSTR(HTTP_INFO_bootver);
      p.replace(T_1,(String)system_get_boot_version());
  }
  #endif  
  else if(id==F("cpufreq")){
    p = FPSTR(HTTP_INFO_cpufreq);
    p.replace(T_1,(String)ESP.getCpuFreqMHz());
  }
  else if(id==F("freeheap")){
    p = FPSTR(HTTP_INFO_freeheap);
    p.replace(T_1,(String)ESP.getFreeHeap());
  }
  else if(id==F("memsketch")){
    p = FPSTR(HTTP_INFO_memsketch);
    p.replace(T_1,(String)(ESP.getSketchSize()));
    p.replace(T_2,(String)(ESP.getSketchSize()+ESP.getFreeSketchSpace()));
  }
  else if(id==F("memsmeter")){
    p = FPSTR(HTTP_INFO_memsmeter);
    p.replace(T_1,(String)(ESP.getSketchSize()));
    p.replace(T_2,(String)(ESP.getSketchSize()+ESP.getFreeSketchSpace()));
  }
  else if(id==F("lastreset")){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_lastreset);
      p.replace(T_1,(String)ESP.getResetReason());
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
    p.replace(T_1,WiFi.softAPIP().toString());
  }
  else if(id==F("apmac")){
    p = FPSTR(HTTP_INFO_apmac);
    p.replace(T_1,(String)WiFi.softAPmacAddress());
  }
  #ifdef ESP32
  else if(id==F("aphost")){
      p = FPSTR(HTTP_INFO_aphost);
      p.replace(T_1,WiFi.softAPgetHostname());
  }
  #endif
  #ifndef WM_NOSOFTAPSSID
  #ifdef ESP8266
  else if(id==F("apssid")){
    p = FPSTR(HTTP_INFO_apssid);
    p.replace(T_1,htmlEntities(WiFi.softAPSSID()));
  }
  #endif
  #endif
  else if(id==F("apbssid")){
    p = FPSTR(HTTP_INFO_apbssid);
    p.replace(T_1,(String)WiFi.BSSIDstr());
  }
  // softAPgetHostname // esp32
  // softAPSubnetCIDR
  // softAPNetworkID
  // softAPBroadcastIP

  else if(id==F("stassid")){
    p = FPSTR(HTTP_INFO_stassid);
    p.replace(T_1,htmlEntities((String)WiFi_SSID()));
  }
  else if(id==F("staip")){
    p = FPSTR(HTTP_INFO_staip);
    p.replace(T_1,WiFi.localIP().toString());
  }
  else if(id==F("stagw")){
    p = FPSTR(HTTP_INFO_stagw);
    p.replace(T_1,WiFi.gatewayIP().toString());
  }
  else if(id==F("stasub")){
    p = FPSTR(HTTP_INFO_stasub);
    p.replace(T_1,WiFi.subnetMask().toString());
  }
  else if(id==F("dnss")){
    p = FPSTR(HTTP_INFO_dnss);
    p.replace(T_1,WiFi.dnsIP().toString());
  }
  else if(id==F("host")){
    p = FPSTR(HTTP_INFO_host);
    #ifdef ESP32
      p.replace(T_1,WiFi.getHostname());
    #else
    p.replace(T_1,WiFi.hostname());
    #endif
  }
  else if(id==F("stamac")){
    p = FPSTR(HTTP_INFO_stamac);
    p.replace(T_1,WiFi.macAddress());
  }
  else if(id==F("conx")){
    p = FPSTR(HTTP_INFO_conx);
    p.replace(T_1,WiFi.isConnected() ? FPSTR(S_y) : FPSTR(S_n));
  }
  #ifdef ESP8266
  else if(id==F("autoconx")){
    p = FPSTR(HTTP_INFO_autoconx);
    p.replace(T_1,WiFi.getAutoConnect() ? FPSTR(S_enable) : FPSTR(S_disable));
  }
  #endif
  #ifdef ESP32
  else if(id==F("temp")){
    // temperature is not calibrated, varying large offsets are present, use for relative temp changes only
    p = FPSTR(HTTP_INFO_temp);
    p.replace(T_1,(String)temperatureRead());
    p.replace(T_2,(String)((temperatureRead()+32)*1.8));
    // p.replace(T_3,(String)hallRead());
    p.replace(T_3,"NA");
  }
  #endif
  else if(id==F("aboutver")){
    p = FPSTR(HTTP_INFO_aboutver);
    p.replace(T_1,F(ESP_ASYNC_WIFIMANAGER_VERSION));
  }
  else if(id==F("aboutarduinover")){
    #ifdef PIO_PACKAGE_FRAMEWORK_ARDUINOESPRESSIF32_DECODED_VERSION
    p = FPSTR(HTTP_INFO_aboutarduino);
    p.replace(T_1,String(PIO_PACKAGE_FRAMEWORK_ARDUINOESPRESSIF32_DECODED_VERSION));
    #endif
  }
  // else if(id==F("aboutidfver")){
  //   #ifdef VER_IDF_STR
  //   p = FPSTR(HTTP_INFO_aboutidf);
  //   p.replace(T_1,(String)esp_get_idf_version());
  //   #endif
  // }
  else if(id==F("aboutsdkver")){
    p = FPSTR(HTTP_INFO_sdkver);
    #ifdef ESP32
      p.replace(T_1,F(PIO_PLATFORM_VERSION_FULL));
    #else
    p.replace(T_1,(String)system_get_sdk_version());
    #endif
  }
  else if(id==F("aboutdate")){
    p = FPSTR(HTTP_INFO_aboutdate);
    p.replace(T_1,String(__DATE__) + " " + String(__TIME__));
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