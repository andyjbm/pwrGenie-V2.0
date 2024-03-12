#ifndef LLG_UTILS_H
  #define LLG_UTILS_H

  //#include "Secrets.h"

  #include <Arduino.h>
  #include "uriwildcard.h"              // Must come b4 wifimanager to override uri.h
  #include <ESPAsync_WiFiManager.h>
  
  void validateInput(const char *input, char *output)
  {
    String tmp = input;
    tmp.trim();
    tmp.replace(' ', '_');
    tmp.toCharArray(output, tmp.length() + 1);
  }

  int validateInt(const char *input)
  {
    String tmp = input;
    tmp.trim();
    tmp.replace(',', '.');
    return tmp.toInt();
  }
  float validateFloat(const char *input)
  {
    String tmp = input;
    tmp.trim();
    tmp.replace(',', '.');
    return tmp.toFloat();
  }


  // String htmlencode(String str)
  // {
  //   String encodedstr = "";
  //   char c;

  //   for (uint16_t i = 0; i < str.length(); i++)
  //   {
  //     c = str.charAt(i);

  //     if (isalnum(c)) {
  //       encodedstr += c;
  //     } else {
  //       encodedstr += "&#";
  //       encodedstr += String((uint8_t)c);
  //       encodedstr += ';';
  //     }
  //   }
  //   return encodedstr;
  // }

  float ReadPsuVolts(float my_vfact) {
    //return 100; // Temp while we debug wifi
    analogRead(A0); // Dump the 1st read because wifi also uses ADC.
    float psuVolts = ((float)(analogRead(A0) / my_vfact));
    return psuVolts;
  }

//  void testhostByName(){
//  IPAddress remote_addr;  
//    if (WiFi.hostByName(((String)EMONCMS_DEFAULT_URL).c_str(), remote_addr)) {
//          CONSOLE(F("Remote addr: ")); CONSOLE(remote_addr);
//        }else{
//          CONSOLE(F("hostByName Failed..."));
//        }
//  }

//  uint32_t getS2MiniChipId(){
//    uint32_t chipId = 0;
//      for(int i=0; i<17; i=i+8) {
//      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
//    }
//    return chipId;
//  }

#endif